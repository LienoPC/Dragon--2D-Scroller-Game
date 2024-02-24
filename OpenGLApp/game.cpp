/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/


#include "game.h"
#include "game_structures/bullet.h"
#include "resource_manager/resource_manager.h"
#include "sprite_renderer/sprite_renderer.h"
#include "game_structures/window_constraints.h"
#include "text_renderer.h"
#include "flat_renderer/flat_renderer.h"

// Game-related State data
SpriteRenderer* Renderer;
TextRenderer* Text;
FlatRenderer* Flat;



Game::Game(unsigned int width, unsigned int height)
    : State(GAME_MENU), Keys(), Width(width), Height(height) {
}

Game::~Game()
{
    delete Renderer;
}

void Game::Init()
{
    // load shaders
    ResourceManager::LoadShader("shaders/sprite.vs", "shaders/sprite.fs", nullptr, "sprite");
    // configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),
        static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    // set render-specific controls
    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
    // load textures
    ResourceManager::LoadTexture("textures/hitbox.png", true, "hitbox");
    ResourceManager::LoadTexture("textures/stalin.png", true, "stalin");
    ResourceManager::LoadTexture("textures/levels/Background4LUNGO.png", true, "level1Grass");
    ResourceManager::LoadTexture("textures/trozky.png", true, "trozky");
    ResourceManager::LoadTexture("textures/lenin.png", true, "lenin");
    // load dragon animation frames
    ResourceManager::LoadTexture("textures/dragon_frame0.png", true, "dragon_f0");
    ResourceManager::LoadTexture("textures/dragon_frame1.png", true, "dragon_f1");
    ResourceManager::LoadTexture("textures/dragon_frame2.png", true, "dragon_f2");
    ResourceManager::LoadTexture("textures/dragon_frame3.png", true, "dragon_f3");
    ResourceManager::LoadTexture("textures/dragon_frame4.png", true, "dragon_f4");
    ResourceManager::LoadTexture("textures/dragon_frame5.png", true, "dragon_f5");
    ResourceManager::LoadTexture("textures/dragon_frame6.png", true, "dragon_f6");
    ResourceManager::LoadTexture("textures/dragon_frame7.png", true, "dragon_f7");

    // create bulletTypes
    Bullet b1(20.0f, 0.0f, ResourceManager::GetTexture("trozky"), glm::vec2(300.0f, 0.0f), glm::vec2(70.0f, 80.0f), glm::vec3(1.0f), glm::vec2(0.8f), HitboxType(SQUARE), (int)'a');
    Bullet b2(50.0f, 0.0f, ResourceManager::GetTexture("lenin"), glm::vec2(100.0f, 0.0f), glm::vec2(70.0f, 80.0f), glm::vec3(1.0f), glm::vec2(0.6f), HitboxType(SQUARE), (int)'b');
    ResourceManager::SetBullet(b1);
    ResourceManager::SetBullet(b2);

    // create all Throwing Windows
    ThrowWindow center(glm::vec2(this->Width/2, 0.0f), glm::vec2(this->Width/8, 0.0f), 0, DirectionStart(UP));
    ThrowWindow leftAngle(glm::vec2(0.0f, 0.0f),glm::vec2(0.0f), 1, DirectionStart(UP));
    ThrowWindow rightAngle(glm::vec2(this->Width, 0.0f), glm::vec2(0.0f), 2, DirectionStart(UP));
    ThrowWindow leftLeft(glm::vec2(0.0f, this->Height / 4), glm::vec2(0.0f, this->Height / 8), 3, DirectionStart(LEFT));
    ThrowWindow rightRight(glm::vec2(this->Width, this->Height / 4), glm::vec2(0.0f, this->Height / 8), 4, DirectionStart(RIGHT));
    ThrowWindow topLeft(glm::vec2(this->Width / 4, 0.0f), glm::vec2(this->Width / 8, 0.0f), 5, DirectionStart(UP));
    ThrowWindow topRight(glm::vec2(this->Width - (this->Width / 4), 0.0f), glm::vec2(this->Width / 8, 0.0f), 6, DirectionStart(UP));
    ResourceManager::SetWindow(center);
    ResourceManager::SetWindow(leftAngle);
    ResourceManager::SetWindow(rightAngle);
    ResourceManager::SetWindow(leftLeft);
    ResourceManager::SetWindow(rightRight);
    ResourceManager::SetWindow(topLeft);
    ResourceManager::SetWindow(topRight);

    //create Menus and Buttons
    int id = 0;  
    Menu mainMenu(id, ResourceManager::GetTexture("hitbox"));
    Button playButton({ 1280.0f/2 - 720.0f/2 , 960.0f/2 - 480.0f/2 }, { 720.0f, 480.0f }, buttonType::link, ResourceManager::GetTexture("hitbox"), ResourceManager::GetTexture("hitbox"));
    mainMenu.addButton(playButton);
    this->Menus.push_back(mainMenu);
    this->currMenu = id;

    //Initialization of the text renderer
    Text = new TextRenderer(this->Width, this->Height);
    Text->Load("fonts/aAbsoluteEmpire.ttf", FONT);

    //Initialization of the flat renderer
    ResourceManager::LoadShader("shaders/flat.vs", "shaders/flat.fs", nullptr, "flat");
    Flat = new FlatRenderer(ResourceManager::GetShader("flat"));

    //HUD initialization
    this->HUD.init();

    // load levels
    // Level 1
    GameLevel l1;
    l1.windowNumber = W_NUMBER_1;
    l1.LoadLevel(SCREEN_HEIGHT, SCREEN_WIDTH);
    this->Levels.push_back(l1);
    this->Level = 0;
}

void Game::Update(float dt)
{
    GameLevel* level = &this->Levels[this->Level];
    level->PlayLevel(dt); // gestisce il lancio di nuovi proiettili e la cancellazione di quelli che escono dalla scena

    //Verifica delle hitbox per ogni proiettile/palle del drago
    //Non so come sono gestite le palle per mo
  
      for (int i = 0; i < level->instancedBullets.size(); i++) {
        // Verifico per ogni proiettile istanziato se ci sono hit
        Bullet b = level->instancedBullets[i];
        switch (b.hitboxT) {
            case SQUARE:
            {
                std::shared_ptr<Square> s = std::dynamic_pointer_cast<Square>(b.hitbox);
                if (verifyDragonCollisionSquare(*s)) {
                    // il bullet i ha colpito il dragòn
                    hitDragon(&level->instancedBullets[i]);
                }

            }
            break;
            case CIRCLE:
            {
                std::shared_ptr<Circle> c = std::dynamic_pointer_cast<Circle>(b.hitbox);
                if (verifyDragonCollisionCircle(*c)) {
                    // il bullet i ha colpito il dragòn

                }

            }
            break;
        }      
    }
}

void Game::ProcessInput(float dt)
{  
    if (Game::State == GAME_MENU) {

    }
    else if (Game::State == GAME_ACTIVE) {
        GameLevel* level = &this->Levels[this->Level];
        // Gestione della velocità (sprint, slowdown)
        static bool sprint = false, slowdown = false;

        if (this->Keys[GLFW_KEY_LEFT_SHIFT] && !sprint) {
            level->player.setVelocityModifier(650.0f);
            sprint = true;       
        }
        else if (!this->Keys[GLFW_KEY_LEFT_SHIFT] && sprint) {
             level->player.setVelocityModifier(400.0f);
             sprint = false;
        }

        if (this->Keys[GLFW_KEY_LEFT_CONTROL] && !slowdown) {
             level->player.setVelocityModifier(250.0f);
             slowdown = true;
        }
        else if (!this->Keys[GLFW_KEY_LEFT_CONTROL] && slowdown) {
             level->player.setVelocityModifier(400.0f);
             slowdown = false;
        }

        // Movimento del drago
        float velocity = dt * level->player.velocityModifier;
        glm::vec2 move, playerPos = level->player.position;

        if (this->Keys[GLFW_KEY_A]) {
            move = glm::vec2(-velocity, 0.0f);

            if (!level->isPlayerOutOfBounds(playerPos + move, SCREEN_HEIGHT, SCREEN_WIDTH))
                level->movePlayer(move);
        }
        if (this->Keys[GLFW_KEY_D]) {
            move = glm::vec2(velocity, 0.0f);

            if(!level->isPlayerOutOfBounds(playerPos + move, SCREEN_HEIGHT, SCREEN_WIDTH))
                level->movePlayer(move);
        }
        if (this->Keys[GLFW_KEY_W]) {
            move = glm::vec2(0.0f, -velocity);

            if (!level->isPlayerOutOfBounds(playerPos + move, SCREEN_HEIGHT, SCREEN_WIDTH))
                level->movePlayer(move);
        }
        if (this->Keys[GLFW_KEY_S]) {
            move = glm::vec2(0.0f, velocity);

            if (!level->isPlayerOutOfBounds(playerPos + move, SCREEN_HEIGHT, SCREEN_WIDTH))
                level->movePlayer(move);
        }
     }   
}

void Game::Render(float dt)
{   
    if (Game::State == GAME_MENU) {
        this->Menus[this->currMenu].drawMenu(*Renderer, NULL);
    }
    else if (Game::State == GAME_ACTIVE) {
        // draw background
        Renderer->DrawScrollingBackground(ResourceManager::GetTexture("level1Grass"), glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height*15), 0.0f, glm::vec3(1.0f), dt);
        // draw level with a bullet in
        this->Levels[this->Level].Draw(*Renderer, dt);
        this->HUD.RenderHUD(*Renderer, *Text, *Flat, this->Levels[this->Level].player);
        
    } 
}

//FUNZIONI HITBOX -------------------------------------------------------------------------------------------
void Game::hitDragon(Bullet* b) {
    // eliminare il bullet
    b->destroyed = true;

    //Se il drago viene colpito, TODO:
    //-Danno agli HP
    //-Proiettile sparisce->Fatto
    //-Effetto visivo per il fatto di essere stati colpiti:
    // -Quello sul drago è gestito dalla classe Dragòn(?)
    // -Quello sul proiettile è gestito direttamente da qua(per evitare overhead del gameLoop)
    this->Levels[this->Level].player.dealDamage(b->Power);
}



bool Game::verifyDragonCollisionSquare(Square h) {
    for (int i = 0; i < this->Levels[this->Level].player.hitboxes.size(); i++) {

        if (checkCollisionSquareSquare(this->Levels[this->Level].player.hitboxes[i], h)) {
            return true;
        }
    }
    return false;
}

bool Game::verifyDragonCollisionCircle(Circle h) {
    for (int i = 0; i < this->Levels[this->Level].player.hitboxes.size(); i++) {
        if (checkCollisionSquareCircle(this->Levels[this->Level].player.hitboxes[i], h)) {
            return true;
        }
    }
    return false;
}




float Game::dotProduct(glm::vec2 a, glm::vec2 b) {
    return a.x * b.x + a.y * b.y;
}

bool Game::checkCollisionSquareSquare(Square hitbox1, Square hitbox2) {
    for (int i = 0; i < 4; ++i) {
        // Calcola l'asse per la proiezione
        glm::vec2 axis = {hitbox1.left_up.y - hitbox1.left_down.y,
                       hitbox1.left_down.x - hitbox1.left_up.x};

        float minhitbox1 = dotProduct(axis, hitbox1.left_up);
        float maxhitbox1 = minhitbox1;

        float minhitbox2 = dotProduct(axis, hitbox2.left_up);
        float maxhitbox2 = minhitbox2;

        // Calcola la proiezione su ogni asse
        for (int j = 1; j < 4; ++j) {
            float projectionhitbox1, projectionhitbox2;

            if (j == 1) {
                projectionhitbox1 = dotProduct(axis, hitbox1.left_down);
                projectionhitbox2 = dotProduct(axis, hitbox2.left_down);
            }
            else if (j == 2) {
                projectionhitbox1 = dotProduct(axis, hitbox1.right_up);
                projectionhitbox2 = dotProduct(axis, hitbox2.right_up);
            }
            else {
                projectionhitbox1 = dotProduct(axis, hitbox1.right_down);
                projectionhitbox2 = dotProduct(axis, hitbox2.right_down);
            }

            minhitbox1 = std::min(minhitbox1, projectionhitbox1);
            maxhitbox1 = std::max(maxhitbox1, projectionhitbox1);

            minhitbox2 = std::min(minhitbox2, projectionhitbox2);
            maxhitbox2 = std::max(maxhitbox2, projectionhitbox2);

            
        }
        // Se non c'è sovrapposizione su questo asse, i rettangoli non si intersecano
        if (maxhitbox1 < minhitbox2 || maxhitbox2 < minhitbox1) {
            return false;
        }
       
    }

    // Sovrapposizione su tutti gli assi, i rettangoli si intersecano
    return true;
}

bool Game::checkCollisionSquareCircle(Square hitboxS, Circle hitboxC) {   //hitboxS: drago //hitboxC:bullet
    float closestX = std::max(hitboxS.left_up.x, std::min(hitboxC.center.x, hitboxS.right_down.x));
    float closestY = std::max(hitboxS.left_up.y, std::min(hitboxC.center.y, hitboxS.right_down.y));

    // Calcola la distanza tra il punto più vicino e il centro del cerchio
    float distanceX = hitboxC.center.x - closestX;
    float distanceY = hitboxC.center.y - closestY;

    // Se la distanza è inferiore al raggio, c'è intersezione
    if ((distanceX * distanceX + distanceY * distanceY) < (hitboxC.radius * hitboxC.radius)) {
        return true;
    }
    return false;
}

bool Game::checkCollisionCircleCircle(Circle hitbox1, Circle hitbox2) {
    // Calcola la distanza tra i centri dei due cerchi
    float distance = std::sqrt(std::pow(hitbox2.center.x - hitbox1.center.x, 2) +   //pow usato per calcolare il quadrato
        std::pow(hitbox2.center.y - hitbox1.center.y, 2));

    if (distance <= (hitbox1.radius + hitbox2.radius)) {
        return true;
    }
    return false;
}
//-------------------------------------------------------------------------------------------------------------------

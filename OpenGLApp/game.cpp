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
#include "game_structures/particle_generators/particle_generator.h"
#include "game_structures/particle_generators/hit_particle_generator.h"
#include "game_structures/particle_generators/continuous_particle_generator.h"

// Game-related State data
SpriteRenderer* Renderer;
TextRenderer* Text;
FlatRenderer* Flat;



Game::Game(unsigned int width, unsigned int height)
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{

}

Game::~Game()
{
    delete Renderer;
}

void Game::Init()
{

    // load shaders
    ResourceManager::LoadShader("shaders/sprite.vs", "shaders/sprite.fs", nullptr, "sprite");
    ResourceManager::LoadShader("shaders/flat.vs", "shaders/flat.fs", nullptr, "flat");
    ResourceManager::LoadShader("shaders/particle.vs", "shaders/particle.fs", nullptr, "particle");
    // configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),
        static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    ResourceManager::GetShader("flat").Use().SetInteger("image", 0);
    ResourceManager::GetShader("flat").SetMatrix4("projection", projection);
    ResourceManager::GetShader("particle").Use().SetInteger("image", 0);
    ResourceManager::GetShader("particle").SetMatrix4("projection", projection);
    // set render-specific controls
    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
    // load textures
    ResourceManager::LoadTexture("textures/hitbox.png", true, "hitbox");
    ResourceManager::LoadTexture("textures/stalin.png", true, "stalin");
    ResourceManager::LoadTexture("textures/levels/Background4LUNGO.png", true, "level1Grass");
    ResourceManager::LoadTexture("textures/levels/Level2.png", true, "level2Rock");
    ResourceManager::LoadTexture("textures/trozky.png", true, "trozky");
    ResourceManager::LoadTexture("textures/lenin.png", true, "lenin");
    ResourceManager::LoadTexture("textures/particlePietra.png", true, "particle");
    ResourceManager::LoadTexture("textures/arrow_sprite.png", true, "arrow");
    ResourceManager::LoadTexture("textures/pietra.png", true, "rock");



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
    Bullet b1(20.0f, 40, ResourceManager::GetTexture("arrow"), glm::vec2(300.0f, 0.0f), glm::vec2(15.0f, 87.0f), glm::vec3(1.0f), glm::vec2(0.8f), HitboxType(SQUARE), (int)'a');
    Bullet b2(60.0f, 30, ResourceManager::GetTexture("rock"), glm::vec2(100.0f, 0.0f), glm::vec2(150.0f, 150.0f), glm::vec3(1.0f), glm::vec2(0.5f), HitboxType(CIRCLE), (int)'b');
    // per ogni bullet istanzio gli effetti particellari associati
    //b1.particles.push_back(std::make_shared<ContinousParticleGenerator>(ContinousParticleGenerator(ResourceManager::GetShader("particle"), ResourceManager::GetTexture("stalin"), b1.ParticlesNumber, ParticleType(CONTINOUS))));
    //b2.particles.push_back(std::make_shared<ContinousParticleGenerator>(ContinousParticleGenerator(ResourceManager::GetShader("particle"), ResourceManager::GetTexture("stalin"), b1.ParticlesNumber, ParticleType(CONTINOUS))));

    b1.particles.push_back(std::make_shared<HitParticleGenerator>(HitParticleGenerator(ResourceManager::GetShader("particle"), ResourceManager::GetTexture("particle"), b1.ParticlesNumber, ParticleType(HIT), glm::vec2())));
    b2.particles.push_back(std::make_shared<HitParticleGenerator>(HitParticleGenerator(ResourceManager::GetShader("particle"), ResourceManager::GetTexture("particle"), b1.ParticlesNumber, ParticleType(HIT))));


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

    //Initialization of the text renderer
    Text = new TextRenderer(this->Width, this->Height);
    Text->Load("fonts/aAbsoluteEmpire.ttf", FONT);

    //Initialization of the flat renderer
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
        if (b.destroyed == false) {
            switch (b.hitboxT) {
            case SQUARE:
            {
                std::shared_ptr<Square> s = std::dynamic_pointer_cast<Square>(b.hitbox);
                if (verifyDragonCollisionSquare(*s)) {
                    // il bullet i ha colpito il dragòn
                    hitDragon(&level->instancedBullets[i], i);
                }

            }
            break;
            case CIRCLE:
            {
                std::shared_ptr<Circle> c = std::dynamic_pointer_cast<Circle>(b.hitbox);
                if (verifyDragonCollisionCircle(*c)) {
                    // il bullet i ha colpito il dragòn
                    hitDragon(&level->instancedBullets[i], i);
                }

            }
            break;
            }

        }
         
    }
    
    
  
    
  

}

void Game::ProcessInput(float dt)
{  
     GameLevel* level = &this->Levels[this->Level];
     if (Game::State == GAME_ACTIVE) {
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
    if (Game::State == GAME_ACTIVE) {
        // draw background
        Renderer->DrawScrollingBackground(ResourceManager::GetTexture("level1Grass"), glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height*15), 0.0f, glm::vec3(1.0f), dt);
        //Renderer->DrawScrollingBackground(ResourceManager::GetTexture("level2Rock"), glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height * 15), 0.0f, glm::vec3(1.0f), dt);
        // draw level with a bullet in
        this->Levels[this->Level].Draw(*Renderer, dt);
        this->HUD.RenderHUD(*Renderer, *Text, *Flat, this->Levels[this->Level].player);
        
    } 
}

//FUNZIONI HITBOX -------------------------------------------------------------------------------------------
void Game::hitDragon(Bullet* b, int i) {
    // eliminare il bullet
    b->destroyed = true;

    //Se il drago viene colpito, TODO:
    //-Danno agli HP
    //-Proiettile sparisce->Fatto
    //-Effetto visivo per il fatto di essere stati colpiti:
    // -Quello sul drago è gestito dalla classe Dragòn(?)
    // -Quello sul proiettile è gestito direttamente da qua(per evitare overhead del gameLoop)->In realtà no
    // -Orchideo ci stiamo dimenticando completamente i suoni
    this->Levels[this->Level].player.dealDamage(b->Power); //Quando si verifica il deal damage faccio partire l'effetto associato al drago
    this->Levels[this->Level].DestroyBullet(i);
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
        // Calcolare le proiezioni dei rettangoli sugli assi x e y
        float left1 = std::min(hitbox1.left_up.x, std::min(hitbox1.left_down.x, std::min(hitbox1.right_up.x, hitbox1.right_down.x)));
        float right1 = std::max(hitbox1.left_up.x, std::max(hitbox1.left_down.x, std::max(hitbox1.right_up.x, hitbox1.right_down.x)));
        float top1 = std::min(hitbox1.left_up.y, std::min(hitbox1.left_down.y, std::min(hitbox1.right_up.y, hitbox1.right_down.y)));
        float bottom1 = std::max(hitbox1.left_up.y, std::max(hitbox1.left_down.y, std::max(hitbox1.right_up.y, hitbox1.right_down.y)));

        float left2 = std::min(hitbox2.left_up.x, std::min(hitbox2.left_down.x, std::min(hitbox2.right_up.x, hitbox2.right_down.x)));
        float right2 = std::max(hitbox2.left_up.x, std::max(hitbox2.left_down.x, std::max(hitbox2.right_up.x, hitbox2.right_down.x)));
        float top2 = std::min(hitbox2.left_up.y, std::min(hitbox2.left_down.y, std::min(hitbox2.right_up.y, hitbox2.right_down.y)));
        float bottom2 = std::max(hitbox2.left_up.y, std::max(hitbox2.left_down.y, std::max(hitbox2.right_up.y, hitbox2.right_down.y)));

        // Controllare se le proiezioni si sovrappongono sugli assi x e y
        return !(right1 < left2 || right2 < left1 || bottom1 < top2 || bottom2 < top1);
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

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


// Game-related State data
SpriteRenderer* Renderer;
// bulletTypes
static std::map<int, Bullet>            bulletTypes;

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
    // configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),
        static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    // set render-specific controls
    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
    // load textures
    ResourceManager::LoadTexture("textures/stalin.png", true, "stalin");
    ResourceManager::LoadTexture("textures/back2.png", true, "background");
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

    Bullet b1(0.0f, 0.0f, ResourceManager::GetTexture("trozky"), glm::vec2(300.0f, 0.0f), glm::vec2(70.0f, 80.0f), glm::vec3(1.0f), glm::vec2(0.5f), hitboxType(AABB), (int)'a');
    Bullet b2(0.0f, 0.0f, ResourceManager::GetTexture("lenin"), glm::vec2(100.0f, 0.0f), glm::vec2(70.0f, 80.0f), glm::vec3(1.0f), glm::vec2(0.3f), hitboxType(AABB), (int)'b');
    bulletTypes[b1.Type] = b1;
    bulletTypes[b2.Type] = b2;

    // load levels
    GameLevel test;
    test.LoadLevel();
    this->Levels.push_back(test);
    this->Level = 0;
}

void Game::Update(float dt)
{
    this->Levels[this->Level].PlayLevel();
}

void Game::ProcessInput(float dt)
{

   
     if (Game::State == GAME_ACTIVE) {
        float velocity = dt * this->Levels[this->Level].player.velocityModifier;
        glm::vec2 move;

        if (this->Keys[GLFW_KEY_A]) {
            move = glm::vec2(-velocity, 0.0f);
            this->Levels[this->Level].movePlayer(move);
        }
        if (this->Keys[GLFW_KEY_D]) {
            move = glm::vec2(velocity, 0.0f);
            this->Levels[this->Level].movePlayer(move);
        }
        if (this->Keys[GLFW_KEY_W]) {
            move = glm::vec2(0.0f, -velocity);
            this->Levels[this->Level].movePlayer(move);
        }
        if (this->Keys[GLFW_KEY_S]) {
            move = glm::vec2(0.0f, velocity);
            this->Levels[this->Level].movePlayer(move);
        }


    }
 
   
}

void Game::Render(float dt)
{


    if (Game::State == GAME_ACTIVE) {
        // draw background
        Renderer->DrawScrollingBackground(ResourceManager::GetTexture("background"), glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f, glm::vec3(1.0f), dt);
        // draw level with a bullet in
        this->Levels[this->Level].Draw(*Renderer, dt);
     
    }
    

    
}

//FUNZIONI HITBOX -------------------------------------------------------------------------------------------
float dotProduct(coord a, coord b) {
    return a.X * b.X + a.Y * b.Y;
}

bool checkCollisionSquareSquare(Square hitbox1, Square hitbox2) {
    for (int i = 0; i < 4; ++i) {
        // Calcola l'asse per la proiezione
        coord axis = { hitbox1.left_up.Y - hitbox1.left_down.Y,
                       hitbox1.left_down.X - hitbox1.left_up.X };

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

bool checkCollisionSquareCircle(Square hitboxS, Circle hitboxC) {   //hitboxS: drago //hitboxC:bullet
    float closestX = std::max(hitboxS.left_up.X, std::min(hitboxC.center.X, hitboxS.right_down.X));
    float closestY = std::max(hitboxS.left_up.Y, std::min(hitboxC.center.Y, hitboxS.right_down.Y));

    // Calcola la distanza tra il punto più vicino e il centro del cerchio
    float distanceX = hitboxC.center.X - closestX;
    float distanceY = hitboxC.center.Y - closestY;

    // Se la distanza è inferiore al raggio, c'è intersezione
    if ((distanceX * distanceX + distanceY * distanceY) < (hitboxC.radius * hitboxC.radius)) {
        return true;
    }
    return false;
}

bool checkCollisionCircleCircle(Circle hitbox1, Circle hitbox2) {
    // Calcola la distanza tra i centri dei due cerchi
    float distance = std::sqrt(std::pow(hitbox2.center.X - hitbox1.center.X, 2) +   //pow usato per calcolare il quadrato
                                std::pow(hitbox2.center.Y - hitbox1.center.Y, 2));

    if (distance <= (hitbox1.radius + hitbox2.radius)) {
        return true;  
    }
    return false;
}
//-------------------------------------------------------------------------------------------------------------------
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

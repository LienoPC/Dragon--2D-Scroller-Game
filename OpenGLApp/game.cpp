/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "game.h"
#include "resource_manager/resource_manager.h"
#include "sprite_renderer/sprite_renderer.h"
// Game-related State data
SpriteRenderer* Renderer;


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
    // load levels
    GameLevel test;
    Bullet b = Bullet::Bullet(0.0f, 0.0f, 0.0f, ResourceManager::GetTexture("stalin"), glm::vec2(300.0f, 100.0f), glm::vec2(5.0f,5.0f), glm::vec3(1.0f), glm::vec2(1.0f));
    test.AddBullet(b);
    this->Levels.push_back(test);
    this->Level = 0;
}

void Game::Update(float dt)
{

}

void Game::ProcessInput(float dt)
{
    if (Game::State == GAME_ACTIVE) {


    }
}

void Game::Render()
{
    if (Game::State == GAME_ACTIVE) {
        // draw background
        Renderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f);
        // draw level with a bullet in
        this->Levels[this->Level].Draw(*Renderer);
        // draw object
        //GameObject o(glm::vec2(300.0f, 100.0f), glm::vec2(5.0f, 5.0f), ResourceManager::GetTexture("stalin"), glm::vec3(1.0f), glm::vec2(1.0f));
        //o.Draw(*Renderer);
    }
}
#pragma once
/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef BACKGROUND_RENDERER_H
#define BACKGROUND_RENDERER_H

#include "glad/include/glad/glad.h"
#include "glm-master/glm/glm.hpp"
#include "glm-master/glm/gtc/matrix_transform.hpp"

#include "shaders_textures/texture.h"
#include "shaders_textures/shader.h"


class BackgroundRenderer
{
public:
    // Constructor (inits shaders/shapes)
    BackgroundRenderer(Shader shader);
    // Destructor
    ~BackgroundRenderer();
    // Renders a defined quad textured with given sprite
    void DrawSprite(Texture2D texture, glm::vec2 position, glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));
    // Renders the scrolling background
    void DrawScrollingBackground(Texture2D texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color);
private:
    // Render state
    Shader       shader;
    unsigned int quadVAO;
    // Initializes and configures the quad's buffer and vertex attributes
    void initRenderData();
};

#endif
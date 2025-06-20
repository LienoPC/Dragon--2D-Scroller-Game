#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include "../glad/include/glad/glad.h"
#include "../glm-master/glm/glm.hpp"
#include "../glm-master/glm/gtc/matrix_transform.hpp"

#include "../shaders_textures/texture.h"
#include "../shaders_textures/shader.h"

#define SCROLLING_SPEED 0.03
class SpriteRenderer
{
public:
    // Constructor (inits shaders/shapes)
    SpriteRenderer(Shader shader);
    // Destructor
    ~SpriteRenderer();
    // Renders a defined quad textured with given sprite
    void DrawSprite(Texture2D texture, glm::vec2 position, glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));
    void DrawScrollingBackground(Texture2D texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color, float dt);
private:
    // Render state
    Shader       shader; 
    unsigned int quadVAO;
    // Initializes and configures the quad's buffer and vertex attributes
    void initRenderData();
};

#endif
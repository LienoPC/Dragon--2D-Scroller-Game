#include "particle_generator.h"
#include "../../resource_manager/resource_manager.h"

ParticleGenerator::ParticleGenerator() {

}

ParticleGenerator::ParticleGenerator(Shader shader, Texture2D texture, unsigned int amount, ParticleType type, float size)
    : shader(shader), texture(texture), amount(amount), type(type)
{
    this->particleEnded = false;
    this->init();
}

void ParticleGenerator::init()
{
    // set up mesh and attribute properties
    unsigned int VBO;
    float particle_quad[] = {
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(this->VAO);
    // fill mesh buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
    // set mesh attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindVertexArray(0);

    // create this->amount default particle instances
    for (unsigned int i = 0; i < this->amount; ++i)
        this->particles.push_back(Particle());
}


// render all particles
void ParticleGenerator::Draw()
{
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    this->shader.Use();
    for (Particle &particle : this->particles)
    {
        if (particle.Life > 0.0f)
        {
            this->shader.SetVector2f("offset", particle.Position);
            this->shader.SetVector4f("color", particle.Color);
            // define the model matrix
            
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(particle.Position, 0.0f)); 
            model = glm::rotate(model, glm::radians(particle.Rotation), glm::vec3(0.0f, 0.0f, 1.0f)); // then rotate
            model = glm::scale(model, glm::vec3(particle.Size, 1.0f)); // last scale
            model = glm::translate(model, glm::vec3(-particle.Position, 0.0f));
            this->shader.SetMatrix4("model", model);
            this->texture.Bind();
            glBindVertexArray(this->VAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
        }
    }
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


Shader ParticleGenerator::getShader() {
    return this->shader;
}
Texture2D ParticleGenerator::getTexture() {
    return this->texture;
}

float ParticleGenerator::getSize() {
    return this->size;
}

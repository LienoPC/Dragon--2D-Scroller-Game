#ifndef PARTICLE_GENERATOR_H
#define PARTICLE_GENERATOR_H
#include "../../glad/include/glad/glad.h"
#include "../../glm-master/glm/glm.hpp"

#include "../../shaders_textures/shader.h"
#include "../../shaders_textures/texture.h"
#include "../../game_structures/game_object.h"
#include "particle.h"
#include <vector>

enum ParticleType {HIT, CONTINOUS};


class ParticleGenerator
{
public:
	ParticleGenerator();

    ~ParticleGenerator() {};

	ParticleGenerator(Shader shader, Texture2D texture, unsigned int amount, ParticleType type, float size);
    ParticleType type;

    bool particleEnded;

    virtual void  Update(float dt, GameObject& object) {

    }
    // render all particles
    void  Draw();
    void DrawSprite(SpriteRenderer& s);
    Shader getShader();
    Texture2D getTexture();

protected:
    // state
    std::vector<Particle> particles;
    unsigned int amount;
    float size;
    // render state
    Shader shader;
    Texture2D texture;
    unsigned int VAO;
 

    // offset rendering of the particle starting from the position of the object
    glm::vec2 Offset;
    // eventual respawning particles
    int NewParticles;

    
    void init();
};
#endif
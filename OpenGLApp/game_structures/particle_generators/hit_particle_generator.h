#ifndef HIT_PARTICLE_H
#define HIT_PARTICLE_H
#include "particle_generator.h"

class HitParticleGenerator : public ParticleGenerator
{
public:
    HitParticleGenerator();
    HitParticleGenerator(Shader shader, Texture2D texture, unsigned int amount, ParticleType type);

    // method implementation    
    void Update(float dt, GameObject& object);
    void HitStart(glm::vec2 position);
    void Draw();
   
private:

    void spawnParticle(Particle& particle, glm::vec2 position, float angle);

};

#endif
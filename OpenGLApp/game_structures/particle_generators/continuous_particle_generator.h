#ifndef CONTINUOUS_PARTICLE_GENERATOR_H
#define CONTINUOUS_PARTICLE_GENERATOR_H

#include "particle_generator.h"
#include "particle.h"


// ParticleGenerator acts as a container for rendering a large number of 
// particles by repeatedly spawning and updating particles and killing 
// them after a given amount of time.
class ContinousParticleGenerator : public ParticleGenerator
{
public:
   
    ContinousParticleGenerator();
    ContinousParticleGenerator(Shader shader, Texture2D texture, unsigned int amount, ParticleType type, glm::vec2 size);
    // method implementation
    void Update(float dt, GameObject& object);
    
protected:
    
    
    // returns the first Particle index that's currently unused e.g. Life <= 0.0f or 0 if no particle is currently inactive
    unsigned int firstUnusedParticle();
    // respawns particle
    void respawnParticle(Particle& particle, GameObject& object, glm::vec2 offset);
};

#endif

#include "continuous_particle_generator.h"


ContinousParticleGenerator::ContinousParticleGenerator() {

}

ContinousParticleGenerator::ContinousParticleGenerator(Shader shader, Texture2D texture, unsigned int amount, ParticleType type) {
    ParticleGenerator(shader, texture, amount, type);
}


void ContinousParticleGenerator::Update(float dt, GameObject& object)
{
    // add new particles 
    for (unsigned int i = 0; i < this->NewParticles; ++i)
    {
        int unusedParticle = this->firstUnusedParticle();
        this->respawnParticle(this->particles[unusedParticle], object, this->Offset);
    }
    // update all particles
    for (unsigned int i = 0; i < this->amount; ++i)
    {

        
        Particle& p = this->particles[i];
        p.Life -= dt; // reduce life
        if (p.Life > 0.0f)
        {	// particle is alive, thus update
            p.Position -= p.Velocity * dt;
            p.Color.a -= dt * 2.5f;
        }
    }
}



// stores the index of the last particle used (for quick access to next dead particle)
unsigned int lastUsedParticle = 0;

unsigned int ContinousParticleGenerator::firstUnusedParticle()
{
    // first search from last used particle, this will usually return almost instantly
    for (unsigned int i = lastUsedParticle; i < this->amount; ++i) {
        if (this->particles[i].Life <= 0.0f) {
            lastUsedParticle = i;
            return i;
        }
    }
    // otherwise, do a linear search
    for (unsigned int i = 0; i < lastUsedParticle; ++i) {
        if (this->particles[i].Life <= 0.0f) {
            lastUsedParticle = i;
            return i;
        }
    }
    // all particles are taken, override the first one (note that if it repeatedly hits this case, more particles should be reserved)
    lastUsedParticle = 0;
    return 0;
}

void ContinousParticleGenerator::respawnParticle(Particle& particle, GameObject& object, glm::vec2 offset)
{
    float random = ((rand() % 100) - 50) / 10.0f;
    float rColor = 0.5f + ((rand() % 100) / 100.0f);
    particle.Position = object.position + random + offset;
    particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
    particle.Life = 1.0f;
    particle.Velocity = object.Direction * 0.1f;
}
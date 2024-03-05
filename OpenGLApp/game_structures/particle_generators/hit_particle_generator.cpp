#include "hit_particle_generator.h"
#include <random>

#define PI 3.14159265
#define VEL_MODIFY 200

HitParticleGenerator::HitParticleGenerator() {


}
HitParticleGenerator::HitParticleGenerator(Shader shader, Texture2D texture, unsigned int amount, ParticleType type, float size) {

	this->shader = shader;
	this->texture = texture;
	this->amount = amount;
	this->type = type;
	this->particleEnded = false;
	this->size = size;
	this->init();
	//ParticleGenerator(shader, texture, amount, type);
	
}



void HitParticleGenerator::Update(float dt, GameObject& object) {
	//move particles and update life
	for (int i = 0; i < this->particles.size(); i++) {
		Particle& p = this->particles[i];
		p.Life -= dt; // reduce life
		if (p.Life > 0.0f)
		{	
			p.Position += p.Velocity * (dt * VEL_MODIFY);
			p.Color.a -= dt * 1.5f;
		}
		else {
			this->particleEnded = true;
			//particle stopped
			if (i != this->particles.size() - 1)
			{
				this->particles[i] = std::move(this->particles.back());
			}
			this->particles.pop_back();
		}
	}
}

void HitParticleGenerator::HitStart(glm::vec2 position){
	for (int i = 0; i < this->particles.size(); i++) {
		spawnParticle(this->particles[i], position,(float) i * 360 / this->amount);
	}
}

void HitParticleGenerator::spawnParticle(Particle& particle, glm::vec2 position, float angle) {
	// assegna i valori iniziali alle particle quando il particle_generator viene creato
	// per ogni particle della lista inizializzo la direzione sulla circonferenza intorno al punto di impatto
	// assegno quindi velocità casuale in un intervallo
	float cosa = std::cos(PI*angle/180);
	float sina = std::sin(PI*angle/180);

	float random = ((rand() % 100) - 50) / 20.0f;
	float rColor = 0.5f + ((rand() % 100) / 100.0f);
	float rotation = (rand() % 360); // random angle
	 
	particle.Rotation = rotation;
	particle.Position = position;
	particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
	particle.Life = 1.0f;
	particle.Velocity = glm::vec2(cosa, sina) * random;
	particle.Size = glm::vec2(this->size);
	
}
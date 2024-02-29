#ifndef GAMEBULLET_H
#define GAMEBULLET_H

#include "../game_structures/game_object.h"

#include "../glad/include/glad/glad.h"
#include "../glm-master/glm/glm.hpp"
#include "particle_generators/particle_generator.h"
#include <memory>


class Bullet : public GameObject {


public:

	
	float	Power;
	float	velApplied;
	std::shared_ptr<Hitbox> hitbox;
	int	ParticlesNumber; 
	std::vector<std::shared_ptr<ParticleGenerator>> particles;

	int		Type;
	Bullet();
	Bullet(float pow, int partNum, Texture2D sprite, glm::vec2 pos, glm::vec2 size, glm::vec3 color, glm::vec2 velocity, HitboxType hitboxType, int type);


	void copyBullet(Bullet b);
	void move(glm::vec2 move);
	void move(float dt);
	void syncRotation();
	void destroy();
	void dealDamage();
	void isOutside();
	void updateHitbox();
	void Draw(SpriteRenderer& renderer);
	void UpdateParticles(float dt);
	void DrawParticles(SpriteRenderer &renderer);

};

#endif

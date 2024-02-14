#ifndef GAMEBULLET_H
#define GAMEBULLET_H

#include "../game_structures/game_object.h"

#include "../glad/include/glad/glad.h"
#include "../glm-master/glm/glm.hpp"
#include <memory>

// Class that defines a bullet in the game.
// Every bullet is considered to deal damage every time it hits something
class Bullet : public GameObject {

	// bullet characteristics
public:

	glm::vec2   Direction; //Direzione di movimento
	float	Power;
	float	velApplied; // Velocity applied
	std::shared_ptr<Hitbox> hitbox;
	// pensare se aggiungere una lista di secondary effect (considerare però che devono valere solo se colpiscono il drago)
	float	ParticlesNumber; //Bho poi vediamo
	 
	int		Type;
	// constructors
	Bullet();
	Bullet(float pow, float partNum, Texture2D sprite, glm::vec2 pos, glm::vec2 size, glm::vec3 color, glm::vec2 velocity, HitboxType hitboxType, int type);



	// methods
	void copyBullet(Bullet b);
	void move(glm::vec2 move);
	void move(float dt);
	void syncRotation();
	void destroy();
	void dealDamage();
	void isOutside();
	void updateHitbox();
	void Draw(SpriteRenderer& renderer);
	
};

#endif

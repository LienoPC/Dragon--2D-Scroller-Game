#ifndef GAMEBULLET_H
#define GAMEBULLET_H




#include <iostream>
#include <sstream>
#include <fstream>

#include "../game_structures/game_object.h"

#include "../glad/include/glad/glad.h"
#include "../glm-master/glm/glm.hpp"
#include "../resource_manager/resource_manager.h"

// Class that defines a bullet in the game.
// Every bullet is considered to deal damage every time it hits something
class Bullet : public GameObject {

	// bullet characteristics
public:
	float	Power;
	// pensare se aggiungere una lista di secondary effect (considerare però che devono valere solo se colpiscono il drago)
	float	ParticlesNumber; //Bho poi vediamo
	float	VelocityMultipler; // velocità effettiva da moltiplicare alla "direzione di lancio" caricata dal livello (o gestita dalla IA)
								// e che ha un valore sempre tra 0.0f e 1.0f (sia x che y)
	int		Type;
	// constructors
	Bullet();
	Bullet(float pow, float partNum, Texture2D sprite, glm::vec2 pos, glm::vec2 size, glm::vec3 color, glm::vec2 velocity, hitboxType hitbox, int type, std::string textureFile);

	// methods
	void copyBullet(Bullet b);
	void move(glm::vec2 move);
	void destroy();
	void dealDamage();
	void isOutside();

	
};

#endif

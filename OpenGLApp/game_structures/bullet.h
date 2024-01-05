#ifndef GAMEBULLET_H
#define GAMEBULLET_H
#include "../game_structures/game_object.h"

// Class that defines a bullet in the game.
// Every bullet is considered to deal damage every time it hits something
class Bullet : private GameObject {

	// bullet characteristics
public:
	float	Power;
	// pensare se aggiungere una lista di secondary effect (considerare però che devono valere solo se colpiscono il drago)
	float	ParticlesNumber; //Bho poi vediamo
	float	Hitbox; //Bho poi vediamo

	// constructors
	Bullet();
	Bullet(float pow, float partNum, float hitb);

	// methods
	void destroy();
	void dealDamage();
};

#endif

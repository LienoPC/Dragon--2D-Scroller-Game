#ifndef DRAGONSTATS_H
#define DRAGONSTATS_H

#define MAX_HP 700
#define MAX_FP 400
#define FIREBALL_COST 60

#include "bullet.h"

enum Medal { NONE, BRONZE, SILVER, GOLD };

class DragonStats {
	
public: 
	
	float HP; //HP
	float FP; //FP (Mana)
	Medal medal;
	std::vector<Bullet> powerup;
	

	DragonStats();
};











#endif

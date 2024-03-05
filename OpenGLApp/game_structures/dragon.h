#ifndef GAMEDRAGON_H
#define GAMEDRAGON_H

#include "../game_structures/game_object.h"
#include "../game_structures/hitbox.h"
#include "../game_structures/dragon_stats.h"
#include "bullet.h"
#include "../glad/include/glad/glad.h"
#include "../glm-master/glm/glm.hpp"
#include "../irrKlang/include/irrKlang.h"
#include <vector>

#define XOFFSET 167
#define YOFFSET 40
#define FIREBALLVEL 500

// Class that defines the dragon in the game.
// The dragon is the main character controlled by the player.
class Dragon : public GameObject {	

public:
	//dragon hitboxes vector
	std::vector<Square> hitboxes;
	std::vector<Bullet> instancedFireballs;

	// dragon characteristics
	DragonStats stats;
	float velocityModifier;
	// dragon animation frames
	std::vector<Texture2D> animationFrames;
	bool playAnimation;
	bool hit;

	//num fireball shooted
	int fbnum = 0;

	//fireball sound
	irrklang::ISoundEngine* sEngine;

	// constructors
	Dragon();
	Dragon(Texture2D sprite, glm::vec2 pos, glm::vec2 size, glm::vec3 color, glm::vec2 velocity, float mvSpeed, HitboxType hitbox);

	// methods
	void move(glm::vec2 move);
	void setVelocityModifier(float newVelMod);
	void Draw(SpriteRenderer& renderer, float dt); //used instead of default GameObject::Draw method
	void dealDamage(double damage);

	void drawHitbox(SpriteRenderer& renderer); //hitbox drawing debug method

	void instanceFireball();
	void instancePowerup();
	void checkFireballs();
	//void isOutside();

};

#endif

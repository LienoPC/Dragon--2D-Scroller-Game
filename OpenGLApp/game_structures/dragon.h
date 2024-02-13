#ifndef GAMEDRAGON_H
#define GAMEDRAGON_H

#include "../game_structures/game_object.h"
#include "../game_structures/hitbox.h"
#include "../game_structures/dragon_stats.h"

#include "../glad/include/glad/glad.h"
#include "../glm-master/glm/glm.hpp"
#include <vector>

// Class that defines the dragon in the game.
// The dragon is the main character controlled by the player.
class Dragon : public GameObject {	

public:
	//dragon hitboxes vector
	std::vector<Square> hitboxes;

	// dragon characteristics
	DragonStats stats;
	float velocityModifier;
	// dragon animation frames
	std::vector<Texture2D> animationFrames;

	// constructors
	Dragon();
	Dragon(Texture2D sprite, glm::vec2 pos, glm::vec2 size, glm::vec3 color, glm::vec2 velocity, float mvSpeed, HitboxType hitbox);

	// methods
	void move(glm::vec2 move);
	void dealDamage(double damage);
	void isOutside();
	void Draw(SpriteRenderer& renderer, float dt); //used instead of default GameObject::Draw method
};

#endif

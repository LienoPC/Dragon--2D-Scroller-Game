#ifndef GAMELEVEL_H
#define GAMELEVEL_H
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "game_object.h"
#include "bullet.h"
#include "../sprite_renderer/sprite_renderer.h"
#include "../resource_manager/resource_manager.h"

class GameLevel {
	
public:
	std::vector<Bullet> bullets;
	std::vector<Bullet> bulletTypes;

	GameLevel();

	void AddBullet(Bullet& bullet);

	void Draw(SpriteRenderer& renderer);

	void MoveBullet(glm::vec2 move, int identificator);

};

#endif
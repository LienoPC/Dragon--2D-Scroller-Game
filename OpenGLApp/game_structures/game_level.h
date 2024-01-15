#ifndef GAMELEVEL_H
#define GAMELEVEL_H
#include <vector>
#include "../glad/include/glad/glad.h"
#include "../glm-master/glm/glm.hpp"

#include "game_object.h"
#include "bullet.h"
#include "dragon.h"
#include "../sprite_renderer/sprite_renderer.h"
#include "../resource_manager/resource_manager.h"


class GameLevel {
	
public:
	std::vector<int> bulletList;
	std::vector<Bullet> instancedBullets;
	std::map<int,Bullet> bulletTypes;
	Dragon player;

	GameLevel();

	void setPlayer(Dragon dragon);
	void movePlayer(glm::vec2 move);

	void AddBullet(int bullet);
	void AddBulletType(Bullet& bullet);
	void instanceBullet(int bullet);	
	// sposta la posizione di un bullet preso dalla lista di quelli istanziati
	void MoveBullet(glm::vec2 move, int identificator);

	void PlayLevel();
	void LoadLevel();
	void Draw(SpriteRenderer& renderer, float dt);

};

#endif
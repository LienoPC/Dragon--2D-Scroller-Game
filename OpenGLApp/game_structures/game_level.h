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
	std::vector<int> bulletList;
	std::vector<Bullet> instancedBullets;
	std::map<int,Bullet> bulletTypes;


	GameLevel();

	void AddBullet(int bullet);

	void AddBulletType(Bullet& bullet);

	void instanceBullet(int bullet);

	void Draw(SpriteRenderer& renderer);

	// sposta la posizione di un bullet dalla lista di quelli istanziati
	void MoveBullet(glm::vec2 move, int identificator);

	void PlayLevel();

	void LoadLevel();

};

#endif
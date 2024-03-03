#ifndef GAMELEVEL_H
#define GAMELEVEL_H

#include "../glad/include/glad/glad.h"
#include "../glm-master/glm/glm.hpp"
#include "dragon.h"
#include "bullet.h"
#include "../sprite_renderer/sprite_renderer.h"
#include "../game_structures/window.h"

#include <vector>
#include <map>


#define PHASES 3
#define SECONDS1 5
#define SECONDS2 10
#define END 120
#define VELINCREASE 150
#define STARTSECONDS 3


class GameLevel {
	
public:
	std::vector<int> bulletList;
	std::vector<Bullet> instancedBullets;
	// ogni tipo di proiettile è identificato da un valore intero associato ad un carattere ASCII
	std::vector<Bullet> powerups;

	int phase;

	// attributi del livello
	std::vector<ThrowWindow> actualWindows;
	int maxInstancedBullet;
	double minVel;
	double maxVel;
	int windowNumber;
	short numRefresh;
	Texture2D backgroundTexture;

	Dragon player;

	GameLevel();

	void setPlayer(Dragon dragon);
	void movePlayer(glm::vec2 move);
	bool isPlayerOutOfBounds(glm::vec2 pos, int height, int width);

	void DrawBackground(SpriteRenderer& Renderer, float dt, glm::vec2 resolution);

	void instanceBullet(int bullet, glm::vec2 pos, double velocity, DirectionStart directionStart);

	// esegue la logica di livello usando la lista di bullet, la phase e il Timer
	void PlayLevel(float dt, unsigned int skin, unsigned int level);
	void LoadLevel(int height, int width, const char* path);
	void startLevel(int height, int width);

	int WindowPick();

	double positionOffsetPick(int sel, int nW);

	void Draw(SpriteRenderer& renderer, float dt);

	void DestroyBullet(unsigned int i);
	
	void IncreasePhase();

	void instanceWindow(int identificator);

	glm::vec2 calculateNormalizedDirection(glm::vec2 bPosition);

	void Die();

	void SpawnPowerUps();
};

#endif
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

#define PHASES 3
#define SECONDS1 20
#define SECONDS2 50
#define SECONDS3 90

class GameLevel {
	
public:
	std::vector<int> bulletList;
	std::vector<Bullet> instancedBullets;
	 // ogni tipo di proiettile è identificato da un valore intero associato ad un carattere ASCII

	int phase;

	// dovremmo avere due file, uno che contiene il livello (con la lista di bullet da lanciare e le posizioni relative)
	// un altro file che modellizza i bullet con tutti i parametri e ne definisce l'identificatore
	Dragon player;

	GameLevel();

	void setPlayer(Dragon dragon);
	void movePlayer(glm::vec2 move);


	void instanceBullet(int bullet, glm::vec2 pos);

	void Draw(SpriteRenderer& renderer);

	void AddBullet(int bullet);
	// sposta la posizione di un bullet dalla lista di quelli istanziati
	void MoveBullet(glm::vec2 move, int identificator);

	// esegue la logica di livello usando la lista di bullet, la phase e il Timer
	void PlayLevel();
	void LoadLevel();
	void Draw(SpriteRenderer& renderer, float dt);



};

#endif
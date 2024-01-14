#ifndef GAMELEVEL_H
#define GAMELEVEL_H
#include <vector>
#include "../glad/include/glad/glad.h"
#include "../glm-master/glm/glm.hpp"

#include "game_object.h"
#include "bullet.h"
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
	std::map<int,Bullet> bulletTypes;

	int phase;

	// dovremmo avere due file, uno che contiene il livello (con la lista di bullet da lanciare e le posizioni relative)
	// un altro file che modellizza i bullet con tutti i parametri e ne definisce l'identificatore


	GameLevel();

	void AddBullet(int bullet);

	void AddBulletType(Bullet& bullet);

	void instanceBullet(int bullet);

	void Draw(SpriteRenderer& renderer);

	// sposta la posizione di un bullet dalla lista di quelli istanziati
	void MoveBullet(glm::vec2 move, int identificator);

	void PlayLevel(double seconds);

	void LoadLevel();

	// metodi per caricare i BulletTypes da un file
	static std::vector<Bullet> loadBulletTypesFromFile(const char* file);
	static void writeBulletTypesOnFile(const char* file, std::vector<Bullet> list);

};

#endif
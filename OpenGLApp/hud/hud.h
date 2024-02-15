#ifndef HUD_H
#define HUD_H

#include <stdlib.h>
#include "../sprite_renderer/sprite_renderer.h"
#include "../resource_manager/resource_manager.h"
#include "../text_renderer.h"
#include "../game_structures/dragon.h"

#define HPY 650
#define HPTextY 700
#define HPBarY 750
#define FPBarY 800
#define timeY 850
#define HUDX 200

enum Medal{BRONZE, SILVER, GOLD};

class HUD {

public:

	// posizione di ogni elemento nell'HUD
	const glm::vec2 posHPText;
	const glm::vec2 posHPBar;
	const glm::vec2 posFPBar;
	const glm::vec2 posTimeText;
	const glm::vec2 posMedal;

	// valore di ogni elemento nell'HUD
	std::string HPText;
	float HPbarLenght;
	float FPbarLenght;
	std::string timeText;
	Medal actualMedal;

	void init();
	void RenderHUD(SpriteRenderer& r, TextRenderer& t, Dragon d);

private:

	void CalculateHPBar(float hp);
	void CalculateFPBar(float fp);
};

#endif // !HUD_H


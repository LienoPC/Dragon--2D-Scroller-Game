#ifndef HUD_H
#define HUD_H

#include <stdlib.h>
#include "../sprite_renderer/sprite_renderer.h"
#include "../resource_manager/resource_manager.h"
#include "../text_renderer.h"
#include "../game_structures/dragon.h"

#define HPTextY 800
#define HPBarY 820
#define HUDX 50
#define BARHEIGHT 20
#define HPBARLENGHT 500
#define FPBARLENGHT 300
#define FONT 26



class HUD {

public:

	// HUD element position
	const glm::vec2 posHPText;
	const glm::vec2 posHPBar;
	const glm::vec2 posFPBar;
	const glm::vec2 posTimeText;
	const glm::vec2 posMedal;
	const glm::vec3 HPcolor;
	const glm::vec3 FPcolor;

	// HUD element value
	std::string HPText;
	float HPbarLenght;
	float FPbarLenght;
	std::string secondTimeText;
	std::string millisTimeText;
	std::string minuteTimeText;
	Medal actualMedal;

	HUD();

	void init();
	void RenderHUD(SpriteRenderer& r, TextRenderer& t, Dragon d);

private:

	void CalculateHPBar(float hp);
	void CalculateFPBar(float fp);
};

#endif // !HUD_H


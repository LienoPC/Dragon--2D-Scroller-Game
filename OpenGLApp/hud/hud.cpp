#include "hud.h"
#define ADVANCEX 80

void HUD::init() {

	ResourceManager::LoadTexture("textures/lifeFrame.png", 1, "HUDlifeFrame");

}

void HUD::RenderHUD(SpriteRenderer& r, TextRenderer& t, Dragon d) {

	this->HPText = std::to_string(d.stats.HP);
	CalculateHPBar(d.stats.HP);
	CalculateFPBar(d.stats.FP);

	t.RenderText(this->HPText, HUDX, HPTextY, 20, glm::vec3(1.0f)); //HP attuali
	t.RenderText(std::to_string(MAX_HP), HUDX + ADVANCEX, HPTextY, 20, glm::vec3(1.0f)); //HP massimi in testo
	r.DrawSprite(ResourceManager::GetTexture("HUDlifeFrame"), this->posHPBar, glm::vec2(1.0f), 0.0f, glm::vec3(1.0));
	




}
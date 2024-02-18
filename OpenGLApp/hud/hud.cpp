#include "hud.h"
#define ADVANCEX 80

HUD::HUD():
	posHPBar(glm::vec2(HUDX, HPBarY)), posFPBar(glm::vec2(HUDX, FPBarY)), posHPText(glm::vec2(HUDX, HPTextY)), posMedal(glm::vec2(medalX, timeY)), posTimeText(glm::vec2(HUDX, timeY)), HPcolor(glm::vec3(0.0f, 1.0f, 1.0f)), FPcolor(glm::vec3(0.0f,0.0f,1.0f))
{}

void HUD::init() {

	ResourceManager::LoadTexture("textures/LifeBar.png", 1, "HUDlifeFrame");
	ResourceManager::LoadTexture("textures/LifeBarFill.png", 1, "HUDlifeFill");
	ResourceManager::LoadTexture("textures/FPBarFill.png", 1, "HUDFPFill");
}

void HUD::RenderHUD(SpriteRenderer& r, TextRenderer& t, FlatRenderer& f, Dragon d) {

	this->HPText = "HP: " + std::to_string((int)d.stats.HP);
	CalculateHPBar(d.stats.HP);
	CalculateFPBar(d.stats.FP);

	std::string s = "/" + std::to_string(MAX_HP);

	// Render del valore attuale della vita
	t.RenderText(this->HPText, this->posHPText.x, this->posHPText.y, 1.0f, glm::vec3(1.0f)); //HP attuali
	t.RenderText(s, this->posHPText.x + ADVANCEX, this->posHPText.y, 1.0f, glm::vec3(1.0f)); //HP massimi in testo
	// Render della barra della vita
	r.DrawSprite(ResourceManager::GetTexture("HUDlifeFrame"), this->posHPBar, glm::vec2(HPBARLENGHT, BARHEIGHT), 0.0f, glm::vec3(1.0));
	r.DrawSprite(ResourceManager::GetTexture("HUDlifeFill"), this->posHPBar, glm::vec2(this->HPbarLenght, BARHEIGHT), 0.0f, this->HPcolor);
	//f.DrawFlatRectangle(glm::vec2(this->posHPBar.x, this->posHPBar.y), glm::vec2(this->HPbarLenght, BARHEIGHT), 0.0f, this->HPcolor);

	// Render della barra del mana
	r.DrawSprite(ResourceManager::GetTexture("HUDlifeFrame"), this->posFPBar, glm::vec2(FPBARLENGHT, BARHEIGHT), 0.0f, glm::vec3(1.0));
	r.DrawSprite(ResourceManager::GetTexture("HUDFPFill"), this->posFPBar, glm::vec2(this->FPbarLenght, BARHEIGHT), 0.0f, this->FPcolor);




}

void HUD::CalculateHPBar(float HP) {
	this->HPbarLenght = HP * HPBARLENGHT / 700;
}

void HUD::CalculateFPBar(float FP) {
	this->FPbarLenght = FP * FPBARLENGHT / 400;
}
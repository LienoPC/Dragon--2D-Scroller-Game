#include "hud.h"
#include "../timer/timerMy.h"
#define ADVANCEX 80
#define MEDALADVANCE 40
#define SCALE 1.2

HUD::HUD():
	posHPBar(glm::vec2(HUDX, HPBarY+((SCALE-1)*(HPBarY-HPTextY)))), posFPBar(glm::vec2(HUDX,HPBarY + BARHEIGHT*SCALE + (SCALE * 8))), posHPText(glm::vec2(HUDX, HPTextY)), posMedal(glm::vec2(HUDX, posFPBar.y+BARHEIGHT+MEDALADVANCE*SCALE/4)), posTimeText(glm::vec2(HUDX+SCALE*MEDALADVANCE+SCALE*4, posFPBar.y+BARHEIGHT+MEDALADVANCE*SCALE/4)), HPcolor(glm::vec3(0.0f, 1.0f, 1.0f)), FPcolor(glm::vec3(0.0f,0.0f,1.0f))
{}

void HUD::init() {

	ResourceManager::LoadTexture("textures/LifeBar.png", 1, "HUDlifeFrame");
	ResourceManager::LoadTexture("textures/LifeBarFill.png", 1, "HUDlifeFill");
	ResourceManager::LoadTexture("textures/FPBarFill.png", 1, "HUDFPFill");
	ResourceManager::LoadTexture("textures/GoldMedal.png", 1, "GoldMedal");
	ResourceManager::LoadTexture("textures/BronzeMedal.png", 1, "BronzeMedal");
	ResourceManager::LoadTexture("textures/SilverMedal.png", 1, "SilverMedal");
}

void HUD::RenderHUD(SpriteRenderer& r, TextRenderer& t, Dragon d) {

	this->HPText = "HP: " + std::to_string((int)d.stats.HP);
	CalculateHPBar(d.stats.HP);
	CalculateFPBar(d.stats.FP);

	std::string s = this->HPText + "/" + std::to_string(MAX_HP);

	// Render life value
	t.RenderText(s, this->posHPText.x, this->posHPText.y, 1.0f, glm::vec3(1.0f)); //HP attuali
	// Render life bar
	r.DrawSprite(ResourceManager::GetTexture("HUDlifeFrame"), this->posHPBar, glm::vec2(HPBARLENGHT*SCALE, BARHEIGHT*SCALE), 0.0f, glm::vec3(1.0));
	r.DrawSprite(ResourceManager::GetTexture("HUDlifeFill"), this->posHPBar, glm::vec2(this->HPbarLenght, BARHEIGHT*SCALE), 0.0f, this->HPcolor);
	//f.DrawFlatRectangle(glm::vec2(this->posHPBar.x, this->posHPBar.y), glm::vec2(this->HPbarLenght, BARHEIGHT), 0.0f, this->HPcolor);

	// Render mana bar
	r.DrawSprite(ResourceManager::GetTexture("HUDlifeFrame"), this->posFPBar, glm::vec2(FPBARLENGHT*SCALE, BARHEIGHT*SCALE), 0.0f, glm::vec3(1.0));
	r.DrawSprite(ResourceManager::GetTexture("HUDFPFill"), this->posFPBar, glm::vec2(this->FPbarLenght, BARHEIGHT*SCALE), 0.0f, this->FPcolor);

	// Render time
	float integer;
	// Text dei millise
	float fraction = std::modf(Timer::getElapsedSeconds(), &integer) * 100;
	this->millisTimeText = ":" + std::to_string((int)fraction);
	// check time over threshold
	if (integer >= 60) {
		if ((int)integer / 60 < 10) {
			this->minuteTimeText = "TIME: 0" + std::to_string((int)integer / 60);

		}
		else {
			this->minuteTimeText = "TIME: " + std::to_string((int)integer / 60);
		}
		if (((int)integer % 60) < 10) {
			this->secondTimeText = "0" + std::to_string((int)integer % 60);

		}
		else {
			this->secondTimeText = std::to_string((int)integer%60);
		}
		t.RenderText(this->minuteTimeText + ":" + this->secondTimeText + this->millisTimeText, this->posTimeText.x, this->posTimeText.y, 1.0f, glm::vec3(1.0f));

	}
	else {
		if (integer < 10) {
			this->secondTimeText = "TIME: 0" + std::to_string((int)integer);

		}
		else {
			this->secondTimeText = "TIME: " + std::to_string((int)integer);
		}
		t.RenderText(this->secondTimeText + this->millisTimeText, this->posTimeText.x, this->posTimeText.y, 1.0f, glm::vec3(1.0f));

	}
	
	// Render medal
	Texture2D medal;
	bool meda = false;
	this->actualMedal = d.stats.medal;
	switch (this->actualMedal) {

	case BRONZE:
		medal = ResourceManager::GetTexture("BronzeMedal");
		meda = true;
		break;
	case SILVER:
		medal = ResourceManager::GetTexture("SilverMedal");
		meda = true;
		break;

	case GOLD:
		medal = ResourceManager::GetTexture("GoldMedal");
		meda = true;
		break;
	}
	if(meda)
		r.DrawSprite(medal, this->posMedal, glm::vec2(MEDALADVANCE*SCALE), 0.0f);

	

}

void HUD::CalculateHPBar(float HP) {
	if (HP >= 0 && HP <= MAX_HP)
		this->HPbarLenght = HP * HPBARLENGHT*SCALE / 700;
	else
		this->HPbarLenght = 0;
}

void HUD::CalculateFPBar(float FP) {
	if (FP >= 0 && FP <= MAX_FP)
		this->FPbarLenght = FP * FPBARLENGHT*SCALE / 400;
	else
		this->FPbarLenght = 0;
}

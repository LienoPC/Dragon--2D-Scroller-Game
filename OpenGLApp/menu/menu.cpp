#include "menu.h"
#include "../game.h"
#include "../game_structures/level_save.h"

Menu::Menu() 
	: id(-1) {}
Menu::Menu(int id, Texture2D background)
	: id(id), background(background) {};

void Menu::addButton(Button b) {
	this->buttons.push_back(b);
}

void Menu::drawMenu(SpriteRenderer& renderer) {
	// Render background
	renderer.DrawSprite(this->background, { 0.0f, 0.0f }, { SCREEN_WIDTH, SCREEN_HEIGHT }, 0.0f, glm::vec3(1.0f));

	// Render medals
	for (int i = 0; i < 3; i++) {
		if(this->id == 2)
			switch (static_cast<int>(Level_save::theme0[i])) {
				case 2: 
					renderer.DrawSprite(ResourceManager::GetTexture("BronzeMedal"), { 244.0f + 363 * i, 603.0f }, { 60.0f, 60.0f }, 0.0f, glm::vec3(1.0f));
					break;
				case 3:
					renderer.DrawSprite(ResourceManager::GetTexture("SilverMedal"), { 244.0f + 363 * i, 603.0f }, { 60.0f, 60.0f }, 0.0f, glm::vec3(1.0f));
					break;
				case 4:
					renderer.DrawSprite(ResourceManager::GetTexture("GoldMedal"), { 244.0f + 363 * i, 603.0f }, { 60.0f, 60.0f }, 0.0f, glm::vec3(1.0f));
					break;
			}
		else if (this->id == 3) {
			switch (static_cast<int>(Level_save::theme1[i])) {
			case 2:
				renderer.DrawSprite(ResourceManager::GetTexture("BronzeMedal"), { 244.0f + 363 * i, 603.0f }, { 60.0f, 60.0f }, 0.0f, glm::vec3(1.0f));
				break;
			case 3:
				renderer.DrawSprite(ResourceManager::GetTexture("SilverMedal"), { 244.0f + 363 * i, 603.0f }, { 60.0f, 60.0f }, 0.0f, glm::vec3(1.0f));
				break;
			case 4:
				renderer.DrawSprite(ResourceManager::GetTexture("GoldMedal"), { 244.0f + 363 * i, 603.0f }, { 60.0f, 60.0f }, 0.0f, glm::vec3(1.0f));
				break;
			}
		}
	}
	// Render buttons
	for (Button b : this->buttons) {
		b.drawButton(renderer);
	}
}

void Menu::updateBackground(int theme, int level) {
	if (theme == 0) {
		if (level < 2 && Level_save::theme0[level + 1] == 1) {
			this->background = ResourceManager::GetTexture("Forest" + std::to_string(level + 2));
			this->addButton(Button({ 164.0f + 362 * (level + 1), 760.0f }, { 200.0f, 72.0f }, buttonType::play, ResourceManager::GetTexture("startButton"), ResourceManager::GetTexture("startButton"), 0, (level + 1)));
		}
	}
	else if (theme == 1) {
		if (level < 2 && Level_save::theme1[level + 1] == 1) {
			this->background = ResourceManager::GetTexture("Mountain" + std::to_string(level + 2));
			this->addButton(Button({ 164.0f + 362 * (level + 1), 760.0f }, { 200.0f, 72.0f }, buttonType::play, ResourceManager::GetTexture("startButton"), ResourceManager::GetTexture("startButton"), 0, (level + 1)));
		}
	}
}
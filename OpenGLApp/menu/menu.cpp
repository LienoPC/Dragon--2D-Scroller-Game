#include "menu.h"
#include "../game.h"

Menu::Menu() 
	: id(-1) {}
Menu::Menu(int id, Texture2D background)
	: id(id), background(background) {};

void Menu::addButton(Button b) {
	this->buttons.push_back(b);
}

void Menu::drawMenu(SpriteRenderer& renderer, Button *mouseOver) {
	Button b;
	//Render the background
	renderer.DrawSprite(this->background, { 0.0f, 0.0f }, { SCREEN_WIDTH, SCREEN_HEIGHT }, 0.0f, glm::vec3(1.0f));
	//Render all the buttons
	for (Button b : this->buttons) {
		if(mouseOver != NULL && b == *mouseOver)
			b.drawButton(renderer, true);
		else
			b.drawButton(renderer, false);
	}
}
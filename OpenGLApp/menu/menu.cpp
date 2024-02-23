#include "menu.h"

//Menu::Menu() {};
Menu::Menu(Texture2D background)
	:background(background) {};

void Menu::addButton(Button b, Menu* m) {
	this->buttons.insert(std::make_pair(b, m));
}

void Menu::drawMenu(SpriteRenderer& renderer) {
	//Render the background
	renderer.DrawSprite(this->background, { 0.0f, 0.0f }, { SCREEN_WIDTH, SCREEN_HEIGHT }, 0.0f, { 0.0f, 0.0f, 0.0f });
	//Render all the buttons
	/*for (std::map<Button, Menu*>::iterator i = this->buttons.begin(); i != this->buttons.end(); ++i)
		b.drawButton;*/
}
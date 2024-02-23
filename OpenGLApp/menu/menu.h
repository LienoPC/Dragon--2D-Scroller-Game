#ifndef GAMEMENU_H
#define GAMEMENU_H

#include "../glad/include/glad/glad.h"
#include "../glm-master/glm/glm.hpp"
#include "../shaders_textures/texture.h"
#include "../sprite_renderer/sprite_renderer.h"
#include "../game.h"
#include "button.h"
#include <map>

class Menu {
public:
	std::map<Button, Menu*> buttons;
	Texture2D background;

	// constructors
	Menu();
	Menu(Texture2D background);

	// methods
	void addButton(Button b, Menu* m);
	void drawMenu(SpriteRenderer& renderer);
};

#endif

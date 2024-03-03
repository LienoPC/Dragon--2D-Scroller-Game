#ifndef GAMEMENU_H
#define GAMEMENU_H

#include <vector>

#include "../glad/include/glad/glad.h"
#include "../glm-master/glm/glm.hpp"
#include "../shaders_textures/texture.h"
#include "../sprite_renderer/sprite_renderer.h"
#include "button.h"

class Menu {
public:
	int id;
	std::vector<Button> buttons;
	Texture2D background;

	// constructors
	Menu();
	Menu(int id, Texture2D background);

	// methods
	void addButton(Button b);
	void drawMenu(SpriteRenderer& renderer);
	void updateBackground(int theme, int level);
};
#endif

#ifndef GAMEBUTTON_H
#define GAMEBUTTON_H

#include "../glad/include/glad/glad.h"
#include "../glm-master/glm/glm.hpp"
#include "../shaders_textures/texture.h"
#include "../sprite_renderer/sprite_renderer.h"
#include <vector>

typedef enum buttonType {link, play};

class Button {
public:
	glm::vec2 position;
	glm::vec2 size;
	buttonType type;
	int subMenuId;
	Texture2D spriteBase;
	Texture2D spriteSelected;

	// constructors
	Button();
	Button(glm::vec2 position, glm::vec2 size, buttonType type, Texture2D spriteBase, Texture2D spriteSelected);
	Button(glm::vec2 position, glm::vec2 size, buttonType type, int subMenuId, Texture2D spriteBase, Texture2D spriteSelected);

	void drawButton(SpriteRenderer& renderer, bool selected);
	bool operator==(const Button& other) const;
	//bool operator<(const Button& other) const;
};
#endif

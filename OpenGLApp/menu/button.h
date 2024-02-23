#ifndef GAMEBUTTON_H
#define GAMEBUTTON_H

#include "../glad/include/glad/glad.h"
#include "../glm-master/glm/glm.hpp"
#include "../shaders_textures/texture.h"
#include "../sprite_renderer/sprite_renderer.h"
#include <vector>
enum buttonType {link, play};

class Button {
public:
	glm::vec2 position;
	glm::vec2 size;
	buttonType type;
	Texture2D spriteBase;
	Texture2D spriteSelected;

	// constructors
	Button();
	Button(glm::vec2 position, glm::vec2 size, buttonType type, Texture2D spriteBase, Texture2D spriteSelected);

	bool operator<(const Button& other) const;
};
#endif

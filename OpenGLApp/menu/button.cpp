#include "button.h"

Button::Button() {};
Button::Button(glm::vec2 position, glm::vec2 size, buttonType type, Texture2D spriteBase, Texture2D spriteSelected)
	:position(position), size(size), type(type), spriteBase(spriteBase), spriteSelected(spriteSelected) {};

bool Button::operator<(const Button& other) const {
	return (this->position.y < other.position.y);
}
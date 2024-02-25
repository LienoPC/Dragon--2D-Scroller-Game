#include "button.h"

Button::Button()
	:selected(false) {}
Button::Button(glm::vec2 position, glm::vec2 size, buttonType type, Texture2D spriteBase, Texture2D spriteSelected)
	:position(position), size(size), type(type), subMenuId(-1), selected(false), spriteBase(spriteBase), spriteSelected(spriteSelected) {}
Button::Button(glm::vec2 position, glm::vec2 size, buttonType type, int subMenuId, Texture2D spriteBase, Texture2D spriteSelected)
	:position(position), size(size), type(type), subMenuId(subMenuId), selected(false), spriteBase(spriteBase), spriteSelected(spriteSelected) {}

void Button::drawButton(SpriteRenderer& renderer) {
	if (this->selected)
		renderer.DrawSprite(this->spriteSelected, this->position, this->size*1.05f, 0.0f, glm::vec3(1.0f));
	else
		renderer.DrawSprite(this->spriteBase, this->position, this->size, 0.0f, glm::vec3(1.0f));
}

bool Button::operator==(const Button& other) const {
	return (this->position.y == other.position.y && this->position.x == this->position.x);
}

//bool Button::operator<(const Button& other) const {
//	return (this->position.y < other.position.y);
//}
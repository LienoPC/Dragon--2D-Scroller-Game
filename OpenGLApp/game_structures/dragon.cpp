#include "dragon.h"

Dragon::Dragon()
	:movementSpeed(0.0f) {};

Dragon::Dragon(Texture2D sprite, glm::vec2 pos, glm::vec2 size, glm::vec3 color, glm::vec2 velocity, float mvSpeed, hitboxType hitbox)
: movementSpeed(mvSpeed) {
	this->Position = pos;
	this->Size = size;
	this->Velocity = velocity;
	this->Color = color;
	this->Sprite = sprite;
	this->Hitbox = hitbox;
	this->Sprite = ResourceManager::GetTexture("dragon");
	// TODO: Setta Hitboxes
}

void Dragon::move(glm::vec2 move) {
	// TODO: Check bordi schermo
	this->Position += move;
}




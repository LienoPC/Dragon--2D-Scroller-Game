#include "bullet.h"

Bullet::Bullet()
	: Power(0), ParticlesNumber(0), Hitbox(0) {}

Bullet::Bullet(float pow, float partNum, float hitb, Texture2D sprite, glm::vec2 pos, glm::vec2 size, glm::vec3 color, glm::vec2 velocity){
	this->Position = pos;
	this->Size = size;
	this->Velocity = velocity;
	this->Color = color;
	this->Sprite = sprite;
	this->Power = pow;
	this->ParticlesNumber = partNum;
	this->Hitbox = hitb;
}

void Bullet::move(glm::vec2 move) {
	this->Position += move;
}


#include "bullet.h"

Bullet::Bullet()
	: Power(0), ParticlesNumber(0), Hitbox(0) {}

Bullet::Bullet(float pow, float partNum, float hitb, Texture2D sprite, glm::vec2 pos, glm::vec2 size, glm::vec3 color, glm::vec2 velocity)
	: Power(pow), ParticlesNumber(partNum), Hitbox(hitb) {
	GameObject::GameObject(pos, size, sprite ,color, velocity);
}

void Bullet::move(float dt) {
	this->Position += dt * this->Velocity;
}
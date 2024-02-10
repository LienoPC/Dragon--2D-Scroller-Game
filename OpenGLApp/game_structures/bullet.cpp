#include "bullet.h"
#include "../glfw-3.3.8.bin.WIN64/include/GLFW/glfw3.h"

Bullet::Bullet()
	: Power(0), ParticlesNumber(0) {}

Bullet::Bullet(float pow, float partNum, Texture2D sprite, glm::vec2 pos, glm::vec2 size, glm::vec3 color, glm::vec2 velocity, HitboxType hitboxT, int type){
	this->position = pos;
	this->size = size;
	this->velocity = velocity;
	this->color = color;
	this->sprite = sprite;
	this->Power = pow;
	this->ParticlesNumber = partNum;
	this->hitboxT = hitboxT;
	this->destroyed = false;
	Square s;
	Circle c;
	// Hitbox creation
	switch (hitboxT) {

	case SQUARE:
		s = Square(this->position, glm::vec2(this->position.x, this->position.y + this->size.y), glm::vec2(this->position.x + this->size.x, this->position.y), this->position + this->size);
		this->hitbox = &s;
		break;

	case CIRCLE:
		c = Circle((float)std::max(this->size.x, this->size.y), this->position + (glm::vec2(this->size.x / 2, this->size.y / 2)));
		this->hitbox = &c;
		break;
	}

	
	this->Type = type;
	
}

/*
   Sembrerebbe non servire

   
   void Bullet::copyBullet(Bullet b) {
	this->Position = b.Position;
	this->Size = b.Size;
	this->Velocity = b.Velocity;
	this->Color = b.Color;
	this->Sprite = b.Sprite;
	this->Power = b.Power;
	this->ParticlesNumber = b.ParticlesNumber;
	this->Hitbox = b.Hitbox;
}
*/


void Bullet::move(glm::vec2 move) {
	this->position += move*this->velocity*this->velApplied;
}

void Bullet::move(float dt) {
	this->position += this->velocity * this->Direction*this->velApplied *dt;
	this->position.y += SCROLLING_SPEED*dt; // Sync the scrolling level with bullets
}

void Bullet::syncRotation() {

	float cosA = this->Direction.x;
	float degrees = std::acos(cosA) * (180.0 / 3.141592653589793238463);
	this->rotation = degrees+90;
}

void Bullet::destroy() {

	this->destroyed = true;
}



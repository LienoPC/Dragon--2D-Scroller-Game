#include "bullet.h"
#include "../glfw-3.3.8.bin.WIN64/include/GLFW/glfw3.h"

Bullet::Bullet()
	: Power(0), ParticlesNumber(0) {}

Bullet::Bullet(float pow, float partNum, Texture2D sprite, glm::vec2 pos, glm::vec2 size, glm::vec3 color, glm::vec2 velocity, hitboxType hitbox, int type){
	this->Position = pos;
	this->Size = size;
	this->Velocity = velocity;
	this->Color = color;
	this->Sprite = sprite;
	this->Power = pow;
	this->ParticlesNumber = partNum;
	this->Hitbox = hitbox;
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
	this->Position += move*this->Velocity*this->velApplied;
}

void Bullet::move(float dt) {
	this->Position += this->Velocity * this->Direction*this->velApplied *dt;
	this->Position.y += SCROLLING_SPEED*dt; // Sync the scrolling level with bullets
}

void Bullet::syncRotation() {

	float cosA = this->Direction.x;
	float degrees = std::acos(cosA) * (180.0 / 3.141592653589793238463);
	this->Rotation = degrees+90;
}



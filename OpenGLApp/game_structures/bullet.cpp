#include "bullet.h"

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
	this->Position += move*this->Velocity;
}




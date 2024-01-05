#include "bullet.h"

Bullet::Bullet()
	: Power(0), ParticlesNumber(0), Hitbox(0) {}

Bullet::Bullet(float pow, float partNum, float hitb)
	: Power(pow), ParticlesNumber(partNum), Hitbox(hitb) {}

#include "../game_structures/hitbox.h"

Hitbox::Hitbox() {};
Hitbox::Hitbox(HitboxType type): 
	type(type) {};

Circle::Circle() {};
Circle::Circle(float radius, coord center): 
	Hitbox(CIRCLE), radius(radius), center(center) {};

Square::Square() {};
Square::Square(coord left_up, coord left_down, coord right_up, coord right_down) :
	Hitbox(SQUARE), left_up(left_up), left_down(left_down), right_up(right_up), right_down(right_down) {};
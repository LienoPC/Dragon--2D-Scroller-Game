#include "../game_structures/hitbox.h"

Hitbox::Hitbox() {};
Hitbox::Hitbox(HitboxType type): 
	type(type) {};

Circle::Circle() {};
Circle::Circle(float radius, glm::vec2 center):
	Hitbox(CIRCLE), radius(radius), center(center) {};


Square::Square() {};
Square::Square(glm::vec2 left_up, glm::vec2 left_down, glm::vec2 right_up, glm::vec2 right_down) :
	Hitbox(SQUARE), left_up(left_up), left_down(left_down), right_up(right_up), right_down(right_down) {};
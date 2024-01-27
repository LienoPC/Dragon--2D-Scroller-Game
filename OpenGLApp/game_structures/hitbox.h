#ifndef HITBOX_H
#define HITBOX_H

#include "../glm-master/glm/glm.hpp"

class Hitbox {
public:
    enum HitboxType { CIRCLE, SQUARE };
    HitboxType type;

    Hitbox();
    Hitbox(HitboxType type); //costruttore
};

class Circle : public Hitbox {
public:
    float radius;
    glm::vec2 center;

    Circle();
    Circle(float radius, glm::vec2 center);
};

class Square : public Hitbox {
public:
    glm::vec2 left_up;
    glm::vec2 left_down;
    glm::vec2 right_up;
    glm::vec2 right_down;

    Square();
    Square(glm::vec2 left_up, glm::vec2 left_down, glm::vec2 right_up, glm::vec2 right_down);
};

/*bool checkCollisionSquareSquare(Square hitbox1, Square hitbox2) {   //hitbox1: drago //hitbox2:bullet
    if (hitbox2.right_up.X >= hitbox1.left_up.X && hitbox2.left_up.X <= hitbox1.right_up.X) {   //controllo orizzontale
        if (hitbox2.left_down.Y <= hitbox1.left_up.Y && hitbox2.left_up.Y <= hitbox1.left_down.Y) { //controllo verticale
            return true;
        }
    }
    return false;
}*/

#endif
#ifndef HITBOX_H
#define HITBOX_H

#include "../glm-master/glm/glm.hpp"

enum HitboxType { CIRCLE, SQUARE };

class Hitbox {
public:
   
    HitboxType type;

    Hitbox();
    Hitbox(HitboxType type); //constructor
    //abstract function
    virtual void verify();
    
};

class Circle : public Hitbox {
public:
    float radius;
    glm::vec2 center;

    Circle();
    Circle(float radius, glm::vec2 center);
    void verify();
};

class Square : public Hitbox {
public:
    glm::vec2 left_up;
    glm::vec2 left_down;
    glm::vec2 right_up;
    glm::vec2 right_down;

    Square();
    Square(glm::vec2 left_up, glm::vec2 left_down, glm::vec2 right_up, glm::vec2 right_down);
    void verify();
};


#endif
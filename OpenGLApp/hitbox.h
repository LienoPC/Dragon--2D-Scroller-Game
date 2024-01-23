#ifndef HITBOX_H
#define HITBOX_H

struct coord {
    float X;
    float Y;
};

class Hitbox {
public:
    enum HitboxType { CIRCLE, SQUARE };
    HitboxType type;

    Hitbox(HitboxType type) : type(type) {} //costruttore
};

class Circle : public Hitbox {
public:
    float radius;
    coord center;
    Circle(float radius, coord center) : Hitbox(CIRCLE), radius(radius), center(center) {} 
};

class Square : public Hitbox {
public:
    coord left_up;
    coord left_down;
    coord right_up;
    coord right_down;
    Square(coord left_up, coord left_down, coord right_up, coord right_down) : Hitbox(SQUARE), left_up(left_up), left_down(left_down), right_up(right_up), right_down(right_down) {}
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
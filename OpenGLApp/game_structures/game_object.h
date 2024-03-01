#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "../glad/include/glad/glad.h"
#include "../glm-master/glm/glm.hpp"

#include "../shaders_textures/texture.h"
#include "../sprite_renderer/sprite_renderer.h"

#include "hitbox.h"


// Container object for holding all state relevant for a single
// game object entity. Each object in the game likely needs the
// minimal of state as described within GameObject.
class GameObject
{
public:
    // object state
    glm::vec2   position, size, velocity; // velocità effettiva da moltiplicare alla "direzione di lancio" caricata dal livello (o gestita dalla IA)
								            // e che ha un valore sempre tra 0.0f e 1.0f (sia x che y)
    glm::vec2   Direction; //Direzione di movimento
    glm::vec3   color;
    HitboxType  hitboxT;
    float       rotation;
    bool        isSolid; //Definisce se l'hitbox è solida
    bool        destroyed; //Indica se l'oggetto è distrutto (può essere anche esteso al giocatore)
    
    // render state
    Texture2D   sprite;

   
    // constructor(s)
    GameObject();
    ~GameObject() {}
    GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f), HitboxType Hitbox = SQUARE);
    // draw sprite
    virtual void Draw(SpriteRenderer& renderer);
    void Move(glm::vec2 move);
};

#endif
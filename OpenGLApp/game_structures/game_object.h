#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "../glad/include/glad/glad.h"
#include "../glm-master/glm/glm.hpp"

#include "../shaders_textures/texture.h"
#include "../sprite_renderer/sprite_renderer.h"

enum hitboxType{AABB, CIRCULAR};

// Container object for holding all state relevant for a single
// game object entity. Each object in the game likely needs the
// minimal of state as described within GameObject.
class GameObject
{
public:
    // object state
    glm::vec2   Position, Size, Velocity;
    glm::vec3   Color;
    hitboxType  Hitbox;
    float       Rotation;
    bool        IsSolid; //Definisce se l'hitbox è solida
    bool        Destroyed; //Indica se l'oggetto è distrutto (può essere anche esteso al giocatore)
    
    // render state
    Texture2D   Sprite;
    // constructor(s)
    GameObject();
    GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f), hitboxType Hitbox = hitboxType(AABB));
    // draw sprite
    virtual void Draw(SpriteRenderer& renderer);
};

#endif
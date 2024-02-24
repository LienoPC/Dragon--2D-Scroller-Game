
#ifndef GAME_H
#define GAME_H

// The width of the screen
const unsigned int SCREEN_WIDTH = 1280;
// The height of the screen
const unsigned int SCREEN_HEIGHT = 960;

#include <vector>
#include <map>

#include "glad/include/glad/glad.h"
#include "glfw-3.3.8.bin.WIN64/include/GLFW/glfw3.h"
#include "game_structures/game_level.h"
#include "hud/hud.h"
#include "menu/menu.h"

// Represents the current state of the game
enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

// Game holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and manageability.
class Game
{
public:
    // game state
    GameState               State;	
    bool                    Keys[1024];
    unsigned int            Width, Height;
    std::vector<GameLevel>  Levels;
    unsigned int            Level;
    std::vector<Menu>       Menus;
    unsigned int            currMenu;
    HUD                     HUD;

    

    // constructor/destructor
    Game(unsigned int width, unsigned int height);
    ~Game();
    // initialize game state (load all shaders/textures/levels)
    void Init();
    // game loop
    void ProcessInput(float dt);
    void Update(float dt);
    void Render(float dt);
    // effettua la logica di controllo delle collisioni su tutti
    // gli oggetti istanziati nella scena
    void DoCollisions();
    // verifica una generica collisione tra due gameObject
    void VerifyCollision(GameObject& one, GameObject& two);

    // prende un bulletType dalla lista di definiti
    static Bullet GetBullet(int type);


    void hitDragon(Bullet* b);

    bool verifyDragonCollisionSquare(Square h);
    bool verifyDragonCollisionCircle(Circle h);

    //funzioni controllo collisioni
    float dotProduct(glm::vec2 a, glm::vec2 b);
    bool checkCollisionSquareSquare(Square hitbox1, Square hitbox2); //virtual per funzione del padre che può essere chiamata dal figlio
    bool checkCollisionSquareCircle(Square hitboxS, Circle hitboxC);
    bool checkCollisionCircleCircle(Circle hitbox1, Circle hitbox2);
};



#endif
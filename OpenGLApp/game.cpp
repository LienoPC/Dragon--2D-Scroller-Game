#include "./game_structures/dragon.h"
#include "game.h"
#include "game_structures/bullet.h"
#include "resource_manager/resource_manager.h"
#include "sprite_renderer/sprite_renderer.h"
#include "game_structures/window_constraints.h"
#include "text_renderer.h"
#include "flat_renderer/flat_renderer.h"
#include "game_structures/particle_generators/particle_generator.h"
#include "game_structures/particle_generators/hit_particle_generator.h"
#include "game_structures/particle_generators/continuous_particle_generator.h"
#include "irrKlang/include/irrKlang.h"
#include "shaders_textures/post_processor.h"


// Game-related State data
SpriteRenderer* Renderer;
TextRenderer* Text;
FlatRenderer* Flat;
irrklang::ISoundEngine* sEngine;
PostProcessor* Effects;

float ShakeTime = 0.0f;

Game::Game(unsigned int width, unsigned int height)
    : State(GAME_MENU), Keys(), Width(width), Height(height), window(window) {
}

Game::~Game()
{
    delete Renderer;
    delete Text;
    delete Flat;
    delete sEngine;
    delete Effects;
}

void Game::Init(GLFWwindow* window)
{
    this->window = window;
    // load shaders
    ResourceManager::LoadShader("shaders/sprite.vs", "shaders/sprite.fs", nullptr, "sprite");
    ResourceManager::LoadShader("shaders/flat.vs", "shaders/flat.fs", nullptr, "flat");
    ResourceManager::LoadShader("shaders/particle.vs", "shaders/particle.fs", nullptr, "particle");
    ResourceManager::LoadShader("shaders/post_processing.vs", "shaders/post_processing.fs", nullptr, "postprocessing");
    // configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),
        static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    ResourceManager::GetShader("flat").Use().SetInteger("image", 0);
    ResourceManager::GetShader("flat").SetMatrix4("projection", projection);
    ResourceManager::GetShader("particle").Use().SetInteger("image", 0);
    ResourceManager::GetShader("particle").SetMatrix4("projection", projection);
    
    // set render-specific controls
    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
    // load textures
    ResourceManager::LoadTexture("textures/hitbox.png", true, "hitbox");
    ResourceManager::LoadTexture("textures/levels/Background4LUNGO.png", true, "level1Grass");
    ResourceManager::LoadTexture("textures/levels/Level2.png", true, "level2Rock");
    ResourceManager::LoadTexture("textures/fireball.png", true, "fireball");
    ResourceManager::LoadTexture("textures/particlePietra.png", true, "particle");
    ResourceManager::LoadTexture("textures/arrow_sprite.png", true, "arrow");
    ResourceManager::LoadTexture("textures/pietra.png", true, "rock");

    // load menu textures
    ResourceManager::LoadTexture("textures/menu/main_menu_bg2.png", true, "mainMenuBG");
    ResourceManager::LoadTexture("textures/menu/level_selection_bg1.png", true, "levelSelBG");
    ResourceManager::LoadTexture("textures/menu/button_gioca.png", true, "playButton");
    ResourceManager::LoadTexture("textures/menu/button_inizia.png", true, "startButton");
    ResourceManager::LoadTexture("textures/menu/button_back.png", true, "backButton");
    // load dragon animation frames
    ResourceManager::LoadTexture("textures/dragon_frame0.png", true, "dragon_f0");
    ResourceManager::LoadTexture("textures/dragon_frame1.png", true, "dragon_f1");
    ResourceManager::LoadTexture("textures/dragon_frame2.png", true, "dragon_f2");
    ResourceManager::LoadTexture("textures/dragon_frame3.png", true, "dragon_f3");
    ResourceManager::LoadTexture("textures/dragon_frame4.png", true, "dragon_f4");
    ResourceManager::LoadTexture("textures/dragon_frame5.png", true, "dragon_f5");
    ResourceManager::LoadTexture("textures/dragon_frame6.png", true, "dragon_f6");
    ResourceManager::LoadTexture("textures/dragon_frame7.png", true, "dragon_f7");

    // create bulletTypes

    Bullet b1(20.0f, 40, ResourceManager::GetTexture("arrow"), glm::vec2(300.0f, 0.0f), glm::vec2(15.0f, 87.0f), glm::vec3(1.0f), glm::vec2(0.8f), HitboxType(SQUARE), (int)'a');
    Bullet b2(60.0f, 25, ResourceManager::GetTexture("rock"), glm::vec2(100.0f, 0.0f), glm::vec2(150.0f, 150.0f), glm::vec3(1.0f), glm::vec2(0.5f), HitboxType(CIRCLE), (int)'b');
    //Bullet fb(50.0f, 40, ResourceManager::GetTexture("fireball"), glm::vec2(200.0f, 200.0f), glm::vec2(50.0f, 90.0f), glm::vec3(1.0f), glm::vec2(0.6f), HitboxType(CIRCLE), (int)'c');
    
    // creo i particles per ogni bullet
    b1.particles.push_back(std::make_shared<HitParticleGenerator>(HitParticleGenerator(ResourceManager::GetShader("particle"), ResourceManager::GetTexture("particle"), b1.ParticlesNumber, ParticleType(HIT), 1.0f)));
    b2.particles.push_back(std::make_shared<HitParticleGenerator>(HitParticleGenerator(ResourceManager::GetShader("particle"), ResourceManager::GetTexture("particle"), b1.ParticlesNumber, ParticleType(HIT), 3.0f)));


    ResourceManager::SetBullet(b1);
    ResourceManager::SetBullet(b2);
    //ResourceManager::SetBullet(fb);

    // create all Throwing Windows
    ThrowWindow center(glm::vec2(this->Width/2, 0.0f), glm::vec2(this->Width/8, 0.0f), 0, DirectionStart(UP));
    ThrowWindow leftAngle(glm::vec2(0.0f, 0.0f),glm::vec2(0.0f), 1, DirectionStart(UP));
    ThrowWindow rightAngle(glm::vec2(this->Width, 0.0f), glm::vec2(0.0f), 2, DirectionStart(UP));
    ThrowWindow leftLeft(glm::vec2(0.0f, this->Height / 4), glm::vec2(0.0f, this->Height / 8), 3, DirectionStart(LEFT));
    ThrowWindow rightRight(glm::vec2(this->Width, this->Height / 4), glm::vec2(0.0f, this->Height / 8), 4, DirectionStart(RIGHT));
    ThrowWindow topLeft(glm::vec2(this->Width / 4, 0.0f), glm::vec2(this->Width / 8, 0.0f), 5, DirectionStart(UP));
    ThrowWindow topRight(glm::vec2(this->Width - (this->Width / 4), 0.0f), glm::vec2(this->Width / 8, 0.0f), 6, DirectionStart(UP));
    ResourceManager::SetWindow(center);
    ResourceManager::SetWindow(leftAngle);
    ResourceManager::SetWindow(rightAngle);
    ResourceManager::SetWindow(leftLeft);
    ResourceManager::SetWindow(rightRight);
    ResourceManager::SetWindow(topLeft);
    ResourceManager::SetWindow(topRight);

    //create Menus and Buttons
    int id = 0;  
    Menu mainMenu(id++, ResourceManager::GetTexture("mainMenuBG"));
    Button button1({ 70.0f, 245.0f }, { 375.0f, 100.0f }, buttonType::link, id, ResourceManager::GetTexture("playButton"), ResourceManager::GetTexture("playButton"));

    Menu subMenu(id++, ResourceManager::GetTexture("levelSelBG"));
    Button button2({ 220.0f, 790.0f }, { 220.0f, 80.0f }, buttonType::play, ResourceManager::GetTexture("startButton"), ResourceManager::GetTexture("startButton"));
    Button button3({ 60.0f, 90.0f }, { 40.0f, 40.0f }, buttonType::link, 0, ResourceManager::GetTexture("backButton"), ResourceManager::GetTexture("backButton"));
    //Aggiungere bottoni sotto agli altri livelli (oltre Foresta)

    mainMenu.addButton(button1);
    subMenu.addButton(button2);
    subMenu.addButton(button3);
    this->Menus.push_back(mainMenu);
    this->Menus.push_back(subMenu);
    this->currMenu = 0;

    //Initialization of the text renderer
    Text = new TextRenderer(this->Width, this->Height);
    Text->Load("fonts/aAbsoluteEmpire.ttf", FONT);

    //Initialization of the flat renderer
    Flat = new FlatRenderer(ResourceManager::GetShader("flat"));

    //HUD initialization
    this->HUD.init();

    // load levels
    // Level 1
    GameLevel l1;
    l1.windowNumber = W_NUMBER_1;
    l1.LoadLevel(SCREEN_HEIGHT, SCREEN_WIDTH);
    this->Levels.push_back(l1);
    this->Level = 0;

    // load sound engine
    sEngine = irrklang::createIrrKlangDevice();
    sEngine->play2D("audio/Megalovania.wav", true);
        
    // configure postprocessing renderer
    Effects = new PostProcessor(ResourceManager::GetShader("postprocessing"), this->Width, this->Height);

}

void Game::Update(float dt){

    if (this->State == GAME_ACTIVE) {
        GameLevel* level = &this->Levels[this->Level];
        level->PlayLevel(dt); // gestisce il lancio di nuovi proiettili e la cancellazione di quelli che escono dalla scena

        //Verifica delle hitbox per ogni proiettile/palle del drago

        for (int i = 0; i < level->instancedBullets.size(); i++) {
        // Verifico per ogni proiettile istanziato se ci sono hit
        Bullet b = level->instancedBullets[i];
            switch(b.hitboxT) {
            case SQUARE: {
                std::shared_ptr<Square> s = std::dynamic_pointer_cast<Square>(b.hitbox);
                if (b.destroyed == false) {
                    for (int j = 0; j < level->player.instancedFireballs.size(); j++) {
                        Bullet fb = level->player.instancedFireballs[j];
                        if (fb.destroyed == false) {
                            std::shared_ptr<Circle> fbHitbox = std::dynamic_pointer_cast<Circle>(fb.hitbox);
                            if (verifyBulletCollisionCircleSquare(*s, *fbHitbox)) {
                                hitBullet(&level->instancedBullets[i], &level->player.instancedFireballs[j], i, j);
                            }
                        }
                    }
                }

                if (b.destroyed == false) {
                    if (verifyDragonCollisionSquare(*s)) {
                        // il bullet i ha colpito il dragòn
                        hitDragon(&level->instancedBullets[i], i);
                    }
                }
            }
            break;

            case CIRCLE:{
                std::shared_ptr<Circle> c = std::dynamic_pointer_cast<Circle>(b.hitbox);
                if (b.destroyed == false) {
                    for (int j = 0; j < level->player.instancedFireballs.size(); j++) {
                        Bullet fb = level->player.instancedFireballs[j];
                        if (fb.destroyed == false) {
                            std::shared_ptr<Circle> fbHitbox = std::dynamic_pointer_cast<Circle>(fb.hitbox);
                            if (verifyBulletCollisionCircleCircle(*c, *fbHitbox)) {
                                hitBullet(&level->instancedBullets[i], &level->player.instancedFireballs[j], i, j);
                            }
                        }
                    }
                }

                if (b.destroyed == false) {
                    if (verifyDragonCollisionCircle(*c)) {
                        // il bullet i ha colpito il dragòn
                        hitDragon(&level->instancedBullets[i], i);
                    }
                }
            }
            break;
            }
        }

        if (ShakeTime > 0.0f){
            ShakeTime -= dt;
            if (ShakeTime <= 0.0f)
                Effects->Shake = false;
        }
    }
}


void Game::ProcessInput(float dt){  
    if (Game::State == GAME_MENU) {
        static Button* cursorOver = NULL, * clicked = NULL;
        double cursorX, cursorY;
        glfwGetCursorPos(this->window, &cursorX, &cursorY);

        if (!isCursorOnButton(cursorX, cursorY, cursorOver)) {
            if (cursorOver != NULL) {
                cursorOver->selected = false;
                cursorOver = NULL;
            }
            for (Button& b : this->Menus[currMenu].buttons) {
                if (isCursorOnButton(cursorX, cursorY, &b)) {
                    cursorOver = &b;
                    b.selected = true;
                    break;
                }
            }
        }

        if (this->MouseButtons[GLFW_MOUSE_BUTTON_LEFT]) {
            if (cursorOver != NULL) {
                clicked = cursorOver;
            }
        }
        else {
            if (clicked != NULL) {
                if (cursorOver != NULL && cursorOver == clicked) {
                    if (clicked->type == buttonType::link) {
                        this->currMenu = clicked->subMenuId;
                        cursorOver = NULL;
                    }
                    else if (clicked->type == buttonType::play) {
                        this->currMenu = 0;
                        cursorOver = NULL;
                        this->State = GAME_ACTIVE;
                    }
                }
            }
            clicked = NULL;
        }
    }   

     if (Game::State == GAME_ACTIVE) {
         GameLevel* level = &this->Levels[this->Level];
         // Gestione della velocità (sprint, slowdown)
         static bool sprint = false, slowdown = false, shoot=false;

        if (this->Keys[GLFW_KEY_LEFT_SHIFT] && !sprint) {
            level->player.setVelocityModifier(650.0f);
            sprint = true;       
        }
        else if (!this->Keys[GLFW_KEY_LEFT_SHIFT] && sprint) {
             level->player.setVelocityModifier(400.0f);
             sprint = false;
        }

        if (this->Keys[GLFW_KEY_LEFT_CONTROL] && !slowdown) {
             level->player.setVelocityModifier(250.0f);
             slowdown = true;
        }
        else if (!this->Keys[GLFW_KEY_LEFT_CONTROL] && slowdown) {
             level->player.setVelocityModifier(400.0f);
             slowdown = false;
        }

         // Movimento del drago
         float velocity = dt * level->player.velocityModifier;
         glm::vec2 move, playerPos = level->player.position;

         if (this->Keys[GLFW_KEY_A]) {
             move = glm::vec2(-velocity, 0.0f);
             if (!level->isPlayerOutOfBounds(playerPos + move, SCREEN_HEIGHT, SCREEN_WIDTH))
                 level->movePlayer(move);
         }
         if (this->Keys[GLFW_KEY_D]) {
             move = glm::vec2(velocity, 0.0f);
             if (!level->isPlayerOutOfBounds(playerPos + move, SCREEN_HEIGHT, SCREEN_WIDTH))
                 level->movePlayer(move);
         }
         if (this->Keys[GLFW_KEY_W]) {
             move = glm::vec2(0.0f, -velocity);
             if (!level->isPlayerOutOfBounds(playerPos + move, SCREEN_HEIGHT, SCREEN_WIDTH))
                 level->movePlayer(move);
         }
         if (this->Keys[GLFW_KEY_S]) {
             move = glm::vec2(0.0f, velocity);
             if (!level->isPlayerOutOfBounds(playerPos + move, SCREEN_HEIGHT, SCREEN_WIDTH))
                 level->movePlayer(move);
         }
         if (this->MouseButtons[GLFW_MOUSE_BUTTON_LEFT] && !shoot) {
            level->player.instanceFireball(level->player.position, 350.0f);
            shoot = true;
        }else if (!this->MouseButtons[GLFW_MOUSE_BUTTON_LEFT] && shoot) {
            shoot = false;
        }
     }
}

void Game::Render(float dt)
{   
    if (Game::State == GAME_MENU) {
        this->Menus[this->currMenu].drawMenu(*Renderer);
    }
    else if (Game::State == GAME_ACTIVE) {
        // prepare postProcesser
        Effects->BeginRender();
        // draw background
        Renderer->DrawScrollingBackground(ResourceManager::GetTexture("level1Grass"), glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height*15), 0.0f, glm::vec3(1.0f), dt);
        //Renderer->DrawScrollingBackground(ResourceManager::GetTexture("level2Rock"), glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height * 15), 0.0f, glm::vec3(1.0f), dt);
        // draw level with a bullet in
        this->Levels[this->Level].Draw(*Renderer, dt);
        this->HUD.RenderHUD(*Renderer, *Text, *Flat, this->Levels[this->Level].player);
        // end rendering to postprocessing framebuffer
        Effects->EndRender();
        // render postprocessing quad
        Effects->Render(glfwGetTime());
    } 
}

//FUNZIONI HITBOX -------------------------------------------------------------------------------------------
void Game::hitDragon(Bullet* b, int i) {
    ShakeTime = 0.10f;
    Effects->Shake = true;
    // eliminare il bullet
    b->destroyed = true;
    
    //Se il drago viene colpito, TODO:
    //-Danno agli HP
    //-Proiettile sparisce->Fatto
    //-Effetto visivo per il fatto di essere stati colpiti:
    // -Quello sul drago � gestito dalla classe Drag�n(?)
    // -Quello sul proiettile è gestito direttamente da qua(per evitare overhead del gameLoop)->In realtà no
    // -Orchideo ci stiamo dimenticando completamente i suoni
    this->Levels[this->Level].player.dealDamage(b->Power); //Quando si verifica il deal damage faccio partire l'effetto associato al drago
    this->Levels[this->Level].DestroyBullet(i);
    //sEngine->play2D("audio/Pain.wav");
}

void Game::hitBullet(Bullet *b, Bullet* fb, int i, int j) {
    GameLevel* level = &this->Levels[this->Level];

    b->destroyed = true;
    this->Levels[this->Level].DestroyBullet(i);

    fb->destroyed = true;
    level->player.instancedFireballs[j].destroy();
}

bool Game::verifyDragonCollisionSquare(Square h) {
    for (int i = 0; i < this->Levels[this->Level].player.hitboxes.size(); i++) {
        if (checkCollisionSquareSquare(this->Levels[this->Level].player.hitboxes[i], h)) {
            return true;
        }
    }
    return false;
}

bool Game::verifyDragonCollisionCircle(Circle h) {
    for (int i = 0; i < this->Levels[this->Level].player.hitboxes.size(); i++) {
        if (checkCollisionSquareCircle(this->Levels[this->Level].player.hitboxes[i], h)) {
            return true;
        }
    }
    return false;
}

bool Game::verifyBulletCollisionCircleSquare(Square s, Circle c) {
    if (checkCollisionSquareCircle(s, c)) {
        return true;
    }
    return false;
}

bool Game::verifyBulletCollisionCircleCircle(Circle c1, Circle c2) {
    if (checkCollisionCircleCircle(c1, c2)) {
        return true;
    }
    return false;
}


float Game::dotProduct(glm::vec2 a, glm::vec2 b) {
    return a.x * b.x + a.y * b.y;
}

bool Game::checkCollisionSquareSquare(Square hitbox1, Square hitbox2) {
       // Calcolare le proiezioni dei rettangoli sugli assi x e y
       float left1 = std::min(hitbox1.left_up.x, std::min(hitbox1.left_down.x, std::min(hitbox1.right_up.x, hitbox1.right_down.x)));
       float right1 = std::max(hitbox1.left_up.x, std::max(hitbox1.left_down.x, std::max(hitbox1.right_up.x, hitbox1.right_down.x)));
       float top1 = std::min(hitbox1.left_up.y, std::min(hitbox1.left_down.y, std::min(hitbox1.right_up.y, hitbox1.right_down.y)));
       float bottom1 = std::max(hitbox1.left_up.y, std::max(hitbox1.left_down.y, std::max(hitbox1.right_up.y, hitbox1.right_down.y)));

       float left2 = std::min(hitbox2.left_up.x, std::min(hitbox2.left_down.x, std::min(hitbox2.right_up.x, hitbox2.right_down.x)));
       float right2 = std::max(hitbox2.left_up.x, std::max(hitbox2.left_down.x, std::max(hitbox2.right_up.x, hitbox2.right_down.x)));
       float top2 = std::min(hitbox2.left_up.y, std::min(hitbox2.left_down.y, std::min(hitbox2.right_up.y, hitbox2.right_down.y)));
       float bottom2 = std::max(hitbox2.left_up.y, std::max(hitbox2.left_down.y, std::max(hitbox2.right_up.y, hitbox2.right_down.y)));

       // Controllare se le proiezioni si sovrappongono sugli assi x e y
       return !(right1 < left2 || right2 < left1 || bottom1 < top2 || bottom2 < top1);
}

bool Game::checkCollisionSquareCircle(Square hitboxS, Circle hitboxC) {   //hitboxS: drago //hitboxC:bullet
    float closestX = std::max(hitboxS.left_up.x, std::min(hitboxC.center.x, hitboxS.right_down.x));
    float closestY = std::max(hitboxS.left_up.y, std::min(hitboxC.center.y, hitboxS.right_down.y));

    // Calcola la distanza tra il punto pi� vicino e il centro del cerchio
    float distanceX = hitboxC.center.x - closestX;
    float distanceY = hitboxC.center.y - closestY;

    // Se la distanza � inferiore al raggio, c'� intersezione
    if ((distanceX * distanceX + distanceY * distanceY) < (hitboxC.radius * hitboxC.radius)) {
        return true;
    }
    return false;
}

bool Game::checkCollisionCircleCircle(Circle hitbox1, Circle hitbox2) {
    // Calcola la distanza tra i centri dei due cerchi
    float distance = std::sqrt(std::pow(hitbox2.center.x - hitbox1.center.x, 2) +   //pow usato per calcolare il quadrato
        std::pow(hitbox2.center.y - hitbox1.center.y, 2));

    if (distance <= (hitbox1.radius + hitbox2.radius)) {
        return true;
    }
    return false;
}
//-------------------------------------------------------------------------------------------------------------------

bool Game::isCursorOnButton(double xpos, double ypos, Button *b) {
    if (b == NULL)
        return false;

    double cursorX, cursorY, bX, bY, bSizeX, bSizeY;
    glfwGetCursorPos(this->window, &cursorX, &cursorY);
    bX = b->position.x;
    bY = b->position.y;
    bSizeX = b->size.x;
    bSizeY = b->size.y;

    return !(b != NULL && (cursorX < bX || cursorX > bX + bSizeX || cursorY < bY || cursorY > bY + bSizeY)) ;
}

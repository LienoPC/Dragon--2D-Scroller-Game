#include "./game_structures/dragon.h"
#include "game.h"
#include "game_structures/bullet.h"
#include "resource_manager/resource_manager.h"
#include "sprite_renderer/sprite_renderer.h"
#include "game_structures/window_constraints.h"
#include "text_renderer.h"
#include "game_structures/particle_generators/particle_generator.h"
#include "game_structures/particle_generators/hit_particle_generator.h"
#include "game_structures/particle_generators/continuous_particle_generator.h"
#include "irrKlang/include/irrKlang.h"
#include "shaders_textures/post_processor.h"
#include "game_structures/level_save.h"
#include "timer/timerMy.h"

#include <iostream>

#define PI 3.14

// Game-related State data
SpriteRenderer* Renderer;
TextRenderer* Text;
irrklang::ISoundEngine* sEngine;
irrklang::ISound* hitSound;
irrklang::ISound* levelSound;
irrklang::ISound* crashingSound;
PostProcessor* Effects;

float ShakeTime = 0.0f;

Game::Game(unsigned int width, unsigned int height)
    : State(GAME_MENU), Keys(), Width(width), Height(height), window(window) {
}

Game::~Game()
{
    delete Renderer;
    delete Text;
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
    // load textures: levels
    ResourceManager::LoadTexture("textures/hitbox.png", true, "hitbox");
    ResourceManager::LoadTexture("textures/levels/Background4LUNGO.png", true, "level1Grass");
    ResourceManager::LoadTexture("textures/levels/Level2.png", true, "level2Rock");
    ResourceManager::LoadTexture("textures/levels/Skin1Lev1.png", true, "Skin1Lev1");
    ResourceManager::LoadTexture("textures/levels/Skin1Lev2.png", true, "Skin1Lev2");
    ResourceManager::LoadTexture("textures/levels/Skin1Lev3.png", true, "Skin1Lev3");
    ResourceManager::LoadTexture("textures/levels/Skin2Lev1.png", true, "Skin2Lev1");
    ResourceManager::LoadTexture("textures/levels/Skin2Lev2.png", true, "Skin2Lev2");
    ResourceManager::LoadTexture("textures/levels/Skin2Lev3.png", true, "Skin2Lev3");



    // load textures: bullets and particles
    ResourceManager::LoadTexture("textures/arrow_sprite.png", true, "arrow");
    ResourceManager::LoadTexture("textures/rock.png", true, "rock");
    ResourceManager::LoadTexture("textures/lavaRock.png", true, "lavaRock");
    ResourceManager::LoadTexture("textures/particleArrow.png", true, "particleArrow");
    ResourceManager::LoadTexture("textures/particleRock.png", true, "particleRock");
    ResourceManager::LoadTexture("textures/particleLava.png", true, "particleLava");

    // load textures: powerups
    ResourceManager::LoadTexture("textures/SmallFireBall.png", true, "fireball");
    ResourceManager::LoadTexture("textures/bigFireBall.png", true, "bigFireball");
    ResourceManager::LoadTexture("textures/a.png", true, "aPowerup");
    ResourceManager::LoadTexture("textures/b.png", true, "bPowerup");



    // load menu textures
    ResourceManager::LoadTexture("textures/menu/main_menu_bg.png", true, "mainMenuBG");
    ResourceManager::LoadTexture("textures/menu/theme_selection.png", true, "themeSelBG");
    ResourceManager::LoadTexture("textures/menu/level_selection.png", true, "levelSelBG");
    ResourceManager::LoadTexture("textures/menu/game_over.png", true, "gameOver");
    ResourceManager::LoadTexture("textures/menu/pause.png", true, "pause");
    ResourceManager::LoadTexture("textures/menu/level_complete.png", true, "levelComplete");
    ResourceManager::LoadTexture("textures/menu/main_button_gioca.png", true, "mainPlayButton");
    ResourceManager::LoadTexture("textures/menu/button_gioca.png", true, "playButton");
    ResourceManager::LoadTexture("textures/menu/button_inizia.png", true, "startButton");
    ResourceManager::LoadTexture("textures/menu/button_riprendi.png", true, "resumeButton");
    ResourceManager::LoadTexture("textures/menu/button_back.png", true, "backButton");
    ResourceManager::LoadTexture("textures/menu/button_esci.png", true, "exitButton");
    ResourceManager::LoadTexture("textures/menu/button_desktop.png", true, "desktopButton");
    ResourceManager::LoadTexture("textures/menu/button_comandi.png", true, "controlsButton");
    ResourceManager::LoadTexture("textures/menu/controls.png", true, "controlsBG");
    ResourceManager::LoadTexture("textures/menu/Forest1.png", true, "Forest1");
    ResourceManager::LoadTexture("textures/menu/Forest2.png", true, "Forest2");
    ResourceManager::LoadTexture("textures/menu/Forest3.png", true, "Forest3");
    ResourceManager::LoadTexture("textures/menu/Mountain1.png", true, "Mountain1");
    ResourceManager::LoadTexture("textures/menu/Mountain2.png", true, "Mountain2");
    ResourceManager::LoadTexture("textures/menu/Mountain3.png", true, "Mountain3");
    // load dragon animation frames
    ResourceManager::LoadTexture("textures/dragon_frame0.png", true, "dragon_f0");
    ResourceManager::LoadTexture("textures/dragon_frame1.png", true, "dragon_f1");
    ResourceManager::LoadTexture("textures/dragon_frame2.png", true, "dragon_f2");
    ResourceManager::LoadTexture("textures/dragon_frame3.png", true, "dragon_f3");
    ResourceManager::LoadTexture("textures/dragon_frame4.png", true, "dragon_f4");
    ResourceManager::LoadTexture("textures/dragon_frame5.png", true, "dragon_f5");
    ResourceManager::LoadTexture("textures/dragon_frame6.png", true, "dragon_f6");
    ResourceManager::LoadTexture("textures/dragon_frame7.png", true, "dragon_f7");

    // bullet types
    Bullet b1(15.0f, 40, ResourceManager::GetTexture("arrow"), glm::vec2(300.0f, 0.0f), glm::vec2(15.0f, 87.0f), glm::vec3(1.0f), glm::vec2(0.8f), HitboxType(SQUARE), (int)'a');
    Bullet b2(60.0f, 25, ResourceManager::GetTexture("rock"), glm::vec2(100.0f, 0.0f), glm::vec2(100.0f, 100.0f), glm::vec3(1.0f), glm::vec2(0.6f), HitboxType(CIRCLE), (int)'b');    
    Bullet b3(150.0f, 40, ResourceManager::GetTexture("lavaRock"), glm::vec2(100.0f, 0.0f), glm::vec2(130.0f, 130.0f), glm::vec3(1.0f), glm::vec2(0.5f), HitboxType(CIRCLE), (int)'c');
    b3.hitNumber = 2;
    // bullet particles
    b1.particles.push_back(std::make_shared<HitParticleGenerator>(HitParticleGenerator(ResourceManager::GetShader("particle"), ResourceManager::GetTexture("particleArrow"), b1.ParticlesNumber, ParticleType(HIT), 1.0f)));
    b2.particles.push_back(std::make_shared<HitParticleGenerator>(HitParticleGenerator(ResourceManager::GetShader("particle"), ResourceManager::GetTexture("particleRock"), b1.ParticlesNumber, ParticleType(HIT), 3.0f)));
    b3.particles.push_back(std::make_shared<HitParticleGenerator>(HitParticleGenerator(ResourceManager::GetShader("particle"), ResourceManager::GetTexture("particleLava"), b1.ParticlesNumber, ParticleType(HIT), 3.0f)));

    ResourceManager::SetBullet(b1);
    ResourceManager::SetBullet(b2);
    ResourceManager::SetBullet(b3);
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

    //Initialization of the text renderer
    Text = new TextRenderer(this->Width, this->Height);
    Text->Load("fonts/aAbsoluteEmpire.ttf", FONT);

    //HUD initialization
    this->HUD.init();

    //load levels
    GameLevel l1;
    l1.windowNumber = W_NUMBER_1;
    l1.LoadLevel(SCREEN_HEIGHT, SCREEN_WIDTH, "levels/Level1.txt");
    GameLevel l2;
    l2.windowNumber = W_NUMBER_2;
    l2.LoadLevel(SCREEN_HEIGHT, SCREEN_WIDTH, "levels/Level2.txt");
    GameLevel l3;
    l3.windowNumber = W_NUMBER_3;
    l3.LoadLevel(SCREEN_HEIGHT, SCREEN_WIDTH, "levels/Level3.txt");


    // load sound engine
    sEngine = irrklang::createIrrKlangDevice();
    //menu (starting) music
    sEngine->play2D("audio/Zelda.wav", true);

    // configure postprocessing renderer
    Effects = new PostProcessor(ResourceManager::GetShader("postprocessing"), this->Width, this->Height);

    // initialize SaveGame
    Level_save::load_state();
    this->Levels.push_back(l1);
    this->Levels.push_back(l2);
    this->Levels.push_back(l3);

    //menu and button creations
    //std::cout << Level_save::theme0[0] << " " << Level_save::theme0[1] << " " << Level_save::theme0[2] << " | " << Level_save::theme1[0] << " " << Level_save::theme1[1] << " " << Level_save::theme1[2] << std::endl;
    Menu mainMenu(0, ResourceManager::GetTexture("mainMenuBG"));
    // button "Gioca"
    mainMenu.addButton(Button({ 70.0f, 245.0f }, { 375.0f, 100.0f }, buttonType::link, 1, ResourceManager::GetTexture("mainPlayButton"), ResourceManager::GetTexture("mainPlayButton")));
    // button "Comandi"
    mainMenu.addButton(Button({ 70.0f, 365.0f }, { 375.0f, 100.0f }, buttonType::link, 7, ResourceManager::GetTexture("controlsButton"), ResourceManager::GetTexture("controlsButton")));
    // button "Desktop"
    mainMenu.addButton(Button({ 70.0f, 480.0f }, { 375.0f, 100.0f }, buttonType::close, ResourceManager::GetTexture("desktopButton"), ResourceManager::GetTexture("desktopButton")));

    Menu themeSel(1, ResourceManager::GetTexture("themeSelBG"));
    // button for "Foresta"
    themeSel.addButton(Button({ 215.0f, 780.0f }, { 220.0f, 80.0f }, buttonType::link, 2, ResourceManager::GetTexture("playButton"), ResourceManager::GetTexture("playButton")));
    // button for "Montagna"
    themeSel.addButton(Button({ 820.0f, 780.0f }, { 220.0f, 80.0f }, buttonType::link, 3, ResourceManager::GetTexture("playButton"), ResourceManager::GetTexture("playButton")));
    // button for going back
    themeSel.addButton(Button({ 60.0f, 90.0f }, { 40.0f, 40.0f }, buttonType::link, 0, ResourceManager::GetTexture("backButton"), ResourceManager::GetTexture("backButton")));

    Menu levelSelForest;    // id = 2
    Menu levelSelMountain;  // id = 3

    // finding the highest level reached before e setting up menu
    int recordForest = 2, recordMountain = 2;
    while (recordForest >= 0) {
        if (Level_save::theme0[recordForest]) {
            levelSelForest = Menu(2, ResourceManager::GetTexture("Forest" + std::to_string(recordForest+1)));
            break;
        }
        recordForest--;
    }
    for (int i = 0; i <= recordForest; i++)
        // Setup buttons "Foresta"
        levelSelForest.addButton(Button({ 164.0f + 362 * i, 760.0f }, { 200.0f, 72.0f }, buttonType::play, ResourceManager::GetTexture("startButton"), ResourceManager::GetTexture("startButton"), 0, i));
    // button "back"
    levelSelForest.addButton(Button({ 60.0f, 90.0f }, { 40.0f, 40.0f }, buttonType::link, 1, ResourceManager::GetTexture("backButton"), ResourceManager::GetTexture("backButton")));

    while (recordMountain >= 0) {
        if (Level_save::theme1[recordMountain]) {
            levelSelMountain = Menu(3, ResourceManager::GetTexture("Mountain" + std::to_string(recordMountain+1)));
            break;
        }
        recordMountain--;
    }
    for (int i = 0; i <= recordMountain; i++) 
        // Setup buttons "Montagna"
        levelSelMountain.addButton(Button({ 164.0f + 362 * i, 760.0f }, { 200.0f, 72.0f }, buttonType::play, ResourceManager::GetTexture("startButton"), ResourceManager::GetTexture("startButton"), 1, i));
    // button "back"
    levelSelMountain.addButton(Button({ 60.0f, 90.0f }, { 40.0f, 40.0f }, buttonType::link, 1, ResourceManager::GetTexture("backButton"), ResourceManager::GetTexture("backButton")));

    Menu gameOver(4, ResourceManager::GetTexture("gameOver"));
    // button "Esci"
    gameOver.addButton(Button({ 1280.0f/2 - 375.0f/2, 960.0f/2 - 100.0f/2 }, { 375.0f, 100.0f }, buttonType::link, 0, ResourceManager::GetTexture("exitButton"), ResourceManager::GetTexture("exitButton")));

    Menu levelComplete(5, ResourceManager::GetTexture("levelComplete"));
    // button "Esci"
    levelComplete.addButton(Button({ 1280.0f / 2 - 375.0f / 2, 960.0f / 2 - 100.0f / 2 }, { 375.0f, 100.0f }, buttonType::link, 0, ResourceManager::GetTexture("exitButton"), ResourceManager::GetTexture("exitButton")));

    Menu pause(6, ResourceManager::GetTexture("pause"));
    // button "Riprendi"
    pause.addButton(Button({ 1280.0f / 2 - 375.0f / 2, 960.0f / 2 - 115.0f }, { 375.0f, 100.0f }, buttonType::play, ResourceManager::GetTexture("resumeButton"), ResourceManager::GetTexture("resumeButton")));
    // button "Esci"
    pause.addButton(Button({ 1280.0f / 2 - 375.0f / 2, 960.0f / 2 + 115.0f }, { 375.0f, 100.0f }, buttonType::link, 0, ResourceManager::GetTexture("exitButton"), ResourceManager::GetTexture("exitButton")));

    Menu controls(7, ResourceManager::GetTexture("controlsBG"));
    // button "back"
    controls.addButton(Button({ 60.0f, 90.0f }, { 40.0f, 40.0f }, buttonType::link, 0, ResourceManager::GetTexture("backButton"), ResourceManager::GetTexture("backButton")));

    this->Menus.push_back(mainMenu);
    this->Menus.push_back(themeSel);
    this->Menus.push_back(levelSelForest);
    this->Menus.push_back(levelSelMountain);
    this->Menus.push_back(gameOver);
    this->Menus.push_back(levelComplete);
    this->Menus.push_back(pause);
    this->Menus.push_back(controls);
    this->currMenu = 0;
}

void Game::Update(float dt){
    if (this->State == GAME_ACTIVE) {
        GameLevel* level = &this->Levels[this->Level];
        level->PlayLevel(dt, this->Skin, this->Level); // manages level logic

        //check hitbox (bullet and dragon)
        for (int i = 0; i < level->instancedBullets.size(); i++) {
        // check for collision
        Bullet b = level->instancedBullets[i];
            switch(b.hitboxT) {
            case SQUARE: {
                std::shared_ptr<Square> s = std::dynamic_pointer_cast<Square>(b.hitbox);
                if (b.destroyed == false) {
                    for (int j = 0; j < level->player.instancedFireballs.size(); j++) {
                        Bullet fb = level->player.instancedFireballs[j];
                        if (fb.destroyed == false) {
                            switch (fb.hitboxT) {
                            case SQUARE:
                            {
                                std::shared_ptr<Square> fbHitbox = std::dynamic_pointer_cast<Square>(fb.hitbox);
                                if (checkCollisionSquareSquare(*s, *fbHitbox)) {
                                    hitBullet(&level->instancedBullets[i], &level->player.instancedFireballs[j], i, j);
                                }
                            }       
                             break;
                            case CIRCLE:
                            {
                                std::shared_ptr<Circle> fbHitbox = std::dynamic_pointer_cast<Circle>(fb.hitbox);
                                if (verifyBulletCollisionCircleSquare(*s, *fbHitbox)) {
                                    hitBullet(&level->instancedBullets[i], &level->player.instancedFireballs[j], i, j);
                                }
                            }
                            break;

                            }
                            
                        }
                    }
                }

                if (b.destroyed == false) {
                    if (verifyDragonCollisionSquare(*s)) {
                        // dragon is hit
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
                            switch (fb.hitboxT) {
                            case SQUARE:
                            {
                                std::shared_ptr<Square> fbHitbox = std::dynamic_pointer_cast<Square>(fb.hitbox);
                                if (checkCollisionSquareCircle(*fbHitbox, * c)) {
                                    hitBullet(&level->instancedBullets[i], &level->player.instancedFireballs[j], i, j);
                                }
                            }
                            break;
                            case CIRCLE:
                            {
                                std::shared_ptr<Circle> fbHitbox = std::dynamic_pointer_cast<Circle>(fb.hitbox);
                                if (checkCollisionCircleCircle(*fbHitbox , *c)) {
                                    hitBullet(&level->instancedBullets[i], &level->player.instancedFireballs[j], i, j);
                                }
                            }
                            break;

                            }
                        }
                    }
                }

                if (b.destroyed == false) {
                    if (verifyDragonCollisionCircle(*c)) {
                        // dragon is hit
                        hitDragon(&level->instancedBullets[i], i);
                    }
                }
            }
            break;
            }
        }

        //check hitbox (powerup)
        for (int i = 0; i < level->powerups.size(); i++) {
            Bullet pow = level->powerups[i];
            switch (pow.hitboxT) {
            case SQUARE:
            {
                std::shared_ptr<Square> s = std::dynamic_pointer_cast<Square>(pow.hitbox);
                if (verifyDragonCollisionSquare(*s)) {
                    // dragon picked up the powerup
                    powerupPick(pow);
                    sEngine->play2D("audio/Power_Up.wav");
                    // remove powerup
                    if (i != level->powerups.size() - 1) {
                        level->powerups[i] = std::move(level->powerups.back());
                    }
                    level->powerups.pop_back();
                }

            }
            break;
            case CIRCLE:
            {
                std::shared_ptr<Circle> c = std::dynamic_pointer_cast<Circle>(pow.hitbox);
                if (verifyDragonCollisionCircle(*c)) {
                    // dragon picked up the powerup
                    powerupPick(pow);
                    // remove powerup
                    if (i != level->powerups.size() - 1) {
                        level->powerups[i] = std::move(level->powerups.back());
                    }
                    level->powerups.pop_back();
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
        // check if the player is alive
        if (this->Levels[this->Level].player.stats.HP <= 0) {
            // the player is dead
            this->currMenu = 4;
            this->State = GAME_PAUSE;
        }
        // check if level is over
        if (this->Levels[this->Level].phase == PHASES) {
            // level is over
            Level_save::update_state(this->Skin, this->Level, this->Levels[this->Level].phase+1);
            this->currMenu = 5;
            this->State = GAME_PAUSE;

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
            for (Button& b : this->Menus[this->currMenu].buttons) {
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
                        if (this->State == GAME_PAUSE) {
                            // update menu selection of levels if new level unlocked
                            if (this->Skin == 0) 
                                this->Menus[2].updateBackground(this->Skin, this->Level);
                            else 
                                this->Menus[3].updateBackground(this->Skin, this->Level);
                         
                            this->State = GAME_MENU;
                        }
                        this->currMenu = clicked->subMenuId;
                        cursorOver = NULL;
                    }
                    else if (clicked->type == buttonType::play) {
                        this->currMenu = 0;  
                        cursorOver = NULL;
                        if (this->State == GAME_MENU && clicked->skin > -1 && clicked->level > -1) {
                            this->Skin = clicked->skin;
                            this->Level = clicked->level;
                            this->Levels[this->Level].backgroundTexture = ResourceManager::GetTexture("Skin" + std::to_string(this->Skin + 1) + "Lev" + std::to_string(this->Level + 1));
                            this->State = GAME_ACTIVE;
                            sEngine->stopAllSounds();
                            levelSound=sEngine->play2D("audio/Lost_Odissey.wav", true, false, true);
                            levelSound->setVolume(0.5f);
                            this->Levels[this->Level].startLevel(this->Width, this->Height);
                        }
                        else if (this->State == GAME_PAUSE) {
                            this->Levels[Level].player.playAnimation = true;
                            this->State = GAME_ACTIVE;
                        }
                        else {
                            std::cerr << "Tentativo di lancio di un livello con valore di skin e/o livello errati (< 0)" << std::endl;
                        }
                    }
                    else if (clicked->type == buttonType::close) {
                        glfwSetWindowShouldClose(this->window, true);
                    }
                }
            }
            clicked = NULL;
        }
    } else if (Game::State == GAME_PAUSE) {
        static Button* cursorOver = NULL, * clicked = NULL;
        double cursorX, cursorY;
        glfwGetCursorPos(this->window, &cursorX, &cursorY);

        if (!isCursorOnButton(cursorX, cursorY, cursorOver)) {
            if (cursorOver != NULL) {
                cursorOver->selected = false;
                cursorOver = NULL;
            }
            for (Button& b : this->Menus[this->currMenu].buttons) {
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
                        if (this->State == GAME_PAUSE) {
                            // update menu selection of levels if new level unlocked
                            if (this->Skin == 0)
                                this->Menus[2].updateBackground(this->Skin, this->Level);
                            else
                                this->Menus[3].updateBackground(this->Skin, this->Level);
                            this->State = GAME_MENU;
                        }
                        sEngine->stopAllSounds();
                        sEngine->play2D("audio/Zelda.wav", true);
                        this->currMenu = clicked->subMenuId;
                        cursorOver = NULL;
                    }
                    else if (clicked->type == buttonType::play) {
                        this->currMenu = 0;
                        cursorOver = NULL;
                        if (this->State == GAME_MENU && clicked->skin > -1 && clicked->level > -1) {
                            this->Skin = clicked->skin;
                            this->Level = clicked->level;
                            this->Levels[this->Level].backgroundTexture = ResourceManager::GetTexture("Skin" + std::to_string(this->Skin + 2) + "Lev" + std::to_string(this->Level + 1));
                            this->State = GAME_ACTIVE;
                            sEngine->stopAllSounds();
                            levelSound=sEngine->play2D("audio/Lost_Odissey.wav", true, false, true);
                            levelSound->setVolume(0.5f);
                            this->Levels[this->Level].startLevel(this->Width, this->Height);
                        }
                        else if (this->State == GAME_PAUSE) {
                            this->Levels[Level].player.playAnimation = true;
                            this->State = GAME_ACTIVE;
                        }
                        else {
                            std::cerr << "Tentativo di lancio di un livello con valore di skin e/o livello errati (< 0)" << std::endl;
                        }
                    }
                    else if (clicked->type == buttonType::close) {
                        glfwSetWindowShouldClose(this->window, true);
                    }
                }
            }
            clicked = NULL;
        }
    }

     else if (Game::State == GAME_ACTIVE) {
        GameLevel* level = &this->Levels[this->Level];
        // speed management (sprint, slowdown)
        static bool sprint = false, slowdown = false, shoot = false;

        // Menu pause
        if (this->Keys[GLFW_KEY_ESCAPE]) {
            this->currMenu = 6;
            this->State = GAME_PAUSE;
        }
        
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

         // dragon movements
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
            level->player.instanceFireball();
            shoot = true;
         }
         else if (this->MouseButtons[GLFW_MOUSE_BUTTON_RIGHT] && !shoot) {
             shoot = true;
             // instance powerups
             level->player.instancePowerup();
         }else if (!this->MouseButtons[GLFW_MOUSE_BUTTON_LEFT] && !this->MouseButtons[GLFW_MOUSE_BUTTON_RIGHT] && shoot) {
             shoot = false;
         }
            
     }
}

void Game::Render(float dt)
{   
    static bool timePaused = false;
    static float prevTime = glfwGetTime(), currTime = 0, deltaTime = 0;

    if (Game::State == GAME_MENU) {

        if (timePaused)
            timePaused = false;
        this->Menus[this->currMenu].drawMenu(*Renderer);
    }
    else if (Game::State == GAME_ACTIVE) {
        if (timePaused)
            timePaused = false;
        // prepare postProcesser
        Effects->BeginRender();
        // draw background
        //Renderer->DrawScrollingBackground(ResourceManager::GetTexture("level1Grass"), glm::vec2(0.0f, 0.0f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT), 0.0f, glm::vec3(1.0f), dt);

        this->Levels[this->Level].DrawBackground(*Renderer, dt, glm::vec2(this->Width, this->Height*15));
        // draw level with a bullet in
        this->Levels[this->Level].Draw(*Renderer, dt);
        this->HUD.RenderHUD(*Renderer, *Text, this->Levels[this->Level].player);
        // end rendering to postprocessing framebuffer
        Effects->EndRender();
        // render postprocessing quad
        Effects->Render(glfwGetTime());
    } 
    else if (Game::State == GAME_PAUSE) {
        if (!timePaused) {     
            prevTime = glfwGetTime();
            timePaused = true;
        }

        // Pause at timer on screen
        currTime = glfwGetTime();
        deltaTime = currTime - prevTime;
        Timer::forceChrono(Timer::start_time + deltaTime);
        prevTime = currTime;

        Effects->BeginRender();
        // stop dragon animation
        this->Levels[this->Level].player.playAnimation = false;

        this->Levels[this->Level].DrawBackground(*Renderer, dt, glm::vec2(this->Width, this->Height * 12));
        Renderer->DrawSprite(this->Levels[this->Level].backgroundTexture, glm::vec2(0.0), glm::vec2(this->Width, this->Height * 12), 0);
        //this->Levels[this->Level].DrawBackground(*Renderer, dt, glm::vec2(this->Width, this->Height * 12));

        // draw level with a bullet in
        this->Levels[this->Level].Draw(*Renderer, dt);
        this->HUD.RenderHUD(*Renderer, *Text, this->Levels[this->Level].player);
        Effects->EndRender();
        // render postprocessing quad
        Effects->Render(glfwGetTime());

        // Render menu in overlay
        this->Menus[this->currMenu].drawMenu(*Renderer);
    }
}

//powerup function
void Game::powerupPick(Bullet pow) {
    // powerup creation
    this->Levels[this->Level].player.stats.powerup.clear();
    switch (pow.Type) {
    case 100:
    {
        
        Bullet fb1, fb2, fb3;
        fb1 = Bullet(FIREBALL_COST, 40, ResourceManager::GetTexture("fireball"), glm::vec2(200.0f, 200.0f), glm::vec2(50.0f, 50.0f), glm::vec3(1.0f), glm::vec2(0.6f), HitboxType(CIRCLE), (int)'c');
        fb2 = Bullet(FIREBALL_COST, 40, ResourceManager::GetTexture("fireball"), glm::vec2(200.0f, 200.0f), glm::vec2(50.0f, 50.0f), glm::vec3(1.0f), glm::vec2(0.6f), HitboxType(CIRCLE), (int)'c');
        fb3 = Bullet(FIREBALL_COST, 40, ResourceManager::GetTexture("fireball"), glm::vec2(200.0f, 200.0f), glm::vec2(50.0f, 50.0f), glm::vec3(1.0f), glm::vec2(0.6f), HitboxType(CIRCLE), (int)'c');

        // apply rotation to side bullets
        fb1.Direction = glm::vec2(cos(PI * 135 / 180), sin(PI * -45 / 180));
        fb2.Direction = glm::vec2(0, -1);
        fb3.Direction = glm::vec2(cos(PI * 45 / 180), sin(PI * -45 / 180));
        fb1.rotation = -45;
        fb2.rotation = 0;
        fb3.rotation = 45;
        fb1.velApplied = 600;
        fb2.velApplied = 600;
        fb3.velApplied = 600;
        // insert in the list of powerup
        this->Levels[this->Level].player.stats.powerup.push_back(fb1);
        this->Levels[this->Level].player.stats.powerup.push_back(fb2);
        this->Levels[this->Level].player.stats.powerup.push_back(fb3);

    }
    break;
        

    case 101:
    {
        Bullet gFb = Bullet(pow.Power, 40, ResourceManager::GetTexture("bigFireball"), glm::vec2(200.0f, 200.0f), glm::vec2(150.0f,150.0f), glm::vec3(1.0f), glm::vec2(1.0f), HitboxType(CIRCLE), 101);
        gFb.Direction = glm::vec2(0, -1);
        gFb.velApplied = 400;
        gFb.hitNumber = 3;
        this->Levels[this->Level].player.stats.powerup.push_back(gFb);

    }
    break;
    }
}


//hitbox functions
void Game::hitDragon(Bullet* b, int i) {
    ShakeTime = 0.05f;
    Effects->Shake = true;
    // delete bullet
    b->destroyed = true;
    
    //if the dragon is hit:
    //- HP damage
    //- bullet disappears
    //- hit effect
    this->Levels[this->Level].player.dealDamage(b->Power); //deal damage + dragon hit effect
    this->Levels[this->Level].DestroyBullet(i);
    hitSound=sEngine->play2D("audio/Critical_Hit.wav", false, false, true);
    hitSound->setVolume(0.2f);
    if (this->Levels[this->Level].player.stats.HP <= 0) {
        this->Levels[this->Level].Die();
        // set state "GAME_OVER" and associated menu
        this->State = GAME_MENU;
    }
}

void Game::hitBullet(Bullet *b, Bullet* fb, int i, int j) {
    GameLevel* level = &this->Levels[this->Level];
    // check if it has to be destroyed or decrease the hit counter
    
    b->hitNumber--;
    fb->hitNumber--;
    if (b->hitNumber <= 0) {
        b->destroyed = true;
        this->Levels[this->Level].DestroyBullet(i);
    }
    
    if (fb->hitNumber <= 0) {
        fb->destroyed = true;
        level->player.instancedFireballs[j].destroy();
    }   
}

//dragon collision functions
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

//bullet collsion functions
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

//collision functions
bool Game::checkCollisionSquareSquare(Square hitbox1, Square hitbox2) {
       // horizontal and vertical projection of the hitbox 1
       float left1 = std::min(hitbox1.left_up.x, std::min(hitbox1.left_down.x, std::min(hitbox1.right_up.x, hitbox1.right_down.x)));
       float right1 = std::max(hitbox1.left_up.x, std::max(hitbox1.left_down.x, std::max(hitbox1.right_up.x, hitbox1.right_down.x)));
       float top1 = std::min(hitbox1.left_up.y, std::min(hitbox1.left_down.y, std::min(hitbox1.right_up.y, hitbox1.right_down.y)));
       float bottom1 = std::max(hitbox1.left_up.y, std::max(hitbox1.left_down.y, std::max(hitbox1.right_up.y, hitbox1.right_down.y)));

       // horizontal and vertical projection of the hitbox 2
       float left2 = std::min(hitbox2.left_up.x, std::min(hitbox2.left_down.x, std::min(hitbox2.right_up.x, hitbox2.right_down.x)));
       float right2 = std::max(hitbox2.left_up.x, std::max(hitbox2.left_down.x, std::max(hitbox2.right_up.x, hitbox2.right_down.x)));
       float top2 = std::min(hitbox2.left_up.y, std::min(hitbox2.left_down.y, std::min(hitbox2.right_up.y, hitbox2.right_down.y)));
       float bottom2 = std::max(hitbox2.left_up.y, std::max(hitbox2.left_down.y, std::max(hitbox2.right_up.y, hitbox2.right_down.y)));

       //check for intersection of projections
       return !(right1 < left2 || right2 < left1 || bottom1 < top2 || bottom2 < top1);
}

bool Game::checkCollisionSquareCircle(Square hitboxS, Circle hitboxC) {  
    //closest point of the square to the circle
    float closestX = std::max(hitboxS.left_up.x, std::min(hitboxC.center.x, hitboxS.right_down.x));
    float closestY = std::max(hitboxS.left_up.y, std::min(hitboxC.center.y, hitboxS.right_down.y));

    //distance between closest point and center
    float distanceX = hitboxC.center.x - closestX;
    float distanceY = hitboxC.center.y - closestY;

    //if distance<radius --> collisioin
    if ((distanceX * distanceX + distanceY * distanceY) < (hitboxC.radius * hitboxC.radius)) {
        return true;
    }
    return false;
}

bool Game::checkCollisionCircleCircle(Circle hitbox1, Circle hitbox2) {
    //distance between two center
    float distance = std::sqrt(std::pow(hitbox2.center.x - hitbox1.center.x, 2) +   //pow==power
        std::pow(hitbox2.center.y - hitbox1.center.y, 2));

    if (distance <= (hitbox1.radius + hitbox2.radius)) {
        return true;//collision
    }
    return false;
}


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

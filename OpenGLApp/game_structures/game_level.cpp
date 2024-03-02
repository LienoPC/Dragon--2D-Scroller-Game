#include "game_level.h"
#include "../resource_manager/resource_manager.h"
#include "../timer/timerMy.h"
#include "window_constraints.h"
#include <ctime>
#include <random>
#include <fstream>
#include <sstream>

short numRefresh = 0; // conto quanti proiettili sono stati distrutti per il refresh

std::vector<int> availabePowerups;
std::map<int, bool> alreadyUsedW;
const glm::vec2 moveAlong(0, 1);
int numDestroyed = 0;

GameLevel::GameLevel()
    : bulletList(NULL), instancedBullets(NULL){}

void GameLevel::setPlayer(Dragon player) {
    this->player = player;
}

void GameLevel::movePlayer(glm::vec2 move) {
    this->player.move(move);
}

bool GameLevel::isPlayerOutOfBounds(glm::vec2 pos, int height, int width) {
    if (pos.x < 0 || pos.x + this->player.size.x > width ||
        pos.y + 265 * this->player.size.y / 800 < 0 || pos.y + 705 * this->player.size.y / 800 > height)
        return true;
    return false;
}

void GameLevel::DrawBackground(SpriteRenderer& Renderer, float dt, glm::vec2 resolution) {
    Renderer.DrawScrollingBackground(this->backgroundTexture, glm::vec2(0.0f, 0.0f), resolution, 0.0f, glm::vec3(1.0f), dt);
}

void GameLevel::instanceBullet(int bullet, glm::vec2 pos, double velocity, DirectionStart directionStart) {
    Bullet b;
    b.copyBullet(ResourceManager::GetBullet(bullet));
    glm::vec2 actualPosition = pos;
    
    switch (directionStart) {

    case UP:
        actualPosition = pos + glm::vec2(0.0f,-b.size.y);
        break;

    case LEFT:
        actualPosition = pos + glm::vec2(-b.size.x, 0.0f);
        break;

    case RIGHT:
        actualPosition = pos + glm::vec2(b.size.x, 0.0f);
        break;

    }
    
    b.position = actualPosition;
    b.Direction = calculateNormalizedDirection(b.position);
    b.velApplied = velocity;
    b.destroyed = false;
    b.syncRotation();
    this->instancedBullets.push_back(b);
}


void GameLevel::instanceWindow(int identificator) {
    ThrowWindow t(ResourceManager::GetWindow(identificator));
    this->actualWindows.push_back(t);
}

glm::vec2 GameLevel::calculateNormalizedDirection(glm::vec2 bPosition) {
    double a = (this->player.position.x+this->player.size.x/2) - bPosition.x;
    double b = (this->player.position.y+this->player.size.y/2) - bPosition.y;
    double c = std::sqrt(std::pow(a, 2) + std::powf(b, 2)); // ipotenusa
    double cosA = a / c;
    double cosB = b / c;
    glm::vec2 returned(cosA, cosB);
    return returned;
}

void GameLevel::LoadLevel(int height, int width, const char* path) {

 
    // Set all Level attributes and constraints
    ResourceManager::LoadLevelF(path, "LevelP");
    this->maxInstancedBullet = ResourceManager::GetLevel("LevelP").maxInstancedBullet;
    this->minVel = ResourceManager::GetLevel("LevelP").minVel;
    this->maxVel = ResourceManager::GetLevel("LevelP").maxVel;
    this->bulletList = ResourceManager::GetLevel("LevelP").bulletList;
    
    // Initiate all windows
    for (int i = 0; i < this->windowNumber; i++) {
        instanceWindow(i);
    }
   
    // Definisco la phase e assegno le finestre
    numRefresh = this->maxInstancedBullet;
    
    // Inizializzo la fase
    this->phase = 0;
}

void GameLevel::startLevel(int height, int width) {
    // Set initial time for the level
    Timer::setChrono();
    glm::vec2 playerSize = { 375.0f, 375.0f };
    Dragon player(ResourceManager::GetTexture("dragon"), glm::vec2(width / 2 - playerSize.x / 2, height / 2), playerSize, glm::vec3(1.0f), glm::vec2(1.0f), 400.0f, HitboxType(SQUARE));
    this->setPlayer(player);
}


void GameLevel::PlayLevel(float dt) {

    // inserire la logica di gioco letta dal file che gestisce il movimento dei bullet

    // verifico che quelli istanziati siano ancora validi (VALUTANDO L'USCITA DAL BASSO)
    // oppure se è stato distrutto
    for (int i = 0; i < this->instancedBullets.size(); i++) {
        Bullet b = this->instancedBullets[i];
        if ((b.position.y > LEV_LIMITY) || ((b.position.y > LEV_LIMITY / 2) && (b.position.x + b.size.x < 0 || b.position.x > LEV_LIMITX))) {
            // il bullet è uscito fuori dal livello
            if (i != this->instancedBullets.size() - 1)
            {
                this->instancedBullets[i] = std::move(this->instancedBullets.back());
            }
            this->instancedBullets.pop_back();
            numRefresh++;
        }
        // valuto se il bullet è stato distrutto
        if (b.destroyed == true) {
            bool a = false;
            // verifico che tutti gli effetti siano terminati
            for (int i = 0; i < b.particles.size(); i++) {
                if (b.particles[i]->particleEnded)
                    a = true;
            }
            if (a == true) {
                // rimuovo il proiettile
                if (i != this->instancedBullets.size() - 1){
                    this->instancedBullets[i] = std::move(this->instancedBullets.back());
                }
                this->instancedBullets.pop_back();
                numRefresh++;
            }
        }
    }

    player.checkFireballs();
    alreadyUsedW.clear();
    // tengo conto delle finestre già usate in un singolo lancio
    for (int i = 0; i < this->actualWindows.size(); i++) {
        alreadyUsedW[actualWindows[i].identificator] = true;
    }
    
    // istanzio i proiettili che mi servono
   
        for (int i = 0; i < numRefresh; i++) {
            if (this->bulletList.size() > 0) {

                int nW = WindowPick();

                double positionOffsetX = positionOffsetPick(0, nW);

                double positionOffsetY = positionOffsetPick(1, nW);

                // velocity selection
                std::random_device rd;
                std::default_random_engine re(rd());
                std::uniform_real_distribution<double> unif3(this->minVel, this->maxVel);
                double velocity = unif3(re);

                // istanzio il proiettile
                instanceBullet(*this->bulletList.begin(), this->actualWindows[nW].Position + glm::vec2(positionOffsetX, positionOffsetY), velocity, this->actualWindows[nW].directionStart);
                this->bulletList.erase(this->bulletList.begin());
            }
            else {
           

            }
        }
    
    
    numRefresh = 0;

    this->SpawnPowerUps();


    // muove tutti i proiettili istanziati nella scena
    for (int i = 0; i < this->instancedBullets.size(); i++) {
        if (this->instancedBullets[i].Type == 'b') {
            this->instancedBullets[i].rotation+= this->instancedBullets[i].velApplied/600;
        }
        this->instancedBullets[i].move(dt);
        if (this->instancedBullets[i].destroyed) {
            numDestroyed++;
        }
    }
    // muovo tutti i proiettili del drago lanciati
    for (int i = 0; i < this->player.instancedFireballs.size(); i++) {
        switch (this->player.instancedFireballs[i].Type) {

        case 99:
            this->player.instancedFireballs[i].rotation += this->player.instancedFireballs[i].velApplied / 600;
            break;
        case 101:
            this->player.instancedFireballs[i].rotation += this->player.instancedFireballs[i].velApplied / 70;
            break;
        }
        this->player.instancedFireballs[i].move(dt);
    }
    
    // muovo tutti i powerup
    for(int i = 0; i < this->powerups.size(); i++){
        // verifico se il powerup è ancora nello schermo
        if (this->powerups[i].position.y < LEV_LIMITY) {
            this->powerups[i].move(dt);
        }
        else {
            // rimuovo il powerup
            if (i != this->powerups.size() - 1) {
                this->powerups[i] = std::move(this->powerups.back());
            }
            this->powerups.pop_back();
        }
        
    }


    // ricarico il mana del giocatore
    if (this->player.stats.FP < MAX_FP) {
        this->player.stats.FP++;
    }

    // verifico il passaggio di fase
    // bronze medal
    if (Timer::getElapsedSeconds() > SECONDS1 && Timer::getElapsedSeconds() < SECONDS2) {
        this->IncreasePhase();
        this->player.stats.medal = BRONZE;

    }
    // silver medal
    if (Timer::getElapsedSeconds() > SECONDS2 && Timer::getElapsedSeconds() < END) {
        this->IncreasePhase();
        this->player.stats.medal = SILVER;
        // inserire funzione che sblocca il livello successivo della stessa skin
    }
    // golden medal
    if (Timer::getElapsedSeconds() > END) {
        this->IncreasePhase();
        this->player.stats.medal = GOLD;
        // end level
    }

}


int GameLevel::WindowPick() {

    // window selection
    std::random_device rd;
    std::default_random_engine re(rd());
    std::uniform_int_distribution<int> unifInt(0, this->windowNumber - 1);
    int nW = unifInt(re);

    while (!alreadyUsedW[nW]) {
        nW = unifInt(re);
    }
    alreadyUsedW[nW] = false;
    return nW;
}

double GameLevel::positionOffsetPick(int sel, int nW) {
    std::random_device rd;
    std::default_random_engine re(rd());
    double lower_bound;
    double upper_bound;
    if (sel == 0) {
        // X offset selection
        lower_bound = -(double)this->actualWindows[nW].offsetInterval.x / 2;
        upper_bound = (double)this->actualWindows[nW].offsetInterval.x / 2;
        std::uniform_real_distribution<double> unif(lower_bound, upper_bound);
        

    }
    else {
        // Y offset selection
        lower_bound = -(double)this->actualWindows[nW].offsetInterval.y / 2;
        upper_bound = (double)this->actualWindows[nW].offsetInterval.y / 2;
        

    }
    std::uniform_real_distribution<double> unif(lower_bound, upper_bound);

    return unif(re);
   
}

void GameLevel::Draw(SpriteRenderer& renderer, float dt) {
    // player draw
    player.Draw(renderer, dt);

    // bullet draw
    for (Bullet b : this->instancedBullets) {
          b.Draw(renderer);
    }
    // dragon bullet draw
    for (int i = 0; i < this->player.instancedFireballs.size(); i++) {
        this->player.instancedFireballs[i].Draw(renderer);
    }
    // powerup draw
    for (int i = 0; i < powerups.size(); i++) {
        powerups[i].Draw(renderer);
    }
}

void GameLevel::DestroyBullet(unsigned int i) {
    if(this->instancedBullets.size() < this->maxInstancedBullet)
        this->numRefresh++;
    this->instancedBullets[i].destroy();
}

void GameLevel::IncreasePhase() {
    if (phase + 1 <= PHASES) {
        phase++;
        this->minVel += VELINCREASE;
        this->maxVel += VELINCREASE;
        switch (phase) {
        case 1:
            availabePowerups.push_back(100);
            break;
        case 2:
            availabePowerups.push_back(101);
            // aggiungo una finestra di lancio
            instanceWindow(this->actualWindows.size());
            break;
        }

    }
    else {
        // end level
        phase++;
    }
    

}

void GameLevel::Die() {

}

// calcola randomicamente se spawnare i powerup in base a:
// -phase
// -vita
void GameLevel::SpawnPowerUps() {

    float prob = 0;
    float ran = 0;
    // calcolo la probabilità
    switch (this->phase) {
    case 0:
        prob += 0;
        break;
    case 1:
        prob += 0.08;
        break;
    case 2:
        prob += 0.12;
        break;
    }
    std::random_device rd;
    std::default_random_engine re(rd());
    std::uniform_real_distribution<double> unif3(0, 1);
    if (phase != 0 && this->player.stats.HP != 0) {
        prob += 0.3 / this->player.stats.HP;
    }
    else {
        prob = 0;
    }
    ran = unif3(re);
    if (ran < prob) {
        // spawno il powerup
        std::uniform_int_distribution<int> unif(0, availabePowerups.size() - 1);
        int i = unif(re);
        std::uniform_real_distribution<double> unif3(20, LEV_LIMITX-20);
        ran = unif3(re); // x value arbitraria di spawn del powerup
        Bullet b;
        switch (availabePowerups[i]) {
        case 100:
            b = Bullet(FIREBALL_COST, 0.0f, ResourceManager::GetTexture("aPowerup"), glm::vec2(ran, -100), glm::vec2(50.0f), glm::vec3(1.0f), glm::vec2(1.0f), HitboxType(CIRCLE), 100);
            b.velApplied = 400;
            b.Direction = moveAlong;
            powerups.push_back(b);
            break;
        case 101:
            b = Bullet(150, 0.0f, ResourceManager::GetTexture("bPowerup"), glm::vec2(ran, -100), glm::vec2(50.0f), glm::vec3(1.0f), glm::vec2(1.0f), HitboxType(CIRCLE), 101);
            b.velApplied = 400;
            b.Direction = moveAlong;
            powerups.push_back(b);
            break;
        }
        
    }

}


/*
 DEPRECATED
void GameLevel::MoveBullet(glm::vec2 move, int identificator) {

    this->instancedBullets[identificator].move(move);

}

*/
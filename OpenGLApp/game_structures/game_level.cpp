#include "game_level.h"
#include "../resource_manager/resource_manager.h"
#include "../timer/timerMy.h"
#include "window_constraints.h"
#include <ctime>
#include <random>
#include <fstream>
#include <sstream>

short numRefresh = 0; // conto quanti proiettili sono stati distrutti per il refresh

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

void GameLevel::LoadLevel(int height, int width) {

 
    // Set all Level attributes and constraints
    ResourceManager::LoadLevelF("levels/lev.txt", "LevelP");
    this->maxInstancedBullet = ResourceManager::GetLevel("LevelP").maxInstancedBullet;
    this->minVel = ResourceManager::GetLevel("LevelP").minVel;
    this->maxVel = ResourceManager::GetLevel("LevelP").maxVel;
    this->bulletList = ResourceManager::GetLevel("LevelP").bulletList;
    
    // Initiate all windows
    for (int i = 0; i < this->windowNumber; i++) {
        instanceWindow(i);
    }
   
    // Set initial time for the level
    Timer::setChrono();

    glm::vec2 playerSize = { 375.0f, 375.0f };
    Dragon player(ResourceManager::GetTexture("dragon"), glm::vec2(width/2 - playerSize.x/2, height/2), playerSize, glm::vec3(1.0f), glm::vec2(1.0f), 400.0f, HitboxType(SQUARE));
    this->setPlayer(player);
    
    // Definisco la phase e assegno le finestre
    numRefresh = this->maxInstancedBullet;
}


void GameLevel::PlayLevel(float dt) {

    // inserire la logica di gioco letta dal file che gestisce il movimento dei bullet

    // verifico che quelli istanziati siano ancora validi (VALUTANDO L'USCITA DAL BASSO)
    // oppure se è stato distrutto
    for (int i = 0; i < this->instancedBullets.size(); i++) {
        Bullet b = this->instancedBullets[i];
        if ((b.position.y > LEV_LIMITY) || ((b.position.y > LEV_LIMITY/2) && (b.position.x + b.size.x < 0 || b.position.x > LEV_LIMITX))) {
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
                if (i != this->instancedBullets.size() - 1)
                {
                    this->instancedBullets[i] = std::move(this->instancedBullets.back());
                }
                this->instancedBullets.pop_back();

            }
        }
       
    
        

    }

    // tengo conto delle finestre già usate in un singolo lancio
    std::map<int, bool> alreadyUsedW;
    for (int i = 0; i < this->actualWindows.size(); i++) {
        alreadyUsedW[actualWindows[i].identificator] = true;
    }
    
    // istanzio i proiettili che mi servono
    for (int i = 0; i < numRefresh; i++) {
        if (this->bulletList.size() > 0) {

               
               
            int nW = WindowPick(alreadyUsedW);
                
            double positionOffsetX = positionOffsetPick(0, nW);

            double positionOffsetY = positionOffsetPick(1, nW);

            // velocity selection
            std::random_device rd;
            std::default_random_engine re(rd());
            std::uniform_real_distribution<double> unif3(this->minVel, this->maxVel);
            double velocity = unif3(re);

            // istanzio il proiettile
            instanceBullet(*this->bulletList.begin(), this->actualWindows[nW].Position+glm::vec2(positionOffsetX, positionOffsetY), velocity, this->actualWindows[nW].directionStart);
            this->bulletList.erase(this->bulletList.begin());
        }
        else {
            // PROIETTILI FINITI -> PROBLEMA?

        }
    }
    numRefresh = 0;


    // muove tutti i proiettili istanziati nella scena
    for (int i = 0; i < this->instancedBullets.size(); i++) {
        this->instancedBullets[i].move(dt);
    }


}


int GameLevel::WindowPick(std::map<int, bool> alreadyUsedW) {

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
    player.Draw(renderer, dt);

    for (Bullet b : this->instancedBullets) {
          b.Draw(renderer);
    }
}

void GameLevel::DestroyBullet(unsigned int i) {
    this->numRefresh++;
    this->instancedBullets[i].destroy();
}

void GameLevel::IncreasePhase() {
    if (phase + 1 <= PHASES) {
        phase++;
        this->minVel += VELINCREASE;
        this->maxVel += VELINCREASE;
        /*
            PER ORA ABBIAMO DECISO DI NO
            switch (phase) {
            case 2:
                instanceWindow(3);
                instanceWindow(4);
                break;
            case 3:
                instanceWindow(5);
                instanceWindow(6);
                break;

            default:

            break;

        }
        */
        
    }
    else {
        // FINE LIVELLO?
    }
    

}



/*
 DEPRECATED
void GameLevel::MoveBullet(glm::vec2 move, int identificator) {

    this->instancedBullets[identificator].move(move);

}

*/
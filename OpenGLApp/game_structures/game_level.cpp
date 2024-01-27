//#include "../resource_manager/resource_manager.h"
#include "game_level.h"
#include "../resource_manager/resource_manager.h"
#include "../timer/timerMy.h"
#include "window_constraints.h"
#include <ctime>
#include <random>
#include <fstream>
#include <sstream>


GameLevel::GameLevel()
    : bulletList(NULL), instancedBullets(NULL){}



void GameLevel::setPlayer(Dragon player) {
    this->player = player;
}

void GameLevel::movePlayer(glm::vec2 move) {
    this->player.move(move);
}



void GameLevel::instanceBullet(int bullet, glm::vec2 pos, double velocity, DirectionStart directionStart) {
    Bullet b(ResourceManager::GetBullet(bullet));
    glm::vec2 actualPosition = pos;
    
    switch (directionStart) {

    case UP:
        actualPosition = pos + glm::vec2(0.0f,-b.Size.y);
        break;

    case LEFT:
        actualPosition = pos + glm::vec2(-b.Size.x, 0.0f);
        break;

    case RIGHT:
        actualPosition = pos + glm::vec2(b.Size.x, 0.0f);
        break;

    }
    
    b.Position = actualPosition;
    b.Direction = calculateNormalizedDirection(b.Position);
    b.velApplied = velocity;
    this->instancedBullets.push_back(b);
}

void GameLevel::instanceWindow(int identificator) {
    ThrowWindow t(ResourceManager::GetWindow(identificator));
    this->actualWindows.push_back(t);
}

glm::vec2 GameLevel::calculateNormalizedDirection(glm::vec2 bPosition) {
    double a = (this->player.Position.x+this->player.Size.x/2) - bPosition.x;
    double b = (this->player.Position.y+this->player.Size.y/2) - bPosition.y;
    double c = std::sqrt(std::pow(a, 2) + std::powf(b, 2)); // ipotenusa
    double cosA = a / c;
    double cosB = b / c;
    glm::vec2 returned(cosA, cosB);
    return returned;
}





void GameLevel::LoadLevel() {

    /*
    * Bullet b1(0.0f, 0.0f, ResourceManager::GetTexture("trozky"), glm::vec2(300.0f, 0.0f), glm::vec2(70.0f, 80.0f), glm::vec3(0.4f), glm::vec2(1.0f), hitboxType(AABB),0);
    Bullet b2(0.0f, 0.0f, ResourceManager::GetTexture("lenin"), glm::vec2(100.0f, 0.0f), glm::vec2(70.0f, 80.0f), glm::vec3(0.3f), glm::vec2(1.0f), hitboxType(AABB), 1);
    this->AddBulletType(b1);
    this->AddBulletType(b2);

    this->AddBullet(0);
    this->AddBullet(0);
    this->AddBullet(1);
    this->AddBullet(0);
    */

 
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

    Dragon player(ResourceManager::GetTexture("dragon"), glm::vec2(300.0f, 400.0f), glm::vec2(200.0f, 200.0f), glm::vec3(1.0f), glm::vec2(1.0f), 300.0f, hitboxType(AABB));
    this->setPlayer(player);
    
    // Definisco la phase e assegno le finestre
}

void GameLevel::PlayLevel(float dt) {

    // inserire la logica di gioco letta dal file che gestisce il movimento dei bullet

    // verifico che quelli istanziati siano ancora validi (VALUTANDO L'USCITA DAL BASSO)
    for (int i = 0; i < this->instancedBullets.size(); i++) {
        Bullet b = this->instancedBullets[i];
        if ((b.Position.y > LEV_LIMITY) || ((b.Position.y > LEV_LIMITY/2) && (b.Position.x < 0 || b.Position.x > LEV_LIMITX))) {
            // il bullet è uscito fuori dal livello
            if (i != this->instancedBullets.size() - 1)
            {
                this->instancedBullets[i] = std::move(this->instancedBullets.back());
            }
            this->instancedBullets.pop_back();
        }

    }

    // tengo conto delle finestre già usate in un singolo lancio
    std::map<int, bool> alreadyUsedW;
    for (int i = 0; i < this->actualWindows.size(); i++) {
        alreadyUsedW[actualWindows[i].identificator] = true;
    }
    
    // istanzio i proiettili che mi servono
    if (this->instancedBullets.size() < this->maxInstancedBullet) {
        int tempSize = this->instancedBullets.size();
        for (int i = 0; i < (this->maxInstancedBullet - tempSize); i++) {
            if (this->bulletList.size() > 0) {

                
              
                // window selection
                std::random_device rd;
                std::default_random_engine re(rd());
                std::uniform_int_distribution<int> unifInt(0, this->windowNumber-1);
                int nW = unifInt(re);
               
                while (!alreadyUsedW[nW]) {
                    nW = unifInt(re);
                }
                alreadyUsedW[nW] = false;
               
                
                // X offset selection
                double lower_bound = -(double)this->actualWindows[nW].offsetInterval.x/2;
                double upper_bound = (double)this->actualWindows[nW].offsetInterval.x/2;
                std::uniform_real_distribution<double> unif(lower_bound, upper_bound);
                double positionOffsetX = unif(re);

                // Y offset selection
                lower_bound = -(double)this->actualWindows[nW].offsetInterval.y / 2;
                upper_bound = (double)this->actualWindows[nW].offsetInterval.y / 2;
                std::uniform_real_distribution<double> unif2(lower_bound, upper_bound);
                double positionOffsetY = unif2(re);

                // velocity selection
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
    }



    // muove tutti i proiettili istanziati nella scena
    for (int i = 0; i < this->instancedBullets.size(); i++) {
        this->instancedBullets[i].move(dt);
    }


}


void GameLevel::Draw(SpriteRenderer& renderer, float dt) {
    player.Draw(renderer, dt);

    for (Bullet b : this->instancedBullets) {
        if (!b.Destroyed)
            b.Draw(renderer);
    }
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
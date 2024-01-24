//#include "../resource_manager/resource_manager.h"
#include "game_level.h"
#include "../resource_manager/resource_manager.h"
#include "../timer/timerMy.h"
#include "window_constraints.h"
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



void GameLevel::instanceBullet(int bullet, glm::vec2 pos) {
    Bullet b(ResourceManager::GetBullet(bullet));
    this->instancedBullets.push_back(b);
}

void GameLevel::instanceWindow(int identificator) {
    ThrowWindow t(ResourceManager::GetWindow(identificator));
    this->actualWindows.push_back(t);
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

void GameLevel::PlayLevel() {

    // inserire la logica di gioco letta dal file che gestisce il movimento dei bullet

    // verifico che quelli istanziati siano ancora validi (VALUTANDO L'USCITA DAL BASSO)
    for (int i = 0; this->instancedBullets.size(); i++) {
        Bullet b = this->instancedBullets[i];
        if ((b.Position.y > LEV_LIMITY) || ((b.Position.y > LEV_LIMITY/2) && (b.Position.x < 0 || b.Position.x > LEV_LIMITX))) {
            // il bullet è uscito fuori dal livello
            if (i != this->instancedBullets.size() - 1)
            {
                // Beware of move assignment to self
                // see http://stackoverflow.com/questions/13127455/
                this->instancedBullets[i] = std::move(this->instancedBullets.back());
            }
            this->instancedBullets.pop_back();
        }

    }

    // istanzio i proiettili che mi servono
    if (this->instancedBullets.size() < this->maxInstancedBullet) {
        for (int i = 0; i < (this->maxInstancedBullet - this->instancedBullets.size()); i++) {
            if (this->bulletList.size() > 0) {

                // random seed setting
                std::srand((int)glfwGetTime());
                
                // window selection
                int nW = (rand() % this->windowNumber) + 1;
                // X offset selection
                double lower_bound = -(double)this->actualWindows[nW].offsetInterval.x/2;
                double upper_bound = (double)this->actualWindows[nW].offsetInterval.x/2;
                std::uniform_real_distribution<double> unif(lower_bound, upper_bound);
                std::default_random_engine re;
                double positionOffsetX = unif(re);

                // Y offset selection
                lower_bound = -(double)this->actualWindows[nW].offsetInterval.y / 2;
                upper_bound = (double)this->actualWindows[nW].offsetInterval.y / 2;
                std::uniform_real_distribution<double> unif2(lower_bound, upper_bound);
                double positionOffsetY = unif2(re);


                // istanzio il proiettile
                instanceBullet(*this->bulletList.begin(), this->actualWindows[nW].Position+glm::vec2(positionOffsetX, positionOffsetY));
                this->bulletList.erase(this->bulletList.begin());
            }
            else {
                // PROIETTILI FINITI -> PROBLEMA?

            }
        }
    }



    for (int i = 0; i < this->bulletList.size(); i++) {
        instanceBullet(this->bulletList[i], glm::vec2(i * 5, 0.0f));
    }

    // muove tutti i proiettili istanziati nella scena
    for (int i = 0; i < this->instancedBullets.size(); i++) {
        this->instancedBullets[i].move(glm::vec2(0.3 + (i / 10), 0.4 + (i / 5)));
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
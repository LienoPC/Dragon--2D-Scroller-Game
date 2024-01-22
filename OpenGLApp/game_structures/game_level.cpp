//#include "../resource_manager/resource_manager.h"
#include "game_level.h"
#include "../resource_manager/resource_manager.h"
#include "../timer/timerMy.h"
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




void GameLevel::AddBullet(int bullet) {
	 this->bulletList.push_back(bullet);
}


void GameLevel::instanceBullet(int bullet, glm::vec2 pos) {
    Bullet b(ResourceManager::GetBullet(bullet));
    this->instancedBullets.push_back(b);
}





void GameLevel::PlayLevel() {

    // inserire la logica di gioco letta dal file che gestisce il movimento dei bullet
    
    // istanzio i proiettili che mi servono e verifico che quelli istanziati siano ancora validi
    for (int i = 0; i < this->bulletList.size(); i++) {
        instanceBullet(this->bulletList[i], glm::vec2(i * 5, 0.0f));
    }

    // muove tutti i proiettili istanziati nella scena
    for (int i = 0; i < this->instancedBullets.size(); i++) {
        this->instancedBullets[i].move(glm::vec2(0.3 + (i/10), 0.4 + (i/5)));
    }


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
    
   

    // Set initial time for the level
    //Timer::setChrono();

    Dragon player(ResourceManager::GetTexture("dragon"), glm::vec2(300.0f, 400.0f), glm::vec2(200.0f, 200.0f), glm::vec3(1.0f), glm::vec2(1.0f), 300.0f, hitboxType(AABB));
    this->setPlayer(player);
}



void GameLevel::Draw(SpriteRenderer& renderer, float dt) {
    player.Draw(renderer, dt);

    for (Bullet b : this->instancedBullets) {
        if (!b.Destroyed)
            b.Draw(renderer);
    }
}





/*
 DEPRECATED
void GameLevel::MoveBullet(glm::vec2 move, int identificator) {

    this->instancedBullets[identificator].move(move);

}

*/
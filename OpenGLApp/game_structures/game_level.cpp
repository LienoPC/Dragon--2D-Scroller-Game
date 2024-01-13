#include "game_level.h"

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

void GameLevel::AddBulletType(Bullet& bullet) {
   
    this->bulletTypes.insert({ bullet.Type , bullet });
}

void GameLevel::instanceBullet(int bulletType) {
    std::map<int, Bullet>::iterator itr;
    itr = this->bulletTypes.find(bulletType);
    if (itr != this->bulletTypes.end()) {
        Bullet b(itr->second);
        this->instancedBullets.push_back(b);
    }  
}

void GameLevel::MoveBullet(glm::vec2 move, int identificator) {

    this->instancedBullets[identificator].move(move);
    
}

void GameLevel::PlayLevel() {

    // inserire la logica di gioco letta dal file che gestisce il movimento dei bullet
    while(this->bulletList.size() > 0){
        this->instanceBullet(this->bulletList.back());
        this->bulletList.pop_back();
    }

    // fai cose
    for (int i = 0; i < this->instancedBullets.size(); i++) {
        this->instancedBullets[i].move(glm::vec2(0.01f, 0.01f));
    }  
}

void GameLevel::LoadLevel() {
    Bullet b1(0.0f, 0.0f, ResourceManager::GetTexture("trozky"), glm::vec2(300.0f, 0.0f), glm::vec2(70.0f, 80.0f), glm::vec3(0.4f), glm::vec2(1.0f), hitboxType(AABB),0);
    Bullet b2(0.0f, 0.0f, ResourceManager::GetTexture("lenin"), glm::vec2(100.0f, 0.0f), glm::vec2(70.0f, 80.0f), glm::vec3(0.3f), glm::vec2(1.0f), hitboxType(AABB), 1);
    this->AddBulletType(b1);
    this->AddBulletType(b2);

    this->AddBullet(0);
    this->AddBullet(0);
    this->AddBullet(1);
    this->AddBullet(0);

    Dragon player(ResourceManager::GetTexture("dragon"), glm::vec2(300.0f, 400.0f), glm::vec2(200.0f, 200.0f), glm::vec3(1.0f), glm::vec2(1.0f), 300.0f, hitboxType(AABB));
    this->setPlayer(player);
}

void GameLevel::Draw(SpriteRenderer& renderer) {
    player.Draw(renderer);

    for (Bullet b : this->instancedBullets) {
        if (!b.Destroyed)
            b.Draw(renderer);
    }
}

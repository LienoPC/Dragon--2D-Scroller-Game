#include "game_level.h"
#include "../game.h"
#include "../timer/timerMy.h"
#include <fstream>
#include <sstream>

GameLevel::GameLevel()
    : bulletList(NULL), instancedBullets(NULL){}

void GameLevel::AddBullet(int bullet) {
	 this->bulletList.push_back(bullet);
}


void GameLevel::instanceBullet(int bullet, glm::vec2 pos) {
    Bullet b(Game::GetBullet(bullet));
    this->instancedBullets.push_back(b);
}


void GameLevel::Draw(SpriteRenderer& renderer){
    for (Bullet b : this->instancedBullets) {
        if (!b.Destroyed)
            b.Draw(renderer);
    }
        
}




void GameLevel::MoveBullet(glm::vec2 move, int identificator) {

    this->instancedBullets[identificator].move(move);
    
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

    // Load the list of bullets for that level
    ResourceManager::LoadBulletList("levels/lev.txt", "LevelP");
    this->bulletList = ResourceManager::GetBulletList("LevelP");
}


/*

DEPRECATED

std::vector<Bullet> GameLevel::loadBulletTypesFromFile(const char* file) {
    std::vector<Bullet> returned;
    std::ifstream input(file, std::ios::binary);

    while (!input.eof()) {


    }

    return returned;
}

void GameLevel::writeBulletTypesOnFile(const char* file, std::vector<Bullet> list) {
    std::ofstream out(file, std::ios::out | std::ios::binary);
    if (!out) {
        std::cout << "Impossibile aprire il file" << std::endl;
    }
    for (int i = 0; i < list.size(); i++) {
        out.write((char*)&list[i].Position, sizeof(glm::vec2));
        out.write((char*)&list[i].Size, sizeof(glm::vec2));
        out.write((char*)&list[i].Velocity, sizeof(glm::vec2));
        out.write((char*)&list[i].Color, sizeof(glm::vec3));
        out.write((char*)&list[i].Hitbox, sizeof(hitboxType));
        out.write((char*)&list[i].Rotation, sizeof(float));
        out.write((char*)&list[i].IsSolid, sizeof(bool));
        out.write((char*)&list[i].Destroyed, sizeof(bool));
        out.write((char*)&list[i].Power, sizeof(float));
        out.write((char*)&list[i].ParticlesNumber, sizeof(float));
        out.write((char*)&list[i].VelocityMultipler, sizeof(float));
        out.write((char*)&list[i].Type, sizeof(int));

    }
    out.close();

}
*/


#include "game_level.h"

#include <fstream>
#include <sstream>

GameLevel::GameLevel()
    : bullets(NULL),bulletTypes(NULL){}

void GameLevel::AddBullet(Bullet& bullet) {
	 this->bullets.push_back(bullet);
}

void GameLevel::Draw(SpriteRenderer& renderer){
    for (Bullet& b : this->bullets)
        if (!b.Destroyed)
            b.Draw(renderer);
}

void GameLevel::MoveBullet(glm::vec2 move, int identificator) {
    
    this->bullets[identificator].move(move);
    /*
    * 
        this->bullets[identificator].Position.x += move.x;
        this->bullets[identificator].Position.y += move.y;
    */
    
}

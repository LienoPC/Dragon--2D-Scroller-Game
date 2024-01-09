#include "game_level.h"

#include <fstream>
#include <sstream>

GameLevel::GameLevel()
    : bullets(NULL) {}

void GameLevel::AddBullet(Bullet bullet) {
	 this->bullets.push_back(bullet);
}

void GameLevel::Draw(SpriteRenderer& renderer){
    for (Bullet& b : this->bullets)
        if (!b.Destroyed)
            b.Draw(renderer);
}

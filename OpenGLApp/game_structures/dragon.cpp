#include "dragon.h"
#include "../resource_manager/resource_manager.h"


Dragon::Dragon()
	:velocityModifier(0.0f) {};

Dragon::Dragon(Texture2D sprite, glm::vec2 pos, glm::vec2 size, glm::vec3 color, glm::vec2 velocity, float mvSpeed, HitboxType hitboxT)
: velocityModifier(mvSpeed), animationFrames(NULL){
	this->position = pos;
	this->size = size;
	this->velocity = velocity;
	this->color = color;
	this->sprite = sprite;
	this->hitboxT = hitboxT;
	this->stats = DragonStats(); //Inzializzo il dragòn a full vita
	this->sprite = ResourceManager::GetTexture("dragon_f0");
	// Save dragon animation frames
	this->animationFrames.push_back(ResourceManager::GetTexture("dragon_f0"));
	this->animationFrames.push_back(ResourceManager::GetTexture("dragon_f1"));
	this->animationFrames.push_back(ResourceManager::GetTexture("dragon_f2"));
	this->animationFrames.push_back(ResourceManager::GetTexture("dragon_f3"));
	this->animationFrames.push_back(ResourceManager::GetTexture("dragon_f4"));
	this->animationFrames.push_back(ResourceManager::GetTexture("dragon_f5"));
	this->animationFrames.push_back(ResourceManager::GetTexture("dragon_f6"));
	this->animationFrames.push_back(ResourceManager::GetTexture("dragon_f7"));
	// Set Hitboxes
	this->hitboxes.push_back(Square(glm::vec2(this->position.x + 60 * size.x / 800, this->position.y + 315 * size.y / 800),
									glm::vec2(this->position.x + 60 * size.x / 800, this->position.y + 440 * size.y / 800),
									glm::vec2(this->position.x + 225 * size.x / 800, this->position.y + 315 * size.y / 800),
									glm::vec2(this->position.x + 225 * size.x / 800, this->position.y + 440 * size.y / 800)));

	this->hitboxes.push_back(Square(glm::vec2(this->position.x + 225 * size.x / 800, this->position.y + 360 * size.y / 800),
									glm::vec2(this->position.x + 225 * size.x / 800, this->position.y + 440 * size.y / 800),
									glm::vec2(this->position.x + 375 * size.x / 800, this->position.y + 360 * size.y / 800),
									glm::vec2(this->position.x + 375 * size.x / 800, this->position.y + 440 * size.y / 800)));

	this->hitboxes.push_back(Square(glm::vec2(this->position.x + 375 * size.x / 800, this->position.y + 285 * size.y / 800),
									glm::vec2(this->position.x + 375 * size.x / 800, this->position.y + 690 * size.y / 800),
									glm::vec2(this->position.x + 405 * size.x / 800, this->position.y + 285 * size.y / 800),
									glm::vec2(this->position.x + 405 * size.x / 800, this->position.y + 690 * size.y / 800)));

	this->hitboxes.push_back(Square(glm::vec2(this->position.x + 405 * size.x / 800, this->position.y + 360 * size.y / 800),
									glm::vec2(this->position.x + 405 * size.x / 800, this->position.y + 440 * size.y / 800),
									glm::vec2(this->position.x + 555 * size.x / 800, this->position.y + 360 * size.y / 800),
									glm::vec2(this->position.x + 555 * size.x / 800, this->position.y + 440 * size.y / 800)));

	this->hitboxes.push_back(Square(glm::vec2(this->position.x + 555 * size.x / 800, this->position.y + 315 * size.y / 800),
									glm::vec2(this->position.x + 555 * size.x / 800, this->position.y + 440 * size.y / 800),
									glm::vec2(this->position.x + 720 * size.x / 800, this->position.y + 315 * size.y / 800),
									glm::vec2(this->position.x + 720 * size.x / 800, this->position.y + 440 * size.y / 800)));

	this->hit = false;
}

void Dragon::move(glm::vec2 move) {
	// Move the dragon
	this->position += move;
	// Move the hitboxes with the dragon
	for (int i = 0; i < this->hitboxes.size(); i++) {
		this->hitboxes[i].left_up += move;
		this->hitboxes[i].left_down += move;
		this->hitboxes[i].right_up += move;
		this->hitboxes[i].right_down += move;
	}
}

void Dragon::setVelocityModifier(float newVelMod) {
	this->velocityModifier = newVelMod;
}

void Dragon::Draw(SpriteRenderer& renderer, float dt) {
	const float FRAME_TIME = 0.0416667; // 24fps
	static float deltaTime;
	static int frame = 0;
	static bool reversed = false;
	static int frameStart;
	deltaTime += dt;
	if (this->hit) {
		float rColor = 0.5f + ((rand() % 100) / 100.0f);
		this->color = glm::vec3(rColor, rColor, rColor);
	}
	renderer.DrawSprite(this->animationFrames.at(frame), this->position, this->size, this->rotation, this->color);
	/*
	if (this->hit && frameStart == 30) {
		frameStart = frame;
	}
	*/

	if (deltaTime >= FRAME_TIME) {
		if (frame == 0) {
			frame++;
			if(reversed)
				reversed = false;
		}
		else if (frame == 7) {
			this->hit = false;
			this->color = glm::vec3(1.0f);
			frame--;
			if (!reversed)
				reversed = true;
			else {
				exit(-1); // can't reach the 8th frame of the animation while rendering it in reverse
			}
				
		}
		else { 
			if (reversed)
				frame--;
			else
				frame++;
		}	
		deltaTime = 0;
	}

	drawHitbox(renderer);
	
}

void Dragon::drawHitbox(SpriteRenderer& renderer) {
	
	for (Square s: this->hitboxes) {
		renderer.DrawSprite(ResourceManager::GetTexture("hitbox"), s.left_up, glm::vec3(10.0f), this->rotation, glm::vec3(1.0f, 1.0f, 1.0f));
		renderer.DrawSprite(ResourceManager::GetTexture("hitbox"), s.left_down, glm::vec3(10.0f), this->rotation, glm::vec3(1.0f, 0.0f, 0.0f));
		renderer.DrawSprite(ResourceManager::GetTexture("hitbox"), s.right_up, glm::vec3(10.0f), this->rotation, glm::vec3(0.0f, 1.0f, 0.0f));
		renderer.DrawSprite(ResourceManager::GetTexture("hitbox"), s.right_down, glm::vec3(10.0f), this->rotation, glm::vec3(0.0f, 0.0f, 1.0f));
	}

}

void Dragon::dealDamage(double damage) {
	this->stats.HP = this->stats.HP - damage;
	this->hit = true;
}
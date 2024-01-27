#include "dragon.h"
#include "../resource_manager/resource_manager.h"


Dragon::Dragon()
	:velocityModifier(0.0f) {};

Dragon::Dragon(Texture2D sprite, glm::vec2 pos, glm::vec2 size, glm::vec3 color, glm::vec2 velocity, float mvSpeed, hitboxType hitbox)
: velocityModifier(mvSpeed), animationFrames(NULL){
	this->position = pos;
	this->size = size;
	this->velocity = velocity;
	this->color = color;
	this->sprite = sprite;
	this->hitbox = hitbox;
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
	// Setta Hitboxes
	coord hitboxCoord;
	hitboxCoord.X = 45 * size.x / 800;
	//this->hitboxes.push_back(Square())
}

void Dragon::move(glm::vec2 move) {
	// TODO: Check bordi schermo
	this->position += move;
}

void Dragon::Draw(SpriteRenderer& renderer, float dt) {
	const float FRAME_TIME = 0.0416667; //24fps
	static float deltaTime;
	static int frame = 0;
	static bool reversed = false;

	deltaTime += dt;
	renderer.DrawSprite(this->animationFrames.at(frame), this->position, this->size, this->rotation, this->color);
	
	if (deltaTime >= FRAME_TIME) {
		if (frame == 0) {
			frame++;
			if(reversed)
				reversed = false;
		}
		else if (frame == 7) {
			frame--;
			if (!reversed)
				reversed = true;
			else
				exit(-1); // can't reach the 8th frame of the animation while rendering it in reverse
		}
		else { 
			if (reversed)
				frame--;
			else
				frame++;
		}	
		deltaTime = 0;
	}
}
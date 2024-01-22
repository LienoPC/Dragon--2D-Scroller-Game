#include "../resource_manager/resource_manager.h"
#include "dragon.h"


Dragon::Dragon()
	:velocityModifier(0.0f) {};

Dragon::Dragon(Texture2D sprite, glm::vec2 pos, glm::vec2 size, glm::vec3 color, glm::vec2 velocity, float mvSpeed, hitboxType hitbox)
: velocityModifier(mvSpeed), animationFrames(NULL){
	this->Position = pos;
	this->Size = size;
	this->Velocity = velocity;
	this->Color = color;
	this->Sprite = sprite;
	this->Hitbox = hitbox;
	this->Sprite = ResourceManager::GetTexture("dragon_f0");
	// Save dragon animation frames
	this->animationFrames.push_back(ResourceManager::GetTexture("dragon_f0"));
	this->animationFrames.push_back(ResourceManager::GetTexture("dragon_f1"));
	this->animationFrames.push_back(ResourceManager::GetTexture("dragon_f2"));
	this->animationFrames.push_back(ResourceManager::GetTexture("dragon_f3"));
	this->animationFrames.push_back(ResourceManager::GetTexture("dragon_f4"));
	this->animationFrames.push_back(ResourceManager::GetTexture("dragon_f5"));
	this->animationFrames.push_back(ResourceManager::GetTexture("dragon_f6"));
	this->animationFrames.push_back(ResourceManager::GetTexture("dragon_f7"));
	// TODO: Setta Hitboxes
}

void Dragon::move(glm::vec2 move) {
	// TODO: Check bordi schermo
	this->Position += move;
}

void Dragon::Draw(SpriteRenderer& renderer, float dt) {
	const float FRAME_TIME = 0.0416667; //24fps
	static float deltaTime;
	static int frame = 0;
	static bool reversed = false;

	deltaTime += dt;
	renderer.DrawSprite(this->animationFrames.at(frame), this->Position, this->Size, this->Rotation, this->Color);
	
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
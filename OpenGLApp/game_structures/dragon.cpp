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
	this->hitboxes.push_back(Square(glm::vec2(this->position.x + 45 * size.x / 800, this->position.y + 290 * size.y / 800),
									glm::vec2(this->position.x + 45 * size.x / 800, this->position.y + 475 * size.y / 800),
									glm::vec2(this->position.x + 250 * size.x / 800, this->position.y + 290 * size.y / 800),
									glm::vec2(this->position.x + 250 * size.x / 800, this->position.y + 475 * size.y / 800)));

	this->hitboxes.push_back(Square(glm::vec2(this->position.x + 250 * size.x / 800, this->position.y + 350 * size.y / 800),
									glm::vec2(this->position.x + 250 * size.x / 800, this->position.y + 465 * size.y / 800),
									glm::vec2(this->position.x + 385 * size.x / 800, this->position.y + 350 * size.y / 800),
									glm::vec2(this->position.x + 385 * size.x / 800, this->position.y + 465 * size.y / 800)));

	this->hitboxes.push_back(Square(glm::vec2(this->position.x + 385 * size.x / 800, this->position.y + 270 * size.y / 800),
									glm::vec2(this->position.x + 385 * size.x / 800, this->position.y + 690 * size.y / 800),
									glm::vec2(this->position.x + 415 * size.x / 800, this->position.y + 270 * size.y / 800),
									glm::vec2(this->position.x + 415 * size.x / 800, this->position.y + 690 * size.y / 800)));

	this->hitboxes.push_back(Square(glm::vec2(this->position.x + 415 * size.x / 800, this->position.y + 350 * size.y / 800),
									glm::vec2(this->position.x + 415 * size.x / 800, this->position.y +  465 * size.y / 800),
									glm::vec2(this->position.x + 550 * size.x / 800, this->position.y + 350 * size.y / 800),
									glm::vec2(this->position.x + 550 * size.x / 800, this->position.y + 465 * size.y / 800)));

	this->hitboxes.push_back(Square(glm::vec2(this->position.x + 550 * size.x / 800, this->position.y + 290 * size.y / 800),
									glm::vec2(this->position.x + 550 * size.x / 800, this->position.y + 475 * size.y / 800),
									glm::vec2(this->position.x + 755 * size.x / 800, this->position.y + 290 * size.y / 800),
									glm::vec2(this->position.x + 755 * size.x / 800, this->position.y + 475 * size.y / 800)));
}

void Dragon::move(glm::vec2 move) {
	// Move the dragon
	this->position += move;
	// Move the hitboxes with the dragon
	this->hitboxes[0].left_up = glm::vec2(this->position.x + 45 * size.x / 800, this->position.y + 290 * size.y / 800);
	this->hitboxes[0].left_down = glm::vec2(this->position.x + 45 * size.x / 800, this->position.y + 475 * size.y / 800);
	this->hitboxes[0].right_up = glm::vec2(this->position.x + 250 * size.x / 800, this->position.y + 290 * size.y / 800);
	this->hitboxes[0].right_down = glm::vec2(this->position.x + 250 * size.x / 800, this->position.y + 475 * size.y / 800);

	this->hitboxes[1].left_up = glm::vec2(this->position.x + 250 * size.x / 800, this->position.y + 350 * size.y / 800);
	this->hitboxes[1].left_down = glm::vec2(this->position.x + 250 * size.x / 800, this->position.y + 465 * size.y / 800);
	this->hitboxes[1].right_up = glm::vec2(this->position.x + 385 * size.x / 800, this->position.y + 350 * size.y / 800);
	this->hitboxes[1].right_down = glm::vec2(this->position.x + 385 * size.x / 800, this->position.y + 465 * size.y / 800);

	this->hitboxes[2].left_up = glm::vec2(this->position.x + 385 * size.x / 800, this->position.y + 270 * size.y / 800);
	this->hitboxes[2].left_down = glm::vec2(this->position.x + 385 * size.x / 800, this->position.y + 690 * size.y / 800);
	this->hitboxes[2].right_up = glm::vec2(this->position.x + 415 * size.x / 800, this->position.y + 270 * size.y / 800);
	this->hitboxes[2].right_down = glm::vec2(this->position.x + 415 * size.x / 800, this->position.y + 690 * size.y / 800);

	this->hitboxes[3].left_up = glm::vec2(this->position.x + 415 * size.x / 800, this->position.y + 350 * size.y / 800);
	this->hitboxes[3].left_down = glm::vec2(this->position.x + 415 * size.x / 800, this->position.y + 465 * size.y / 800);
	this->hitboxes[3].right_up = glm::vec2(this->position.x + 550 * size.x / 800, this->position.y + 350 * size.y / 800);
	this->hitboxes[3].right_down = glm::vec2(this->position.x + 550 * size.x / 800, this->position.y + 465 * size.y / 800);

	this->hitboxes[4].left_up = glm::vec2(this->position.x + 550 * size.x / 800, this->position.y + 290 * size.y / 800);
	this->hitboxes[4].left_down = glm::vec2(this->position.x + 550 * size.x / 800, this->position.y + 475 * size.y / 800);
	this->hitboxes[4].right_up = glm::vec2(this->position.x + 755 * size.x / 800, this->position.y + 290 * size.y / 800);
	this->hitboxes[4].right_down = glm::vec2(this->position.x + 755 * size.x / 800, this->position.y + 475 * size.y / 800);
}

void Dragon::Draw(SpriteRenderer& renderer, float dt) {
	const float FRAME_TIME = 0.0416667; // 24fps
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
}
#include "dragon.h"
#include "../resource_manager/resource_manager.h"
#include "./game_level.h"
#include "window_constraints.h"

Dragon::Dragon()
	:velocityModifier(0.0f), instancedFireballs(NULL){};

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
	for (Square& s : this->hitboxes) {
		s.left_up += move;
		s.left_down += move;
		s.right_up += move;
		s.right_down += move;
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
	deltaTime += dt;
	
	if (this->hit) {
		float rColor = 0.5f + ((rand() % 100) / 100.0f);
		this->color = glm::vec3(rColor, rColor, rColor);
	}
	
	
	renderer.DrawSprite(this->animationFrames.at(frame), this->position, this->size, this->rotation, this->color);

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

	for (int i = 0; i < this->instancedFireballs.size(); i++) {
		this->instancedFireballs[i].move(dt);
		instancedFireballs[i].Draw(renderer);
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

void Dragon::instanceFireball(glm::vec2 pos, double velocity) {
	Bullet fb(50.0f, 40, ResourceManager::GetTexture("fireball"), glm::vec2(200.0f, 200.0f), glm::vec2(50.0f, 90.0f), glm::vec3(1.0f), glm::vec2(0.6f), HitboxType(CIRCLE), (int)'c');
	pos.x = pos.x + 167.0;
	fb.position = pos;
	fb.Direction = glm::vec2(0, -1);
	fb.velApplied = velocity;
	fb.destroyed = false;
	this->instancedFireballs.push_back(fb);
}

void Dragon::checkFireballs() {
	for (int i = 0; i < this->instancedFireballs.size(); i++) {
		Bullet fb = this->instancedFireballs[i];
		if ((fb.position.y > LEV_LIMITY) || ((fb.position.y > LEV_LIMITY / 2) && (fb.position.x + fb.size.x < 0 || fb.position.x > LEV_LIMITX))) {
			// il bullet è uscito fuori dal livello
			if (i != this->instancedFireballs.size() - 1)
			{
				this->instancedFireballs[i] = std::move(this->instancedFireballs.back());
			}
			this->instancedFireballs.pop_back();
		}


		// valuto se il bullet è stato distrutto
		if (fb.destroyed == true) {
			// rimuovo il proiettile
			if (i != this->instancedFireballs.size() - 1) {
				this->instancedFireballs[i] = std::move(this->instancedFireballs.back());
			}
			this->instancedFireballs.pop_back();
		}
	}
}
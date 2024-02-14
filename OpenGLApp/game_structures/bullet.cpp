#include "bullet.h"
#include "../glfw-3.3.8.bin.WIN64/include/GLFW/glfw3.h"
#include <memory>
#include "../resource_manager/resource_manager.h"

Bullet::Bullet() {}

Bullet::Bullet(float pow, float partNum, Texture2D sprite, glm::vec2 pos, glm::vec2 size, glm::vec3 color, glm::vec2 velocity, HitboxType hitboxT, int type){
	this->position = pos;
	this->size = size;
	this->velocity = velocity;
	this->color = color;
	this->sprite = sprite;
	this->Power = pow;
	this->ParticlesNumber = partNum;
	this->hitboxT = hitboxT;
	this->destroyed = false;
	std::shared_ptr<Square> s;
	std::shared_ptr<Circle> c;
	// Hitbox creation
	switch (hitboxT) {

	case SQUARE:
	{
		s = std::make_shared<Square>(Square(this->position, glm::vec2(this->position.x, this->position.y + this->size.y), glm::vec2(this->position.x + this->size.x, this->position.y), this->position + this->size));
		this->hitbox = s;
		
		
	}
	break;
		

	case CIRCLE:
	{
		c = std::make_shared<Circle>(Circle((float)std::max(this->size.x, this->size.y), this->position + (glm::vec2(this->size.x / 2, this->size.y / 2))));
		this->hitbox = c;

	}	
	break;
	
	}

	
	this->Type = type;
	
}

   
 void Bullet::copyBullet(Bullet b) {
	this->position = b.position;
	this->size = b.size;
	this->velocity = b.velocity;
	this->color = b.color;
	this->sprite = b.sprite;
	this->Power = b.Power;
	this->ParticlesNumber = b.ParticlesNumber;
	this->hitboxT = b.hitboxT;
	std::shared_ptr<Square> s;
	std::shared_ptr<Circle> c;
	// Hitbox creation
	switch (hitboxT) {

	case SQUARE:
		s = std::make_shared<Square>(Square(this->position, glm::vec2(this->position.x, this->position.y + this->size.y), glm::vec2(this->position.x + this->size.x, this->position.y), this->position + this->size));
		this->hitbox = s;
		break;

	case CIRCLE:
		c = std::make_shared<Circle>(Circle((float)std::max(this->size.x, this->size.y), this->position + (glm::vec2(this->size.x / 2, this->size.y / 2))));
		this->hitbox = c;
		break;
	}

}


void Bullet::move(glm::vec2 move) {
	this->position += move*this->velocity*this->velApplied;
	// aggiorna la hitbox
	updateHitbox();
}

void Bullet::move(float dt) {
	this->position += this->velocity * this->Direction*this->velApplied *dt;
	this->position.y += SCROLLING_SPEED*dt; // Sync the scrolling level with bullets
	// aggiorna la hitbox
	updateHitbox();
}

void Bullet::syncRotation() {

	float cosA = this->Direction.x;
	float degrees = std::acos(cosA) * (180.0 / 3.141592653589793238463);
	this->rotation = degrees+90;
	// aggiorna la hitbox
	updateHitbox();
}

void Bullet::updateHitbox() {
	// set new left-up (origin of the sprite)
	glm::vec4 pos;
	switch (this->hitboxT) {

	case SQUARE:
	{
		std::shared_ptr<Square> s = std::dynamic_pointer_cast<Square>(this->hitbox);
		
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(this->position, 0.0f));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)

		model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); // move origin of rotation to center of quad
		model = glm::rotate(model, glm::radians(this->rotation+180), glm::vec3(0.0f, 0.0f, 1.0f)); // then rotate
		model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // move origin back
		model = glm::translate(model, glm::vec3(-this->position, 0.0f));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)


		s->left_up = this->position;
		s->left_down = glm::vec2(this->position.x, this->position.y + this->size.y);
		s->right_down = this->position + this->size;
		s->right_up = glm::vec2(this->position.x + this->size.x, this->position.y);

		//ruoto i punti
		pos = model * glm::vec4(s->left_up, 0.0, 1.0);
		s->left_up.x = pos.x;
		s->left_up.y = pos.y;

		pos = model * glm::vec4(s->left_down, 0.0, 1.0);
		s->left_down.x = pos.x;
		s->left_down.y = pos.y;
		
		pos = model * glm::vec4(s->right_down, 0.0, 1.0);
		s->right_down.x = pos.x;
		s->right_down.y = pos.y;

		pos = model * glm::vec4(s->right_up, 0.0, 1.0);
		s->right_up.x = pos.x;
		s->right_up.y = pos.y;
		
		
		
		
	}

	break;

	case CIRCLE:
	{
		std::shared_ptr<Circle> c = std::dynamic_pointer_cast<Circle>(this->hitbox);
		c->center = this->position + (glm::vec2(this->size.x / 2, this->size.y / 2));
	}
	break;
	}

}

void Bullet::Draw(SpriteRenderer& renderer) {

	renderer.DrawSprite(this->sprite, this->position, this->size, this->rotation, this->color);
	switch (this->hitboxT) {

	case SQUARE:
		{
			Square s = *std::dynamic_pointer_cast<Square>(this->hitbox);
			renderer.DrawSprite(ResourceManager::GetTexture("hitbox"), s.left_up, glm::vec3(10.0f), this->rotation, glm::vec3(1.0f,1.0f,1.0f));
			renderer.DrawSprite(ResourceManager::GetTexture("hitbox"), s.left_down, glm::vec3(10.0f), this->rotation, glm::vec3(0.0f, 0.0f, 0.0f));
			renderer.DrawSprite(ResourceManager::GetTexture("hitbox"), s.right_up, glm::vec3(10.0f), this->rotation, glm::vec3(0.0f, 0.0f, 0.0f));
			renderer.DrawSprite(ResourceManager::GetTexture("hitbox"), s.right_down, glm::vec3(10.0f), this->rotation, glm::vec3(0.0f, 0.0f, 0.0f));
	}
		break;
	case CIRCLE:
		{

		}
		break;


	}
}




void Bullet::destroy() {

	this->destroyed = true;
}



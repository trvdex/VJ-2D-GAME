#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Spider.h"
#include "Game.h"

#define JUMP_ANGLE_STEP 5
#define JUMP_HEIGHT 96
#define FALL_STEP 4

enum SpiderAnims
{
	STANDING, MOVING, ATTACK_LEFT, ATTACK_RIGHT
};

enum retreatState
{
	NO_RETREAT, RETREAT_UP, RETREAT_DOWN
};

void Spider::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	dropped = false;
	bullet = new Bullet();
	bullet->init(glm::ivec2(32, 16), shaderProgram);
	
	retreat = NO_RETREAT;
	retreatDistnace = 0;
	timer = 0;
	shoot = false;

	health = 1; // Iniciamos con 1 PUNTO DE VIDA
	bDead = false;
	invulnerabilityTime = 0;
	sizeSpider = glm::ivec2(16, 16);
	spritesheet.loadFromFile("sprites/spritesheet_spider.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	// El tamaño de cada sprite es 32x32
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.25f, 0.5f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);

	sprite->setAnimationSpeed(STANDING, 8);
	sprite->addKeyframe(STANDING, glm::vec2(0.0f, 0.f));

	sprite->setAnimationSpeed(MOVING, 8);
	sprite->addKeyframe(MOVING, glm::vec2(0.25f, 0.f));
	sprite->addKeyframe(MOVING, glm::vec2(0.5f, 0.f));

	sprite->setAnimationSpeed(ATTACK_LEFT, 8);
	sprite->addKeyframe(ATTACK_LEFT, glm::vec2(0.75f, 0.f));

	sprite->setAnimationSpeed(ATTACK_RIGHT, 8);
	sprite->addKeyframe(ATTACK_RIGHT, glm::vec2(0.0f, 0.5f));

	sprite->changeAnimation(STANDING);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posSpider.x), float(tileMapDispl.y + posSpider.y)));
}

void Spider::update(int deltaTime, const glm::ivec2 playerPos)
{
	sprite->update(deltaTime);

	// Actualizar tiempo de invulnerabilidad
	if (invulnerabilityTime > 0) {
		invulnerabilityTime -= deltaTime;
		if (invulnerabilityTime <= 0) {
			invulnerabilityTime = 0;
		}
	}

	// Si el enemigo está muerto, no seguir con el update
	if (bDead)
		return;

	if (shoot) timer += deltaTime;

	int horizontalDistance = abs(playerPos.x - posSpider.x);
	int verticalDistance = abs(playerPos.y - posSpider.y);
	if (retreat == NO_RETREAT && horizontalDistance < 300 && verticalDistance < 200)
	{
		//printf("Spider pos: %d %d\n", posSpider.x, posSpider.y);
		int distance = (playerPos.y + 40) - posSpider.y;

		if (distance > 2 && distance < 200) //DOWN
		{
			if (sprite->animation() != MOVING)
				sprite->changeAnimation(MOVING);
			posSpider.y += 1;
		}
		else if (distance < -2 && distance > -200) //UP
		{
			if (sprite->animation() != MOVING)
				sprite->changeAnimation(MOVING);
			posSpider.y -= 1;
		}
		else if (distance <= 2 && distance >= -2)
		{
			if (playerPos.x >= posSpider.x) //SHOOT RIGHT
			{
				if (sprite->animation() != ATTACK_RIGHT)
					sprite->changeAnimation(ATTACK_RIGHT);
				if (horizontalDistance < 200)
				{
					shoot = true;
					
				}
				if (timer > 200)
				{
					shoot = false;
					timer = 0;
					AudioEngine::getInstance().play2D("sounds/spider_shoot_sound.mp3", false, 0.3f);
					bullet->setPosition(glm::vec2(float(tileMapDispl.x + posSpider.x - 30), float(tileMapDispl.y + posSpider.y - 16)));
					bullet->changeOrientation(2);
					sprite->changeAnimation(MOVING);
					if ((playerPos.y + 40) >= posSpider.y) retreat = RETREAT_UP;
					else retreat = RETREAT_DOWN;
				}
			}
			else //SHOOT LEFT
			{
				if (sprite->animation() != ATTACK_LEFT)
					sprite->changeAnimation(ATTACK_LEFT);
				if (horizontalDistance < 100)
				{
					shoot = true;
				}
				if (timer > 200)
				{
					shoot = false;
					timer = 0;
					AudioEngine::getInstance().play2D("sounds/spider_shoot_sound.mp3", false, 0.3f);
					bullet->setPosition(glm::vec2(float(tileMapDispl.x + posSpider.x - 30), float(tileMapDispl.y + posSpider.y - 16)));
					bullet->changeOrientation(1);
					sprite->changeAnimation(MOVING);
					if ((playerPos.y + 40) >= posSpider.y) retreat = RETREAT_UP;
					else retreat = RETREAT_DOWN;
				}
			}
		}
		else
		{
			if (sprite->animation() != STANDING)
				sprite->changeAnimation(STANDING);
		}
	}
	else if (retreat == RETREAT_UP)
	{
		posSpider.y -= 3;
		retreatDistnace += 3;
		if (retreatDistnace > 120)
		{
			retreat = NO_RETREAT;
			retreatDistnace = 0;
		}
	}
	else if (retreat == RETREAT_DOWN)
	{
		posSpider.y += 3;
		retreatDistnace += 3;
		if (retreatDistnace > 120)
		{
			retreat = NO_RETREAT;
			retreatDistnace = 0;
		}
	}

	//MAP LIMITS
	if (posSpider.y >= 1584 + 32)
	{
		posSpider.y = 1584 + 32;
		retreat = NO_RETREAT;
		retreatDistnace = 0;
	}

	if (posSpider.x == 2080 && posSpider.y < 565+64)
	{
		if (posSpider.y >= 560 + 32)
		{
			posSpider.y = 560 + 32;
			retreat = NO_RETREAT;
			retreatDistnace = 0;
		}
	}

	if (posSpider.x == 2288 && posSpider.y < 565 + 64)
	{
		if (posSpider.y >= 560 + 32)
		{
			posSpider.y = 560 + 32;
			retreat = NO_RETREAT;
			retreatDistnace = 0;
		}
	}

	if (posSpider.x == 2080 && posSpider.y > 1345)
	{
		if (posSpider.y <= 1350)
		{
			posSpider.y = 1350;
			retreat = NO_RETREAT;
			retreatDistnace = 0;
		}
	}

	if (posSpider.x == 2288 && posSpider.y > 1345)
	{
		if (posSpider.y <= 1350)
		{
			posSpider.y = 1350;
			retreat = NO_RETREAT;
			retreatDistnace = 0;
		}
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posSpider.x), float(tileMapDispl.y + posSpider.y)));
	bullet->update(deltaTime);
}

void Spider::render()
{
	// Si está muerto, no renderizar
	if (bDead)
		return;

	// Efecto de parpadeo si está invulnerable
	if (invulnerabilityTime > 0 && (invulnerabilityTime / 100) % 2 == 0) {
		// No renderizar (efecto de parpadeo)
	}
	else {
		sprite->render();
		bullet->render();
	}
}

void Spider::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Spider::setPosition(const glm::vec2& pos)
{
	posSpider = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posSpider.x), float(tileMapDispl.y + posSpider.y)));
	bullet->setPosition(glm::vec2(float(tileMapDispl.x + posSpider.x), float(tileMapDispl.y + posSpider.y - 10)));
}

glm::ivec2 Spider::getPos()
{
	return posSpider;
}

glm::ivec2 Spider::getSize()
{
	return sizeSpider;
}

void Spider::takeDamage()
{
	// No tomar daño si está en periodo de invulnerabilidad
	if (invulnerabilityTime > 0)
		return;

	health--;

	// Si ya no tiene vida, marcar como muerto
	if (health <= 0) {
		bDead = true;
		AudioEngine::getInstance().play2D("sounds/enemy_death.mp3", false, 0.4f);
		return;
	}

	// Hacer invulnerable por un corto periodo
	invulnerabilityTime = 800; // 800ms de invulnerabilidad

}

bool Spider::isDead() const
{
	return bDead;
}

glm::ivec2 Spider::getBulletPos()
{
	return bullet->getPos();
}

glm::ivec2 Spider::getBulletSize()
{
	return bullet->getSize();
}
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Mole.h"
#include "Game.h"

#define JUMP_ANGLE_STEP 5
#define JUMP_HEIGHT 96
#define FALL_STEP 4

enum MoleAnims
{
	MOVING_LEFT, MOVING_RIGHT
};

enum MoleOrientations
{
	LEFT, RIGHT
};


void Mole::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	timer = 0;
	health = 3; // Iniciamos con 3 puntos de vida
	bDead = false;
	invulnerabilityTime = 0;
	sizeMole = glm::ivec2(32, 32);
	jumping = false;
	spritesheet.loadFromFile("sprites/spritesheet_mole.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	// El tamaño de cada sprite es 32x32
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.25f, 0.5f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(MOVING_LEFT, 6);
	sprite->addKeyframe(MOVING_LEFT, glm::vec2(0.0f, 0.f));
	sprite->addKeyframe(MOVING_LEFT, glm::vec2(0.25f, 0.f));

	sprite->setAnimationSpeed(MOVING_RIGHT, 6);
	sprite->addKeyframe(MOVING_RIGHT, glm::vec2(0.5f, 0.f));
	sprite->addKeyframe(MOVING_RIGHT, glm::vec2(0.75f, 0.f));

	sprite->changeAnimation(MOVING_LEFT);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posMole.x), float(tileMapDispl.y + posMole.y)));
}

void Mole::update(int deltaTime, const glm::ivec2 playerPos)
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

	int horizontalDistance = abs(playerPos.x - posMole.x);
	int verticalDistance = abs(playerPos.y - posMole.y);
	if (horizontalDistance < 300 && verticalDistance < 200)
	{
		if (playerPos.x < posMole.x)
		{
			if (sprite->animation() != MOVING_LEFT)
				sprite->changeAnimation(MOVING_LEFT);
			posMole.x -= 1;
			if (map->collisionMoveLeft(posMole, glm::ivec2(32, 32)))
				posMole.x += 1;
		}
		else if (playerPos.x > posMole.x)
		{
			if (sprite->animation() != MOVING_RIGHT)
				sprite->changeAnimation(MOVING_RIGHT);
			posMole.x += 1.5;
			if (map->collisionMoveRight(posMole, glm::ivec2(32, 32)))
				posMole.x -= 1.5;
		}
		else
		{
			if (sprite->animation() != MOVING_LEFT)
				sprite->changeAnimation(MOVING_LEFT);
		}

		if (!jumping && jumpAngle == 0 && (playerPos.y < (posMole.y - 60))) timer += deltaTime;
		if (timer > 500)
		{
			jumping = true;
			timer = 0;
		}

		if (jumping)
		{
			jumpAngle += JUMP_ANGLE_STEP;
			if (jumpAngle == 180)
			{
				jumping = false;
				posMole.y = startY;
			}
			else
			{
				posMole.y = int(startY - 70 * sin(3.14159f * jumpAngle / 180.f));
				if (jumpAngle > 90)
					jumping = !map->collisionMoveDown(posMole, glm::ivec2(32, 32), &posMole.y);
			}
		}
		else
		{
			posMole.y += FALL_STEP;
			if (map->collisionMoveDown(posMole, sizeMole, &posMole.y))
			{
				jumpAngle = 0;
				startY = posMole.y;
			}
		}
	}
	if (posMole.x >= 3806) posMole.x = 3806; //cant enter boss room
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posMole.x), float(tileMapDispl.y + posMole.y)));
}

void Mole::render()
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
	}
}

void Mole::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Mole::setPosition(const glm::vec2& pos)
{
	posMole = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posMole.x), float(tileMapDispl.y + posMole.y)));
}

glm::ivec2 Mole::getPos()
{
	return posMole;
}

glm::ivec2 Mole::getSize()
{
	return sizeMole;
}

void Mole::takeDamage(int damage)
{
	// No tomar daño si está en periodo de invulnerabilidad
	if (invulnerabilityTime > 0)
		return;

	health -= damage;

	// Si ya no tiene vida, marcar como muerto
	if (health <= 0) {
		bDead = true;
		AudioEngine::getInstance().play2D("sounds/enemy_death.mp3", false, 0.4f);
		return;
	}

	// Hacer invulnerable por un corto periodo
	invulnerabilityTime = 800; // 800ms de invulnerabilidad

	// Hacer que salte cuando recibe daño
	if (!jumping) {
		jumping = true;
		jumpAngle = 0;
		startY = posMole.y;
	}
}

bool Mole::isDead() const
{
	return bDead;
}
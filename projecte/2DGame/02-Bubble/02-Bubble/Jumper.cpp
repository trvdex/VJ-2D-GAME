#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Jumper.h"
#include "Game.h"
#include "Scene.h"

#define JUMP_ANGLE_STEP 5
#define JUMP_HEIGHT 96
#define FALL_STEP 4

enum JumperAnims
{
	STANDING_LEFT, STANDING_RIGHT, START_LEFT, START_RIGHT, JUMP_LEFT, JUMP_RIGHT
};

enum jumperOrientations
{
	LEFT, RIGHT
};


void Jumper::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	timer = 0;
	health = 2; // Iniciamos con 2 puntos de vida
	dropped = false;
	bDead = false;
	invulnerabilityTime = 0;
	sizeJumper = glm::ivec2(32, 32);
	orientation = LEFT; //starts jumping to the left
	jumping = false;
	spritesheet.loadFromFile("sprites/spritesheet_ogre.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
    // El tamaño de cada sprite es 32x32
    // El tamaño relativo en la textura es 0.2x1.0 (32/160=0.2, 32/32=1.0)
    sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.2f, 0.5f), &spritesheet, &shaderProgram);
    sprite->setNumberAnimations(6);

	sprite->setAnimationSpeed(STANDING_LEFT, 8);
	sprite->addKeyframe(STANDING_LEFT, glm::vec2(0.0f, 0.f));

	sprite->setAnimationSpeed(STANDING_RIGHT, 8);
	sprite->addKeyframe(STANDING_RIGHT, glm::vec2(0.8f, 0.5f));

	sprite->setAnimationSpeed(START_LEFT, 8);
	sprite->addKeyframe(START_LEFT, glm::vec2(0.2f, 0.f));

	sprite->setAnimationSpeed(START_RIGHT, 8);
	sprite->addKeyframe(START_RIGHT, glm::vec2(0.6f, 0.5f));

	sprite->setAnimationSpeed(JUMP_LEFT, 12);
	sprite->addKeyframe(JUMP_LEFT, glm::vec2(0.4f, 0.f));
	sprite->addKeyframe(JUMP_LEFT, glm::vec2(0.6f, 0.f));
	sprite->addKeyframe(JUMP_LEFT, glm::vec2(0.8f, 0.f));

	sprite->setAnimationSpeed(JUMP_RIGHT, 12);
	sprite->addKeyframe(JUMP_RIGHT, glm::vec2(0.4f, 0.5f));
	sprite->addKeyframe(JUMP_RIGHT, glm::vec2(0.2f, 0.5f));	
	sprite->addKeyframe(JUMP_RIGHT, glm::vec2(0.0f, 0.5f));

    sprite->changeAnimation(JUMP_RIGHT);
    tileMapDispl = tileMapPos;
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posJumper.x), float(tileMapDispl.y + posJumper.y)));
}

void Jumper::update(int deltaTime, const glm::ivec2 playerPos)
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

	int horizontalDistance = abs(playerPos.x - posJumper.x);
	int verticalDistance = abs(playerPos.y - posJumper.y);
	if (horizontalDistance < 300 && verticalDistance < 200)
	{
		timer += deltaTime;

		if (timer < 1000)
		{
			if (playerPos.x < posJumper.x)
			{
				map->collisionMoveDown(posJumper + glm::ivec2(10, 0), glm::ivec2(32 - 20, 32), &posJumper.y);
				orientation = LEFT;
				if (sprite->animation() != STANDING_LEFT)
					sprite->changeAnimation(STANDING_LEFT);
			}
			else if (playerPos.x > posJumper.x)
			{
				map->collisionMoveDown(posJumper + glm::ivec2(10, 0), glm::ivec2(32 - 20, 32), &posJumper.y);
				orientation = RIGHT;
				if (sprite->animation() != STANDING_RIGHT)
					sprite->changeAnimation(STANDING_RIGHT);
			}
		}
		else
		{
			if (orientation == LEFT) //left jump
			{
				if (sprite->animation() != JUMP_LEFT)
					sprite->changeAnimation(JUMP_LEFT);
				posJumper.x -= 2;
				if (map->collisionMoveLeft(posJumper, sizeJumper))
				{
					posJumper.x += 2;
					orientation = RIGHT;
				}
			}
			else //right jump
			{
				if (sprite->animation() != JUMP_RIGHT)
					sprite->changeAnimation(JUMP_RIGHT);
				posJumper.x += 2;
				if (map->collisionMoveRight(posJumper, sizeJumper))
				{
					posJumper.x -= 2;
					orientation = RIGHT;
				}
			}

			if (jumping)
			{
				jumpAngle += JUMP_ANGLE_STEP;
				if (jumpAngle == 180)
				{
					jumping = false;
					posJumper.y = startY;
				}
				else
				{
					posJumper.y = int(startY - 50 * sin(3.14159f * jumpAngle / 180.f));
					if (jumpAngle > 90)
						jumping = !map->collisionMoveDown(posJumper + glm::ivec2(10, 0), glm::ivec2(32 - 20, 32), &posJumper.y);
				}
			}
			else
			{
				posJumper.y += FALL_STEP;
				if (map->collisionMoveDown(posJumper + glm::ivec2(10, 0), glm::ivec2(32 - 20, 32), &posJumper.y))
				{
					timer = 0;
					jumping = true;
					jumpAngle = 0;
					startY = posJumper.y;
				}
			}
		}
	}
	
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posJumper.x), float(tileMapDispl.y + posJumper.y)));
}

void Jumper::render()
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

void Jumper::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Jumper::setPosition(const glm::vec2 &pos)
{
	posJumper = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posJumper.x), float(tileMapDispl.y + posJumper.y)));
}

glm::ivec2 Jumper::getPos()
{
	return posJumper;
}

glm::ivec2 Jumper::getSize()
{
	return sizeJumper;
}

void Jumper::takeDamage(int damage)
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
	invulnerabilityTime = 800;

	// Hacer que salte cuando recibe daño
	if (!jumping) {
		jumping = true;
		jumpAngle = 0;
		startY = posJumper.y;
	}
}

bool Jumper::isDead() const
{
	return bDead;
}
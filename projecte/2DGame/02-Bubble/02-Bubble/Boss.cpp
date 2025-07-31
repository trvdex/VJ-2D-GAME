#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Boss.h"
#include "Game.h"
#include "Scene.h"

#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4

enum BossAnims
{
	JUMP_LEFT, JUMP_RIGHT, STANDING
};

enum BossOrientations
{
	LEFT, RIGHT
};


void Boss::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	entranceDone = false;
	sound_played = false;
	sound_played2 = false;
	timer = 0;
	health = 15; // Iniciamos con 3 puntos de vida
	bDead = false;
	invulnerabilityTime = 0;
	sizeBoss = glm::ivec2(64, 64);
	orientation = LEFT; //starts jumping to the left
	jumping = false;
	spritesheet.loadFromFile("sprites/spritesheet_boss.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);

	sprite = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(0.25f, 0.5f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(3);

	sprite->setAnimationSpeed(JUMP_LEFT, 2);
	sprite->addKeyframe(JUMP_LEFT, glm::vec2(0.0f, 0.5f));
	sprite->addKeyframe(JUMP_LEFT, glm::vec2(0.25f, 0.5f));

	sprite->setAnimationSpeed(JUMP_RIGHT, 2);
	sprite->addKeyframe(JUMP_RIGHT, glm::vec2(0.0f, 0.0f));
	sprite->addKeyframe(JUMP_RIGHT, glm::vec2(0.25f, 0.0f));

	sprite->setAnimationSpeed(STANDING, 3);
	sprite->addKeyframe(STANDING, glm::vec2(0.5f, 0.0f));
	sprite->addKeyframe(STANDING, glm::vec2(0.75f, 0.0f));

	sprite->changeAnimation(STANDING);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBoss.x), float(tileMapDispl.y + posBoss.y)));


	//FACE 1
	face1 = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(0.25f, 0.5f), &spritesheet, &shaderProgram);
	face1->setNumberAnimations(1);

	face1->setAnimationSpeed(0, 2);
	face1->addKeyframe(0, glm::vec2(0.5f, 0.0f));
	face1->addKeyframe(0, glm::vec2(0.75f, 0.0f));

	face1->changeAnimation(0);
	face1->setPosition(glm::vec2(float(tileMapDispl.x + posBoss.x), float(tileMapDispl.y + posBoss.y)));

	//FACE 2
	face2 = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(0.25f, 0.5f), &spritesheet, &shaderProgram);
	face2->setNumberAnimations(1);

	face2->setAnimationSpeed(0, 2);
	face2->addKeyframe(0, glm::vec2(0.5f, 0.0f));
	face2->addKeyframe(0, glm::vec2(0.75f, 0.0f));

	face2->changeAnimation(0);
	face2->setPosition(glm::vec2(float(tileMapDispl.x + posBoss.x), float(tileMapDispl.y + posBoss.y)));

	//FACE 3
	face3 = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(0.25f, 0.5f), &spritesheet, &shaderProgram);
	face3->setNumberAnimations(1);

	face3->setAnimationSpeed(0, 2);
	face3->addKeyframe(0, glm::vec2(0.5f, 0.0f));
	face3->addKeyframe(0, glm::vec2(0.75f, 0.0f));

	face3->changeAnimation(0);
	face3->setPosition(glm::vec2(float(tileMapDispl.x + posBoss.x), float(tileMapDispl.y + posBoss.y)));

	//FACE 4
	face4 = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(0.25f, 0.5f), &spritesheet, &shaderProgram);
	face4->setNumberAnimations(1);

	face4->setAnimationSpeed(0, 2);
	face4->addKeyframe(0, glm::vec2(0.5f, 0.0f));
	face4->addKeyframe(0, glm::vec2(0.75f, 0.0f));

	face4->changeAnimation(0);
	face4->setPosition(glm::vec2(float(tileMapDispl.x + posBoss.x), float(tileMapDispl.y + posBoss.y)));
}

void Boss::update(int deltaTime, bool isBossLocked)
{
	sprite->update(deltaTime);

	// Si el enemigo está muerto, no seguir con el update
	if (bDead)
		return;

	// Actualizar tiempo de invulnerabilidad
	if (invulnerabilityTime > 0) {
		if (sprite->animation() != STANDING) {
			sprite->changeAnimation(STANDING);
		}
		invulnerabilityTime -= deltaTime;
		if (invulnerabilityTime <= 0) {
			invulnerabilityTime = 0;
		}
		face1->update(deltaTime);
		face2->update(deltaTime);
		face3->update(deltaTime);
		face4->update(deltaTime);
		face1pos.x = tileMapDispl.x + posBoss.x + 50 * sin(invulnerabilityTime * 0.003);
		face1pos.y = tileMapDispl.y + posBoss.y;

		face2pos.x = tileMapDispl.x + posBoss.x - 50 * sin(invulnerabilityTime * 0.003);
		face2pos.y = tileMapDispl.y + posBoss.y;

		face3pos.x = tileMapDispl.x + posBoss.x;
		face3pos.y = tileMapDispl.y + posBoss.y + 50 * sin(invulnerabilityTime * 0.003);

		face4pos.x = tileMapDispl.x + posBoss.x;
		face4pos.y = tileMapDispl.y + posBoss.y - 50 * sin(invulnerabilityTime * 0.003);

		face1->setPosition(face1pos);
		face2->setPosition(face2pos);
		face3->setPosition(face3pos);
		face4->setPosition(face4pos);

	}

	else if (isBossLocked)
	{
		if (posBoss.y >= 550 && !entranceDone) //ENTRANCE ANIMATION
		{
			posBoss.y -= 1;
		}
		else if (!entranceDone)
		{
			entranceDone = true;
		}
		else
		{
			if (orientation == LEFT) //Player is at the left
			{
				if (sprite->animation() != JUMP_LEFT)
					sprite->changeAnimation(JUMP_LEFT);
				posBoss.x -= 2;
				if (posBoss.x <= 3820)
				{
					posBoss.x += 1.5;
					orientation = RIGHT;
				}
			}
			else //Player is at the right
			{
				if (sprite->animation() != JUMP_RIGHT)
					sprite->changeAnimation(JUMP_RIGHT);
				posBoss.x += 2;
				if (posBoss.x >= 4035)
				{
					posBoss.x -= 1.5;
					orientation = LEFT;
				}
			}
			if (jumping)
			{
				jumpAngle += JUMP_ANGLE_STEP;
				if (jumpAngle == 180)
				{
					jumping = false;
					posBoss.y = startY;
				}
				else
				{
					posBoss.y = int(startY - 50 * sin(3.14159f * jumpAngle / 180.f));
					if (jumpAngle > 90)
						jumping = !map->collisionMoveDown(posBoss + glm::ivec2(15, 0), sizeBoss + glm::ivec2(-30, -5), &posBoss.y);
				}
			}
			else
			{
				posBoss.y += FALL_STEP;
				if (map->collisionMoveDown(posBoss + glm::ivec2(15, 0), sizeBoss + glm::ivec2 (-30, -5), &posBoss.y))
				{
					timer = 0;
					jumping = true;
					jumpAngle = 0;
					startY = posBoss.y;
				}
			}
		}	
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBoss.x), float(tileMapDispl.y + posBoss.y)));
}

void Boss::render()
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
		if (invulnerabilityTime > 0 && invulnerabilityTime <= 2000)
		{
			face1->render();
			face2->render();
			face3->render();
			face4->render();
			if (!sound_played)
			{
				AudioEngine::getInstance().play2D("sounds/boss_attack.mp3", false, 0.2f);
				sound_played = true;
			}
			if (invulnerabilityTime <= 1000 && !sound_played2)
			{
				AudioEngine::getInstance().play2D("sounds/boss_attack.mp3", false, 0.2f);
				sound_played2 = true;
			}
		}
		else
		{
			sound_played = false;
			sound_played2 = false;
		}
	}
}

void Boss::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Boss::setPosition(const glm::vec2& pos)
{
	posBoss = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBoss.x), float(tileMapDispl.y + posBoss.y)));
}

glm::ivec2 Boss::getPos()
{
	return posBoss;
}

glm::ivec2 Boss::getSize()
{
	return sizeBoss;
}

void Boss::takeDamage()
{
	// No tomar daño si está en periodo de invulnerabilidad
	if (invulnerabilityTime > 0 || !entranceDone)
		return;

	health--;

	// Si ya no tiene vida, marcar como muerto
	if (health <= 0) {
		bDead = true;
		return;
	}

	// Hacer invulnerable por un corto periodo
	invulnerabilityTime = 2500;

	// Hacer que salte cuando recibe daño
	if (!jumping) {
		jumping = true;
		jumpAngle = 0;
		startY = posBoss.y;
	}
}

bool Boss::isDead() const
{
	return bDead;
}

glm::vec2 Boss::getFace1Pos()
{
	return face1pos;
}

glm::vec2 Boss::getFace2Pos()
{
	return face2pos;
}

glm::vec2 Boss::getFace3Pos()
{
	return face3pos;
}

glm::vec2 Boss::getFace4Pos()
{
	return face4pos;
}

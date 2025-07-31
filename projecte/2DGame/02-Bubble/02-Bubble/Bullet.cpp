#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Bullet.h"
#include "Game.h"

enum bulletAnims
{
	BULLET_LEFT, BULLET_RIGHT
};

enum bulletOrientations
{
	STANDBY, LEFT, RIGHT
};

void Bullet::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	orientation = STANDBY;
	sizeBullet = glm::ivec2(16, 16);
	spritesheet.loadFromFile("sprites/spritesheet_spider.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	// El tamaño de cada sprite es 32x32
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.25f, 0.5f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(BULLET_LEFT, 8);
	sprite->addKeyframe(BULLET_LEFT, glm::vec2(0.25f, 0.5f));

	sprite->setAnimationSpeed(BULLET_RIGHT, 8);
	sprite->addKeyframe(BULLET_RIGHT, glm::vec2(0.5f, 0.5f));

	sprite->changeAnimation(BULLET_LEFT);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBullet.x), float(tileMapDispl.y + posBullet.y)));
}

void Bullet::update(int deltaTime)
{
	if (orientation == LEFT) {
		posBullet.x -= 2;
		if (sprite->animation() != BULLET_LEFT)
			sprite->changeAnimation(BULLET_LEFT);
	}
	else {
		posBullet.x += 2;
		if (sprite->animation() != BULLET_RIGHT)
			sprite->changeAnimation(BULLET_RIGHT);
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBullet.x), float(tileMapDispl.y + posBullet.y)));
}

void Bullet::render()
{
	sprite->render();
	//printf("PosBullet: %d %d\n", posBullet.x, posBullet.y);
}

void Bullet::setPosition(const glm::vec2& pos)
{
	posBullet = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBullet.x), float(tileMapDispl.y + posBullet.y)));
}

void Bullet::changeOrientation(int orientation)
{
	this->orientation = orientation;
	if (orientation == LEFT) {
		if (sprite->animation() != BULLET_LEFT)
			sprite->changeAnimation(BULLET_LEFT);
	}
	else {
		if (sprite->animation() != BULLET_RIGHT)
			sprite->changeAnimation(BULLET_RIGHT);
	}
}

glm::ivec2 Bullet::getPos()
{
	return posBullet;
}

glm::ivec2 Bullet::getSize()
{
	return sizeBullet;
}
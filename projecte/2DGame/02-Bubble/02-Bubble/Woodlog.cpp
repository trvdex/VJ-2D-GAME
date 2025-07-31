#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Woodlog.h"
#include "Game.h"

enum WoodlogAnims
{
	Woodlog_normal
};

enum WoodlogOrientations
{
	UP, DOWN
};

void Woodlog::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, int maximum_y, int minimum_y)
{
	maxy = maximum_y;
	miny = minimum_y;
	orientation = UP;
	sizeWoodlog = glm::ivec2(32, 32);
	spritesheet.loadFromFile("sprites/spritesheet_woodlog.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	// El tamaño de cada sprite es 32x32
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(1.f, 1.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(Woodlog_normal, 8);
	sprite->addKeyframe(Woodlog_normal, glm::vec2(0.0f, 0.0f));

	sprite->changeAnimation(Woodlog_normal);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posWoodlog.x), float(tileMapDispl.y + posWoodlog.y)));
}

void Woodlog::update(int deltaTime)
{
	if (orientation == UP)
	{
		posWoodlog.y -= 1;
		if (posWoodlog.y <= miny)
		{
			orientation = DOWN;
		}
	}
	else if (orientation == DOWN)
	{
		posWoodlog.y += 1;
		if (posWoodlog.y >= maxy)
		{
			orientation = UP;
		}
	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posWoodlog.x), float(tileMapDispl.y + posWoodlog.y)));
}

void Woodlog::render()
{
	sprite->render();
	//printf("PosWoodlog: %d %d\n", posWoodlog.x, posWoodlog.y);
}

void Woodlog::setPosition(const glm::vec2& pos)
{
	posWoodlog = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posWoodlog.x), float(tileMapDispl.y + posWoodlog.y)));
}


glm::ivec2 Woodlog::getPos()
{
	return posWoodlog;
}

glm::ivec2 Woodlog::getSize()
{
	return sizeWoodlog;
}
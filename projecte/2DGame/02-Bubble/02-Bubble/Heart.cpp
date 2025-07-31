#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Heart.h"
#include "Game.h"


enum HeartAnims
{
	IDLE
};


void Heart::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	sprite = nullptr;
	wobbleAmplitude = 1.0f; 
	wobbleSpeed = 0.04f;
	wobbleTime = 0.0f;
	bActive = true;
	totalTime = 0.0f;
	moveInterval = 50;
	initialPosY = 0;
	sizeHeart = glm::ivec2(16, 16);
	
	spritesheet.loadFromFile("sprites/spritesheet_items.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);

	// El tamaño del spritesheet completo es 80x80
	// Cada sprite individual es 16x16
	// Por lo tanto, cada sprite ocupa 1/5 = 0.2 del ancho y alto total
	sprite = Sprite::createSprite(sizeHeart, glm::vec2(0.2f, 0.2f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(IDLE, 2);
	// Corazón en fila 2, columna 1 (contando desde 0)
	// Coordenada X: 0.2 * 0 = 0.0 (primera columna)
	// Coordenada Y: 0.2 * 1 = 0.2 (segunda fila)
	sprite->addKeyframe(IDLE, glm::vec2(0.0f, 0.2f));
	
	sprite->changeAnimation(IDLE);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posHeart.x), float(tileMapDispl.y + posHeart.y)));
}

void Heart::update(int deltaTime)
{
	sprite->update(deltaTime);

	if (bActive) {
		wobbleTime += wobbleSpeed * deltaTime;
		float wobbleOffset = wobbleAmplitude * sin(wobbleTime);

		totalTime += deltaTime;

		int pixelsToMove = totalTime / moveInterval;

		posHeart.y = initialPosY - pixelsToMove;

		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posHeart.x + wobbleOffset),
			float(tileMapDispl.y + posHeart.y)));

		if (posHeart.y < -sizeHeart.y) {
			bActive = false;
		}
	}
}



void Heart::render()
{
	if (bActive) {
		sprite->render();
	}
}

void Heart::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Heart::setPosition(const glm::vec2 &pos)
{
	posHeart = pos;
	initialPosY = pos.y;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posHeart.x), float(tileMapDispl.y + posHeart.y)));
}

glm::ivec2 Heart::getPos()
{
	return posHeart;
}

glm::ivec2 Heart::getSize()
{
	return sizeHeart;
}

bool Heart::isActive()
{
	return bActive;
}

void Heart::setActive(bool active)
{
	bActive = active;
}

#ifndef _WOODLOG_INCLUDE
#define _WOODLOG_INCLUDE


#include "Sprite.h"


class Woodlog
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, int maximum_y, int minimum_y);
	void update(int deltaTime);
	void render();

	void setPosition(const glm::vec2& pos);
	void setOrientation(int orientation) { this->orientation = orientation; } //0 = Down, 1 = Up

	glm::ivec2 getPos();
	glm::ivec2 getSize();

private:
	glm::ivec2 tileMapDispl, posWoodlog, sizeWoodlog;
	Texture spritesheet;
	Sprite* sprite;
	int orientation;
	int maxy, miny;
};


#endif // _WOODLOG_INCLUDE
#pragma once
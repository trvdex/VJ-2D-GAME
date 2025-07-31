#ifndef _BULLET_INCLUDE
#define _BULLET_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


class Bullet
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	void setPosition(const glm::vec2& pos);

	void changeOrientation(int orientation); //0 = standby, 1 = left, 2 = right
	int getOrientation() { return orientation; }

	glm::ivec2 getPos();
	glm::ivec2 getSize();

private:
	glm::ivec2 tileMapDispl, posBullet, sizeBullet;
	int orientation;
	Texture spritesheet;
	Sprite* sprite;

};


#endif // _BULLET_INCLUDE
#pragma once
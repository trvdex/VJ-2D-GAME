#ifndef _MOLE_INCLUDE
#define _MOLE_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


class Mole
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime, const glm::ivec2 playerPos);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	void takeDamage(int damage = 1);

	bool isDead() const;

	bool hasDropped() const { return dropped; }
	void setDropped(bool dropped) { this->dropped = dropped; }

	glm::ivec2 getPos();
	glm::ivec2 getSize();

private:
	glm::ivec2 tileMapDispl, posMole, sizeMole;
	int jumpAngle, startY, timer;
	bool jumping;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
	int health;
	bool bDead;
	int invulnerabilityTime;
	bool dropped; 

};


#endif // _MOLE_INCLUDE
#pragma once

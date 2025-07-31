#ifndef _SPIDER_INCLUDE
#define _SPIDER_INCLUDE


#include "Sprite.h"
#include "Bullet.h"
#include "TileMap.h"


class Spider
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime, const glm::ivec2 playerPos);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	void takeDamage();

	bool isDead() const;


	glm::ivec2 getPos();
	glm::ivec2 getSize();

	glm::ivec2 getBulletPos();
	glm::ivec2 getBulletSize();

	bool hasDropped() const { return dropped; }
	void setDropped(bool dropped) { this->dropped = dropped; }

private:
	glm::ivec2 tileMapDispl, posSpider, sizeSpider;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
	Bullet* bullet;
	int retreat; //0 = no retreat; 1 = up retreat; 2 = down retreat
	int retreatDistnace;
	int timer;
	bool shoot;
	int health;
	bool bDead;
	int invulnerabilityTime;
	bool dropped;

};


#endif // _SPIDER_INCLUDE
#pragma once

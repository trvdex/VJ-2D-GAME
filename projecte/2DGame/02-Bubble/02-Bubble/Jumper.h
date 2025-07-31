#ifndef _JUMPER_INCLUDE
#define _JUMPER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


class Jumper
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime, const glm::ivec2 playerPos);
	void render();
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	void takeDamage(int damage = 1);

	bool isDead() const;

	bool hasDropped() const { return dropped; }
	void setDropped(bool drop) { dropped = drop; }

	glm::ivec2 getPos();
	glm::ivec2 getSize();
	
private:
	glm::ivec2 tileMapDispl, posJumper, sizeJumper;
	int jumpAngle, startY, timer;
	bool jumping;
	int orientation; 
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	int health;
	bool bDead;
	int invulnerabilityTime;
	//used to see if an enemy has already dropped an item
	bool dropped;

};


#endif // _JUMPER_INCLUDE

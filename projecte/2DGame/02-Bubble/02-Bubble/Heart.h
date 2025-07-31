#ifndef _HEART_INCLUDE
#define _HEART_INCLUDE


#include "Sprite.h"
#include "TileMap.h"



class Heart
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	
	glm::ivec2 getPos();
	glm::ivec2 getSize();
	
	bool isActive();
	void setActive(bool active);
	
private:
	bool bActive;
	glm::ivec2 tileMapDispl, posHeart, sizeHeart;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;

	float wobbleAmplitude; 
	float wobbleSpeed;
	float wobbleTime;
	float totalTime;     
	int moveInterval;      
	int initialPosY;

};


#endif // _HEART_INCLUDE

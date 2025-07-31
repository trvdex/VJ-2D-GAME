#ifndef _BOSS_INCLUDE
#define _BOSS_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "AudioEngine.h"


class Boss
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime, bool isBossLocked);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	void takeDamage();

	bool isDead() const;

	glm::ivec2 getPos();
	glm::ivec2 getSize();

	glm::vec2 getFace1Pos();
	glm::vec2 getFace2Pos();
	glm::vec2 getFace3Pos();
	glm::vec2 getFace4Pos();

	int getInvulnerabilityTime() const { return invulnerabilityTime; }

private:
	glm::ivec2 tileMapDispl, posBoss, sizeBoss;
	int jumpAngle, startY, timer;
	bool jumping;
	int orientation;
	Texture spritesheet;
	Sprite* sprite;
	Sprite* face1;
	Sprite* face2;
	Sprite* face3;
	Sprite* face4;
	glm::vec2 face1pos, face2pos, face3pos, face4pos;
	TileMap* map;
	int health;
	bool bDead;
	bool entranceDone;
	int invulnerabilityTime;
	bool sound_played, sound_played2;

};


#endif // _Boss_INCLUDE

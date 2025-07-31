#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "HealthBar.h"
#include "Potions.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, HealthBar* healthBar, Potions* potions);
	void update(int deltaTime);
	void render();
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	void takeDamage();

	glm::ivec2 getPos();
	glm::ivec2 getSize();
	bool isInDamageState();
	bool isInvulnerable();
	bool isAttacking();
	glm::ivec2 getSpearTipPos();
	glm::ivec2 getSpearTipSize();

	bool isJumping();
	bool isProtecting();
	void setCurrentWeapon(int weaponType);
	void setInvulnerable(bool invulnerable);
	int invulnerabilityTime;

	bool isBossLocked();

	void setPaused(bool paused);
	bool isPaused() const;
	void setVictoryPose();
	
private:
	bool bJumping;
	bool bDamage;
	bool bInvulnerable;
	bool bAttacking;
	int facingDirection; // RIGHT = 1, LEFT = 0
	glm::ivec2 tileMapDispl, posPlayer, sizePlayer;
	int jumpAngle, startY;
	int damageJumpAngle, damageStartY;
	int damageAnimationTime;

	bool bAttackKeyReleased;
	bool bAttackActive;
	bool bJumpKeyReleased;
	bool bProtecting;


	Texture spritesheet;
	Sprite *sprite;
	TileMap* map;

	glm::ivec2 posSpearTip, sizeSpearTip;
	int attackAnimationTime;
	Sprite* spearTip;

	int currentWeapon;
	Sprite* fireSprite;
	int fireAnimTime;

	int currentTime;

	HealthBar* healthBar;
	Potions* potions;
	bool bossLocked;

	bool bPaused;
	bool bVictoryPose;

};


#endif // _PLAYER_INCLUDE



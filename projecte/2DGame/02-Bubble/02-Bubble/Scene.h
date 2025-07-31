#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include <vector>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Jumper.h"
#include "Mole.h"
#include "Spider.h"
#include "Woodlog.h"
#include "Boss.h"
#include "Heart.h"
#include "HealthBar.h"
#include "BossBar.h"
#include "WeaponSelector.h"
#include "Potions.h"
#include "BigHeart.h"
#include "Canteen.h"
#include "DeerSkin.h"
#include "Flint.h"
#include "AudioEngine.h"
#include "FinalItem.h"
// Scene contains all the entities of our game.
// It is responsible for updating and render them.



class Scene
{

public:
	Scene();
	~Scene();

	enum class ItemType {
		CANTEEN,
		HEART,
		BIG_HEART,
		DEER_SKIN,
		FLINT
	};

	static Scene& instance()
	{
		static Scene S;

		return S;
	}

	void init();
	void update(int deltaTime);
	void render();
	bool playerCollisionEnemy();
	bool playerCollisionHeart(int index);
	bool playerCollisionBigHeart(int index);
	pair<int,int> spearTipCollisionEnemy();
	pair<int, int> playerTopCollisionEnemy();
	bool playerCollisionCanteen(int index);
	bool playerCollisionDeerSkin(int index);
	bool playerCollisionFlint(int index);
	void handleFlintUsage();


	void dropItem(const glm::vec2& position, ItemType itemType);
	void createCanteen(const glm::vec2& position);
	void createHeart(const glm::vec2& position);
	void createBigHeart(const glm::vec2& position);
	void createDeerSkin(const glm::vec2& position);
	void createFlint(const glm::vec2& position);

	//USED TO DECIDE WHEN TO CHANGE FROM SCENE TO CREDITS IN GAME.CPP
	int getBossDefeatedTimer() const { return bossDefeatedTimer; }
	bool isPlayerDead() const { return deadPlayer; }


private:
	void initShaders();
	std::vector<Canteen*> canteens;
	std::vector<Heart*> hearts;
	std::vector<BigHeart*> bigHearts;
	std::vector<DeerSkin*> deerSkins;
	std::vector<Flint*> flints;
	int canteenCount;

	int invincibilityTimer;

	int lastJumperHitTime;
	int lastMoleHitTime;
	int lastSpiderHitTime;
	int hitCooldownTime;

	TileMap *map, *background;
	Player *player;
	vector<Jumper*> jumpers;
	vector<Mole*> moles;
	vector<Spider*> spiders;
	vector<Woodlog*> woodlogs;
	Boss* boss;
	Heart* heart;
	BigHeart* bigHeart;
	HealthBar* healthBar;
	BossBar* bossBar;
	WeaponSelector* weaponSelector;
	Potions* potions;
	DeerSkin* deerSkin;
	Flint* flint;
	int remainingFlintHits;
	Sprite* flintHudSprite;
	bool flintHudInitialized;
	Sprite* flintEffectSprite;
	bool flintEffectActive;
	glm::vec2 flintEffectPosition;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	bool bossDefeated;
	int bossDefeatedTimer;
	bool deadPlayer;
	int deadTimer;

	
	bool bossShakeActive;
	int bossShakeTimer;
	float bossShakeIntensity;


	bool finalItemDropped;
	bool finalItemActive;
	glm::vec2 finalItemPos;
	glm::vec2 finalItemVelocity;
	
	FinalItem* finalItem;
	bool bossDefeatedEffectActive;
	int bossDefeatedEffectTimer;

	Sprite* victoryItemSprite;
	bool victoryItemVisible;

	ItemType determineDropItem();
};


#endif // _SCENE_INCLUDE


#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include <random>


#define SCREEN_X 32
#define SCREEN_Y 16

#define INIT_PLAYER_X_TILES 4
#define INIT_PLAYER_Y_TILES 2


Scene::Scene()
{
	map = NULL;
	background = NULL;
	player = NULL;
	jumpers = vector<Jumper*>(0);
	moles = vector<Mole*>(0);
	spiders = vector<Spider*>(0);
	boss = NULL;
	healthBar = NULL;
	bossBar = NULL;
	weaponSelector = NULL;
	potions = NULL;
	flintHudSprite = nullptr;
	flintHudInitialized = false;
	flintEffectSprite = nullptr;
	flintEffectActive = false;
	invincibilityTimer = 0;
	canteenCount = 0;
	remainingFlintHits = 0;
	lastJumperHitTime = -1000;
	lastMoleHitTime = -1000;
	lastSpiderHitTime = -1000;
	hitCooldownTime = 300;
	bossShakeActive = false;
	bossShakeTimer = 0;
	bossShakeIntensity = 0.0f;
	finalItemDropped = false;
	finalItemActive = false;
	finalItem = NULL;
	bossDefeatedEffectActive = false;
	bossDefeatedEffectTimer = 0;
	victoryItemSprite = nullptr;
	victoryItemVisible = false;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if (background != NULL)
		delete background;
	if(player != NULL)
		delete player;
	if (finalItem != NULL)
		delete finalItem;
	if (victoryItemSprite != nullptr)
		delete victoryItemSprite;
	if (jumpers.size() != 0) {
		for (Jumper* jumper : jumpers) {
			if (jumper != nullptr)
				delete jumper;
		}
		jumpers.clear();
	}
	if (moles.size() != 0) {
		for (Mole* mole : moles) {
			if (mole != nullptr)
				delete mole;
		}
		moles.clear();
	}
	if (spiders.size() != 0) {
		for (Spider* spider : spiders) {
			if (spider != nullptr)
				delete spider;
		}
		spiders.clear();
	}
	if (boss != NULL)
		delete boss;
	if (healthBar != NULL)
		delete healthBar;
	if (bossBar != NULL)
		delete bossBar;
	if (weaponSelector != NULL)
		delete weaponSelector;
	if (potions != NULL)
		delete potions;
	if (flintHudSprite != nullptr)
		delete flintHudSprite;
	if (flintEffectSprite != nullptr)
		delete flintEffectSprite;
	for (Canteen* canteen : canteens) {
		if (canteen != nullptr)
			delete canteen;
	}
	canteens.clear();
	for (Heart* h : hearts) {
		if (h != nullptr)
			delete h;
	}
	hearts.clear();

	for (BigHeart* bh : bigHearts) {
		if (bh != nullptr)
			delete bh;
	}
	bigHearts.clear();

	for (DeerSkin* ds : deerSkins) {
		if (ds != nullptr)
			delete ds;
	}
	deerSkins.clear();

	for (Flint* f : flints) {
		if (f != nullptr)
			delete f;
	}
	flints.clear();
}


void Scene::init()
{
	initShaders();
	woodlogs = vector<Woodlog*>(6);
	for (int i = 0; i < 6; ++i)
	{
		woodlogs[i] = new Woodlog();
		switch (i)
		{
		case 0:
			woodlogs[i]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 860, 665);
			woodlogs[i]->setPosition(glm::vec2(3135, 760));
			woodlogs[i]->setOrientation(0);
			break;
		case 1:
			woodlogs[i]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 860, 665);
			woodlogs[i]->setPosition(glm::vec2(3135 + 32, 720));
			woodlogs[i]->setOrientation(1);
			break;
		case 2:
			woodlogs[i]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 860, 665);
			woodlogs[i]->setPosition(glm::vec2(3135 + 32 * 2, 680));
			woodlogs[i]->setOrientation(0);
			break;
		case 3:
			woodlogs[i]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 665, 550);
			woodlogs[i]->setPosition(glm::vec2(3135, 620));
			woodlogs[i]->setOrientation(1);
			break;
		case 4:
			woodlogs[i]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 665, 550);
			woodlogs[i]->setPosition(glm::vec2(3135 + 32, 580));
			woodlogs[i]->setOrientation(0);
			break;
		case 5:
			woodlogs[i]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 665, 550);
			woodlogs[i]->setPosition(glm::vec2(3135 + 32 * 2, 600));
			woodlogs[i]->setOrientation(1);
			break;
		default:
			break;
		}
	}

	//USED TO DECIDE WHEN TO CHANGE FROM SCENE TO CREDITS
	bossDefeated = false;
	bossDefeatedTimer = 0;
	deadPlayer = false;
	deadTimer = 0;

	map = TileMap::createTileMap("levels/level01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram, woodlogs);
	background = TileMap::createTileMap("levels/level02.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram, woodlogs);

	healthBar = new HealthBar();
	healthBar->init(texProgram);

	bossBar = new BossBar();
	bossBar->init(texProgram);

	potions = new Potions();
	potions->init(texProgram);

	finalItem = new FinalItem();
	finalItem->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	finalItem->setTileMap(map);

	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, healthBar, potions);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), 8 * map->getTileSize()));
	//player->setPosition(glm::vec2(3600, 500));
	player->setTileMap(map);

	for (int i = 0; i < 6; ++i)
	{
		jumpers.push_back(new Jumper());
		jumpers[i]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		switch (i)
		{
		case 0:
			jumpers[i]->setPosition(glm::vec2(20 * map->getTileSize(), 10 * map->getTileSize()));
			break;
		case 1:
			jumpers[i]->setPosition(glm::vec2(30 * map->getTileSize(), 10 * map->getTileSize()));
			break;
		case 2:
			jumpers[i]->setPosition(glm::vec2(45 * map->getTileSize(), 2 * map->getTileSize()));
			break;
		case 3:
			jumpers[i]->setPosition(glm::vec2(55 * map->getTileSize(), 2 * map->getTileSize()));
			break;
		case 4:
			jumpers[i]->setPosition(glm::vec2(3135,1300));
			break;
		case 5:
			jumpers[i]->setPosition(glm::vec2(3135 + 32, 1200));
			break;
		}
		jumpers[i]->setTileMap(map);
	}

	for (int i = 0; i < 5; ++i)
	{
		moles.push_back(new Mole());
		moles[i]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		switch (i)
		{
		case 0:
			moles[i]->setPosition(glm::vec2(100 * map->getTileSize(), 6 * map->getTileSize()));
			break;
		case 1:
			moles[i]->setPosition(glm::vec2(134 * map->getTileSize(), 100 * map->getTileSize()));
			break;
		case 2:
			moles[i]->setPosition(glm::vec2(165 * map->getTileSize(), 100 * map->getTileSize()));
			break;
		case 3:
			moles[i]->setPosition(glm::vec2(175 * map->getTileSize(), 100 * map->getTileSize()));
			break;
		case 4:
			moles[i]->setPosition(glm::vec2(225 * map->getTileSize(), 36 * map->getTileSize()));
			break;
		}
		moles[i]->setTileMap(map);
	}

	for (int i = 0; i < 12; ++i)
	{
		spiders.push_back(new Spider());
		spiders[i]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		switch (i)
		{
		case 0:
			spiders[i]->setPosition(glm::vec2(134 * map->getTileSize(), 20 * map->getTileSize()));
			break;
		case 1:
			spiders[i]->setPosition(glm::vec2(139 * map->getTileSize(), 10 * map->getTileSize()));
			break;
		case 2:
			spiders[i]->setPosition(glm::vec2(130 * map->getTileSize(), 34 * map->getTileSize()));
			break;
		case 3:
			spiders[i]->setPosition(glm::vec2(139 * map->getTileSize(), 27 * map->getTileSize()));
			break;
		case 4:
			spiders[i]->setPosition(glm::vec2(134*map->getTileSize(), 45*map->getTileSize()));
			break;
		case 5:
			spiders[i]->setPosition(glm::vec2(139*map->getTileSize(), 50*map->getTileSize()));
			break;
		case 6:
			spiders[i]->setPosition(glm::vec2(134*map->getTileSize(), 76*map->getTileSize()));
			break;
		case 7:
			spiders[i]->setPosition(glm::vec2(139* map->getTileSize(), 71* map->getTileSize()));
			break;
		case 8:
			spiders[i]->setPosition(glm::vec2(130* map->getTileSize(), 90* map->getTileSize()));
			break;
		case 9:
			spiders[i]->setPosition(glm::vec2(134* map->getTileSize(), 93* map->getTileSize()));
			break;
		case 10:
			spiders[i]->setPosition(glm::vec2(139*map->getTileSize(), 98* map->getTileSize()));
			break;
		case 11:
			spiders[i]->setPosition(glm::vec2(143* map->getTileSize(), 96* map->getTileSize()));
			break;
		default:
			break;
		}
		spiders[i]->setTileMap(map);
	}

	boss = new Boss();
	boss->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	boss->setPosition(glm::vec2(3935, 700));
	boss->setTileMap(map);

	heart = new Heart();
	heart->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	heart->setPosition(glm::vec2(10 * map->getTileSize(), 12 * map->getTileSize()));
	heart->setTileMap(map);

	

	weaponSelector = new WeaponSelector();
	weaponSelector->init(texProgram);

	

	bigHeart = new BigHeart();
	bigHeart->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	bigHeart->setPosition(glm::vec2(12 * map->getTileSize(), 10 * map->getTileSize()));
	bigHeart->setTileMap(map);

	deerSkin = new DeerSkin();
	deerSkin->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	deerSkin->setPosition(glm::vec2(16 * map->getTileSize(), 5 * map->getTileSize()));
	deerSkin->setTileMap(map);

	flint = new Flint();
	flint->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	flint->setPosition(glm::vec2(8 * map->getTileSize(), 8 * map->getTileSize()));
	flint->setTileMap(map);
	remainingFlintHits = 0;


	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
	//projection = glm::ortho(200.f, 440.0f, 280.0f, 200.0f);
	currentTime = 0.0f;
	canteenCount = 0;
}

bool Scene::playerCollisionEnemy()
{
	//esto habria que cambiarlo entero para que compruebe iterando en un vector de cada clase de enemigos si estan vivos y si hay colisiones
	if (player->isInvulnerable() || invincibilityTimer == INT_MAX)
		return false;

	bool collisionX;
	bool collisionY;
	glm::ivec2 initplayerPos = player->getPos();
	glm::ivec2 initplayerSize = player->getSize();
	glm::ivec2 playerPos;
	glm::ivec2 playerSize;
	//adding offset to both sides of the player to make the collision box smaller and adding offset in vertical position to lower the collision box

	for (int i = 0; i < jumpers.size(); ++i)
	{
		if (!jumpers[i]->isDead())
		{
			playerPos.x = initplayerPos.x + 5;
			playerPos.y = initplayerPos.y + 32;
			playerSize.x = initplayerSize.x -15;
			//jumper collision
			glm::ivec2 jumperPos = jumpers[i]->getPos();
			glm::ivec2 jumperSize = jumpers[i]->getSize();


			collisionX = playerPos.x + playerSize.x >= jumperPos.x && playerPos.x + playerSize.x <= jumperPos.x + jumperSize.x;
			collisionX = collisionX || (playerPos.x >= jumperPos.x && playerPos.x <= jumperPos.x + jumperSize.x);
			collisionY = playerPos.y + playerSize.y >= jumperPos.y && playerPos.y + playerSize.y <= jumperPos.y + jumperSize.y;
			collisionY = collisionY || (playerPos.y >= jumperPos.y && playerPos.y <= jumperPos.y + jumperSize.y);

			if (collisionX && collisionY) return true;
		}
	}

	for (int i = 0; i < moles.size(); ++i)
	{
		if (!moles[i]->isDead())
		{
			playerPos.x = initplayerPos.x + 5;
			playerPos.y = initplayerPos.y + 32;
			playerSize.x = initplayerSize.x - 15;
			//mole collision
			glm::ivec2 molePos = moles[i]->getPos();
			molePos.y -= 30;
			molePos.x += 5;
			glm::ivec2 moleSize = moles[i]->getSize();
			moleSize.x -= 10;
			collisionX = playerPos.x + playerSize.x >= molePos.x && playerPos.x + playerSize.x <= molePos.x + moleSize.x;
			collisionX = collisionX || (playerPos.x >= molePos.x && playerPos.x <= molePos.x + moleSize.x);
			collisionY = playerPos.y + playerSize.y >= molePos.y && playerPos.y + playerSize.y <= molePos.y + moleSize.y;
			collisionY = collisionY || (playerPos.y >= molePos.y && playerPos.y <= molePos.y + moleSize.y);
			if (collisionX && collisionY) return true;
		}
	}

	for (int i = 0; i < spiders.size(); ++i)
	{
		if (!spiders[i]->isDead())
		{
			playerPos.x = initplayerPos.x + 5;
			playerPos.y = initplayerPos.y + 32;
			playerSize.x = initplayerSize.x - 10;
			playerSize.y = initplayerSize.y;
			//spider collision
			glm::ivec2 spiderPos = spiders[i]->getPos();
			glm::ivec2 spiderSize = spiders[i]->getSize();
			collisionX = playerPos.x < spiderPos.x + spiderSize.x &&
				playerPos.x + playerSize.x > spiderPos.x;
			collisionY = playerPos.y < spiderPos.y + spiderSize.y &&
				playerPos.y + playerSize.y > spiderPos.y;
			if (collisionX && collisionY) return true;
			//check bullet collision
			glm::ivec2 bulletPos = spiders[i]->getBulletPos();
			glm::ivec2 bulletSize = spiders[i]->getBulletSize();
			collisionX = playerPos.x < bulletPos.x + bulletSize.x &&
				playerPos.x + playerSize.x > bulletPos.x;
			collisionY = playerPos.y < bulletPos.y + bulletSize.y &&
				playerPos.y + playerSize.y > bulletPos.y;
			if (collisionX && collisionY) return true;
		}
	}

	if (!boss->isDead())
	{
		playerPos.x = initplayerPos.x + 5;
		playerPos.y = initplayerPos.y + 32;
		playerSize.x = initplayerSize.x - 15;
		playerSize.y = initplayerSize.y;
		glm::ivec2 bossPos = boss->getPos();
		bossPos.x += 20;
		bossPos.y += 15;
		glm::ivec2 bossSize = boss->getSize();
		bossSize.x -= 40;
		bossSize.y -= 30;
		collisionX = playerPos.x < bossPos.x + bossSize.x &&
			playerPos.x + playerSize.x > bossPos.x;
		collisionY = playerPos.y < bossPos.y + bossSize.y &&
			playerPos.y + playerSize.y > bossPos.y;
		if (collisionX && collisionY) return true;

		if (boss->getInvulnerabilityTime() > 0 && boss->getInvulnerabilityTime() <= 2000)
		{
			//check face1, face2, face3 and face4 collision
			glm::vec2 face1Pos = boss->getFace1Pos();
			face1Pos.x -= 10;
			//face1Pos.y += 15;
			glm::vec2 face1Size = glm::vec2(64, 64);
			face1Size.x -= 30;
			face1Size.y -= 30;
			collisionX = playerPos.x < face1Pos.x + face1Size.x &&
				playerPos.x + playerSize.x > face1Pos.x;
			collisionY = playerPos.y < face1Pos.y + face1Size.y&&
				playerPos.y + playerSize.y > face1Pos.y;
			if (collisionX && collisionY) return true;
			
			glm::vec2 face2Pos = boss->getFace2Pos();
			face2Pos.x -= 10;
			//face2Pos.y += 15;
			glm::vec2 face2Size = glm::vec2(64, 64);
			face2Size.x -= 30;
			face2Size.y -= 30;
			collisionX = playerPos.x < face2Pos.x + face2Size.x &&
				playerPos.x + playerSize.x > face2Pos.x;
			collisionY = playerPos.y < face2Pos.y + face2Size.y&&
				playerPos.y + playerSize.y > face2Pos.y;
			if (collisionX && collisionY) return true;

			glm::vec2 face3Pos = boss->getFace3Pos();
			face3Pos.x -= 10;
			//face3Pos.y += 15;
			glm::vec2 face3Size = glm::vec2(64, 64);
			face3Size.x -= 30;
			face3Size.y -= 30;
			collisionX = playerPos.x < face3Pos.x + face3Size.x &&
				playerPos.x + playerSize.x > face3Pos.x;
			collisionY = playerPos.y < face3Pos.y + face3Size.y&&
				playerPos.y + playerSize.y > face3Pos.y;
			if (collisionX && collisionY) return true;

			glm::vec2 face4Pos = boss->getFace4Pos();
			face4Pos.x -= 10;
			//face4Pos.y += 15;
			glm::vec2 face4Size = glm::vec2(64, 64);
			face4Size.x -= 30;
			face4Size.y -= 30;
			collisionX = playerPos.x < face4Pos.x + face4Size.x &&
				playerPos.x + playerSize.x > face4Pos.x;
			collisionY = playerPos.y < face4Pos.y + face4Size.y&&
				playerPos.y + playerSize.y > face4Pos.y;
			if (collisionX && collisionY) return true;
			
		}
	}

	return false;
}
bool Scene::playerCollisionHeart(int index)
{
	if (!hearts[index]->isActive())
		return false;

	glm::ivec2 heartPos = hearts[index]->getPos();
	glm::ivec2 heartSize = hearts[index]->getSize();
	glm::ivec2 playerPos = player->getPos();
	glm::ivec2 playerSize = player->getSize();
	playerPos.x += 5;
	playerSize.x -= 10;
	playerPos.y += 32;

	bool collisionX = playerPos.x + playerSize.x >= heartPos.x && playerPos.x <= heartPos.x + heartSize.x;
	bool collisionY = playerPos.y + playerSize.y >= heartPos.y && playerPos.y <= heartPos.y + heartSize.y;

	return collisionX && collisionY;
}

bool Scene::playerCollisionBigHeart(int index)
{
	if (!bigHearts[index]->isActive())
		return false;

	glm::ivec2 bigHeartPos = bigHearts[index]->getPos();
	glm::ivec2 bigHeartSize = bigHearts[index]->getSize();
	glm::ivec2 playerPos = player->getPos();
	glm::ivec2 playerSize = player->getSize();
	playerPos.x += 5;
	playerSize.x -= 10;
	playerPos.y += 32;

	bool collisionX = playerPos.x + playerSize.x >= bigHeartPos.x && playerPos.x <= bigHeartPos.x + bigHeartSize.x;
	bool collisionY = playerPos.y + playerSize.y >= bigHeartPos.y && playerPos.y <= bigHeartPos.y + bigHeartSize.y;

	return collisionX && collisionY;
}

//PRIMER PAR
//Devuelve 0 si no ha impactado con ningun enemigo
//Devuelve 1 si ha impactado con el jumper
//Devuelve 2 si ha impactado con el mole
// Devuelve 3 si ha impactado con el spider
// Devuelve 4 si ha impactado con el boss
//SEGUNDO PAR
//La posicion en el vector del enemigo con el que ha impactado
pair<int,int> Scene::spearTipCollisionEnemy()
{
	//esto tambien habria que cambiarlo por vectores de enemigos

	if (!player->isAttacking())	return make_pair(0,0);

	glm::ivec2 spearTipPos = player->getSpearTipPos();
	glm::ivec2 spearTipSize = player->getSpearTipSize();
	spearTipPos.x += 5;
	spearTipPos.y += 45;
	spearTipSize.x -= 15;
	spearTipSize.y -= 64;

	bool collisionX;
	bool collisionY;

	for (int i = 0; i < jumpers.size(); ++i)
	{
		if (!jumpers[i]->isDead())
		{
			// Comprobamos la colisiÃ³n con el jumper
			glm::ivec2 jumperPos = jumpers[i]->getPos();
			glm::ivec2 jumperSize = jumpers[i]->getSize();

			collisionX = spearTipPos.x + spearTipSize.x >= jumperPos.x && spearTipPos.x <= jumperPos.x + jumperSize.x;
			collisionY = spearTipPos.y + spearTipSize.y >= jumperPos.y && spearTipPos.y <= jumperPos.y + jumperSize.y;
	
			if (collisionX && collisionY) return make_pair(1, i);
		}
	}

	for (int i = 0; i < moles.size(); ++i)
	{
		if (!moles[i]->isDead())
		{
			// Comprobamos la colisiÃ³n con el mole
			glm::ivec2 molePos = moles[i]->getPos();
			glm::ivec2 moleSize = moles[i]->getSize();
			collisionX = spearTipPos.x + spearTipSize.x >= molePos.x && spearTipPos.x <= molePos.x + moleSize.x;
			collisionY = spearTipPos.y + spearTipSize.y >= molePos.y && spearTipPos.y <= molePos.y + moleSize.y;
			if (collisionX && collisionY) return make_pair(2, i);
		}
	}

	for (int i = 0; i < spiders.size(); ++i)
	{
		if (!spiders[i]->isDead())
		{
			// Comprobamos la colisiÃ³n con el spider
			glm::ivec2 spiderPos = spiders[i]->getPos();
			glm::ivec2 spiderSize = spiders[i]->getSize();
			collisionX = spearTipPos.x + spearTipSize.x >= spiderPos.x && spearTipPos.x <= spiderPos.x + spiderSize.x;
			collisionY = spearTipPos.y + spearTipSize.y >= spiderPos.y && spearTipPos.y <= spiderPos.y + spiderSize.y;
			if (collisionX && collisionY) return make_pair(3, i);
		}
	}

	if (!boss->isDead())
	{
		// Comprobamos la colisión con el boss
		glm::ivec2 bossPos = boss->getPos();
		bossPos.x += 15;
		bossPos.y += 10;
		glm::ivec2 bossSize = boss->getSize();
		bossSize.x -= 30;
		bossSize.y -= 20;
		collisionX = spearTipPos.x + spearTipSize.x >= bossPos.x && spearTipPos.x <= bossPos.x + bossSize.x;
		collisionY = spearTipPos.y + spearTipSize.y >= bossPos.y && spearTipPos.y <= bossPos.y + bossSize.y;
		if (collisionX && collisionY) return make_pair(4,0);
	}

	return make_pair(0,0);
}

//PRIMER PAR
//Devuelve 0 si no ha impactado con ningun enemigo
//Devuelve 1 si ha impactado con el jumper
//Devuelve 2 si ha impactado con el mole
// Devuelve 3 si ha impactado con el spider
// Devuelve 4 si ha impactado con el boss
//SEGUNDO PAR
//La posicion en el vector del enemigo con el que ha impactado
pair<int,int> Scene::playerTopCollisionEnemy()
{
	if (!player->isJumping()) return make_pair(0,0);

	glm::ivec2 playerPos = player->getPos();
	glm::ivec2 playerSize = player->getSize();
	playerPos.x += 20;
	playerSize.x -= 40;
	// SÃ³lo nos interesa la parte superior del jugador (primeros 16 pixels)
	int playerTopHeight = 16;
	bool collisionX;
	bool collisionY;

	for (int i = 0; i < jumpers.size(); ++i)
	{
		if (!jumpers[i]->isDead())
		{
			// Comprobamos la colisiÃ³n con el jumper
			glm::ivec2 jumperPos = jumpers[i]->getPos();
			glm::ivec2 jumperSize = jumpers[i]->getSize();
			collisionX = playerPos.x + playerSize.x >= jumperPos.x && playerPos.x <= jumperPos.x + jumperSize.x;
			collisionY = playerPos.y <= jumperPos.y + jumperSize.y &&
				playerPos.y + playerTopHeight >= jumperPos.y;
			if (collisionX && collisionY) return make_pair(1, i);
		}
	}

	for (int i = 0; i < moles.size(); ++i)
	{
		if (!moles[i]->isDead())
		{
			// Comprobamos la colisiÃ³n con el mole
			glm::ivec2 molePos = moles[i]->getPos();
			glm::ivec2 moleSize = moles[i]->getSize();
			collisionX = playerPos.x + playerSize.x >= molePos.x && playerPos.x <= molePos.x + moleSize.x;
			collisionY = playerPos.y <= molePos.y + moleSize.y &&
				playerPos.y + playerTopHeight >= molePos.y;
			if (collisionX && collisionY) return make_pair(2, i);
		}
	}

	for (int i = 0; i < spiders.size(); ++i)
	{
		if (!spiders[i]->isDead())
		{
			// Comprobamos la colisiÃ³n con el spider
			glm::ivec2 spiderPos = spiders[i]->getPos();
			spiderPos.x -= 20;
			spiderPos.y -= 20;
			glm::ivec2 spiderSize = spiders[i]->getSize();
			spiderSize.x += 40;
			//spiderSize.y += 20;
			collisionX = playerPos.x + playerSize.x >= spiderPos.x && playerPos.x <= spiderPos.x + spiderSize.x;
			collisionY = playerPos.y <= spiderPos.y + spiderSize.y &&
				playerPos.y + playerTopHeight >= spiderPos.y;
			if (collisionX && collisionY) return make_pair(3, i);
		}
	}

	if (!boss->isDead())
	{
		// Comprobamos la colisión con el boss
		glm::ivec2 bossPos = boss->getPos();
		bossPos.x += 10;
		glm::ivec2 bossSize = boss->getSize();
		bossSize.x -= 20;
		bossSize.y - 15;
		collisionX = playerPos.x + playerSize.x >= bossPos.x && playerPos.x <= bossPos.x + bossSize.x;
		collisionY = playerPos.y <= bossPos.y + bossSize.y &&
			playerPos.y + playerTopHeight >= bossPos.y;
		if (collisionX && collisionY) return make_pair(4,0);
	}

	return make_pair(0,0);
}

Scene::ItemType Scene::determineDropItem()
{
	// Generador de números aleatorios
	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_int_distribution<int> distribution(0, 99);
	int randomNumber = distribution(gen);

	// 50% - Canteen: 0-49
	// 20% - Heart: 50-69
	// 10% - BigHeart: 70-79
	// 10% - DeerSkin: 80-89
	// 10% - Flint: 90-99

	if (randomNumber < 50) {
		return ItemType::CANTEEN;
	}
	else if (randomNumber < 70) {
		return ItemType::HEART;
	}
	else if (randomNumber < 80) {
		return ItemType::BIG_HEART;
	}
	else if (randomNumber < 90) {
		return ItemType::DEER_SKIN;
	}
	else {
		return ItemType::FLINT;
	}
}

void Scene::dropItem(const glm::vec2& position, ItemType itemType)
{
	switch (itemType) {
	case ItemType::CANTEEN:
		createCanteen(position);
		break;
	case ItemType::HEART:
		createHeart(position);
		break;
	case ItemType::BIG_HEART:
		createBigHeart(position);
		break;
	case ItemType::DEER_SKIN:
		createDeerSkin(position);
		break;
	case ItemType::FLINT:
		createFlint(position);
		break;
	}
}

void Scene::createHeart(const glm::vec2& position)
{
	Heart* newHeart = new Heart();
	newHeart->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	newHeart->setPosition(position);
	newHeart->setTileMap(map);
	newHeart->setActive(true);
	hearts.push_back(newHeart);
}

void Scene::createBigHeart(const glm::vec2& position)
{
	BigHeart* newBigHeart = new BigHeart();
	newBigHeart->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	newBigHeart->setPosition(position);
	newBigHeart->setTileMap(map);
	newBigHeart->activate(position);
	bigHearts.push_back(newBigHeart);
}

void Scene::createDeerSkin(const glm::vec2& position)
{
	DeerSkin* newDeerSkin = new DeerSkin();
	newDeerSkin->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	newDeerSkin->setPosition(position);
	newDeerSkin->setTileMap(map);
	newDeerSkin->activate(position);
	deerSkins.push_back(newDeerSkin);
}

void Scene::createFlint(const glm::vec2& position)
{
	Flint* newFlint = new Flint();
	newFlint->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	newFlint->setPosition(position);
	newFlint->setTileMap(map);
	newFlint->activate(position);
	flints.push_back(newFlint);
}

void Scene::createCanteen(const glm::vec2& position)
{
	Canteen* canteen = new Canteen();
	canteen->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	canteen->setPosition(position);
	canteen->setTileMap(map);
	canteen->activate(position);
	canteens.push_back(canteen);
}
// MÃ©todo para la colisiÃ³n con cantimploras
bool Scene::playerCollisionCanteen(int index)
{
	Canteen* canteen = canteens[index];
	if (!canteen->isActive())
		return false;

	glm::ivec2 canteenPos = canteen->getPos();
	glm::ivec2 canteenSize = canteen->getSize();
	glm::ivec2 playerPos = player->getPos();
	glm::ivec2 playerSize = player->getSize();
	playerPos.x += 10;
	playerPos.y += 32;
	playerSize.x -= 20;

	bool collisionX = playerPos.x + playerSize.x >= canteenPos.x && playerPos.x <= canteenPos.x + canteenSize.x;
	bool collisionY = playerPos.y + playerSize.y >= canteenPos.y && playerPos.y <= canteenPos.y + canteenSize.y;

	return collisionX && collisionY;
}

bool Scene::playerCollisionDeerSkin(int index)
{
	if (!deerSkins[index]->isActive())
		return false;

	glm::ivec2 deerSkinPos = deerSkins[index]->getPos();
	glm::ivec2 deerSkinSize = deerSkins[index]->getSize();
	glm::ivec2 playerPos = player->getPos();
	glm::ivec2 playerSize = player->getSize();
	playerPos.x += 5;
	playerSize.x -= 10;
	playerPos.y += 32;

	bool collisionX = playerPos.x + playerSize.x >= deerSkinPos.x && playerPos.x <= deerSkinPos.x + deerSkinSize.x;
	bool collisionY = playerPos.y + playerSize.y >= deerSkinPos.y && playerPos.y <= deerSkinPos.y + deerSkinSize.y;

	return collisionX && collisionY;
}

bool Scene::playerCollisionFlint(int index)
{
	if (!flints[index]->isActive())
		return false;

	glm::ivec2 flintPos = flints[index]->getPos();
	glm::ivec2 flintSize = flints[index]->getSize();
	glm::ivec2 playerPos = player->getPos();
	glm::ivec2 playerSize = player->getSize();
	playerPos.x += 10;
	playerPos.y += 32;
	playerSize.x -= 20;

	bool collisionX = playerPos.x + playerSize.x >= flintPos.x && playerPos.x <= flintPos.x + flintSize.x;
	bool collisionY = playerPos.y + playerSize.y >= flintPos.y && playerPos.y <= flintPos.y + flintSize.y;

	return collisionX && collisionY;
}


void Scene::handleFlintUsage()
{
	if (remainingFlintHits > 0) {
		remainingFlintHits--;
		printf("Flint usado: %d golpes restantes\n", remainingFlintHits);

		// Si se agotaron los usos, mostrar el efecto
		if (remainingFlintHits == 0) {
			if (flintEffectSprite == nullptr) {
				flintEffectSprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.2f, 0.2f), flint->getTexture(), &texProgram);
				flintEffectSprite->setNumberAnimations(1);
				flintEffectSprite->setAnimationSpeed(0, 8);
				flintEffectSprite->addKeyframe(0, glm::vec2(0.0f, 0.4f));
				flintEffectSprite->changeAnimation(0);
			}

			flintEffectActive = true;
			flintEffectPosition = glm::vec2(player->getPos().x + 36, player->getPos().y + 30);
		}
	}
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	player->update(deltaTime);
	for (int i = 0; i < jumpers.size(); ++i) jumpers[i]->update(deltaTime, player->getPos());

	for (int i = 0; i < moles.size(); ++i) moles[i]->update(deltaTime, player->getPos());
	for (int i = 0; i < spiders.size(); ++i)spiders[i]->update(deltaTime, player->getPos());
	boss->update(deltaTime, player->isBossLocked());

	for (int i = 0; i < woodlogs.size(); ++i)
	{
		woodlogs[i]->update(deltaTime);
	}
	heart->update(deltaTime);
	healthBar->update(deltaTime);
	bossBar->update(deltaTime);
	weaponSelector->update(deltaTime);
	bigHeart->update(deltaTime);
	deerSkin->update(deltaTime);
	flint->update(deltaTime);

	// Pasar el arma seleccionada al jugador
	player->setCurrentWeapon(weaponSelector->getCurrentWeapon());

	static bool godModeKeyReleased = true;
	static bool godModeActive = false;

	if (boss->isDead()) {
		bossDefeated = true;

		// Si aún no hemos activado el efecto de victoria
		if (!bossDefeatedEffectActive) {
			bossDefeatedEffectActive = true;
			bossDefeatedEffectTimer = 2000; // 2 segundos
			bossShakeActive = true;
			bossShakeIntensity = 10.0f;

			player->setPaused(true);

			// Sonido de victoria
			AudioEngine::getInstance().stopAll();
			AudioEngine::getInstance().play2D("sounds/win_music.mp3", false, 0.5f);
		}

		// Actualizar el efecto
		if (bossDefeatedEffectActive) {
			bossDefeatedEffectTimer -= deltaTime;
			bossShakeIntensity *= 0.98f; // Reducir más lentamente

			// Cuando termina el tiempo del efecto
			if (bossDefeatedEffectTimer <= 0 && !finalItem->isActive() && !finalItemDropped) {
				bossShakeActive = false;

				// Activar y posicionar el item final
				finalItem->setPosition(glm::vec2(boss->getPos().x + 35, boss->getPos().y + 20));
				finalItem->setActive(true);
				finalItem->startMovingToPlayer(glm::vec2(player->getPos().x + 32, player->getPos().y + 32));
			}
		}

		// Actualizar el movimiento del item
		finalItem->update(deltaTime);
		if (finalItem->isActive()) {
			// Usar el mismo método de detección de colisión que los otros items
			glm::ivec2 itemPos = finalItem->getPos();
			glm::ivec2 itemSize = finalItem->getSize();
			glm::ivec2 playerPos = player->getPos();
			glm::ivec2 playerSize = player->getSize();

			// Usar los mismos ajustes que se usan para los corazones y otros ítems
			playerPos.x -= 6;      
			playerSize.x += 14;      
			playerPos.y -= 6;       
			playerSize.y += 14;

			bool collisionX = playerPos.x + playerSize.x >= itemPos.x && playerPos.x <= itemPos.x + itemSize.x;
			bool collisionY = playerPos.y + playerSize.y >= itemPos.y && playerPos.y <= itemPos.y + itemSize.y;

			if (collisionX && collisionY) {
				finalItem->setActive(false);
				finalItemDropped = true;

				// Crear el sprite visual si aún no existe
				if (victoryItemSprite == nullptr) {
					victoryItemSprite = Sprite::createSprite(finalItem->getSize(),
						glm::vec2(0.2f, 0.2f),
						finalItem->getTexture(),
						&texProgram);
					victoryItemSprite->setNumberAnimations(1);
					victoryItemSprite->setAnimationSpeed(0, 8);
					victoryItemSprite->addKeyframe(0, glm::vec2(0.2f, 0.6f));
					victoryItemSprite->changeAnimation(0);
				}
				victoryItemVisible = true;

				bool wasPaused = player->isPaused();
				player->setPaused(false);
				player->setVictoryPose();
				player->setPaused(wasPaused);

				// Reproducir sonido
				AudioEngine::getInstance().play2D("sounds/power_up.mp3", false, 0.7f);
			}
		}

		bossDefeatedTimer += deltaTime;
	}

	if (Game::instance().getKey('G')) {
		if (godModeKeyReleased) {
			godModeActive = !godModeActive;

			if (godModeActive) {
				player->setInvulnerable(true);
				// Establecer directamente en player para evitar que se sobrescriba
				player->invulnerabilityTime = INT_MAX;
				invincibilityTimer = INT_MAX;
				printf("God Mode: ON\n");
			}
			else {
				player->setInvulnerable(false);
				invincibilityTimer = 0;
				printf("God Mode: OFF\n");
			}

			godModeKeyReleased = false;
		}
	}
	else {
		godModeKeyReleased = true;
	}

	static bool healKeyReleased = true;

	if (Game::instance().getKey('H')) {
		if (healKeyReleased) {
			// Restaurar la salud del jugador al mÃ¡ximo
			healthBar->resetHealth();
			AudioEngine::getInstance().play2D("sounds/heal_sound.mp3", false, 0.6f);

			// Restaurar las pociones al mÃ¡ximo (si existe un mÃ©todo para ello)
			potions->resetPotions();

			printf("Health and Potions restored!\n");
			healKeyReleased = false;
		}
	}
	else {
		healKeyReleased = true;
	}


	// Si el jugador estÃ¡ saltando y protegiÃ©ndose, y la colisiÃ³n es en la parte superior
	if (player->isJumping() && player->isProtecting()) {
		pair<int, int> collision = playerTopCollisionEnemy();
		switch (collision.first)
		{
		case 1:
			jumpers[collision.second]->takeDamage();
			break;
		case 2:
			moles[collision.second]->takeDamage();
			break;
		case 3:
			spiders[collision.second]->takeDamage();
			break;
		case 4:
			boss->takeDamage();
			if (boss->isDead() || boss->getInvulnerabilityTime() == 2500)
			{
				bossBar->decreaseHealth();
			}
			break;
		default:
			break;
		}
	}

	// Colision jugador-enemigo
	else if (playerCollisionEnemy())
	{
			// El jugador recibe daÃ±o (comportamiento modificado)
			healthBar->decreaseHealth();
			player->takeDamage();

			// Solo reposicionamos al jugador al inicio si estÃ¡ sin vidas
			if (healthBar->isDead() && potions->hasPotions()) {
				// Restaurar la salud del jugador
				healthBar->resetHealth();
				AudioEngine::getInstance().play2D("sounds/heal_sound.mp3", false, 0.6f);


				// Usar una pociÃ³n
				potions->usePotion();
			}
	}

	if (healthBar->isDead() && potions->getPotionCount() == 0) deadTimer += deltaTime;
	if (deadTimer > 500) deadPlayer = true;


	pair<int, int> spearTipCollision = spearTipCollisionEnemy();
	switch (spearTipCollision.first)
	{
	case 1: // Jumper
		if (currentTime - lastJumperHitTime > hitCooldownTime) {
			lastJumperHitTime = currentTime;
			jumpers[spearTipCollision.second]->takeDamage(remainingFlintHits > 0 ? 2 : 1);
			handleFlintUsage();
		}
		break;
	case 2: // Mole
		if (currentTime - lastMoleHitTime > hitCooldownTime) {
			lastMoleHitTime = currentTime;
			moles[spearTipCollision.second]->takeDamage(remainingFlintHits > 0 ? 2 : 1);
			handleFlintUsage();
		}
		break;
	case 3: // Spider
		if (currentTime - lastSpiderHitTime > hitCooldownTime) {
			lastSpiderHitTime = currentTime;
			spiders[spearTipCollision.second]->takeDamage();
			handleFlintUsage();
		}
		break;
  case 4:
    boss->takeDamage();
    if (boss->isDead() || boss->getInvulnerabilityTime() == 2500)
    {
      bossBar->decreaseHealth();
    }
    break; 
	default:
		break;
	}


	for (size_t i = 0; i < hearts.size(); i++) {
		if (playerCollisionHeart(i)) {
			hearts[i]->setActive(false);
			healthBar->increaseHealth();
			AudioEngine::getInstance().play2D("sounds/power_up.mp3", false, 0.7f);
		}
	}

	for (size_t i = 0; i < bigHearts.size(); i++) {
		if (playerCollisionBigHeart(i)) {
			bigHearts[i]->setActive(false);
			healthBar->resetHealth();
			AudioEngine::getInstance().play2D("sounds/big_hearth.mp3", false, 0.7f);
		}
	}

	if (invincibilityTimer > 0 && invincibilityTimer != INT_MAX) {
		invincibilityTimer -= deltaTime;
		if (invincibilityTimer <= 0) {
			invincibilityTimer = 0;
			player->setInvulnerable(false);
		}
	}

	for (size_t i = 0; i < deerSkins.size(); i++) {
		if (playerCollisionDeerSkin(i)) {
			deerSkins[i]->setActive(false);
			if (!godModeActive) {
				if (player->isInvulnerable() && invincibilityTimer < 6000 && invincibilityTimer != INT_MAX) {
					invincibilityTimer = 6000;
					player->invulnerabilityTime = 6000;
				}
				else if (!player->isInvulnerable()) {
					invincibilityTimer = 6000;
					player->setInvulnerable(true);
				}
			}
			AudioEngine::getInstance().play2D("sounds/power_up.mp3", false, 0.7f);
		}
	}

	for (size_t i = 0; i < flints.size(); i++) {
		if (playerCollisionFlint(i)) {
			flints[i]->setActive(false);
			remainingFlintHits = 4;

			if (!flintHudInitialized) {
				flintHudSprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.2f, 0.2f), flint->getTexture(), &texProgram);
				flintHudSprite->setNumberAnimations(1);
				flintHudSprite->setAnimationSpeed(0, 8);
				flintHudSprite->addKeyframe(0, glm::vec2(0.0f, 0.4f));
				flintHudSprite->changeAnimation(0);
				flintHudInitialized = true;
			}
			AudioEngine::getInstance().play2D("sounds/power_up.mp3", false, 0.7f);
		}
	}

	if (flintEffectActive) {
		flintEffectPosition.y -= 2;
		if (flintEffectPosition.y < 0) {
			flintEffectActive = false;
		}
	}

	for (size_t i = 0; i < canteens.size(); i++) {
		canteens[i]->update(deltaTime);
	}
	for (size_t i = 0; i < hearts.size(); i++) {
		hearts[i]->update(deltaTime);
	}
	for (size_t i = 0; i < bigHearts.size(); i++) {
		bigHearts[i]->update(deltaTime);
	}
	for (size_t i = 0; i < deerSkins.size(); i++) {
		deerSkins[i]->update(deltaTime);
	}
	for (size_t i = 0; i < flints.size(); i++) {
		flints[i]->update(deltaTime);
	}

    // JUMPER DROP ITEM
	for (int i = 0; i < jumpers.size(); ++i)
	{
		if (jumpers[i]->isDead() && !jumpers[i]->hasDropped()) {
			// Debug para verificar que entra aquí
			printf("Jumper %d murió, dejando item\n", i);
			jumpers[i]->setDropped(true);
			ItemType itemToDrop = determineDropItem();
			dropItem(jumpers[i]->getPos(), itemToDrop);
		}
	}

	// MOLE DROP ITEM
	for (int i = 0; i < moles.size(); ++i)
	{
		if (moles[i]->isDead() && !moles[i]->hasDropped()) {
			// Debug para verificar que entra aquí
			printf("Mole %d murió, dejando item\n", i);
			moles[i]->setDropped(true);
			ItemType itemToDrop = determineDropItem();
			dropItem(moles[i]->getPos(), itemToDrop);
		}
	}

	// SPIDER DROP ITEM
	for (int i = 0; i < spiders.size(); ++i)
	{
		if (spiders[i]->isDead() && !spiders[i]->hasDropped()) {
			// Debug para verificar que entra aquí
			printf("Spider %d murió, dejando item\n", i);
			spiders[i]->setDropped(true);
			ItemType itemToDrop = determineDropItem();
			dropItem(spiders[i]->getPos(), itemToDrop);
		}
	}


	// Verificar colisiones con cantimploras
	for (size_t i = 0; i < canteens.size(); i++) {
		if (playerCollisionCanteen(i)) {
			canteens[i]->setActive(false);
			canteenCount++;

			// Si ha recogido 5 cantimploras, aÃ±adir un corazÃ³n, 1 momentaneamente para probar
			if (canteenCount >= 5) {
				healthBar->addHeart();
				canteenCount = 0;
			}
			AudioEngine::getInstance().play2D("sounds/power_up.mp3", false, 0.7f);
		}
	}
	// Actualizar la posiciÃ³n de la cÃ¡mara basada en la posiciÃ³n del jugador
	glm::vec2 playerPos = player->getPos();
	glm::vec2 cameraPos = playerPos - glm::vec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

	//left limit
	if (cameraPos.y < -134)
		cameraPos.y = -134;
	if (cameraPos.x < -183)
		cameraPos.x = -183;

	//vertical section
	if (cameraPos.x >= 1740 && cameraPos.x <= 1957 && cameraPos.y < 1500)
	{
		cameraPos.x = 1861;
		cameraPos.y += 30;
		if (cameraPos.y >= 1329)cameraPos.y = 1329;
	}

	if (cameraPos.x > 1957 && cameraPos.x < 2810) cameraPos.y = 1329;


	//tree section
	if (cameraPos.x >= 2810 && cameraPos.x <= 2939)
	{
		cameraPos.x = 2870;
		if (cameraPos.y <=330) cameraPos.y = 330;
		if (cameraPos.y >= 1329) cameraPos.y = 1329;
	}

	//upper tree
	if (cameraPos.x >= 2939)
	{
		cameraPos.y = 330;
	}

	if (cameraPos.x >= 3516) cameraPos.x = 3637;


	if (remainingFlintHits > 0 && flintHudInitialized) {
		flintHudSprite->update(deltaTime);
	}


	static bool bossMusicPlaying = false;
	static bool winMusicPlaying = false;

	// Condición para la música del boss
	if (player->isBossLocked() && !boss->isDead()) {
		if (!bossMusicPlaying) {
			AudioEngine::getInstance().stopAll();
			AudioEngine::getInstance().play2D("sounds/boss_music.mp3", true, 0.5f);
			bossMusicPlaying = true;
			winMusicPlaying = false;
		}
	}

	/*Condición para la música de victoria
	else if (boss->isDead()) {
		if (!winMusicPlaying) {
			AudioEngine::getInstance().stopAll();
			AudioEngine::getInstance().play2D("sounds/win_music.mp3", false, 0.5f);
			winMusicPlaying = true;
			bossMusicPlaying = false;
		}
	}
	*/

	//printf("Camera pos: %f, %f\n", cameraPos.x, cameraPos.y);
	if (bossShakeActive) {
		float offsetX = (((rand() % 100) / 100.0f) * 2 - 1) * bossShakeIntensity;
		float offsetY = (((rand() % 100) / 100.0f) * 2 - 1) * bossShakeIntensity;

		projection = glm::ortho(
			cameraPos.x + 200 + 35 + offsetX,
			cameraPos.x + SCREEN_WIDTH - 200 + 35 + offsetX,
			cameraPos.y + SCREEN_HEIGHT - 150 + 20 + offsetY,
			cameraPos.y + 150 + 20 + offsetY
		);
	}
	else {
		projection = glm::ortho(
			cameraPos.x + 200 + 35,
			cameraPos.x + SCREEN_WIDTH - 200 + 35,
			cameraPos.y + SCREEN_HEIGHT - 150 + 20,
			cameraPos.y + 150 + 20
		);
	}

	if (victoryItemVisible && victoryItemSprite != nullptr) {
		glm::ivec2 playerPos = player->getPos();
		// Posicionar el ítem visual sobre la cabeza del jugador
		victoryItemSprite->setPosition(glm::vec2(float(playerPos.x + 36), float(playerPos.y + 33)));
		victoryItemSprite->update(deltaTime);
	}

}

void Scene::render()
{
	glm::mat4 modelview;
	//Elementos del juego
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	background->render();
	map->render();
	for (int i = 0; i < jumpers.size(); ++i) jumpers[i]->render();
	for (int i = 0; i < moles.size(); ++i) moles[i]->render();
	for (int i = 0; i < spiders.size(); ++i)spiders[i]->render();
	boss->render();
	for (int i = 0; i < woodlogs.size(); ++i) {
		woodlogs[i]->render();
	}

	player->render();
	if (finalItem->isActive()) {
		finalItem->render();
	}

	// Si el ítem visual de victoria está activo, renderizarlo
	if (victoryItemVisible && victoryItemSprite != nullptr) {
		victoryItemSprite->render();
	}

	for (Heart* h : hearts) {
		if (h->isActive()) h->render();
	}
	for (BigHeart* bh : bigHearts) {
		if (bh->isActive()) bh->render();
	}
	for (DeerSkin* ds : deerSkins) {
		if (ds->isActive()) ds->render();
	}
	for (Flint* f : flints) {
		if (f->isActive()) f->render();
	}
	for (Canteen* canteen : canteens) {
		canteen->render();
	}

	if (flintEffectActive && flintEffectSprite != nullptr) {
		flintEffectSprite->setPosition(glm::vec2(float(flintEffectPosition.x), float(flintEffectPosition.y)));
		flintEffectSprite->render();
	}

	//HUD
	glm::mat4 hudProjection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
	texProgram.setUniformMatrix4f("projection", hudProjection);
	healthBar->render();
	if (player->isBossLocked() && !boss->isDead()) bossBar->render();
	weaponSelector->render();
	potions->render();

	if (remainingFlintHits > 0 && flintHudInitialized) {
		flintHudSprite->setPosition(glm::vec2(20.0f, SCREEN_HEIGHT - 40.0f));
		flintHudSprite->render();
	}
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}





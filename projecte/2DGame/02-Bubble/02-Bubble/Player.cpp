#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Player.h"
#include "Game.h"
#include "Scene.h"


#define JUMP_ANGLE_STEP 5
#define DAMAGE_JUMP_ANGLE_STEP 6 
#define JUMP_HEIGHT 96
#define FALL_STEP 4


enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, DAMAGE_LEFT, DAMAGE_RIGHT, 
	JUMP_LEFT, JUMP_RIGHT, CROUCH_LEFT, CROUCH_RIGHT,
	ATTACK_STAND_LEFT, ATTACK_STAND_RIGHT, ATTACK_MOVE_LEFT, ATTACK_MOVE_RIGHT,
	ATTACK_CROUCH_LEFT, ATTACK_CROUCH_RIGHT, PROTECT_LEFT, PROTECT_RIGHT,
	PROTECT_JUMP_LEFT, PROTECT_JUMP_RIGHT, VICTORY_POSE
};

enum SpearTipAnims
{
	SPEAR_TIP_LEFT, SPEAR_TIP_RIGHT
};

enum FireAnims
{
	FIRE_RIGHT, FIRE_LEFT
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, HealthBar* healthBar, Potions* potions)
{
	bossLocked = false;
	bPaused = false;
	bVictoryPose = false;
	this->healthBar = healthBar;
	this->potions = potions;
	sizePlayer = glm::ivec2(32, 32);
	bJumping = false;
	bDamage = false;
	bInvulnerable = false;
	bAttacking = false;
	invulnerabilityTime = 0;
	damageAnimationTime = 0;
	facingDirection = 1;
	attackAnimationTime = 0;
	bAttackKeyReleased = true;
	bJumpKeyReleased = true;
	bAttackActive = false;
	bProtecting = false;
	currentTime = 0;
	currentWeapon = WeaponSelector::SPEAR;
	spritesheet.loadFromFile("sprites/spritesheet_soaring_eagle.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(32, 64), glm::vec2(0.125, 0.125), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(21);
	
		sprite->setAnimationSpeed(STAND_LEFT, 8);
		sprite->addKeyframe(STAND_LEFT, glm::vec2(0.875f, 0.25f));
		
		sprite->setAnimationSpeed(STAND_RIGHT, 8);
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.25f, 0.f));
		
		sprite->setAnimationSpeed(MOVE_LEFT, 16);
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.375f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.125f, 0.375f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.25f, 0.375f));
		
		sprite->setAnimationSpeed(MOVE_RIGHT, 16);
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.375f, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.5f, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.625f, 0.f));

		sprite->setAnimationSpeed(DAMAGE_RIGHT, 8);
		sprite->addKeyframe(DAMAGE_RIGHT, glm::vec2(0.875f, 0.125f)); 

		sprite->setAnimationSpeed(DAMAGE_LEFT, 8);
		sprite->addKeyframe(DAMAGE_LEFT, glm::vec2(0.375f, 0.375f));

		sprite->setAnimationSpeed(JUMP_LEFT, 8);
		sprite->addKeyframe(JUMP_LEFT, glm::vec2(0.5f, 0.375f));

		sprite->setAnimationSpeed(JUMP_RIGHT, 8);
		sprite->addKeyframe(JUMP_RIGHT, glm::vec2(0.375f, 0.125f));

		sprite->setAnimationSpeed(CROUCH_LEFT, 8);
		sprite->addKeyframe(CROUCH_LEFT, glm::vec2(0.5f, 0.375f));

		sprite->setAnimationSpeed(CROUCH_RIGHT, 8);
		sprite->addKeyframe(CROUCH_RIGHT, glm::vec2(0.375f, 0.125f));

		sprite->setAnimationSpeed(ATTACK_STAND_LEFT, 8);
		sprite->addKeyframe(ATTACK_STAND_LEFT, glm::vec2(0.375f, 0.5f));

		sprite->setAnimationSpeed(ATTACK_STAND_RIGHT, 8);
		sprite->addKeyframe(ATTACK_STAND_RIGHT, glm::vec2(0.75f, 0.0f));
		
		sprite->setAnimationSpeed(ATTACK_MOVE_LEFT, 16);
		sprite->addKeyframe(ATTACK_MOVE_LEFT, glm::vec2(0.25f, 0.5f));
		sprite->addKeyframe(ATTACK_MOVE_LEFT, glm::vec2(0.125f, 0.5f));
		sprite->addKeyframe(ATTACK_MOVE_LEFT, glm::vec2(0.0f, 0.5f));

		sprite->setAnimationSpeed(ATTACK_MOVE_RIGHT, 16);
		sprite->addKeyframe(ATTACK_MOVE_RIGHT, glm::vec2(0.875f, 0.0f)); 
		sprite->addKeyframe(ATTACK_MOVE_RIGHT, glm::vec2(0.0f, 0.125f));  
		sprite->addKeyframe(ATTACK_MOVE_RIGHT, glm::vec2(0.125f, 0.125f));

		sprite->setAnimationSpeed(ATTACK_CROUCH_RIGHT, 8);
		sprite->addKeyframe(ATTACK_CROUCH_RIGHT, glm::vec2(0.5f, 0.125f));

		sprite->setAnimationSpeed(ATTACK_CROUCH_LEFT, 8);
		sprite->addKeyframe(ATTACK_CROUCH_LEFT, glm::vec2(0.625f, 0.375f));

		sprite->setAnimationSpeed(PROTECT_RIGHT, 8);
		sprite->addKeyframe(PROTECT_RIGHT, glm::vec2(0.25f, 0.125f));

		sprite->setAnimationSpeed(PROTECT_LEFT, 8);
		sprite->addKeyframe(PROTECT_LEFT, glm::vec2(0.75f, 0.375f));

		sprite->setAnimationSpeed(PROTECT_JUMP_RIGHT, 8);
		sprite->addKeyframe(PROTECT_JUMP_RIGHT, glm::vec2(0.625f, 0.125f));

		sprite->setAnimationSpeed(PROTECT_JUMP_LEFT, 8);
		sprite->addKeyframe(PROTECT_JUMP_LEFT, glm::vec2(0.875f, 0.375f));

		sprite->setAnimationSpeed(VICTORY_POSE, 8);
		sprite->addKeyframe(VICTORY_POSE, glm::vec2(0.375f, 0.25f));

		
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

	// Inicializar el sprite de la punta de la lanza
	sizeSpearTip = glm::ivec2(32, 64);
	spearTip = Sprite::createSprite(sizeSpearTip, glm::vec2(0.125, 0.125), &spritesheet, &shaderProgram);
	spearTip->setNumberAnimations(2);

	spearTip->setAnimationSpeed(SPEAR_TIP_LEFT, 16);
	spearTip->addKeyframe(0, glm::vec2(0.375f, 0.625f));
	spearTip->addKeyframe(0, glm::vec2(0.25f, 0.625f));
	spearTip->addKeyframe(0, glm::vec2(0.125f, 0.625f));
	spearTip->addKeyframe(0, glm::vec2(0.25f, 0.625f));
	spearTip->addKeyframe(0, glm::vec2(0.375f, 0.625f));

	spearTip->setAnimationSpeed(SPEAR_TIP_RIGHT, 16);
	spearTip->addKeyframe(1, glm::vec2(0.5f, 0.5f));
	spearTip->addKeyframe(1, glm::vec2(0.625f, 0.5f));
	spearTip->addKeyframe(1, glm::vec2(0.75f, 0.5f));
	spearTip->addKeyframe(1, glm::vec2(0.625f, 0.5f));
	spearTip->addKeyframe(1, glm::vec2(0.5f, 0.5f));

	spearTip->changeAnimation(0);
	spearTip->setPosition(glm::vec2(float(tileMapDispl.x + posSpearTip.x), float(tileMapDispl.y + posSpearTip.y)));


	// Inicializar el sprite de fuego
	fireSprite = Sprite::createSprite(glm::ivec2(32, 64), glm::vec2(0.125, 0.125), &spritesheet, &shaderProgram);
	fireSprite->setNumberAnimations(2);

	fireSprite->setAnimationSpeed(FIRE_RIGHT, 8);
	fireSprite->addKeyframe(FIRE_RIGHT, glm::vec2(0.5f, 0.625f));
	fireSprite->addKeyframe(FIRE_RIGHT, glm::vec2(0.625f, 0.625f));


	fireSprite->setAnimationSpeed(FIRE_LEFT, 8);
	fireSprite->addKeyframe(FIRE_LEFT, glm::vec2(0.75f, 0.625f));
	fireSprite->addKeyframe(FIRE_LEFT, glm::vec2(0.875f, 0.625f));

	fireSprite->changeAnimation(0);
	fireAnimTime = 0;

}

void Player::update(int deltaTime)
{		
	sprite->update(deltaTime);
	currentTime += deltaTime;

	if (bVictoryPose) {
		// Solo actualizar la posición del sprite
		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
		return;
	}

	if (bPaused) {
		if (facingDirection == 0) {
			sprite->changeAnimation(STAND_LEFT);
		}
		else {
			sprite->changeAnimation(STAND_RIGHT);
		}
		return;
	}

	if (bAttacking && currentWeapon == WeaponSelector::FIRE) {
		fireSprite->update(deltaTime);
		fireAnimTime += deltaTime;
	}

	// Si esta en estado de invulnerabilidad
	if (bInvulnerable) {
		if (invulnerabilityTime != INT_MAX) {
			invulnerabilityTime -= deltaTime;
			if (invulnerabilityTime <= 0) {
				bInvulnerable = false;
				invulnerabilityTime = 0;
			}
		}
	}
	// Si esta en estado de daño
	if (bDamage) {
		damageJumpAngle += JUMP_ANGLE_STEP;
		posPlayer.y = int(damageStartY - 48 * sin(3.14159f * damageJumpAngle / 180.f));

		
		if (Game::instance().getKey(GLFW_KEY_LEFT)) {
			posPlayer.x -= 1; 
			facingDirection = 0;
			if (map->collisionMoveLeft(posPlayer + glm::ivec2(5, 32), glm::ivec2(32-10, 32))) {
				posPlayer.x += 1;
			}

		}
		else if (Game::instance().getKey(GLFW_KEY_RIGHT)) {
			posPlayer.x += 1;
			facingDirection = 1;
			if (map->collisionMoveRight(posPlayer + glm::ivec2(5, 32), glm::ivec2(32-10, 32))) {
				posPlayer.x -= 1;
			}
		}
		// Comprobar si ha completado el salto o ha tocado el suelo
		if (damageJumpAngle > 90) {
			int collision = map->collisionMoveDown(posPlayer + glm::ivec2(5, 0), glm::ivec2(32 - 10, 64), &posPlayer.y);
			bool onGround = collision != 0;

			if (onGround || damageJumpAngle >= 180) {
				bDamage = false;
				damageJumpAngle = 180;

				// Verificar si la tecla X está siendo presionada para volver al estado de ataque
				if (Game::instance().getKey('X')) {
					bAttacking = true;
					bAttackKeyReleased = false;
					attackAnimationTime = (5 * 1000) / 16; 

					// Determinar la animación de ataque correspondiente según la posición y dirección
					if (Game::instance().getKey(GLFW_KEY_DOWN) && !bJumping) {
						// Ataque agachado
						if (facingDirection == 0) {
							sprite->changeAnimation(ATTACK_CROUCH_LEFT);
							spearTip->changeAnimation(SPEAR_TIP_LEFT);
							for (int i = 0; i < 4; i++) {
								spearTip->update(1000);
							}
							spearTip->update(0);
						}
						else {
							sprite->changeAnimation(ATTACK_CROUCH_RIGHT);
							spearTip->changeAnimation(SPEAR_TIP_RIGHT);
							for (int i = 0; i < 4; i++) {
								spearTip->update(1000);
							}
							spearTip->update(0);
						}
					}
					else if (Game::instance().getKey(GLFW_KEY_LEFT) || Game::instance().getKey(GLFW_KEY_RIGHT)) {
						// Ataque en movimiento
						if (facingDirection == 0) {
							sprite->changeAnimation(ATTACK_MOVE_LEFT);
							spearTip->changeAnimation(SPEAR_TIP_LEFT);
							for (int i = 0; i < 4; i++) {
								spearTip->update(1000);
							}
							spearTip->update(0);
						}
						else {
							sprite->changeAnimation(ATTACK_MOVE_RIGHT);
							spearTip->changeAnimation(SPEAR_TIP_RIGHT);
							for (int i = 0; i < 4; i++) {
								spearTip->update(1000);
							}
							spearTip->update(0);
						}
					}
					else {
						// Ataque parado
						if (facingDirection == 0) {
							sprite->changeAnimation(ATTACK_STAND_LEFT);
							spearTip->changeAnimation(SPEAR_TIP_LEFT);
							for (int i = 0; i < 4; i++) {
								spearTip->update(1000);
							}
							spearTip->update(0);
						}
						else {
							sprite->changeAnimation(ATTACK_STAND_RIGHT);
							spearTip->changeAnimation(SPEAR_TIP_RIGHT);
							for (int i = 0; i < 4; i++) {
								spearTip->update(1000);
							}
							spearTip->update(0);
						}
					}
				}
				else {
					bAttacking = false;

					if (facingDirection == 1) { 
						if (Game::instance().getKey(GLFW_KEY_RIGHT))
							sprite->changeAnimation(MOVE_RIGHT);
						else
							sprite->changeAnimation(STAND_RIGHT);
					}
					else {
						if (Game::instance().getKey(GLFW_KEY_LEFT))
							sprite->changeAnimation(MOVE_LEFT);
						else
							sprite->changeAnimation(STAND_LEFT);
					}
				}
			}
		}


	}
	// Si no esta en estado de daño
	else {
		
		if (bAttacking) {
			attackAnimationTime += deltaTime;
			if (attackAnimationTime >= (5 * 1000) / 16) {
				if (Game::instance().getKey('X')) {
					attackAnimationTime = (5 * 1000) / 16;
					bAttackActive = false;
					// Forzar el frame específico que queremos mostrar como "frame final"
					if (facingDirection == 0) {
						spearTip->changeAnimation(SPEAR_TIP_LEFT);
						for (int i = 0; i < 4; i++) {
							spearTip->update(1000); // Avanza la animación hasta llegar al frame deseado
						}
						spearTip->update(0); // Detiene la animación
					}
					else {
						spearTip->changeAnimation(SPEAR_TIP_RIGHT);
						for (int i = 0; i < 4; i++) {
							spearTip->update(1000); // Avanza la animación hasta llegar al frame deseado
						}
						spearTip->update(0); // Detiene la animación
					}
				} else {
					// Si ya no está presionada la X, volver a la animación normal
					bAttacking = false;
					bAttackActive = false;
					attackAnimationTime = 0;
					
					if (facingDirection == 0)
						sprite->changeAnimation(STAND_LEFT);
					else
						sprite->changeAnimation(STAND_RIGHT);
				}
			}
			else {
				// Durante la animación activa de ataque
				bAttackActive = true;
			}
	}

	if (!Game::instance().getKey('X')) {
		bAttackKeyReleased = true;
	}
	if (!Game::instance().getKey('Z')) {
		bJumpKeyReleased = true;
	}

	// Protección al pulsar flecha arriba 
	if (Game::instance().getKey(GLFW_KEY_UP) && !bAttacking && !bDamage)
	{
		bProtecting = true;

		// Si está saltando, usar animación de protección en salto
		if (bJumping) {
			AudioEngine::getInstance().play2D("sounds/spear_attack.mp3", false, 0.8f);
			if (facingDirection == 0)
				sprite->changeAnimation(PROTECT_JUMP_LEFT);
			else
				sprite->changeAnimation(PROTECT_JUMP_RIGHT);

			if (Game::instance().getKey(GLFW_KEY_LEFT)) {
				facingDirection = 0;
				sprite->changeAnimation(PROTECT_JUMP_LEFT);

				posPlayer.x -= 2;
				if (map->collisionMoveLeft(posPlayer + glm::ivec2(5, 32), glm::ivec2(32 - 10, 32)))
					posPlayer.x += 2;
			}
			else if (Game::instance().getKey(GLFW_KEY_RIGHT)) {
				facingDirection = 1;
				sprite->changeAnimation(PROTECT_JUMP_RIGHT);

				posPlayer.x += 2;
				if (map->collisionMoveRight(posPlayer + glm::ivec2(5, 32), glm::ivec2(32 - 10, 32)))
					posPlayer.x -= 2;
			}
		}
		// Si no está saltando, usar animación normal de protección (sin movimiento)
		else {
			if (Game::instance().getKey(GLFW_KEY_LEFT))
			{
				facingDirection = 0;
				sprite->changeAnimation(PROTECT_LEFT);
			}
			else if (Game::instance().getKey(GLFW_KEY_RIGHT))
			{
				facingDirection = 1;
				sprite->changeAnimation(PROTECT_RIGHT);
			}
			else
			{
				if (facingDirection == 0)
					sprite->changeAnimation(PROTECT_LEFT);
				else
					sprite->changeAnimation(PROTECT_RIGHT);
			}
		}
	}
	else
	{
		bProtecting = false;
	}
    
	if (Game::instance().getKey('X') && !bDamage && !bAttacking && bAttackKeyReleased) {
		bAttacking = true;
		bAttackActive = true;
		bAttackKeyReleased = false;
		attackAnimationTime = 0;

		// Reproducir sonido según el tipo de arma seleccionada
		if (currentWeapon == WeaponSelector::SPEAR) {
			// Sonido de lanza - solo una vez al iniciar el ataque
			AudioEngine::getInstance().play2D("sounds/spear_attack.mp3", false, 0.8f);
		}

		if (Game::instance().getKey(GLFW_KEY_DOWN) && !bJumping) {
			if (facingDirection == 0)
				sprite->changeAnimation(ATTACK_CROUCH_LEFT);
			else
				sprite->changeAnimation(ATTACK_CROUCH_RIGHT);
		}
		else if (bJumping) {
			if (facingDirection == 0)
				sprite->changeAnimation(ATTACK_CROUCH_LEFT);
			else
				sprite->changeAnimation(ATTACK_CROUCH_RIGHT);
		}
		else if (facingDirection == 0)
			sprite->changeAnimation(ATTACK_STAND_LEFT);
		else
			sprite->changeAnimation(ATTACK_STAND_RIGHT);

		// Configurar la animación de la punta de lanza según el arma seleccionada
		if (currentWeapon == WeaponSelector::SPEAR) {
			spearTip->changeAnimation(facingDirection);
		}
		else if (currentWeapon == WeaponSelector::FIRE) {
			spearTip->changeAnimation(facingDirection);
		}
	}

	// Para el arma de fuego, reproducir el sonido continuamente mientras se mantiene presionado X
	if (bAttacking && currentWeapon == WeaponSelector::FIRE) {
		static int fireSoundTimer = 0;
		fireSoundTimer += deltaTime;

		if (fireSoundTimer >= 500) {
			AudioEngine::getInstance().play2D("sounds/fire_attack.mp3", false, 0.5f);
			fireSoundTimer = 0;
		}
	}


	
	if (!bProtecting) {
		// si está agachando
		if (Game::instance().getKey(GLFW_KEY_DOWN) && !bJumping)
		{
			if (bAttacking) {
				if (Game::instance().getKey(GLFW_KEY_LEFT)) {
					facingDirection = 0;
					sprite->changeAnimation(ATTACK_CROUCH_LEFT);
				}
				else if (Game::instance().getKey(GLFW_KEY_RIGHT)) {
					facingDirection = 1;
					sprite->changeAnimation(ATTACK_CROUCH_RIGHT);
				}
				else {
					if (facingDirection == 0)
						sprite->changeAnimation(ATTACK_CROUCH_LEFT);
					else
						sprite->changeAnimation(ATTACK_CROUCH_RIGHT);
				}
			}
			else {
				if (Game::instance().getKey(GLFW_KEY_LEFT))
				{
					facingDirection = 0;
					sprite->changeAnimation(CROUCH_LEFT);
				}
				else if (Game::instance().getKey(GLFW_KEY_RIGHT))
				{
					facingDirection = 1;
					sprite->changeAnimation(CROUCH_RIGHT);
				}
				else
				{
					if (facingDirection == 1)
					{
						sprite->changeAnimation(CROUCH_RIGHT);
					}
					else
					{
						sprite->changeAnimation(CROUCH_LEFT);
					}
				}
			}
		}
		// Si no esta agachando
		else if (Game::instance().getKey(GLFW_KEY_LEFT))
		{
			facingDirection = 0;
			if (bAttacking) {
				if (sprite->animation() != ATTACK_MOVE_LEFT) {
					sprite->changeAnimation(ATTACK_MOVE_LEFT);
				}
			}
			else if (bJumping) {
				sprite->changeAnimation(JUMP_LEFT);
			}
			else if (sprite->animation() != MOVE_LEFT) {
				sprite->changeAnimation(MOVE_LEFT);
			}

			posPlayer.x -= 2;
			if (map->collisionMoveLeft(posPlayer + glm::ivec2(5, 32), glm::ivec2(32 - 10, 32)))
			{
				posPlayer.x += 2;
				if (bAttacking)
					sprite->changeAnimation(ATTACK_STAND_LEFT);
				else
					sprite->changeAnimation(STAND_LEFT);
			}
		}
		else if (Game::instance().getKey(GLFW_KEY_RIGHT)) {
			facingDirection = 1;
			if (bAttacking) {
				if (sprite->animation() != ATTACK_MOVE_RIGHT) {
					sprite->changeAnimation(ATTACK_MOVE_RIGHT);
				}
			}
			else if (bJumping) {
				sprite->changeAnimation(JUMP_RIGHT);
			}
			else if (sprite->animation() != MOVE_RIGHT) {
				sprite->changeAnimation(MOVE_RIGHT);
			}

			posPlayer.x += 2;
			if (map->collisionMoveRight(posPlayer + glm::ivec2(5, 32), glm::ivec2(32 - 10, 32)))
			{
				posPlayer.x -= 2;
				if (bAttacking)
					sprite->changeAnimation(ATTACK_STAND_RIGHT);
				else
					sprite->changeAnimation(STAND_RIGHT);
			}
		}
		else
		{
			if (bAttacking) {
				// Si esta atacando mientras esta parado
				if (facingDirection == 0)
					sprite->changeAnimation(ATTACK_STAND_LEFT);
				else
					sprite->changeAnimation(ATTACK_STAND_RIGHT);
			}
			else if (bJumping) {
				if (facingDirection == 0)
					sprite->changeAnimation(JUMP_LEFT);
				else
					sprite->changeAnimation(JUMP_RIGHT);
			}
			else
			{
				if (facingDirection == 0)
					sprite->changeAnimation(STAND_LEFT);
				else
					sprite->changeAnimation(STAND_RIGHT);
			}
		}
	}

		// Saltar
		if (bJumping)
		{
			jumpAngle += JUMP_ANGLE_STEP;
			if (jumpAngle == 180)
			{
				bJumping = false;
				posPlayer.y = startY;

				if (bAttacking) {
					if (facingDirection == 0)
						sprite->changeAnimation(ATTACK_STAND_LEFT);
					else
						sprite->changeAnimation(ATTACK_STAND_RIGHT);
				}
			}
			else
			{
				posPlayer.y = int(startY - 50 * sin(3.14159f * jumpAngle / 180.f));
				if (jumpAngle > 90) {
					bool wasJumping = bJumping;
					int collision = map->collisionMoveDown(posPlayer + glm::ivec2(5, 0), glm::ivec2(32 - 10, 64), &posPlayer.y);
					bJumping = collision == 0;

					if (wasJumping && !bJumping) {
						// Reproducir sonido de aterrizaje

						if (bAttacking) {
							if (facingDirection == 0)
								sprite->changeAnimation(ATTACK_STAND_LEFT);
							else
								sprite->changeAnimation(ATTACK_STAND_RIGHT);
						}
					}
				}
				if (bProtecting) {
					if (facingDirection == 0)
						sprite->changeAnimation(PROTECT_JUMP_LEFT);
					else
						sprite->changeAnimation(PROTECT_JUMP_RIGHT);
				}
				if (bAttacking) {
					if (facingDirection == 0)
						sprite->changeAnimation(ATTACK_CROUCH_LEFT);
					else
						sprite->changeAnimation(ATTACK_CROUCH_RIGHT);
				}
			}
		}
		else
		{
			posPlayer.y += FALL_STEP;
			int collision = map->collisionMoveDown(posPlayer + glm::ivec2(5, 0), glm::ivec2(32 - 10, 64), &posPlayer.y);
			if (collision == 1)
			{
				if (Game::instance().getKey('Z') && bJumpKeyReleased)
				{
					bJumping = true;
					bJumpKeyReleased = false;
					jumpAngle = 0;
					startY = posPlayer.y;
					AudioEngine::getInstance().play2D("sounds/jump.mp3", false, 0.6f);
				}
			}
			else if (collision == 2 && !bInvulnerable && !bDamage)
			{
				takeDamage();
				healthBar->decreaseHealth();
				// Solo reposicionamos al jugador al inicio si estÃ¡ sin vidas
				if (healthBar->isDead() && potions->hasPotions()) {
					// Restaurar la salud del jugador
					healthBar->resetHealth();
					AudioEngine::getInstance().play2D("sounds/heal_sound.mp3", false, 0.6f);

					// Usar una pociÃ³n
					potions->usePotion();
				}
			}
			else if (collision == 2)
			{
				if (Game::instance().getKey('Z') && bJumpKeyReleased)
				{
					bJumping = true;
					bJumpKeyReleased = false;
					jumpAngle = 0;
					startY = posPlayer.y;

					AudioEngine::getInstance().play2D("sounds/jump.mp3", false, 0.6f);
				}
			}

			else if (collision == 0 && sprite->animation() == MOVE_LEFT)
				sprite->changeAnimation(JUMP_LEFT);
			else if (collision == 0 && sprite->animation() == MOVE_RIGHT)
				sprite->changeAnimation(JUMP_RIGHT);
		}

	}

	if (bAttacking) {
		if (spearTip->animation() != facingDirection)
			spearTip->changeAnimation(facingDirection);

		if (currentWeapon == WeaponSelector::FIRE) {
			if (facingDirection == 0) { 
				spearTip->changeAnimation(SPEAR_TIP_LEFT);
				spearTip->update(0); 
			}
			else {
				spearTip->changeAnimation(SPEAR_TIP_RIGHT);
				spearTip->update(0); 
			}

			
			if (fireSprite->animation() != (facingDirection == 1 ? FIRE_RIGHT : FIRE_LEFT))
				fireSprite->changeAnimation(facingDirection == 1 ? FIRE_RIGHT : FIRE_LEFT);
		}

		bool bCrouching = (Game::instance().getKey(GLFW_KEY_DOWN) && !bJumping) || bJumping;

		
		float offsetX = 0.0f;

		
		if (currentWeapon == WeaponSelector::SPEAR) {
			// Calcular un factor de progresión para la extensión y retracción de la lanza
			float maxTime = (5.0f * 1000) / 16;
			float progressTime = (attackAnimationTime < maxTime) ? attackAnimationTime : maxTime;
			float animProgress;

			// Si estamos en la primera mitad de la animación (extendiéndose)
			if (progressTime < maxTime / 2) {
				// De 0 a 1 (extendiendo)
				animProgress = progressTime / (maxTime / 2);
			}
			else {
				// De 1 a 0 (retrayendo)
				animProgress = 1.0f - ((progressTime - maxTime / 2) / (maxTime / 2));
			}

			// Convertir el progreso en un desplazamiento
			offsetX = animProgress * 2.0f;
		}

		if (facingDirection == 1) { // Derecha
			if (bCrouching) {
				posSpearTip = glm::ivec2(posPlayer.x + 16 + offsetX, posPlayer.y + 8);
			}
			else {
				posSpearTip = glm::ivec2(posPlayer.x + 17 + offsetX, posPlayer.y);
			}
		}
		else { // Izquierda
			if (bCrouching) {
				posSpearTip = glm::ivec2(posPlayer.x - 14 - offsetX, posPlayer.y + 8);
			}
			else {
				posSpearTip = glm::ivec2(posPlayer.x - 15 - offsetX, posPlayer.y);
			}
		}

		spearTip->setPosition(glm::vec2(float(tileMapDispl.x + posSpearTip.x), float(tileMapDispl.y + posSpearTip.y)));

		
		if (currentWeapon == WeaponSelector::SPEAR) {
			spearTip->update(deltaTime);
		}

		if (currentWeapon == WeaponSelector::FIRE) {
			glm::vec2 firePos;
			if (facingDirection == 1) { 
				firePos = glm::vec2(float(tileMapDispl.x + posSpearTip.x + 12), float(tileMapDispl.y + posSpearTip.y));
			}
			else { 
				firePos = glm::vec2(float(tileMapDispl.x + posSpearTip.x - 12), float(tileMapDispl.y + posSpearTip.y));
			}
			fireSprite->setPosition(firePos);
			fireSprite->update(deltaTime); 
		}
	}
	if (posPlayer.x >= 3838) bossLocked = true; //lock in boss room
	if (bossLocked && posPlayer.x <= 3838) posPlayer.x = 3838; //lock in boss room
	//printf("PosPlayer: %d %d\n", posPlayer.x, posPlayer.y);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::takeDamage()
{
	if (!bDamage) {
		bDamage = true;
		bInvulnerable = true;
		bAttacking = false;
		damageJumpAngle = 0;
		damageStartY = posPlayer.y;
		invulnerabilityTime = 3000;

		AudioEngine::getInstance().play2D("sounds/hurt.mp3", false, 1);

		if (facingDirection == 1)
			sprite->changeAnimation(DAMAGE_RIGHT);
		else
			sprite->changeAnimation(DAMAGE_LEFT);
	}
}

bool Player::isInvulnerable()
{
	return bInvulnerable;
}

void Player::setInvulnerable(bool invulnerable)
{
	bInvulnerable = invulnerable;
	if (invulnerable) {
		if (invulnerabilityTime != INT_MAX) {
			invulnerabilityTime = 6000;
		}
	}
	else {
		invulnerabilityTime = 0;
	}
}

bool Player::isInDamageState()
{
	return bDamage;
}

void Player::render()
{
	if (bVictoryPose) {
		sprite->render();
		return;
	}

	if (bPaused) {
		sprite->render();
		return;
	}

	if (bInvulnerable && ((currentTime / 100) % 2 == 0)) {
	}
	else {
		if (bAttacking && !bPaused) {
			spearTip->render();
			if (currentWeapon == WeaponSelector::FIRE) {
				fireSprite->render();
			}
		}
		sprite->render();
	}
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

glm::ivec2 Player::getPos()
{
	return posPlayer;
}

glm::ivec2 Player::getSize()
{
	return sizePlayer;
}

bool Player::isAttacking()
{
	if (currentWeapon == WeaponSelector::SPEAR) {
		return bAttacking && bAttackActive;
	}
	else if (currentWeapon == WeaponSelector::FIRE) {
		return bAttacking;
	}

	return false;
}

glm::ivec2 Player::getSpearTipPos()
{
	return posSpearTip;
}

glm::ivec2 Player::getSpearTipSize()
{
	return sizeSpearTip;
}

void Player::setCurrentWeapon(int weaponType)
{
	currentWeapon = weaponType;
}

bool Player::isJumping()
{ 
	return bJumping; 
}
bool Player::isProtecting()
{ 
	return bProtecting; 
}

bool Player::isBossLocked() 
{
	return bossLocked;
}

void Player::setPaused(bool paused)
{
	bPaused = paused;
}

bool Player::isPaused() const
{
	return bPaused;
}
void Player::setVictoryPose()
{
	sprite->changeAnimation(VICTORY_POSE);
	sprite->update(0);
	bVictoryPose = true;
}
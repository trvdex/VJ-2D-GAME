#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "DeerSkin.h"
#include "Game.h"

#define JUMP_ANGLE_STEP 8

enum DeerSkinAnims
{
    IDLE
};

DeerSkin::DeerSkin()
{
    sprite = nullptr;
    active = false;
    bJumping = false;
}

DeerSkin::~DeerSkin()
{
    if (sprite != nullptr)
        delete sprite;
}

void DeerSkin::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
    sizeDeerSkin = glm::ivec2(16, 16);

    spritesheet.loadFromFile("sprites/spritesheet_items.png", TEXTURE_PIXEL_FORMAT_RGBA);
    spritesheet.setMinFilter(GL_NEAREST);
    spritesheet.setMagFilter(GL_NEAREST);

    sprite = Sprite::createSprite(sizeDeerSkin, glm::vec2(0.2f, 0.2f), &spritesheet, &shaderProgram);
    sprite->setNumberAnimations(1);

    sprite->setAnimationSpeed(IDLE, 2);
    sprite->addKeyframe(IDLE, glm::vec2(0.4f, 0.4f));

    sprite->changeAnimation(IDLE);
    tileMapDispl = tileMapPos;
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posDeerSkin.x), float(tileMapDispl.y + posDeerSkin.y)));
}

void DeerSkin::update(int deltaTime)
{
    sprite->update(deltaTime);

    if (active) {
        if (bJumping) {
            // Actualizar ángulo de salto
            jumpAngle += JUMP_ANGLE_STEP;

            // Calcular nueva posición Y basada en el ángulo de salto
            posDeerSkin.y = int(initPosY - 30 * sin(3.14159f * jumpAngle / 180.f));

            // Mover horizontalmente si tiene velocidad X
            posDeerSkin.x += jumpVelocityX;

            // Verificar colisiones con el mapa si está cayendo
            if (jumpAngle > 90) {
                bool onGround = map->collisionMoveDown(posDeerSkin, sizeDeerSkin, &posDeerSkin.y);

                // Si toca el suelo o completa el arco, terminar el salto
                if (onGround || jumpAngle >= 180) {
                    bJumping = false;
                }
            }

            // Verificar colisiones laterales
            if (jumpVelocityX > 0) {
                if (map->collisionMoveRight(posDeerSkin, sizeDeerSkin)) {
                    jumpVelocityX = -jumpVelocityX;
                }
            }
            else if (jumpVelocityX < 0) {
                if (map->collisionMoveLeft(posDeerSkin, sizeDeerSkin)) {
                    jumpVelocityX = -jumpVelocityX;
                }
            }
        }
        else {
            // Si no está saltando, verifica si debe caer
            int prevY = posDeerSkin.y;
            posDeerSkin.y += 2;

            if (map->collisionMoveDown(posDeerSkin, sizeDeerSkin, &posDeerSkin.y)) {
                // Está en el suelo
            }
        }

        sprite->setPosition(glm::vec2(float(tileMapDispl.x + posDeerSkin.x), float(tileMapDispl.y + posDeerSkin.y)));
    }
}

void DeerSkin::render()
{
    if (active) {
        sprite->render();
    }
}

void DeerSkin::setTileMap(TileMap* tileMap)
{
    map = tileMap;
}

void DeerSkin::setPosition(const glm::vec2& pos)
{
    posDeerSkin = pos;
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posDeerSkin.x), float(tileMapDispl.y + posDeerSkin.y)));
}

glm::ivec2 DeerSkin::getPos()
{
    return posDeerSkin;
}

glm::ivec2 DeerSkin::getSize()
{
    return sizeDeerSkin;
}

bool DeerSkin::isActive() const
{
    return active;
}

void DeerSkin::setActive(bool active)
{
    this->active = active;
}

void DeerSkin::activate(const glm::vec2& startPos)
{
    posDeerSkin = startPos;
    active = true;
    bJumping = true;
    jumpAngle = 0;
    initPosY = startPos.y;

    // Velocidad horizontal aleatoria pequeña
    jumpVelocityX = (rand() % 3 - 1) * 0.5f;

    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posDeerSkin.x), float(tileMapDispl.y + posDeerSkin.y)));
}

Texture* DeerSkin::getTexture()
{
    return &spritesheet;
}

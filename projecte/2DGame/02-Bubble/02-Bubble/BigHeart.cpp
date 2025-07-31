#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <random>
#include "BigHeart.h"
#include "Game.h"
#include "Scene.h"

#define JUMP_ANGLE_STEP 8

enum BigHeartAnims
{
    IDLE
};

BigHeart::BigHeart()
{
    sprite = nullptr;
    active = false;
    bJumping = false;
}

BigHeart::~BigHeart()
{
    if (sprite != nullptr)
        delete sprite;
}

void BigHeart::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
    sizeBigHeart = glm::ivec2(16, 16);

    spritesheet.loadFromFile("sprites/spritesheet_items.png", TEXTURE_PIXEL_FORMAT_RGBA);
    spritesheet.setMinFilter(GL_NEAREST);
    spritesheet.setMagFilter(GL_NEAREST);

    sprite = Sprite::createSprite(sizeBigHeart, glm::vec2(0.2f, 0.2f), &spritesheet, &shaderProgram);
    sprite->setNumberAnimations(1);

    sprite->setAnimationSpeed(IDLE, 2);
    sprite->addKeyframe(IDLE, glm::vec2(0.2f, 0.2f));

    sprite->changeAnimation(IDLE);
    tileMapDispl = tileMapPos;
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBigHeart.x), float(tileMapDispl.y + posBigHeart.y)));
}

void BigHeart::update(int deltaTime)
{
    sprite->update(deltaTime);

    if (active) {
        if (bJumping) {
            // Actualizar ángulo de salto
            jumpAngle += JUMP_ANGLE_STEP;

            // Calcular nueva posición Y basada en el ángulo de salto
            posBigHeart.y = int(initPosY - 30 * sin(3.14159f * jumpAngle / 180.f));

            // Mover horizontalmente si tiene velocidad X
            posBigHeart.x += jumpVelocityX;

            // Verificar colisiones con el mapa si está cayendo
            if (jumpAngle > 90) {
                bool onGround = map->collisionMoveDown(posBigHeart, sizeBigHeart, &posBigHeart.y);

                // Si toca el suelo o completa el arco, terminar el salto
                if (onGround || jumpAngle >= 180) {
                    bJumping = false;
                }
            }

            // Verificar colisiones laterales
            if (jumpVelocityX > 0) {
                if (map->collisionMoveRight(posBigHeart, sizeBigHeart)) {
                    jumpVelocityX = -jumpVelocityX;
                }
            }
            else if (jumpVelocityX < 0) {
                if (map->collisionMoveLeft(posBigHeart, sizeBigHeart)) {
                    jumpVelocityX = -jumpVelocityX;
                }
            }
        }
        else {
            // Si no está saltando, verifica si debe caer
            int prevY = posBigHeart.y;
            posBigHeart.y += 2;

            if (map->collisionMoveDown(posBigHeart, sizeBigHeart, &posBigHeart.y)) {
                // Está en el suelo
            }
        }

        sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBigHeart.x), float(tileMapDispl.y + posBigHeart.y)));
    }
}

void BigHeart::render()
{
    if (active) {
        sprite->render();
    }
}

void BigHeart::setTileMap(TileMap* tileMap)
{
    map = tileMap;
}

void BigHeart::setPosition(const glm::vec2& pos)
{
    posBigHeart = pos;
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBigHeart.x), float(tileMapDispl.y + posBigHeart.y)));
}

glm::ivec2 BigHeart::getPos()
{
    return posBigHeart;
}

glm::ivec2 BigHeart::getSize()
{
    return sizeBigHeart;
}

bool BigHeart::isActive() const
{
    return active;
}

void BigHeart::setActive(bool active)
{
    this->active = active;
}

void BigHeart::activate(const glm::vec2& startPos)
{
    posBigHeart = startPos;
    active = true;
    bJumping = true;
    jumpAngle = 0;
    initPosY = startPos.y;

    // Velocidad horizontal aleatoria pequeña
    jumpVelocityX = (rand() % 3 - 1) * 0.5f;

    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBigHeart.x), float(tileMapDispl.y + posBigHeart.y)));
}

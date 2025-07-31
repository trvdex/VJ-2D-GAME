#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Flint.h"
#include "Game.h"

#define JUMP_ANGLE_STEP 8

enum FlintAnims
{
    IDLE
};

Flint::Flint()
{
    sprite = nullptr;
    active = false;
    bJumping = false;
}

Flint::~Flint()
{
    if (sprite != nullptr)
        delete sprite;
}

void Flint::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
    sizeFlint = glm::ivec2(16, 16);

    spritesheet.loadFromFile("sprites/spritesheet_items.png", TEXTURE_PIXEL_FORMAT_RGBA);
    spritesheet.setMinFilter(GL_NEAREST);
    spritesheet.setMagFilter(GL_NEAREST);

    sprite = Sprite::createSprite(sizeFlint, glm::vec2(0.2f, 0.2f), &spritesheet, &shaderProgram);
    sprite->setNumberAnimations(1);

    sprite->setAnimationSpeed(IDLE, 2);
    sprite->addKeyframe(IDLE, glm::vec2(0.0f, 0.4f));

    sprite->changeAnimation(IDLE);
    tileMapDispl = tileMapPos;
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posFlint.x), float(tileMapDispl.y + posFlint.y)));
}

void Flint::update(int deltaTime)
{
    sprite->update(deltaTime);

    if (active) {
        if (bJumping) {
            // Actualizar ángulo de salto
            jumpAngle += JUMP_ANGLE_STEP;

            // Calcular nueva posición Y basada en el ángulo de salto
            posFlint.y = int(initPosY - 30 * sin(3.14159f * jumpAngle / 180.f));

            // Mover horizontalmente si tiene velocidad X
            posFlint.x += jumpVelocityX;

            // Verificar colisiones con el mapa si está cayendo
            if (jumpAngle > 90) {
                bool onGround = map->collisionMoveDown(posFlint, sizeFlint, &posFlint.y);

                // Si toca el suelo o completa el arco, terminar el salto
                if (onGround || jumpAngle >= 180) {
                    bJumping = false;
                }
            }

            // Verificar colisiones laterales
            if (jumpVelocityX > 0) {
                if (map->collisionMoveRight(posFlint, sizeFlint)) {
                    jumpVelocityX = -jumpVelocityX;
                }
            }
            else if (jumpVelocityX < 0) {
                if (map->collisionMoveLeft(posFlint, sizeFlint)) {
                    jumpVelocityX = -jumpVelocityX;
                }
            }
        }
        else {
            // Si no está saltando, verifica si debe caer
            int prevY = posFlint.y;
            posFlint.y += 2;

            if (map->collisionMoveDown(posFlint, sizeFlint, &posFlint.y)) {
                // Está en el suelo
            }
        }

        sprite->setPosition(glm::vec2(float(tileMapDispl.x + posFlint.x), float(tileMapDispl.y + posFlint.y)));
    }
}

void Flint::render()
{
    if (active) {
        sprite->render();
    }
}

void Flint::setTileMap(TileMap* tileMap)
{
    map = tileMap;
}

void Flint::setPosition(const glm::vec2& pos)
{
    posFlint = pos;
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posFlint.x), float(tileMapDispl.y + posFlint.y)));
}

glm::ivec2 Flint::getPos()
{
    return posFlint;
}

glm::ivec2 Flint::getSize()
{
    return sizeFlint;
}

bool Flint::isActive() const
{
    return active;
}

void Flint::setActive(bool active)
{
    this->active = active;
}

void Flint::activate(const glm::vec2& startPos)
{
    posFlint = startPos;
    active = true;
    bJumping = true;
    jumpAngle = 0;
    initPosY = startPos.y;

    // Velocidad horizontal aleatoria pequeña
    jumpVelocityX = (rand() % 3 - 1) * 0.5f;

    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posFlint.x), float(tileMapDispl.y + posFlint.y)));
}

Texture* Flint::getTexture()
{
    return &spritesheet;
}

#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Canteen.h"
#include "Game.h"
#include "Scene.h"

#define JUMP_ANGLE_STEP 8

enum CanteenAnims
{
    IDLE
};

Canteen::Canteen()
{
    sprite = nullptr;
    bActive = false;
    bJumping = false;
}

Canteen::~Canteen()
{
    if (sprite != nullptr)
        delete sprite;
}

void Canteen::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
    sizeCanteen = glm::ivec2(16, 16);

    spritesheet.loadFromFile("sprites/spritesheet_items.png", TEXTURE_PIXEL_FORMAT_RGBA);
    spritesheet.setMinFilter(GL_NEAREST);
    spritesheet.setMagFilter(GL_NEAREST);

    sprite = Sprite::createSprite(sizeCanteen, glm::vec2(0.2f, 0.2f), &spritesheet, &shaderProgram);
    sprite->setNumberAnimations(1);

    sprite->setAnimationSpeed(IDLE, 2);
    sprite->addKeyframe(IDLE, glm::vec2(0.6f, 0.4f));

    sprite->changeAnimation(IDLE);
    tileMapDispl = tileMapPos;
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posCanteen.x), float(tileMapDispl.y + posCanteen.y)));
}

void Canteen::update(int deltaTime)
{
    sprite->update(deltaTime);

    if (bActive) {
        if (bJumping) {
            // Actualizar ángulo de salto
            jumpAngle += JUMP_ANGLE_STEP;

            // Calcular nueva posición Y basada en el ángulo de salto
            posCanteen.y = int(jumpStartY - 30 * sin(3.14159f * jumpAngle / 180.f));

            // Mover horizontalmente si tiene velocidad X
            posCanteen.x += jumpVelocityX;

            // Verificar colisiones con el mapa si está cayendo
            if (jumpAngle > 90) {
                bool onGround = map->collisionMoveDown(posCanteen, sizeCanteen, &posCanteen.y);

                // Si toca el suelo o completa el arco, terminar el salto
                if (onGround || jumpAngle >= 180) {
                    bJumping = false;
                }
            }

            // Verificar colisiones laterales
            if (jumpVelocityX > 0) {
                if (map->collisionMoveRight(posCanteen, sizeCanteen)) {
                    jumpVelocityX = -jumpVelocityX;
                }
            }
            else if (jumpVelocityX < 0) {
                if (map->collisionMoveLeft(posCanteen, sizeCanteen)) {
                    jumpVelocityX = -jumpVelocityX;
                }
            }
        }
        else {
            // Si no está saltando, verifica si debe caer
            int prevY = posCanteen.y;
            posCanteen.y += 2;

            if (map->collisionMoveDown(posCanteen, sizeCanteen, &posCanteen.y)) {
                // Está en el suelo
            }
        }

        sprite->setPosition(glm::vec2(float(tileMapDispl.x + posCanteen.x), float(tileMapDispl.y + posCanteen.y)));
    }
}

void Canteen::render()
{
    if (bActive) {
        sprite->render();
    }
}

void Canteen::setTileMap(TileMap* tileMap)
{
    map = tileMap;
}

void Canteen::setPosition(const glm::vec2& pos)
{
    posCanteen = pos;
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posCanteen.x), float(tileMapDispl.y + posCanteen.y)));
}

glm::ivec2 Canteen::getPos()
{
    return posCanteen;
}

glm::ivec2 Canteen::getSize()
{
    return sizeCanteen;
}

bool Canteen::isActive() const
{
    return bActive;
}

void Canteen::setActive(bool active)
{
    bActive = active;
}

void Canteen::activate(const glm::vec2& startPos)
{
    posCanteen = startPos;
    bActive = true;
    bJumping = true;
    jumpAngle = 0;
    jumpStartY = startPos.y;
    jumpStartX = startPos.x;

    jumpVelocityX = (rand() % 3 - 1) * 0.5f;

    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posCanteen.x), float(tileMapDispl.y + posCanteen.y)));
}

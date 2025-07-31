#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "FinalItem.h"

FinalItem::FinalItem()
{
    bActive = false;
    bMoving = false;
    sprite = nullptr;
}

FinalItem::~FinalItem()
{
    if (sprite != nullptr)
        delete sprite;
}

void FinalItem::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
    sizeFinalItem = glm::ivec2(16, 16);

    spritesheet.loadFromFile("sprites/spritesheet_items.png", TEXTURE_PIXEL_FORMAT_RGBA);
    spritesheet.setMinFilter(GL_NEAREST);
    spritesheet.setMagFilter(GL_NEAREST);

    sprite = Sprite::createSprite(sizeFinalItem, glm::vec2(0.2f, 0.2f), &spritesheet, &shaderProgram);
    sprite->setNumberAnimations(1);
    sprite->setAnimationSpeed(0, 8);
    sprite->addKeyframe(0, glm::vec2(0.2f, 0.6f));
    sprite->changeAnimation(0);

    tileMapDispl = tileMapPos;
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posFinalItem.x), float(tileMapDispl.y + posFinalItem.y)));

    bActive = false;
    bMoving = false;
}


void FinalItem::render()
{
    if (bActive)
        sprite->render();
}

void FinalItem::setTileMap(TileMap* tileMap)
{
    map = tileMap;
}

void FinalItem::setPosition(const glm::vec2& pos)
{
    posFinalItem = pos;
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posFinalItem.x), float(tileMapDispl.y + posFinalItem.y)));
}

void FinalItem::setActive(bool active)
{
    bActive = active;
}

bool FinalItem::isActive() const
{
    return bActive;
}

void FinalItem::startMovingToPlayer(const glm::vec2& playerPos)
{
    bMoving = false;
    bActive = true;
    targetPlayerPos = playerPos;


    movementDelay = 2000;
}

void FinalItem::update(int deltaTime)
{
    if (!bActive)
        return;

    sprite->update(deltaTime);

    
    if (!bMoving && movementDelay > 0) {
        movementDelay -= deltaTime;

       
        if (movementDelay <= 0) {
            bMoving = true;
        }
    }

    if (bMoving) {
        
        glm::vec2 direction = targetPlayerPos - glm::vec2(posFinalItem);
        float distance = glm::length(direction);

        
        if (distance < 1.0f) {
            return;
        }

        
        direction = direction / distance;

      
        float speed = 2.0f;
        posFinalItem.x += direction.x * speed;
        posFinalItem.y += direction.y * speed;

        sprite->setPosition(glm::vec2(float(tileMapDispl.x + posFinalItem.x), float(tileMapDispl.y + posFinalItem.y)));
    }
}

void FinalItem::updateTargetPosition(const glm::vec2& newPosition)
{
    targetPlayerPos = newPosition;
}

glm::ivec2 FinalItem::getPos() const
{
    return posFinalItem;
}

glm::ivec2 FinalItem::getSize() const
{
    return sizeFinalItem;
}

Texture* FinalItem::getTexture() const
{
    return const_cast<Texture*>(&spritesheet);
}

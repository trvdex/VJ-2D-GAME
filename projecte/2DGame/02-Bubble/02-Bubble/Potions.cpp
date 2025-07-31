#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Potions.h"
#include "Game.h"

Potions::Potions()
{
    sprite = nullptr;
    potionCount = 2;
}

Potions::~Potions()
{
    if (sprite != nullptr)
        delete sprite;
}

void Potions::init(ShaderProgram& shaderProgram)
{
    spritesheet.loadFromFile("sprites/spritesheet_items.png", TEXTURE_PIXEL_FORMAT_RGBA);
    spritesheet.setMinFilter(GL_NEAREST);
    spritesheet.setMagFilter(GL_NEAREST);

    sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.2f, 0.2f), &spritesheet, &shaderProgram);
    sprite->setNumberAnimations(1);

    sprite->setAnimationSpeed(0, 8);
    sprite->addKeyframe(0, glm::vec2(0.4f, 0.2f));

    sprite->changeAnimation(0);
}

void Potions::update(int deltaTime)
{
    sprite->update(deltaTime);
}

void Potions::render()
{
    
    float x = 60.0f; 

    
    for (int i = 0; i < potionCount; i++) {
        sprite->setPosition(glm::vec2(x + i * 20.0f, 6.0f)); 
        sprite->render();
    }
}

void Potions::usePotion()
{
    if (potionCount > 0) {
        potionCount--;
    }
}

bool Potions::hasPotions() const
{
    return potionCount > 0;
}

void Potions::addPotion()
{
    potionCount++;
}

int Potions::getPotionCount() const
{
    return potionCount;
}

void Potions::resetPotions()
{
    potionCount = 2;
}

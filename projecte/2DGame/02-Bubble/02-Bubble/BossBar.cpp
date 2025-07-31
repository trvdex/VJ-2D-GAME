#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "BossBar.h"
#include "Game.h"

enum HealthBarAnims
{
    HEALTH_FULL, HEALTH_TWO_THIRDS, HEALTH_ONE_THIRD, HEALTH_EMPTY
};

void BossBar::init(ShaderProgram& shaderProgram)
{
    maxHearts = 5;
    hearts = maxHearts;
    heartHealth = new int[maxHearts];
    for (int i = 0; i < maxHearts; i++) {
        heartHealth[i] = 3;
    }

    spritesheet.loadFromFile("sprites/spritesheet_bossbar.png", TEXTURE_PIXEL_FORMAT_RGBA);
    spritesheet.setMinFilter(GL_NEAREST);
    spritesheet.setMagFilter(GL_NEAREST);

    sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.25f, 1), &spritesheet, &shaderProgram);
    sprite->setNumberAnimations(4);

    sprite->setAnimationSpeed(HEALTH_FULL, 2);
    sprite->addKeyframe(HEALTH_FULL, glm::vec2(0.0f, 0.0f));

    sprite->setAnimationSpeed(HEALTH_TWO_THIRDS, 2);
    sprite->addKeyframe(HEALTH_TWO_THIRDS, glm::vec2(0.25f, 0.0f));

    sprite->setAnimationSpeed(HEALTH_ONE_THIRD, 2);
    sprite->addKeyframe(HEALTH_ONE_THIRD, glm::vec2(0.5f, 0.0f));

    sprite->setAnimationSpeed(HEALTH_EMPTY, 2);
    sprite->addKeyframe(HEALTH_EMPTY, glm::vec2(0.75f, 0.0f));

    sprite->changeAnimation(HEALTH_FULL);
    sprite->setPosition(glm::vec2(16.0f, 16.0f));
}

void BossBar::update(int deltaTime)
{

}

void BossBar::render()
{
    for (int i = 0; i < maxHearts; i++) {
        if (heartHealth[i] == 3)
            sprite->changeAnimation(HEALTH_FULL);
        else if (heartHealth[i] == 2)
            sprite->changeAnimation(HEALTH_TWO_THIRDS);
        else if (heartHealth[i] == 1)
            sprite->changeAnimation(HEALTH_ONE_THIRD);
        else
            sprite->changeAnimation(HEALTH_EMPTY);

        sprite->setPosition(glm::vec2(32.0f, 32.0f + i * 16.0f));
        sprite->render();
    }
}

void BossBar::decreaseHealth()
{
    for (int i = maxHearts - 1; i >= 0; i--) {
        if (heartHealth[i] > 0) {
            heartHealth[i]--;
            break;
        }
    }
}

int BossBar::getHealth()
{
    int totalHealth = 0;
    for (int i = 0; i < maxHearts; i++) {
        totalHealth += heartHealth[i];
    }
    return totalHealth;
}

bool BossBar::isDead()
{
    for (int i = 0; i < maxHearts; i++) {
        if (heartHealth[i] > 0)
            return false;
    }
    return true;
}

BossBar::~BossBar()
{
    if (heartHealth != nullptr)
        delete[] heartHealth;
}

#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "HealthBar.h"
#include "Game.h"

enum HealthBarAnims
{
    HEALTH_FULL, HEALTH_TWO_THIRDS, HEALTH_ONE_THIRD, HEALTH_EMPTY
};

void HealthBar::init(ShaderProgram& shaderProgram)
{
    maxHearts = 4;
    hearts = maxHearts;
    heartHealth = new int[maxHearts];
    for (int i = 0; i < maxHearts; i++) {
        heartHealth[i] = 3; 
    }

    spritesheet.loadFromFile("sprites/spritesheet_items.png", TEXTURE_PIXEL_FORMAT_RGBA);
    spritesheet.setMinFilter(GL_NEAREST);
    spritesheet.setMagFilter(GL_NEAREST);

    sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.2f, 0.2f), &spritesheet, &shaderProgram);
    sprite->setNumberAnimations(4);

    sprite->setAnimationSpeed(HEALTH_FULL, 2);
    sprite->addKeyframe(HEALTH_FULL, glm::vec2(0.0f, 0.0f));

    sprite->setAnimationSpeed(HEALTH_TWO_THIRDS, 2);
    sprite->addKeyframe(HEALTH_TWO_THIRDS, glm::vec2(0.2f, 0.0f));

    sprite->setAnimationSpeed(HEALTH_ONE_THIRD, 2);
    sprite->addKeyframe(HEALTH_ONE_THIRD, glm::vec2(0.4f, 0.0f));

    sprite->setAnimationSpeed(HEALTH_EMPTY, 2);
    sprite->addKeyframe(HEALTH_EMPTY, glm::vec2(0.6f, 0.0f));

    sprite->changeAnimation(HEALTH_FULL);
    sprite->setPosition(glm::vec2(16.0f, 16.0f));
}

void HealthBar::update(int deltaTime)
{
    sprite->update(deltaTime);

    
    if (isAnimatingHealth) {
        animationTime += deltaTime;

        
        if (animationTime >= 100 && healthToAnimate > 0) {
            animationTime = 0;
            healthToAnimate--;

            for (int i = 0; i < maxHearts && healthToAnimate >= 0; i++) {
                if (heartHealth[i] < 3) {
                    
                    heartHealth[i]++;
                    break;
                }
            }
        }
        if (healthToAnimate <= 0) {
            isAnimatingHealth = false;
        }
    }
}

void HealthBar::render()
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

        sprite->setPosition(glm::vec2(12.0f, 32.0f + i * 16.0f));
        sprite->render();
    }
}

void HealthBar::setHealth(int h)
{
    if (h > getHealth())
        increaseHealth();
    else if (h < getHealth())
        decreaseHealth();
}

void HealthBar::decreaseHealth()
{
    for (int i = maxHearts - 1; i >= 0; i--) {
        if (heartHealth[i] > 0) {
            heartHealth[i]--;
            break;
        }
    }
}

void HealthBar::increaseHealth()
{
 
    isAnimatingHealth = true;
    healthToAnimate = 3; 
    animationTime = 0;
}

int HealthBar::getHealth()
{
    int totalHealth = 0;
    for (int i = 0; i < maxHearts; i++) {
        totalHealth += heartHealth[i];
    }
    return totalHealth;
}

bool HealthBar::isDead()
{
    for (int i = 0; i < maxHearts; i++) {
        if (heartHealth[i] > 0)
            return false;
    }
    return true;
}

HealthBar::~HealthBar()
{
    if (heartHealth != nullptr)
        delete[] heartHealth;
}

void HealthBar::resetHealth()
{
    
    int totalHealthNeeded = maxHearts * 3; 
    int currentHealth = getHealth();
    int healthToRestore = totalHealthNeeded - currentHealth;

    isAnimatingHealth = true;
    healthToAnimate = healthToRestore;
    animationTime = 0;
}
void HealthBar::addHeart()
{
    if (maxHearts < 6) {
        // Crear un nuevo array con un tamaño mayor
        int* newHeartHealth = new int[maxHearts + 1];

        newHeartHealth[0] = 3;

        // Copiar los valores actuales
        for (int i = 0; i < maxHearts; i++) {
            newHeartHealth[i + 1] = heartHealth[i];
        }

        // Liberar el array antiguo y asignar el nuevo
        delete[] heartHealth;
        heartHealth = newHeartHealth;

        // Incrementar el contador de corazones máximos
        maxHearts++;
    }
}
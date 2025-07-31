#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "WeaponSelector.h"
#include "Game.h"

enum WeaponSelectorAnims
{
    WEAPON_SPEAR, WEAPON_FIRE
};

WeaponSelector::WeaponSelector()
{
    sprite = nullptr;
    currentWeapon = SPEAR;
    isAnimatingSelection = false;
    animationTime = 0;
}

WeaponSelector::~WeaponSelector()
{
    if (sprite != nullptr)
        delete sprite;
}

void WeaponSelector::init(ShaderProgram& shaderProgram)
{
    spritesheet.loadFromFile("sprites/spritesheet_items.png", TEXTURE_PIXEL_FORMAT_RGBA);
    spritesheet.setMinFilter(GL_NEAREST);
    spritesheet.setMagFilter(GL_NEAREST);

    sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.2f, 0.2f), &spritesheet, &shaderProgram);
    sprite->setNumberAnimations(2);

 
    sprite->setAnimationSpeed(WEAPON_SPEAR, 8);
    sprite->addKeyframe(WEAPON_SPEAR, glm::vec2(0.0f, 0.6f));

    
    sprite->setAnimationSpeed(WEAPON_FIRE, 8);
    sprite->addKeyframe(WEAPON_FIRE, glm::vec2(0.4f, 0.6f)); 


    sprite->changeAnimation(WEAPON_SPEAR);
}

void WeaponSelector::update(int deltaTime)
{
    sprite->update(deltaTime);

    if (isAnimatingSelection) {
        animationTime += deltaTime;

        if (animationTime >= 300) {
            isAnimatingSelection = false;
        }
    }

    static bool enterWasPressed = false;
    bool enterIsPressed = Game::instance().getKey(GLFW_KEY_ENTER);

    if (enterIsPressed && !enterWasPressed) {
        nextWeapon();
        std::cout << "Arma cambiada a: " << (currentWeapon == SPEAR ? "Lanza" : "Fuego") << std::endl;
    }

    enterWasPressed = enterIsPressed;
}

void WeaponSelector::render()
{
    float scale = 1.0f;
    if (isAnimatingSelection) {
        scale = 1.0f + 0.3f * sin(animationTime * 0.02f);
    }

    sprite->setPosition(glm::vec2(20.0f, 6.0f));
    sprite->changeAnimation(currentWeapon);
    sprite->render();
}

void WeaponSelector::nextWeapon()
{
    currentWeapon = (currentWeapon == SPEAR) ? FIRE : SPEAR;
    isAnimatingSelection = true;
    animationTime = 0;
}

int WeaponSelector::getCurrentWeapon() const
{
    return currentWeapon;
}

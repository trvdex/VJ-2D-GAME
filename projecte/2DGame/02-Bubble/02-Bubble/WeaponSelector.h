// WeaponSelector.h
#ifndef _WEAPON_SELECTOR_INCLUDE
#define _WEAPON_SELECTOR_INCLUDE

#include "Sprite.h"
#include "Texture.h"

class WeaponSelector {
public:
    enum WeaponType {
        SPEAR, FIRE
    };

    WeaponSelector();
    ~WeaponSelector();

    void init(ShaderProgram& shaderProgram);
    void update(int deltaTime);
    void render();

    void nextWeapon();
    int getCurrentWeapon() const;

private:
    Texture spritesheet;
    Sprite* sprite;

    int currentWeapon;
    bool isAnimatingSelection;
    int animationTime;
};

#endif // _WEAPON_SELECTOR_INCLUDE

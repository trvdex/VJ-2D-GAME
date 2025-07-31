#ifndef _POTIONS_INCLUDE
#define _POTIONS_INCLUDE

#include "Sprite.h"
#include "Texture.h"

class Potions {
public:
    Potions();
    ~Potions();

    void init(ShaderProgram& shaderProgram);
    void update(int deltaTime);
    void render();

    void usePotion();
    bool hasPotions() const;
    void addPotion();
    int getPotionCount() const;
    void resetPotions();

private:
    Texture spritesheet;
    Sprite* sprite;
    int potionCount;
};

#endif // _POTIONS_INCLUDE

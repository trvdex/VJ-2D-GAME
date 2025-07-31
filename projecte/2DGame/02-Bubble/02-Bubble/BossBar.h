#ifndef _BOSS_BAR_INCLUDE
#define _BOSS_BAR_INCLUDE

#include "Sprite.h"
#include "TileMap.h"

class BossBar
{
public:
    BossBar() : heartHealth(nullptr) {}
    ~BossBar();

    void init(ShaderProgram& shaderProgram);
    void update(int deltaTime);
    void render();

    void decreaseHealth();
    int getHealth();
    bool isDead();

private:
    int Health;
    int maxHearts;
    int hearts;
    int* heartHealth;

    Texture spritesheet;
    Sprite* sprite;
};

#endif // _BOSS_BAR_INCLUDE

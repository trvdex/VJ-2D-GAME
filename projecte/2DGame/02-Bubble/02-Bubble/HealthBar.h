#ifndef _HEALTH_BAR_INCLUDE
#define _HEALTH_BAR_INCLUDE

#include "Sprite.h"
#include "TileMap.h"

class HealthBar
{
public:
    HealthBar() : heartHealth(nullptr), isAnimatingHealth(false), healthToAnimate(0), animationTime(0) {}
    ~HealthBar();

    void init(ShaderProgram& shaderProgram);
    void update(int deltaTime);
    void render();

    void setHealth(int h);
    void decreaseHealth();
    void increaseHealth();
    int getHealth();
    bool isDead();
    bool isAnimating() const { return isAnimatingHealth; }
    void resetHealth();
    void addHeart();

private:
    int health;
    int maxHearts;
    int hearts;       
    int* heartHealth; 

    
    bool isAnimatingHealth;
    int healthToAnimate;
    int animationTime;

    Texture spritesheet;
    Sprite* sprite;
};

#endif // _HEALTH_BAR_INCLUDE

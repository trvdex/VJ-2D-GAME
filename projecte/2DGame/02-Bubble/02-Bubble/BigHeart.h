#ifndef _BIG_HEART_INCLUDE
#define _BIG_HEART_INCLUDE

#include "Sprite.h"
#include "TileMap.h"

class BigHeart
{
public:
    BigHeart();
    ~BigHeart();

    void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
    void update(int deltaTime);
    void render();

    void setTileMap(TileMap* tileMap);
    void setPosition(const glm::vec2& pos);
    glm::ivec2 getPos();
    glm::ivec2 getSize();

    bool isActive() const;
    void setActive(bool active);
    void activate(const glm::vec2& startPos);

private:
    glm::ivec2 tileMapDispl, posBigHeart, sizeBigHeart;
    Texture spritesheet;
    Sprite* sprite;
    TileMap* map;
    bool active;
    bool bJumping;
    int jumpAngle;
    float initPosY;
    float jumpVelocityX;
};

#endif // _BIG_HEART_INCLUDE
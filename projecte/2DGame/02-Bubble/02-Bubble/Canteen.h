#ifndef _CANTEEN_INCLUDE
#define _CANTEEN_INCLUDE

#include "Sprite.h"
#include "TileMap.h"

class Canteen
{
public:
    Canteen();
    ~Canteen();

    void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
    void update(int deltaTime);
    void render();

    void setTileMap(TileMap* tileMap);
    void setPosition(const glm::vec2& pos);
    glm::ivec2 getPos();
    glm::ivec2 getSize();

    void activate(const glm::vec2& startPos);
    bool isActive() const;
    void setActive(bool active);

private:
    glm::ivec2 tileMapDispl, posCanteen, sizeCanteen;
    Texture spritesheet;
    Sprite* sprite;
    TileMap* map;
    bool bActive;

    bool bJumping;
    int jumpAngle;
    float jumpStartY;
    float jumpStartX;
    float jumpVelocityX;
};

#endif // _CANTEEN_INCLUDE


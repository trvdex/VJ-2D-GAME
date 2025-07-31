#ifndef _FINAL_ITEM_INCLUDE
#define _FINAL_ITEM_INCLUDE

#include "Sprite.h"
#include "TileMap.h"

class FinalItem
{
public:
    FinalItem();
    ~FinalItem();

    void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
    void update(int deltaTime);
    void render();

    void setTileMap(TileMap* tileMap);
    void setPosition(const glm::vec2& pos);
    void setActive(bool active);
    bool isActive() const;

    void startMovingToPlayer(const glm::vec2& playerPos);
    glm::ivec2 getPos() const;
    glm::ivec2 getSize() const;
    Texture* getTexture() const;

    void updateTargetPosition(const glm::vec2& newPosition);

private:
    bool bActive;
    bool bMoving;
    glm::ivec2 posFinalItem;
    glm::ivec2 sizeFinalItem;
    glm::ivec2 tileMapDispl;
    glm::vec2 velocity;
    glm::vec2 targetPlayerPos;
    int movementDelay; 

    Texture spritesheet;
    Sprite* sprite;
    TileMap* map;
};

#endif // _FINAL_ITEM_INCLUDE
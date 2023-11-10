#ifndef _LETTER_INCLUDE
#define _LETTER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Letter represents a sprite with a single char

class Letter
{

public:
    void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, int letter);
    void update(int deltaTime);
    void render();
    bool isOnPlay();

    void setTileMap(TileMap* tileMap);
    void setPosition(const glm::vec2& pos);
    glm::ivec2 posLetter;
    int currentLetter;

private:
    glm::ivec2 tileMapDispl;
    Texture spritesheet;
    Sprite* sprite;
    TileMap* map;
    bool isPlay = false;     //nomes s'usa si es un pointer
};


#endif // _LETTER_INCLUDE


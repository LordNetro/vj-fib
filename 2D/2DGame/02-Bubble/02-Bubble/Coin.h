#ifndef _COIN_INCLUDE
#define _COIN_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Coin is basically a Sprite that represents the powerup

class Coin
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	glm::ivec2 posCoin;

private:
	glm::ivec2 tileMapDispl;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;

};


#endif // _COIN_INCLUDE
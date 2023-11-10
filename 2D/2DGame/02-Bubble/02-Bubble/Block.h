#ifndef _BLOCK_INCLUDE
#define _BLOCK_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Block is basically a Sprite that represents the powerup

class Block
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, bool isjoint);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	glm::ivec2 posBlock;
	bool isAir;

private:
	glm::ivec2 tileMapDispl;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;

};


#endif // _BLOCK_INCLUDE
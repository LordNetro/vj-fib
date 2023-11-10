#ifndef _POWERUP_INCLUDE
#define _POWERUP_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Powerup is basically a Sprite that represents the powerup

class Powerup
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, bool isjoint);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	glm::ivec2 posPowerup;
	bool isJoint;
	bool isPicked;

private:
	glm::ivec2 tileMapDispl;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;

};


#endif // _POWERUP_INCLUDE
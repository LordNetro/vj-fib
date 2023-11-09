#ifndef _GOOMBA_INCLUDE
#define _GOOMBA_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Goomba is basically a Sprite that represents the enemy. As such it has
// all properties it needs to track its movement, jumping, and collisions.

class Goomba
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	glm::ivec2 posGoomba;
	bool isDying;

private:
	bool bJumping;
	glm::ivec2 tileMapDispl;
	int jumpAngle, startY;
	Texture spritesheet;
	Movements Movement;
	Sprite* sprite;
	TileMap* map;
	float speedX;    
	float accelX;
	float maxSpeedX; 

};


#endif // _GOOMBA_INCLUDE



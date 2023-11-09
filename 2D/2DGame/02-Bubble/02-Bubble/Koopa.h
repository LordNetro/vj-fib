#ifndef _KOOPA_INCLUDE
#define _KOOPA_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Koopa is basically a Sprite that represents the enemy. As such it has
// all properties it needs to track its movement, jumping, and collisions.
class Koopa
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	glm::ivec2 posKoopa;

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


#endif // _KOOPA_INCLUDE
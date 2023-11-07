#ifndef _GOMBA_INCLUDE
#define _GOMBA_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Gomba is basically a Sprite that represents the enemy. As such it has
// all properties it needs to track its movement, jumping, and collisions.
enum Movements
{
	LEFT, RIGHT
};

class Gomba
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	glm::ivec2 posGomba;

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


#endif // _GOMBA_INCLUDE



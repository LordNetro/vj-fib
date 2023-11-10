#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	bool isJumpingOrFalling();
	glm::ivec2 posPlayer;
	bool isDying;
	bool breakedBlock;
	bool powerupBlock;
	int drugType;
	bool changed;
	bool isInvincible;
	bool isInvincibleHigh;
	float invincibleTimer;
	float invincibleDuration; // 2 segundos
	int altura;
	glm::vec2 blockPos;

private:
	bool bJumping;
	glm::ivec2 tileMapDispl;
	int jumpAngle, startY;
	Texture spritesheet;
	ShaderProgram shaderProgram;
	Sprite* sprite;
	Sprite* sprite_small;
	Sprite *sprite_high;
	TileMap *map;
	float speedX;          // Velocidad horizontal actual del jugador
	float accelX;    // Aceleración horizontal
	float maxSpeedX; // Velocidad máxima horizontal
	bool isFalling;

};


#endif // _PLAYER_INCLUDE



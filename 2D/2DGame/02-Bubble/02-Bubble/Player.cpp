#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"

#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 6
#include <windows.h>
#include <string>
#include <sstream>

void pollaGorda(const std::string& str) {
	std::wstring wstr = std::wstring(str.begin(), str.end());
	OutputDebugString(wstr.c_str());
}

enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, STOPPING_LEFT, STOPPING_RIGHT, JUMP_LEFT, JUMP_RIGHT
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	speedX = 0.f;
	accelX = 0.1f;
	maxSpeedX = 3.f;
	bJumping = false;
	spritesheet.loadFromFile("images/test2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.0625, 0.0625), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(8);
	
		sprite->setAnimationSpeed(STAND_LEFT, 8);
		sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.0625f));
		
		sprite->setAnimationSpeed(STAND_RIGHT, 8);
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.f));

		sprite->setAnimationSpeed(STOPPING_LEFT, 8);
		sprite->addKeyframe(STOPPING_LEFT, glm::vec2(0.245f, 0.f));

		sprite->setAnimationSpeed(STOPPING_RIGHT, 8);
		sprite->addKeyframe(STOPPING_RIGHT, glm::vec2(0.245f, 0.0625f));
		
		sprite->setAnimationSpeed(MOVE_LEFT, 8);
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.0625f, 0.0625f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.125f, 0.0625f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.1875f, 0.0625f));
		
		sprite->setAnimationSpeed(MOVE_RIGHT, 8);
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0625f, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.125f, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.1875f, 0.f));

		sprite->setAnimationSpeed(JUMP_LEFT, 8);
		sprite->addKeyframe(JUMP_LEFT, glm::vec2(0.3125f, 0.0625f));

		sprite->setAnimationSpeed(JUMP_RIGHT, 8);
		sprite->addKeyframe(JUMP_RIGHT, glm::vec2(0.3125f, 0.f));
		
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	
}

void Player::update(int deltaTime)
{
	sprite->update(deltaTime);
	float friction = 0.92f;  // Valor de fricción. 0.92

	glm::vec2 initPos = posPlayer;
	glm::vec2 futurePos = initPos;  // Creamos una posición futura para verificar colisiones antes de mover al jugador.
	pollaGorda("posPlayer x: " + std::to_string(posPlayer.x)+"\n");
	if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
	{
		if (speedX == 0) speedX = -1.f;
		speedX -= accelX;
		if (speedX < -maxSpeedX) speedX = -maxSpeedX;
		if (!bJumping) {
			if (speedX < 0 && sprite->animation() != MOVE_LEFT)
				sprite->changeAnimation(MOVE_LEFT);
			else if (speedX > 0) {
				sprite->changeAnimation(STOPPING_RIGHT);
			}
		}

		futurePos.x += speedX;
		pollaGorda("futurePos x: " + std::to_string(futurePos.x) + "\n");

		if (!map->collisionMoveLeft(futurePos, glm::ivec2(16, 16)))
			posPlayer.x = futurePos.x;
		else
		{
			if(!bJumping) sprite->changeAnimation(STAND_LEFT);
			speedX = 0;  // Detén el jugador si hay colisión.
		}
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
	{
		futurePos = initPos;
		if (speedX == 0) speedX = 1.f;
		speedX += accelX;
		if (speedX > maxSpeedX) speedX = maxSpeedX;
		if(!bJumping){
			if (speedX > 0 && sprite->animation() != MOVE_RIGHT)
				sprite->changeAnimation(MOVE_RIGHT);
			else if (speedX < 0) {
				sprite->changeAnimation(STOPPING_LEFT);
			}
		}

		futurePos.x += speedX;

		if (!map->collisionMoveRight(futurePos, glm::ivec2(16, 16)))
			posPlayer.x = futurePos.x;
		else
		{
			if(!bJumping) sprite->changeAnimation(STAND_RIGHT);
			speedX = 0;  // Detén el jugador si hay colisión.
		}
	}
	else
	{
		futurePos = initPos;
		speedX *= friction;
		if (abs(speedX) < 0.5f) speedX = 0.f;

		futurePos.x += speedX;

		if (speedX < 0 && !map->collisionMoveLeft(futurePos, glm::ivec2(16, 16))) {
			if (!bJumping && sprite->animation() != STOPPING_RIGHT) sprite->changeAnimation(STOPPING_RIGHT);
			posPlayer.x = futurePos.x;
		}
		else if (speedX > 0 && !map->collisionMoveRight(futurePos, glm::ivec2(16, 16))) {
			if (!bJumping && sprite->animation() != STOPPING_LEFT) sprite->changeAnimation(STOPPING_LEFT);
			posPlayer.x = futurePos.x;
		}
		else {
			speedX = 0;
			if (!bJumping) {
				if (sprite->animation() == MOVE_LEFT || sprite->animation() == STOPPING_RIGHT || sprite->animation() == JUMP_LEFT) {
					sprite->changeAnimation(STAND_LEFT);
				}
				else if (sprite->animation() == MOVE_RIGHT || sprite->animation() == STOPPING_LEFT || sprite->animation() == JUMP_RIGHT) {
					sprite->changeAnimation(STAND_RIGHT);
				}
			}
		}
	}
	
	if(bJumping)
	{
		futurePos = initPos;
		if (speedX < 0 && sprite->animation() != JUMP_LEFT || sprite->animation() == STAND_LEFT) {
			sprite->changeAnimation(JUMP_LEFT);
		}
		else if (speedX > 0 || sprite->animation() == STAND_RIGHT){
			sprite->changeAnimation(JUMP_RIGHT);
		}
		// Lógica para cuando el jugador está saltando.
		jumpAngle += JUMP_ANGLE_STEP;
		if (jumpAngle == 180)
		{
			bJumping = false;
			posPlayer.y = startY;
		}
		else
		{
			futurePos.y = int(startY - JUMP_HEIGHT * sin(3.14159f * jumpAngle / 180.f));
			if (!map->collisionMoveDown(futurePos, glm::ivec2(16, 16), &posPlayer.y) && !map->collisionMoveUp(futurePos, glm::ivec2(16, 16), &posPlayer.y))
			{
				posPlayer.y = futurePos.y;
			}
			else
			{
				bJumping = false;
			}
		}
	}
	else
	{
		futurePos = initPos;
		// Logica para cuando el jugador está cayendo.
		futurePos.y += FALL_STEP;
		if (!map->collisionMoveDown(futurePos, glm::ivec2(16, 16), &posPlayer.y))
		{
			posPlayer.y = futurePos.y;
		}
		else if (map->collisionMoveDown(futurePos, glm::ivec2(16, 16), &posPlayer.y) && Game::instance().getSpecialKey(GLUT_KEY_UP) && !bJumping)
		{
			bJumping = true;
			jumpAngle = 0;
			startY = posPlayer.y;
		}
	}
	
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::render()
{
	sprite->render();
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}





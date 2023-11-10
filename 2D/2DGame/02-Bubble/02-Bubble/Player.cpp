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

void debugMessage(const std::string& str) {
	std::wstring wstr = std::wstring(str.begin(), str.end());
	OutputDebugString(wstr.c_str());
}

enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, STOPPING_LEFT, STOPPING_RIGHT, JUMP_LEFT, JUMP_RIGHT, DEAD
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	breakedBlock = false;
	powerupBlock = false;
	altura = 16;
	isInvincible = false;
	isInvincibleHigh = false;
	invincibleTimer = 0.0f;
	invincibleDuration = 2.0f; // 2 segundos
	speedX = 0.f;
	accelX = 0.1f;
	maxSpeedX = 2.5f;
	bJumping = false;
	isDying = false;
	shaderProgram = shaderProgram;
	spritesheet.loadFromFile("images/super_barrio_spritesheet_new.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite_small = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.0625, 0.0625), &spritesheet, &shaderProgram);
	sprite_small->setNumberAnimations(9);
	
		sprite_small->setAnimationSpeed(STAND_LEFT, 8);
		sprite_small->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.1875f));
		
		sprite_small->setAnimationSpeed(STAND_RIGHT, 8);
		sprite_small->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.0625f));

		sprite_small->setAnimationSpeed(STOPPING_LEFT, 8);
		sprite_small->addKeyframe(STOPPING_LEFT, glm::vec2(0.25f, 0.0625f));

		sprite_small->setAnimationSpeed(STOPPING_RIGHT, 8);
		sprite_small->addKeyframe(STOPPING_RIGHT, glm::vec2(0.25f, 0.1875f));
		
		sprite_small->setAnimationSpeed(MOVE_LEFT, 8);
		sprite_small->addKeyframe(MOVE_LEFT, glm::vec2(0.0625f, 0.1875f));
		sprite_small->addKeyframe(MOVE_LEFT, glm::vec2(0.125f, 0.1875f));
		sprite_small->addKeyframe(MOVE_LEFT, glm::vec2(0.1875f, 0.1875f));
		
		sprite_small->setAnimationSpeed(MOVE_RIGHT, 8);
		sprite_small->addKeyframe(MOVE_RIGHT, glm::vec2(0.0625f, 0.0625f));
		sprite_small->addKeyframe(MOVE_RIGHT, glm::vec2(0.125f, 0.0625f));
		sprite_small->addKeyframe(MOVE_RIGHT, glm::vec2(0.1875f, 0.0625f));

		sprite_small->setAnimationSpeed(JUMP_LEFT, 8);
		sprite_small->addKeyframe(JUMP_LEFT, glm::vec2(0.3125f, 0.1875f));

		sprite_small->setAnimationSpeed(JUMP_RIGHT, 8);
		sprite_small->addKeyframe(JUMP_RIGHT, glm::vec2(0.3125f, 0.0625f));

		sprite_small->setAnimationSpeed(DEAD, 8);
		sprite_small->addKeyframe(DEAD, glm::vec2(0.375f, 0.0625f));
		

	sprite_high = Sprite::createSprite(glm::ivec2(16, 32), glm::vec2(0.0625, 0.125), &spritesheet, &shaderProgram);
	sprite_high->setNumberAnimations(9);

	sprite_high->setAnimationSpeed(STAND_LEFT, 8);
	sprite_high->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.375f));

	sprite_high->setAnimationSpeed(STAND_RIGHT, 8);
	sprite_high->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.25f));

	sprite_high->setAnimationSpeed(STOPPING_LEFT, 8);
	sprite_high->addKeyframe(STOPPING_LEFT, glm::vec2(0.25f, 0.375f));

	sprite_high->setAnimationSpeed(STOPPING_RIGHT, 8);
	sprite_high->addKeyframe(STOPPING_RIGHT, glm::vec2(0.25f, 0.25f));

	sprite_high->setAnimationSpeed(MOVE_LEFT, 8);
	sprite_high->addKeyframe(MOVE_LEFT, glm::vec2(0.0625f, 0.375f));
	sprite_high->addKeyframe(MOVE_LEFT, glm::vec2(0.125f, 0.375f));
	sprite_high->addKeyframe(MOVE_LEFT, glm::vec2(0.1875f, 0.375f));

	sprite_high->setAnimationSpeed(MOVE_RIGHT, 8);
	sprite_high->addKeyframe(MOVE_RIGHT, glm::vec2(0.0625f, 0.25f));
	sprite_high->addKeyframe(MOVE_RIGHT, glm::vec2(0.125f, 0.25f));
	sprite_high->addKeyframe(MOVE_RIGHT, glm::vec2(0.1875f, 0.25f));

	sprite_high->setAnimationSpeed(JUMP_LEFT, 8);
	sprite_high->addKeyframe(JUMP_LEFT, glm::vec2(0.3125f, 0.375f));

	sprite_high->setAnimationSpeed(JUMP_RIGHT, 8);
	sprite_high->addKeyframe(JUMP_RIGHT, glm::vec2(0.3125f, 0.25f));

	sprite_high->setAnimationSpeed(DEAD, 8);
	sprite_high->addKeyframe(DEAD, glm::vec2(0.1875f, 0.5f));
	sprite_high->addKeyframe(DEAD, glm::vec2(0.25f, 0.5f));
	sprite_high->addKeyframe(DEAD, glm::vec2(0.3125f, 0.5f));
	sprite_high->addKeyframe(DEAD, glm::vec2(0.375f, 0.5f));

	sprite = sprite_small;
	sprite->changeAnimation(0);
	changed = true;
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	
}

bool Player::isJumpingOrFalling() {
	return bJumping || jumpAngle < 180;
}

void Player::update(int deltaTime)
{
	if (isInvincible) {
		invincibleTimer += deltaTime / 1000.0f; // Convertir milisegundos a segundos
		if (invincibleTimer >= invincibleDuration) {
			isInvincible = false;
			isInvincibleHigh = false;
			invincibleTimer = 0.0f;
		}
	}
	if (drugType == 1 && !changed) {
		altura = 32;
		sprite = sprite_high;
		sprite->changeAnimation(0);
		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
		changed = true;
	}
	else if (drugType == 0 && !changed){
		altura = 16;
		sprite = sprite_small;
		sprite->changeAnimation(0);
		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
		changed = true;
	}
	sprite->update(deltaTime);
	breakedBlock = false;
	powerupBlock = false;

	//int mod = Game::instance().getModifiers();
	if (Game::instance().getKey('a')) {
		accelX = 0.1f;
		maxSpeedX = 4.f;
	}
	else {
		accelX = 0.1f;
		maxSpeedX = 2.5f;
	}

	float friction = 0.9f;  // Valor de fricción. 0.92

	glm::vec2 futurePos = posPlayer;

	if (!isDying) {
		//debugMessage("posPlayer x: " + std::to_string(posPlayer.x)+"\n");
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
			//debugMessage("futurePos x: " + std::to_string(futurePos.x) + "\n");

			if (!map->collisionMoveLeft(futurePos, glm::ivec2(16, altura)))
				posPlayer.x = futurePos.x;
			else
			{
				if (!bJumping) sprite->changeAnimation(STAND_LEFT);
				speedX = 0;  // Detén el jugador si hay colisión.
			}
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
		{
			if (speedX == 0) speedX = 1.f;
			speedX += accelX;
			if (speedX > maxSpeedX) speedX = maxSpeedX;
			if (!bJumping) {
				if (speedX > 0 && sprite->animation() != MOVE_RIGHT)
					sprite->changeAnimation(MOVE_RIGHT);
				else if (speedX < 0) {
					sprite->changeAnimation(STOPPING_LEFT);
				}
			}

			futurePos.x += speedX;

			if (!map->collisionMoveRight(futurePos, glm::ivec2(16, altura)))
				posPlayer.x = futurePos.x;
			else
			{
				if (!bJumping) sprite->changeAnimation(STAND_RIGHT);
				speedX = 0;  // Detén el jugador si hay colisión.
			}
		}
		else
		{
			speedX *= friction;
			if (abs(speedX) < 0.2f) speedX = 0.f;

			futurePos.x += speedX;

			if (speedX < 0 && !map->collisionMoveLeft(futurePos, glm::ivec2(16, altura))) {
				if (!bJumping && sprite->animation() != STOPPING_RIGHT) sprite->changeAnimation(STOPPING_RIGHT);
				posPlayer.x = futurePos.x;
			}
			else if (speedX > 0 && !map->collisionMoveRight(futurePos, glm::ivec2(16, altura))) {
				if (!bJumping && sprite->animation() != STOPPING_LEFT) sprite->changeAnimation(STOPPING_LEFT);
				posPlayer.x = futurePos.x;
			}
			else {
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

		if (bJumping)
		{
			if (speedX < 0 && sprite->animation() != JUMP_LEFT || sprite->animation() == STAND_LEFT) {
				sprite->changeAnimation(JUMP_LEFT);
			}
			else if (speedX > 0 || sprite->animation() == STAND_RIGHT) {
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
				posPlayer.y = int(startY - JUMP_HEIGHT * sin(3.14159f * jumpAngle / 180.f));
				if (!map->collisionMoveDown(posPlayer, glm::ivec2(16, altura), &posPlayer.y))
				{
					posPlayer.y = int(startY - JUMP_HEIGHT * sin(3.14159f * jumpAngle / 180.f));
					int blockType = 0;
					if (!map->collisionMoveUp(posPlayer, glm::ivec2(16, altura), &posPlayer.y, blockType, drugType == 1)) {

					}
					else
					{
						debugMessage("blockType = " + std::to_string(blockType) + "\n");
						if (blockType == 3) breakedBlock = true;
						else if (blockType == 13){
							powerupBlock = true;
						}
						bJumping = false;
					}
				}
				else
				{
					bJumping = false;
				}
			}
		}
		else
		{
			// Logica para cuando el jugador está cayendo.
			posPlayer.y += FALL_STEP;
			if (map->collisionMoveDown(posPlayer, glm::ivec2(16, altura), &posPlayer.y))
			{
				if ((Game::instance().getSpecialKey(GLUT_KEY_UP) || Game::instance().getKey(32)) && !bJumping)
				{
					bJumping = true;
					jumpAngle = 0;
					startY = posPlayer.y;
				}
			}

		}
	}
	else {
		if (sprite->animation() != DEAD) {
			sprite->changeAnimation(DEAD);
		}
		// Lógica para cuando el jugador está saltando.
		//jumpAngle += JUMP_ANGLE_STEP;
		//if (jumpAngle == 180)
		//{
		//	bJumping = false;
		//	posPlayer.y = startY;
		//}
		//else
		//{
		//	futurePos.y = int(startY - JUMP_HEIGHT * sin(3.14159f * jumpAngle / 180.f));
		//	if (!map->collisionMoveDown(futurePos, glm::ivec2(16, 16), &posPlayer.y) && !map->collisionMoveUp(futurePos, glm::ivec2(16, 16), &posPlayer.y))
		//	{
		//		posPlayer.y = futurePos.y;
		//	}
		//	else
		//	{
		//		bJumping = false;
		//	}
		//}

		//// Logica para cuando el jugador está cayendo.
		//futurePos.y += FALL_STEP;
		//if (!map->collisionMoveDown(futurePos, glm::ivec2(16, 16), &posPlayer.y))
		//{
		//	posPlayer.y = futurePos.y;
		//}
		//else if (map->collisionMoveDown(futurePos, glm::ivec2(16, 16), &posPlayer.y) && Game::instance().getSpecialKey(GLUT_KEY_UP) && !bJumping)
		//{
		//	bJumping = true;
		//	jumpAngle = 0;
		//	startY = posPlayer.y;
		//}
	}
	//debugMessage("speedX: " + std::to_string(speedX)+"\n");
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





#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"

#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4


enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, STOPPING_LEFT, STOPPING_RIGHT
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	speedX = 0.f;
	accelX = 0.1f;
	maxSpeedX = 3.f;
	bJumping = false;
	spritesheet.loadFromFile("images/super_barrio_spritesheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.0625, 0.0625), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);
	
		sprite->setAnimationSpeed(STAND_LEFT, 8);
		sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.0625f));
		
		sprite->setAnimationSpeed(STAND_RIGHT, 8);
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.f));

		sprite->setAnimationSpeed(STOPPING_LEFT, 8);
		sprite->addKeyframe(STOPPING_LEFT, glm::vec2(0.3125f, 0.f));

		sprite->setAnimationSpeed(STOPPING_RIGHT, 8);
		sprite->addKeyframe(STOPPING_RIGHT, glm::vec2(0.3125f, 0.0625f));
		
		sprite->setAnimationSpeed(MOVE_LEFT, 8);
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.0625f, 0.0625f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.125f, 0.0625f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.1875f, 0.0625f));
		
		sprite->setAnimationSpeed(MOVE_RIGHT, 8);
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0625f, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.125f, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.1875f, 0.f));
		
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	
}

void Player::update(int deltaTime)
{
	sprite->update(deltaTime);
	float friction = 0.92f;  // Valor de fricción.

	glm::vec2 futurePos = posPlayer;  // Creamos una posición futura para verificar colisiones antes de mover al jugador.

	if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
	{
		speedX -= accelX;
		if (speedX < -maxSpeedX) speedX = -maxSpeedX;

		if (speedX < 0 && sprite->animation() != MOVE_LEFT)
			sprite->changeAnimation(MOVE_LEFT);
		else if (speedX > 0 && sprite->animation() != MOVE_LEFT){
			sprite->changeAnimation(STOPPING_RIGHT);
		}

		futurePos.x += speedX;

		if (!map->collisionMoveLeft(glm::ivec2(futurePos.x, futurePos.y), glm::ivec2(16, 16)))
			posPlayer = futurePos;
		else
		{
			sprite->changeAnimation(STAND_LEFT);
			speedX = 0;  // Detén el jugador si hay colisión.
		}
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
	{
		speedX += accelX;
		if (speedX > maxSpeedX) speedX = maxSpeedX;

		if (speedX > 0 && sprite->animation() != MOVE_RIGHT)
			sprite->changeAnimation(MOVE_RIGHT);
		else if (speedX < 0 && sprite->animation() != MOVE_RIGHT) {
			sprite->changeAnimation(STOPPING_LEFT);
		}

		futurePos.x += speedX;

		if (!map->collisionMoveRight(glm::ivec2(futurePos.x, futurePos.y), glm::ivec2(16, 16)))
			posPlayer = futurePos;
		else
		{
			sprite->changeAnimation(STAND_RIGHT);
			speedX = 0;  // Detén el jugador si hay colisión.
		}
	}
	else
	{
		speedX *= friction;
		if (abs(speedX) < 0.5f) speedX = 0.f;

		futurePos.x += speedX;

		if (speedX < 0 && !map->collisionMoveLeft(glm::ivec2(futurePos.x, futurePos.y), glm::ivec2(16, 16)))
			posPlayer = futurePos;
		else if (speedX > 0 && !map->collisionMoveRight(glm::ivec2(futurePos.x, futurePos.y), glm::ivec2(16, 16)))
			posPlayer = futurePos;
		else
			speedX = 0;
			if (sprite->animation() == MOVE_LEFT)
				sprite->changeAnimation(STAND_LEFT);
			else if (sprite->animation() == MOVE_RIGHT)
				sprite->changeAnimation(STAND_RIGHT);
	}
	
	if(bJumping)
	{
		jumpAngle += JUMP_ANGLE_STEP;
		if(jumpAngle == 180)
		{
			bJumping = false;
			posPlayer.y = startY;
		}
		else
		{
			posPlayer.y = int(startY - 96 * sin(3.14159f * jumpAngle / 180.f));
			if(jumpAngle > 90)
				bJumping = !map->collisionMoveDown(posPlayer, glm::ivec2(16, 16), &posPlayer.y);
		}
	}
	else
	{
		posPlayer.y += FALL_STEP;
		if(map->collisionMoveDown(posPlayer, glm::ivec2(16, 16), &posPlayer.y))
		{
			if(Game::instance().getSpecialKey(GLUT_KEY_UP))
			{
				bJumping = true;
				jumpAngle = 0;
				startY = posPlayer.y;
			}
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





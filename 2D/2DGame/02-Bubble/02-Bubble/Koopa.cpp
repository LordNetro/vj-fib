#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Koopa.h"
#include "Game.h"

#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 6


enum KoopaAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, STAND_DEAD, MOVE_DEAD
};

void Koopa::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	speedX = 0.f;
	accelX = 0.1f;
	maxSpeedX = 2.f;
	bJumping = false;
	spritesheet.loadFromFile("images/super_barrio_koopa.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 24), glm::vec2(0.0625f, 0.09375f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);

	sprite->setAnimationSpeed(STAND_LEFT, 8);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(STAND_RIGHT, 8);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.09375f));

	sprite->setAnimationSpeed(MOVE_LEFT, 8);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.0625f, 0.f));

	sprite->setAnimationSpeed(MOVE_RIGHT, 8);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.09375f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0625f, 0.09375f));

	sprite->setAnimationSpeed(STAND_DEAD, 8);
	sprite->addKeyframe(STAND_DEAD, glm::vec2(0.f, 0.46875f));

	sprite->setAnimationSpeed(MOVE_DEAD, 8);
	sprite->addKeyframe(MOVE_DEAD, glm::vec2(0.f, 0.375f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posKoopa.x), float(tileMapDispl.y + posKoopa.y)));

}

void Koopa::update(int deltaTime)
{
	sprite->update(deltaTime);
	float friction = 0.92f;  // Valor de fricción. 0.92

	glm::vec2 futurePos = posKoopa;  // Creamos una posición futura para verificar colisiones antes de mover al jugador.

	// Inicializar el generador de números aleatorios con el tiempo actual
	// para que los resultados sean diferentes en cada ejecución.
	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	if (Movement == NULL) Movement = LEFT;

	if (Movement == LEFT)
	{
		speedX -= accelX;
		if (speedX < -maxSpeedX) speedX = -maxSpeedX;

		if (speedX < 0 && sprite->animation() != MOVE_LEFT)
			sprite->changeAnimation(MOVE_LEFT);

		futurePos.x += speedX;

		if (!map->collisionMoveLeft(futurePos, glm::ivec2(16, 24)))
			posKoopa.x = futurePos.x;
		else
		{
			sprite->changeAnimation(STAND_LEFT);
			speedX = 0;  // Detén el jugador si hay colisión.
			Movement = RIGHT;
		}
	}
	else if (Movement == RIGHT)
	{
		speedX += accelX;
		if (speedX > maxSpeedX) speedX = maxSpeedX;

		if (speedX > 0 && sprite->animation() != MOVE_RIGHT)
			sprite->changeAnimation(MOVE_RIGHT);

		futurePos.x += speedX;

		if (!map->collisionMoveRight(futurePos, glm::ivec2(16, 24)))
			posKoopa.x = futurePos.x;
		else
		{
			sprite->changeAnimation(STAND_RIGHT);
			speedX = 0;  // Detén el koopa si hay colisión.
			Movement = LEFT;
		}
	}
	else
	{
		speedX *= friction;
		if (abs(speedX) < 0.5f) speedX = 0.f;

		futurePos.x += speedX;

		if (speedX < 0 && !map->collisionMoveLeft(futurePos, glm::ivec2(16, 24))) {
			if (sprite->animation() != MOVE_LEFT) sprite->changeAnimation(MOVE_LEFT);
			posKoopa.x = futurePos.x;
		}
		else if (speedX > 0 && !map->collisionMoveRight(futurePos, glm::ivec2(16, 24))) {
			if (sprite->animation() != MOVE_RIGHT) sprite->changeAnimation(MOVE_RIGHT);
			posKoopa.x = futurePos.x;
		}
		else {
			speedX = 0;
			if (sprite->animation() == MOVE_LEFT) {
				sprite->changeAnimation(STAND_LEFT);
			}
			else {
				sprite->changeAnimation(STAND_RIGHT);
			}
		}
	}

	if (bJumping)
	{
		// Lógica para cuando el jugador está saltando.
		jumpAngle += JUMP_ANGLE_STEP;
		if (jumpAngle == 180)
		{
			bJumping = false;
			posKoopa.y = startY;
		}
		else
		{
			futurePos.y = int(startY - JUMP_HEIGHT * sin(3.14159f * jumpAngle / 180.f));
			if (!map->collisionMoveDown(futurePos, glm::ivec2(16, 24), &posKoopa.y) && !map->collisionMoveUp(futurePos, glm::ivec2(16, 24), &posKoopa.y))
			{
				posKoopa.y = futurePos.y;
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
		futurePos.y += FALL_STEP;
		if (!map->collisionMoveDown(futurePos, glm::ivec2(16, 24), &posKoopa.y))
		{
			posKoopa.y = futurePos.y;
		}
		if (map->collisionMoveDown(futurePos, glm::ivec2(16, 24), &posKoopa.y) && !bJumping && false)
		{
			bJumping = true;
			jumpAngle = 0;
			startY = posKoopa.y;
		}
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posKoopa.x), float(tileMapDispl.y + posKoopa.y)));
}

void Koopa::render()
{
	sprite->render();
}

void Koopa::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Koopa::setPosition(const glm::vec2& pos)
{
	posKoopa = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posKoopa.x), float(tileMapDispl.y + posKoopa.y)));
}
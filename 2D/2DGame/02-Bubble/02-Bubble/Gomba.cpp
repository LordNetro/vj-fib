#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Gomba.h"
#include "Game.h"

#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 6


enum GombaAnims
{
	MOVE, DIE, STAND_LEFT, STAND_RIGHT
};

enum Movements
{
	LEFT, RIGHT
};

void Gomba::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	speedX = 0.f;
	accelX = 0.1f;
	maxSpeedX = 2.f;
	bJumping = false;
	spritesheet.loadFromFile("images/test2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.0625, 0.0625), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);

	sprite->setAnimationSpeed(STAND_LEFT, 8);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(0.9375f, 0.f));

	sprite->setAnimationSpeed(STAND_RIGHT, 8);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(1.f, 0.f));

	sprite->setAnimationSpeed(MOVE, 8);
	sprite->addKeyframe(MOVE, glm::vec2(0.8125f, 0.f));
	sprite->addKeyframe(MOVE, glm::vec2(0.875f, 0.f));

	sprite->setAnimationSpeed(DIE, 8);
	sprite->addKeyframe(DIE, glm::vec2(0.9375f, 0.f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posGomba.x), float(tileMapDispl.y + posGomba.y)));

}

void Gomba::update(int deltaTime)
{
	sprite->update(deltaTime);
	float friction = 0.92f;  // Valor de fricción. 0.92

	glm::vec2 futurePos = posGomba;  // Creamos una posición futura para verificar colisiones antes de mover al jugador.

	// Inicializar el generador de números aleatorios con el tiempo actual
    // para que los resultados sean diferentes en cada ejecución.
	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	// Generar un número aleatorio entre 0 y la cantidad de movimientos - 1.
	Movements randomMovement = static_cast<Movements>(std::rand() % 2);

	if (randomMovement == LEFT)
	{
		speedX -= accelX;
		if (speedX < -maxSpeedX) speedX = -maxSpeedX;

		if (speedX < 0 && sprite->animation() != MOVE)
			sprite->changeAnimation(MOVE);
		else if (speedX > 0) {
			sprite->changeAnimation(MOVE);
		}

		futurePos.x += speedX;

		if (!map->collisionMoveLeft(futurePos, glm::ivec2(16, 16)))
			posGomba.x = futurePos.x;
		else
		{
			sprite->changeAnimation(STAND_LEFT);
			speedX = 0;  // Detén el jugador si hay colisión.
		}
	}
	else if (randomMovement == RIGHT)
	{
		speedX += accelX;
		if (speedX > maxSpeedX) speedX = maxSpeedX;

		if (speedX > 0 && sprite->animation() != MOVE)
			sprite->changeAnimation(MOVE);
		else if (speedX < 0) {
			sprite->changeAnimation(MOVE);
		}
		futurePos.x += speedX;

		if (!map->collisionMoveRight(futurePos, glm::ivec2(16, 16)))
			posGomba.x = futurePos.x;
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

		if (speedX < 0 && !map->collisionMoveLeft(futurePos, glm::ivec2(16, 16))) {
			if (sprite->animation() != MOVE) sprite->changeAnimation(MOVE);
			posGomba.x = futurePos.x;
		}
		else if (speedX > 0 && !map->collisionMoveRight(futurePos, glm::ivec2(16, 16))) {
			if (sprite->animation() != MOVE) sprite->changeAnimation(MOVE);
			posGomba.x = futurePos.x;
		}
		else {
			speedX = 0;
			if (sprite->animation() == MOVE) {
				sprite->changeAnimation(STAND_LEFT);
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
			posGomba.y = startY;
		}
		else
		{
			futurePos.y = int(startY - JUMP_HEIGHT * sin(3.14159f * jumpAngle / 180.f));
			if (!map->collisionMoveDown(futurePos, glm::ivec2(16, 16), &posGomba.y) && !map->collisionMoveUp(futurePos, glm::ivec2(16, 16), &posGomba.y))
			{
				posGomba.y = futurePos.y;
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
		if (!map->collisionMoveDown(futurePos, glm::ivec2(16, 16), &posGomba.y))
		{
			posGomba.y = futurePos.y;
		}
		if (map->collisionMoveDown(futurePos, glm::ivec2(16, 16), &posGomba.y) && !bJumping && false)
		{
			bJumping = true;
			jumpAngle = 0;
			startY = posGomba.y;
		}
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posGomba.x), float(tileMapDispl.y + posGomba.y)));
}

void Gomba::render()
{
	sprite->render();
}

void Gomba::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Gomba::setPosition(const glm::vec2& pos)
{
	posGomba = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posGomba.x), float(tileMapDispl.y + posGomba.y)));
}





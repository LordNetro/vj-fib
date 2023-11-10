#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Powerup.h"
#include "Game.h"

enum PowerupAnims
{
	STAND
};

void Powerup::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, bool isjoint)
{
	isJoint = isjoint;
	spritesheet.loadFromFile("images/super_barrio_items.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.0625, 0.0625), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);
	if (isJoint) {
		sprite->setAnimationSpeed(STAND, 8);
		sprite->addKeyframe(STAND, glm::vec2(0.f, 0.0625f));
		sprite->addKeyframe(STAND, glm::vec2(0.0625f, 0.0625f));
		sprite->addKeyframe(STAND, glm::vec2(0.125f, 0.0625f));
	}
	else {
		sprite->setAnimationSpeed(STAND, 8);
		sprite->addKeyframe(STAND, glm::vec2(0.f, 0.f));
	}
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPowerup.x), float(tileMapDispl.y + posPowerup.y)));
}

void Powerup::update(int deltaTime)
{
	sprite->update(deltaTime);
}

void Powerup::render()
{
	sprite->render();
}

void Powerup::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Powerup::setPosition(const glm::vec2& pos)
{
	posPowerup = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPowerup.x), float(tileMapDispl.y + posPowerup.y)));
}
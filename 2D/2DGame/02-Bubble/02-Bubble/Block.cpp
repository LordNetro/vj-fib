#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Block.h"
#include "Game.h"

enum BlockAnims
{
	STAND
};

void Block::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, bool isAir)
{
	spritesheet.loadFromFile("images/tilesheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.125, 0.125), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);
	if (isAir) {
		sprite->setAnimationSpeed(STAND, 8);
		sprite->addKeyframe(STAND, glm::vec2(0.375f, 0.f));
	}
	else {
		sprite->setAnimationSpeed(STAND, 8);
		sprite->addKeyframe(STAND, glm::vec2(0.5f, 0.f));
	}
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBlock.x), float(tileMapDispl.y + posBlock.y)));
}

void Block::update(int deltaTime)
{
	sprite->update(deltaTime);
}

void Block::render()
{
	sprite->render();
}

void Block::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Block::setPosition(const glm::vec2& pos)
{
	posBlock = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBlock.x), float(tileMapDispl.y + posBlock.y)));
}
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Coin.h"
#include "Game.h"

enum CoinAnims
{
	STAND
};

void Coin::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, bool isAir)
{
	spritesheet.loadFromFile("images/super_barrio_items.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.0625, 0.0625), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(STAND, 8);
	sprite->addKeyframe(STAND, glm::vec2(0.f, 0.125f));
	sprite->addKeyframe(STAND, glm::vec2(0.0625f, 0.125f));
	sprite->addKeyframe(STAND, glm::vec2(0.125f, 0.125f));
	sprite->addKeyframe(STAND, glm::vec2(0.1875f, 0.125f));

	sprite->changeAnimation(0);

	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posCoin.x), float(tileMapDispl.y + posCoin.y)));
}

void Coin::update(int deltaTime)
{
	sprite->update(deltaTime);
}

void Coin::render()
{
	sprite->render();
}

void Coin::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Coin::setPosition(const glm::vec2& pos)
{
	posCoin = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posCoin.x), float(tileMapDispl.y + posCoin.y)));
}
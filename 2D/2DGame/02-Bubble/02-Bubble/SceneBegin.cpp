#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "SceneBegin.h"
#include "Game.h"
#include <GL/glut.h>

#include <glm/gtx/string_cast.hpp>
#include <windows.h>
#include <string>
#include <sstream>

#define SCREEN_X 32
#define SCREEN_Y 16

#define INIT_POINTER_X_TILES 15
#define INIT_POINTER_Y_TILES 24


//scene constructor
SceneBegin::SceneBegin()
{
	map = NULL;
	map2 = NULL;
	title = NULL;
	pointer = NULL;
	
}

SceneBegin::~SceneBegin()
{
	if (map != NULL) {
		delete map;
	}
	if (map2 != NULL) {
		delete map2;
	}
	if (title != NULL) {
		delete title;
	}
	if (pointer != NULL) {
		delete pointer;
	}
}


void SceneBegin::init()
{
	isInitialized = true;

	initShaders();
	map = TileMap::createTileMap("interface/templateBegin.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);

	Letter* newLetter = new Letter();
	newLetter->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 42);
	newLetter->setPosition(glm::vec2((INIT_POINTER_X_TILES) * map->getTileSize(), INIT_POINTER_Y_TILES * map->getTileSize()));
	newLetter->setTileMap(map);
	pointer = newLetter;
	
	zoomFactor = 4;
	left = 0;
	right = SCREEN_WIDTH/16 * zoomFactor;
	top = SCREEN_HEIGHT/8 * zoomFactor;
	bottom = 0;
	
	projection = glm::ortho(left+48, right+48, top-100, bottom+16);
	currentTime = 0.0f;
	isInitialized = true;
}

void SceneBegin::update(int deltaTime)
{
	currentTime += deltaTime;
	pointer->update(deltaTime);
	if (pointer->isOnPlay() && Game::instance().getKey('z')) {
		beginGame = true;
	}
}

bool SceneBegin::isBegin() {
	return beginGame;
}

void SceneBegin::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	map->render();
	pointer->render();

}

void SceneBegin::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}




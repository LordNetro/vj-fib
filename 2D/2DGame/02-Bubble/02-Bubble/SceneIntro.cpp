#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "SceneIntro.h"
#include "Game.h"
#include <GL/glut.h>

#include <glm/gtx/string_cast.hpp>
#include <windows.h>
#include <string>
#include <sstream>

#define SCREEN_X 32
#define SCREEN_Y 16

#define INIT_PLAYER_X_TILES 4
#define INIT_PLAYER_Y_TILES 13


//scene constructor
SceneIntro::SceneIntro()
{
	map = NULL;
	deco = NULL;
	player = NULL;
}

SceneIntro::~SceneIntro()
{
	if (map != NULL)
		delete map;
	if (deco != NULL)
		delete deco;
	if (player != NULL)
		delete player;
}

//test



//endtest

void SceneIntro::init()
{
	initShaders();
	map = TileMap::createTileMap("levels/barriolevel01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	deco = TileMap::createTileMap("levels/barriolevel01deco.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	
	left = 0;
	right = SCREEN_WIDTH;
	top = SCREEN_HEIGHT;
	bottom = 0;
	zoomFactor = 6;
	projection = glm::ortho(left, right, top, bottom);
	currentTime = 0.0f;
}

void SceneIntro::update(int deltaTime)
{
	currentTime += deltaTime;


}

void SceneIntro::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	deco->render();
	map->render();
}

void SceneIntro::initShaders()
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




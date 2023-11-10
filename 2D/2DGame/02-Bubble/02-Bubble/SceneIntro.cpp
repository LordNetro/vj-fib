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
	map = TileMap::createTileMap("interface/template.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	
	zoomFactor = 4;
	left = 0;
	right = SCREEN_WIDTH/16 * zoomFactor;
	top = SCREEN_HEIGHT/8 * zoomFactor;
	bottom = 0;
	
	projection = glm::ortho(left+48, right+48, top-100, bottom+16);
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




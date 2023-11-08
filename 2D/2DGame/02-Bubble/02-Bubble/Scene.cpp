#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
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

#define INIT_GOMBA_X_TILES 16
#define INIT_GOMBA_Y_TILES 13

void print(const std::string& str) {
	std::wstring wstr = std::wstring(str.begin(), str.end());
	OutputDebugString(wstr.c_str());
}

Scene::Scene()
{
	map = NULL;
	deco = NULL;
	player = NULL;
	std::vector<Gomba*> gombas;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if (deco != NULL)
		delete deco;
	if(player != NULL)
		delete player;
	for (auto& gomba : gombas) {
		if (gomba != NULL) {
			delete gomba;
		}
	}
}


void Scene::init()
{
	initShaders();
	map = TileMap::createTileMap("levels/barriolevel01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	deco = TileMap::createTileMap("levels/barriolevel01deco.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);

	// Asegúrate de que el vector esté vacío antes de empezar a añadir Gombas
	gombas.clear();

	for (int i = 0; i < 5; ++i) {
		Gomba* newGomba = new Gomba();
		newGomba->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		newGomba->setPosition(glm::vec2((INIT_GOMBA_X_TILES + i * 2) * map->getTileSize(), INIT_GOMBA_Y_TILES * map->getTileSize())); // Puedes cambiar la posición de cada uno si es necesario
		newGomba->setTileMap(map);
		gombas.push_back(newGomba);
	}

	zoomFactor = 6;
	left = float(player->posPlayer.x) - (SCREEN_WIDTH / zoomFactor);
	right = float(player->posPlayer.x) + (SCREEN_WIDTH / zoomFactor);
	top = float(player->posPlayer.y) - (SCREEN_HEIGHT / zoomFactor) + 16;
	bottom = float(player->posPlayer.y) + (SCREEN_HEIGHT / zoomFactor) + 16;
	projection = glm::ortho(left, right, bottom, top);
	currentTime = 0.0f;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	player->update(deltaTime);
	for (auto& gomba : gombas) {
		gomba->update(deltaTime);
	}
	projection = glm::ortho(float(player->posPlayer.x) - (SCREEN_WIDTH / zoomFactor), float(player->posPlayer.x) + (SCREEN_WIDTH / zoomFactor), bottom, top);
}

void Scene::render()
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
	player->render();
	for (auto& gomba : gombas) {
		gomba->render();
	}
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}




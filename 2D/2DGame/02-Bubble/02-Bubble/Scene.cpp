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
#define INIT_PLAYER_Y_TILES 25

#define INIT_GOOMBA_X_TILES 16
#define INIT_GOOMBA_Y_TILES 25

#define INIT_KOOPA_X_TILES 32
#define INIT_KOOPA_Y_TILES 25

void print(const std::string& str) {
	std::wstring wstr = std::wstring(str.begin(), str.end());
	OutputDebugString(wstr.c_str());
}

Scene::Scene()
{
	map = NULL;
	player = NULL;
	std::vector<Goomba*> goombas;
	std::vector<Koopa*> koopas;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
	for (auto& goomba : goombas) {
		if (goomba != NULL) {
			delete goomba;
		}
	}
	for (auto& koopa : koopas) {
		if (koopa != NULL) {
			delete koopa;
		}
	}
}


void Scene::init()
{
	initShaders();
	map = TileMap::createTileMap("levels/level01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);

	// Asegurarse de que el vector esté vacío antes de empezar a añadir Goombas
	goombas.clear();

	for (int i = 0; i < 5; ++i) {
		Goomba* newGoomba = new Goomba();
		newGoomba->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		newGoomba->setPosition(glm::vec2((INIT_GOOMBA_X_TILES + i * 2) * map->getTileSize(), INIT_GOOMBA_Y_TILES * map->getTileSize()));
		newGoomba->setTileMap(map);
		goombas.push_back(newGoomba);
	}

	// Asegurarse de que el vector esté vacío antes de empezar a añadir Koopas
	koopas.clear();

	for (int i = 0; i < 5; ++i) {
		Koopa* newKoopa = new Koopa();
		newKoopa->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		newKoopa->setPosition(glm::vec2((INIT_KOOPA_X_TILES + i * 2) * map->getTileSize(), INIT_KOOPA_Y_TILES * map->getTileSize()));
		newKoopa->setTileMap(map);
		koopas.push_back(newKoopa);
	}

	zoomFactor = 5;
	left = float(player->posPlayer.x) - (SCREEN_WIDTH / zoomFactor) + 50;
	right = float(player->posPlayer.x) + (SCREEN_WIDTH / zoomFactor) + 50;
	top = float(player->posPlayer.y) - (SCREEN_HEIGHT / zoomFactor) - 50;
	bottom = float(player->posPlayer.y) + (SCREEN_HEIGHT / zoomFactor) - 50;
	projection = glm::ortho(left, right, bottom, top);
	currentTime = 0.0f;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	player->update(deltaTime);

	//TODO Utilizamos un índice inverso para iterar y eliminar goombas sin problemas
	for (int i = goombas.size() - 1; i >= 0; --i) {
		bool update = true;
		// Asumiendo que posPlayer y posGoomba son glm::ivec2 y representan la esquina inferior izquierda del sprite
		if (goombas[i]->isDying) {
			update = false;
			delete goombas[i];
			goombas.erase(goombas.begin() + i); // Elimina el elemento del vector
		}
		else if (player->isJumpingOrFalling() &&
			player->posPlayer.x < goombas[i]->posGoomba.x + 16 && // El jugador está a la izquierda del borde derecho del goomba 16
			player->posPlayer.x + 16 > goombas[i]->posGoomba.x && // El jugador está a la derecha del borde izquierdo del goomba 16
			player->posPlayer.y <= goombas[i]->posGoomba.y - 14 &&
			player->posPlayer.y >= goombas[i]->posGoomba.y - 16) { // El jugador está justo encima del goomba
			print("DYING DYING DYING DYING");
			goombas[i]->isDying = true;
		}
		else if (
			player->posPlayer.x + 14 <= goombas[i]->posGoomba.x && // El jugador está a la izquierda del borde derecho del goomba 16
			player->posPlayer.x >= goombas[i]->posGoomba.x + 16 &&
			player->posPlayer.y > goombas[i]->posGoomba.y - 13 &&
			player->posPlayer.y <= goombas[i]->posGoomba.y) {
			print("HIT HIT HIT HIT");
		}
		if(update) goombas[i]->update(deltaTime);
	}

	// Actualizar koopas
	for (auto& koopa : koopas) {
		koopa->update(deltaTime);
	}

	player->update(deltaTime);

	// Actualización de la proyección
	projection = glm::ortho(float(player->posPlayer.x) - (SCREEN_WIDTH / zoomFactor),
		float(player->posPlayer.x) + (SCREEN_WIDTH / zoomFactor),
		bottom, top);
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

	map->render();
	player->render();
	for (auto& goomba : goombas) {
		goomba->render();
	}
	for (auto& koopa : koopas) {
		koopa->render();
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




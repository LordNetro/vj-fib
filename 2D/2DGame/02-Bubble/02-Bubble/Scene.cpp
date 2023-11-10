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

#define INIT_GOOMBA_X_TILES 16
#define INIT_GOOMBA_Y_TILES 13

#define INIT_KOOPA_X_TILES 32
#define INIT_KOOPA_Y_TILES 13

#define INIT_LETTER_X_TILES 5
#define INIT_LETTER_Y_TILES 10


void print(const std::string& str) {
	std::wstring wstr = std::wstring(str.begin(), str.end());
	OutputDebugString(wstr.c_str());
}

//scene constructor
Scene::Scene()
{
	map = NULL;
	deco = NULL;
	player = NULL;
	std::vector<Goomba*> goombas;
	std::vector<Koopa*> koopas;
	std::vector<Letter*> letters;
	std::vector<Letter*> blocks;
}

Scene::~Scene()
{
	if (map != NULL)
		delete map;
	if (deco != NULL)
		delete deco;
	if (player != NULL)
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
	for (auto& letter : letters) {
		if (letter != NULL) {
			delete letter;
		}
	}
	for (auto& powerup : powerups) {
		if (powerup != NULL) {
			delete powerup;
		}
	}
	for (auto& block : blocks) {
		if (block != NULL) {
			delete block;
		}
	}
}


void Scene::init()
{
	initShaders();
	highMode = false;
	map = TileMap::createTileMap("levels/barriolevel01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	deco = TileMap::createTileMap("levels/barriolevel01deco.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);

	// Asegurarse de que el vector estï¿½ vacï¿½o antes de empezar a aï¿½adir Goombas
	goombas.clear();

	for (int i = 0; i < 1; ++i) {
		Goomba* newGoomba = new Goomba();
		newGoomba->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		newGoomba->setPosition(glm::vec2((INIT_GOOMBA_X_TILES + i * 2) * map->getTileSize(), INIT_GOOMBA_Y_TILES * map->getTileSize()));
		newGoomba->setTileMap(map);
		goombas.push_back(newGoomba);
	}

	// Asegurarse de que el vector estï¿½ vacï¿½o antes de empezar a aï¿½adir Koopas
	koopas.clear();

	for (int i = 0; i < 1; ++i) {
		Koopa* newKoopa = new Koopa();
		newKoopa->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		newKoopa->setPosition(glm::vec2((INIT_KOOPA_X_TILES + i * 2) * map->getTileSize(), INIT_KOOPA_Y_TILES * map->getTileSize()));
		newKoopa->setTileMap(map);
		koopas.push_back(newKoopa);
	}

	// Asegurarse de que el vector estï¿½ vacï¿½o antes de empezar a aï¿½adir letters
	letters.clear();

	//afegir com a caracters
	int size = 6;
	int word[6] = {25, 10, cl('T'), 10, cl('Y'), cl('Z')};

	for (int i = 0; i < size; ++i) {
		Letter* newLetter = new Letter();
		newLetter->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, word[i]);
		newLetter->setPosition(glm::vec2((INIT_LETTER_X_TILES + i) * map->getTileSize(), INIT_LETTER_Y_TILES * map->getTileSize()));
		newLetter->setTileMap(map);
		letters.push_back(newLetter);
	}
	// Asegurarse de que el vector estï¿½ vacï¿½o antes de empezar a aï¿½adir Koopas
	powerups.clear();

	for (int i = 0; i < 4; ++i) {
		Powerup* newPowerup = new Powerup();
		newPowerup->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, i%2 == 0);
		newPowerup->setPosition(glm::vec2((INIT_KOOPA_X_TILES + i * 2) * map->getTileSize(), INIT_KOOPA_Y_TILES * map->getTileSize()));
		newPowerup->setTileMap(map);
		powerups.push_back(newPowerup);
	}

	blocks.clear();

	for (int i = 0; i < 0; ++i) {
		Block* newBlock = new Block();
		newBlock->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, i % 2 == 0);
		newBlock->setPosition(glm::vec2((INIT_KOOPA_X_TILES + i * 2) * map->getTileSize(), INIT_KOOPA_Y_TILES * map->getTileSize()));
		newBlock->setTileMap(map);
		blocks.push_back(newBlock);
	}

	zoomFactor = 6;
	left = float(player->posPlayer.x) - (SCREEN_WIDTH / zoomFactor);
	right = float(player->posPlayer.x) + (SCREEN_WIDTH / zoomFactor);
	top = float(player->posPlayer.y) - (SCREEN_HEIGHT / zoomFactor) + 16;
	bottom = float(player->posPlayer.y) + (SCREEN_HEIGHT / zoomFactor) + 16;
	projection = glm::ortho(left, right, bottom, top);
	currentTime = 0.0f;
}

//char majus to lletra
int Scene::cl(char c) {
	return c - 'A' + 10;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	player->update(deltaTime);
	if (player->powerupBlock) {
		std::srand(static_cast<unsigned int>(std::time(nullptr)));

		// Generar bool aleatoria para generar el powerup (lucky try)
		Powerup* newPowerup = new Powerup();
		newPowerup->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, std::rand() % 2 == 0);
		newPowerup->setPosition(glm::vec2(player->posPlayer.x, player->posPlayer.y - map->getTileSize()*2));
		newPowerup->setTileMap(map);
		powerups.push_back(newPowerup);

		Block* newBlock = new Block();
		newBlock->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, false);
		//newBlock->setPosition(glm::vec2(player->posPlayer.x/map->getTileSize(), player->posPlayer.y / map->getTileSize()));
		float bPosX = player->posPlayer.x - 8 - ((player->posPlayer.x  - 8) % map->getTileSize());
		//newBlock->setPosition(glm::vec2(bPosX + map->getTileSize(), player->posPlayer.y - map->getTileSize() - 1)); 
		newBlock->setPosition(glm::vec2(player->blockPos.x, player->blockPos.y - map->getTileSize()));
		newBlock->setTileMap(map);
		blocks.push_back(newBlock);
		//print("????? BLOCK AND ADDED ?? HITTED ON pos: " + std::to_string(player->posPlayer.x - (player->posPlayer.x % map->getTileSize())) + " " + std::to_string(player->posPlayer.y - map->getTileSize() - 1) + "\n");
		//print("????? BLOCK AND ADDED ?? HITTED ON pos: "+std::to_string(player->blockPos.x)+" "+std::to_string(player->blockPos.y) + "\n");

	}
	if (player->breakedBlock) {
		print("BREAKED BLOCK AND ADDED AIR");
		Block* newBlock = new Block();
		newBlock->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, true);
		newBlock->setPosition(glm::vec2(player->blockPos.x, player->blockPos.y - map->getTileSize()));
		newBlock->setTileMap(map);
		blocks.push_back(newBlock);
	}
	//print("SPAWN: " + std::to_string(INIT_PLAYER_Y_TILES * map->getTileSize()));
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
			player->posPlayer.y >= goombas[i]->posGoomba.y - 17) { // El jugador está justo encima del goomba

			print("DYING DYING DYING DYING");
			goombas[i]->isDying = true;
		}
		else if (
			player->posPlayer.x + 16 >= goombas[i]->posGoomba.x && // El jugador está a la izquierda del borde derecho del goomba 16
			player->posPlayer.x <= goombas[i]->posGoomba.x + 16 &&
			player->posPlayer.y > goombas[i]->posGoomba.y - player->altura-2 &&
			player->posPlayer.y <= goombas[i]->posGoomba.y) {
			print("HIT HIT HIT HIT\n");
			if (player->isInvincibleHigh) {
				goombas[i]->isDying = true;
			}
			else if (player->drugType == 1) {
				player->isInvincible = true;
				player->invincibleTimer = 0.f;
				player->invincibleDuration = 2.f;
				player->drugType = 0;
				player->changed = false;
			}
			else if (!player->isInvincible){
				player->isDying = true;
			}
		}
		if (update) goombas[i]->update(deltaTime);
	}

	//TODO Utilizamos un índice inverso para iterar y eliminar koopas sin problemas
	for (int i = koopas.size() - 1; i >= 0; --i) {
		bool update = true;
		// Asumiendo que posPlayer y posKoopa son glm::ivec2 y representan la esquina inferior izquierda del sprite
		//TODO if (koopas[i]->isDying) {
			//update = false;
			//delete koopas[i];
			//koopas.erase(koopas.begin() + i); // Elimina el elemento del vector
		//}
		//print("posPlayer.y: " + std::to_string(player->posPlayer.y) + "\n");
		//print("posKoopa.y: " + std::to_string(koopas[i]->posKoopa.y) + "\n");
		/*else*/if (player->isJumpingOrFalling() &&
			player->posPlayer.x < koopas[i]->posKoopa.x + 16 && // El jugador está a la izquierda del borde derecho del goomba 16
			player->posPlayer.x + 16 > koopas[i]->posKoopa.x && // El jugador está a la derecha del borde izquierdo del goomba 16
			player->posPlayer.y <= koopas[i]->posKoopa.y - 22 &&
			player->posPlayer.y >= koopas[i]->posKoopa.y - 25 &&
			!koopas[i]->isDying && !koopas[i]->isPushed) { // El jugador está justo encima del goomba
			print("DYING DYING DYING DYING");
			koopas[i]->isDying = true;
		}
		else if (
			player->posPlayer.x + 16 >= koopas[i]->posKoopa.x && // El jugador está a la izquierda del borde derecho del goomba 16
			player->posPlayer.x <= koopas[i]->posKoopa.x + 16 &&
			player->posPlayer.y > koopas[i]->posKoopa.y - 22 + 8 &&
			player->posPlayer.y <= koopas[i]->posKoopa.y + 8) {
			if (!koopas[i]->isDying) {
				print("HIT HIT HIT HIT\n");
				if (player->isInvincibleHigh) {
					koopas[i]->isDying = true;
				}
				else if (player->drugType == 1) {
					player->isInvincible = true;
					player->invincibleTimer = 0.f;
					player->invincibleDuration = 2.f;
					player->drugType = 0;
					player->changed = false;
				}
				else if (!player->isInvincible){
					player->isDying = true;
				}

			}
			else if (!koopas[i]->isPushed) {
				print("PUSHED PUSHED PUSHED PUSHED\n");
				koopas[i]->isDying = false;
				koopas[i]->isPushed = true;
				koopas[i]->accelX = 1.f;
				if (player->posPlayer.x <= koopas[i]->posKoopa.x + 8) {
					koopas[i]->Movement = RIGHT;
					koopas[i]->posKoopa = glm::ivec2(player->posPlayer.x + 30, player->posPlayer.y);
				}
				else {
					koopas[i]->Movement = LEFT;
					koopas[i]->posKoopa = glm::ivec2(player->posPlayer.x - 30, player->posPlayer.y);
				}
			}
		}
		if (update) koopas[i]->update(deltaTime);
	}

	int aux = letters.size();
	for (int i = 0; i < aux; ++i) {
		
		letters[i]->setPosition(glm::vec2((INIT_LETTER_X_TILES + i) * map->getTileSize(), INIT_LETTER_Y_TILES * map->getTileSize()));
		letters[i]->update(deltaTime);
	}


	//TODO Utilizamos un índice inverso para iterar y eliminar koopas sin problemas
	for (int i = powerups.size() - 1; i >= 0; --i) {
		bool update = true;
		if (player->posPlayer.x < powerups[i]->posPowerup.x + 16 && // El jugador está a la izquierda del borde derecho del powerup 16
			player->posPlayer.x + 16 > powerups[i]->posPowerup.x && // El jugador está a la derecha del borde izquierdo del powerup 16
			player->posPlayer.y <= powerups[i]->posPowerup.y &&
			player->posPlayer.y >= powerups[i]->posPowerup.y - 16) { // El jugador está justo encima del goomba
			print("PICKED UP!!!");
			if (powerups[i]->isJoint) {
				player->isInvincible = true;
				player->isInvincibleHigh = true;
				player->invincibleTimer = 0.f;
				player->invincibleDuration = 6.f;
			}
			else {
				player->drugType = 1;
				player->changed = false;
			}
			
			update = false;
			delete powerups[i];
			powerups.erase(powerups.begin() + i); // Elimina el elemento del vector

		}
		if (update) powerups[i]->update(deltaTime);
	}

	for (int i = 0; i < blocks.size(); ++i) {
		blocks[i]->update(deltaTime);
	}

	if (player->isInvincibleHigh) highMode = true;
	else highMode = false;
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

	deco->render();
	map->render();
	for (auto& block : blocks) {
		block->render();
	}
	player->render();
	for (auto& goomba : goombas) {
		goomba->render();
	}
	for (auto& koopa : koopas) {
		koopa->render();
	}
	for (auto& letter : letters) {
		letter->render();
	}
	for (auto& powerup : powerups) {
		powerup->render();
	}
}



void Scene::initShaders()
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


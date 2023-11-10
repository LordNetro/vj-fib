#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Goomba.h"
#include "Koopa.h"
#include "Letter.h"
#include "Block.h"
#include "Coin.h"
#include "Powerup.h"
#include <vector>


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init();
	void update(int deltaTime);
	void render();
	bool highMode;
private:
	void initShaders();
	void spawnCoins();
	void spawnEnemies();
	int Scene::cl(char c);

private:
	TileMap *map;
	TileMap *deco;
	Player* player;
	ShaderProgram texProgram;
	std::vector<Goomba*> goombas;  // Vector para almacenar punteros a Goomba
	std::vector<Koopa*> koopas;  // Vector para almacenar punteros a Goomba
	std::vector<Letter*> letters;
	std::vector<Powerup*> powerups;  // Vector para almacenar punteros a Powerup
	std::vector<Block*> blocks;  // Vector para almacenar punteros a Powerup
	std::vector<Coin*> coins;  // Vector para almacenar punteros a Powerup
	float currentTime;
	glm::mat4 projection;
	float zoomFactor;
	float left;
	float right;
	float top;
	float bottom;
};


#endif // _SCENE_INCLUDE


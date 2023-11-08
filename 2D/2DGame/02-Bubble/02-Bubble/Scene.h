#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Gomba.h"
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

private:
	void initShaders();

private:
	TileMap *map;
	TileMap* deco;
	Player* player;
	Gomba* gomba;
	ShaderProgram texProgram;
	std::vector<Gomba*> gombas;  // Vector para almacenar punteros a Gomba
	float currentTime;
	glm::mat4 projection;
	float zoomFactor;
	float left;
	float right;
	float top;
	float bottom;
};


#endif // _SCENE_INCLUDE


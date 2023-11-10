#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Goomba.h"
#include "Koopa.h"
#include "Letter.h"
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
	int Scene::cl(char c);

private:
	TileMap *map;
	TileMap *deco;
	Player* player;
	Goomba* goomba;
	Koopa* koopa;
	ShaderProgram texProgram;
	std::vector<Goomba*> goombas;  // Vector para almacenar punteros a Goomba
	std::vector<Koopa*> koopas;  // Vector para almacenar punteros a Goomba
	std::vector<Letter*> letters;
	float currentTime;
	glm::mat4 projection;
	float zoomFactor;
	float left;
	float right;
	float top;
	float bottom;
};


#endif // _SCENE_INCLUDE


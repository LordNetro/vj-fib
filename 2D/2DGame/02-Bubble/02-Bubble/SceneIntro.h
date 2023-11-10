#ifndef _SCENEINTRO_INCLUDE
#define _SCENEINTRO_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include <vector>


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class SceneIntro
{

public:
	SceneIntro();
	~SceneIntro();

	void init();
	void update(int deltaTime);
	void render();

private:
	void initShaders();

private:
	TileMap *map;
	TileMap *deco;
	Player* player;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	float zoomFactor;
	float left;
	float right;
	float top;
	float bottom;
};


#endif // _SCENEINTRO_INCLUDE


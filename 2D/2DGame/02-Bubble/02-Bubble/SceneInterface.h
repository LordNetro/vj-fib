#ifndef _SCENEINTERFACE_INCLUDE
#define _SCENEINTERFACE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include <vector>


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class SceneInterface
{

public:
	SceneInterface();
	~SceneInterface();

	void init();
	void update(int deltaTime);
	void render();

	bool isInitialized = false;

private:
	void initShaders();

private:
	TileMap *map;
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


#ifndef _SCENEBEGIN_INCLUDE
#define _SCENEBEGIN_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Letter.h"
#include <vector>


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class SceneBegin
{

public:
	SceneBegin();
	~SceneBegin();

	void init();
	void update(int deltaTime);
	void render();
	bool isInitialized = false;
	bool SceneBegin::isBegin();

private:
	void initShaders();

private:
	TileMap *map;
	TileMap* map2;
	TileMap* title;
	Letter* pointer;
	bool beginGame = false;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	float zoomFactor;
	float left;
	float right;
	float top;
	float bottom;


};


#endif // _SCENEBEGIN_INCLUDE


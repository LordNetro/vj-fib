#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"
#include <cstdlib>
#include <ctime>


void Game::init()
{
	bPlay = true;
	glClearColor(148 / 255.0f, 148 / 255.0f, 1.0f, 1.0f);
	switch (pantallaActual) {
		case 0:
			sceneBegin.init();
			interface.init();
			break;
		case 1:
			scene.init();
			interface.init();
			break;
		case 2:
			scene.init();
			interface.init();
			break;
	}
}

void Game::resetFirstLevel()
{
	scene.init();
	interface.init();
}

bool Game::update(int deltaTime)
{	
	switch (pantallaActual) {
	case 0:
		if (sceneBegin.isInitialized) sceneBegin.update(deltaTime);
		else sceneBegin.init();

		if (interface.isInitialized) interface.update(deltaTime);
		else interface.init();
		break;
	case 1:
		if (scene.isInitialized) scene.update(deltaTime);
		else scene.init();
		if (interface.isInitialized) interface.update(deltaTime);
		else interface.init();
		break;
	case 2:
		if (scene.isInitialized) scene.update(deltaTime);
		else scene.init();
		if (interface.isInitialized) interface.update(deltaTime);
		else interface.init();
		break;
	}

	if (sceneBegin.isBegin()) {
		pantallaActual = 1;
	}
	
	if (scene.highMode) {
		// Generar color aleatorio para el cielo
		std::srand(static_cast<unsigned int>(std::time(nullptr)));
		glClearColor(std::rand() % 255 / 255.0f, std::rand() % 255 / 255.0f, std::rand() % 255 / 255.0f, 1.0f);
	}
	else {
		glClearColor(148 / 255.0f, 148 / 255.0f, 1.0f, 1.0f);
	}
	return bPlay;

}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	switch (pantallaActual) {
		case 0:
			sceneBegin.render();
			interface.render();
			break;
		case 1:
			scene.render();
			interface.render();
			break;
		case 2:
			scene2.render();
			interface.render();
			break;
	}
}

void Game::keyPressed(int key)
{
	//updateModifiers();
	switch (key) {
		case 27:
			bPlay = false;
			break;
		case '0':
			pantallaActual = 0;
			init();
			render();
			break;
		case '1':
			pantallaActual = 1;
			init();
			render();
			break;
		case '2':
			pantallaActual = 2;
			init();
			render();
			break;
	}

	keys[key] = true;
	

}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::specialKeyPressed(int key)
{

	specialKeys[key] = true;
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}

bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}

//void Game::updateModifiers() {
//	modifiers = glutGetModifiers();
//}




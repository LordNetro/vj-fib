#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"
#include <cstdlib>
#include <ctime>


void Game::init()
{
	bPlay = true;
	glClearColor(148/255.0f, 148/255.0f, 1.0f, 1.0f);
	scene.init();
	scene2.init();
}

bool Game::update(int deltaTime)
{
	scene.update(deltaTime);
	scene2.update(deltaTime);
	
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
	scene.render();
	scene2.render();
}

void Game::keyPressed(int key)
{
	//updateModifiers();
	if(key == 27) {// Escape code
		bPlay = false;
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




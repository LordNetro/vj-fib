#include "Letter.h"

#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"

/*
, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE,
    A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z,
    DASH,CROSS,EXCLAMATION,DOT,COPYRIGHT,COIN,POINTER
    */
enum LetterAnims
{
    ZERO, ONE
};

//step between letters: 0.0625 horizontally, 0.25 vertically
//assume  0 <= currentLetter < 43
void Letter::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, int letter)
{
    currentLetter = letter;
    spritesheet.loadFromFile("images/letters.png", TEXTURE_PIXEL_FORMAT_RGBA); 
    sprite = Sprite::createSprite(glm::ivec2(8,8), glm::vec2(0.0625f, 0.25f), &spritesheet, &shaderProgram);
    
    /*  enum has 43 elements
    *   per cada element possible assignem l'animacio corresponent.
    *   la spritesheet te 4 files de 16 caracters amb mida 8x8. per tant, cada 16 (aka. 2^4) unitats cal passar a la fila següent
    *   factor de correccio aplicat als keyframes
    */
    LetterAnims la;
    
    sprite->setNumberAnimations(43);
    
    for (int i = 0; i < 43; ++i) {
        la = static_cast<LetterAnims>(i);
        sprite->setAnimationSpeed(i, 8);
        sprite->addKeyframe(i, glm::vec2(0.0625f*float(i%16), float((i>>4)/4.0) + (i >> 4)*0.075 + 0.03));
    }
    
    /*
    sprite->setNumberAnimations(2);

    sprite->setAnimationSpeed(ZERO, 8);
    sprite->addKeyframe(ZERO, glm::vec2(0.f, 0.f));

    sprite->setAnimationSpeed(ONE, 8);
    sprite->addKeyframe(ONE, glm::vec2(0.0625f, 0.f));
    */

    sprite->changeAnimation(letter);
    tileMapDispl = tileMapPos;
    sprite->setPosition(glm::vec2( float(tileMapDispl.x + posLetter.x), float(tileMapDispl.y + posLetter.y)));
}

void Letter::update(int deltaTime)
{
    sprite->update(deltaTime);
    sprite->changeAnimation(currentLetter);
}

void Letter::render()
{
    sprite->render();
}

void Letter::setTileMap(TileMap* tileMap)
{
    map = tileMap;
}

void Letter::setPosition(const glm::vec2& pos)
{
    posLetter = pos;
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posLetter.x), float(tileMapDispl.y + posLetter.y)));
}
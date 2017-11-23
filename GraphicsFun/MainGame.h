#pragma once

#include <SDL/SDL.h>
#include <GL/glew.h>

#include "Sprite.h"
#include "GLSLProgram.h"
#include "GLTexture.h"


enum class GameState {
	PLAY,
	EXIT
};


class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();


private:
	// Functions
	void initSystem();
	void gameLoop();
	void processInput();
	void drawGame();
	void initShaders();


	SDL_Window* _window;
	GameState _gameState;

	Sprite _sprite;
	GLSLProgram _colorProgram;
	GLTexture _playerTexture;


	int _screenWidth;
	int _screenHeight;
	float _time;
};


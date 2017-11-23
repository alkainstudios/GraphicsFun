#include "MainGame.h"

#include <iostream>
#include <string>
#include "Errors.h"
#include "ImageLoader.h"

// ------------------------------------------------------------------------------------------
MainGame::MainGame() : 
	_window(nullptr), 
	_gameState(GameState::PLAY), 
	_screenWidth(1024), 
	_screenHeight(768), 
	_time(0)
{

}

// ------------------------------------------------------------------------------------------
MainGame::~MainGame()
{
}

// ------------------------------------------------------------------------------------------
void MainGame::run() {
	initSystem();
	_sprite.initSprite(-1.0f, -1.0f, 2.0f, 2.0f);
	_playerTexture = ImageLoader::loadPNG("Textures/JimmyJumpPack/PNG/Player/CharacterRight_Standing.png");
	gameLoop();
}
// ------------------------------------------------------------------------------------------
void MainGame::initSystem() {
	// Initialize SDL
	SDL_Init(SDL_INIT_EVERYTHING);

	_window = SDL_CreateWindow("Graphics Fun", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _screenWidth, _screenHeight, SDL_WINDOW_OPENGL);

#pragma region Inits
	if (_window == nullptr) {
		fatalError("SDL Window could not be created!");
	}

	SDL_GLContext glContext = SDL_GL_CreateContext(_window);
	if (glContext == nullptr) {
		fatalError("SDL_GL context could ont be created!");
	}

	GLenum error = glewInit();
	if (error != GLEW_OK) {
		fatalError("Could not initialize glew!");
	}

#pragma endregion
	
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	glClearColor(0.2f, 0.2f, 0.7f, 1.0f);

	initShaders();
}

// ------------------------------------------------------------------------------------------
void MainGame::initShaders() {
	// Compile Shaders.
	_colorProgram.compileShaders("Shaders/colorShading.vert", "Shaders/colorShading.frag");
	// Add attributes to the list.
	_colorProgram.addAttribute("vertexPosition");
	_colorProgram.addAttribute("vertexColor");
	_colorProgram.addAttribute("vertexUV");
	// Link shaders to things.
	_colorProgram.linkShaders();
}

// ------------------------------------------------------------------------------------------
// Main Game Loop
void MainGame::gameLoop() {
	while (_gameState != GameState::EXIT) {
		// Processes the input from the user
		processInput();

		_time += 0.01f;

		// Draws graphics to the screen
		drawGame();
	}
}

// ------------------------------------------------------------------------------------------
// Processes the input from the user
void MainGame::processInput() {
	// Check event
	SDL_Event evnt;

	// Process event
	while (SDL_PollEvent(&evnt)) {
		switch (evnt.type) {
		case SDL_QUIT:
			_gameState = GameState::EXIT;
			break;
		}
	}

}

// ------------------------------------------------------------------------------------------
//
void MainGame::drawGame() {

	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Start to use the shaders.
	_colorProgram.use();

	// Set active texture to player texture id.
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _playerTexture.id);

	GLint textureLocation = _colorProgram.getUniformLocation("mySampler");
	glUniform1i(textureLocation, 0);

	GLint timeLocation = _colorProgram.getUniformLocation("time");
	glUniform1f(timeLocation, _time);

	_sprite.drawSprite();

	glBindTexture(GL_TEXTURE_2D, 0);
	_colorProgram.unuse();

	SDL_GL_SwapWindow(_window);

}
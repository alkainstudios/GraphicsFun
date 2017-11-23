#include "Errors.h"

#include <cstdlib>

#include <iostream>
#include <SDL/SDL.h>

void fatalError(string errorString) {
	cout << errorString << endl;
	cout << "Enter any key to quit... ";
	int tmp;
	cin >> tmp;
	SDL_Quit();
	exit(69);
}
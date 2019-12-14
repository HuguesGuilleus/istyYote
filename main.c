// Projet Yoté -- IATIC 3 ISTY
// See the authors in AUTHOR.md

#include "main.h"

int main(int argc, char const *argv[]) {
	srand(time(NULL));
	initDisplay();

	initDev();
	display();
	// displayBoard(FALSE);

	// Pour le développement
	SDL_Event event;
	do {
		SDL_WaitEvent(&event);
	} while(event.type != SDL_QUIT);

	// partie();

	// TODO: faire une fonction propre pour quiter la SDL.
	SDL_Quit();
	return 0;
}

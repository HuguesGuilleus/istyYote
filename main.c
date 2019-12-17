// Projet Yoté -- IATIC 3 ISTY
// See the authors in AUTHOR.md

#include "main.h"

int main(int argc, char const *argv[]) {
	srand(time(NULL));
	initDisplay();

	partie();

	SDL_Quit();
	return 0;
}

void end(SDL_Event _) {
	SDL_Quit();
	exit(0);
}

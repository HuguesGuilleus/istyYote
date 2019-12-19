// Projet Yoté -- IATIC 3 ISTY
// See the authors in AUTHOR.md

#include "main.h"



int main(int argc, char const *argv[]) {
	srand(time(NULL));
	initDisplay();
	initBoard();
	partie();
	SDL_Quit();
	return 0;
}

// Termine le programme, utile pour quitter le programme lorsqu'un joueur,
// déclenche un évènement SDL_QUIT.
void end(SDL_Event _) {
	SDL_Quit();
	exit(0);
}

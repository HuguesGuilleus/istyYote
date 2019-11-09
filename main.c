// Projet Yoté -- IATIC 3 ISTY
// See the authors in AUTHOR.md

int main(int argc, char const *argv[]) {
	initDisplay();
	initBoard();

	displayBoard();
	sleep(1);

	SDL_Quit();
	return 0;
}

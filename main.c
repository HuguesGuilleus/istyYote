#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char const *argv[]) {
	printf("Hello World\n");

	SDL_Init(SDL_INIT_VIDEO);

	SDL_Surface *bg = SDL_SetVideoMode(500, 500, 32, SDL_HWSURFACE);
	if (bg == NULL) {
		printf("Error launch SDL\n");
		printf("%s\n", SDL_GetError());
		return 1;
	}

	sleep(1);

	SDL_Quit();
	return 0;
}

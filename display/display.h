// Projet Yoté -- IATIC 3 ISTY
// See the authors in AUTHOR.md

#ifndef DISPLAY_H
	#define DISPLAY_H
	#include "../main.h"

	void initDisplay();
	void fatal(void* pt, char ms[]);
	void display();
	void displayBoard(bool flip);
	void displayPawn(SDL_Surface* square, int x, int y);

#endif

#ifndef CONTROL_H
	#define CONTROL_H

	#include "../main.h"


	void partie(void);
	bool clickOnPiece(SDL_Event *click, coord * c, raceJoueur gamer);
	bool clickToCoord(SDL_Event * click, coord * c);
	void printfCoord(coord * c);

	void gamerRand(raceJoueur * gamer);
	void gamerPrint(raceJoueur gamer);
	void gamerSwitch(raceJoueur * gamer);

	int ActionJoueur(raceJoueur joueur,coord *c);
	int verifClic1 (int x, int y, raceJoueur joueur);
	coord placement(raceJoueur joueur);
	bool verifClic2Placement(int x, int y);

#endif

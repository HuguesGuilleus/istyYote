#ifndef CONTROL_H
	#define CONTROL_H

	#include "../main.h"

	void partie2(void);

	void partie(void);
	bool clickOnPiece(SDL_Event *click, coord * c, raceJoueur gamer);
	// bool clickToCoord(SDL_Event * click, coord * c);
	void printfCoord(coord * c);

	int ActionJoueur(raceJoueur joueur,coord *c,int *continuer);
	int verifClic1 (int x, int y, raceJoueur joueur);
	coord placement(raceJoueur joueur);
	bool verifClic2Placement(int x, int y);
	coord deplacement(raceJoueur joueur,coord c1);
	bool verifClic2Deplacement(int x,int y,coord c1);

#endif

#ifndef CONTROL_H
	#define CONTROL_H

	#include "../main.h"


	void partie(void);
	bool clickOnPiece(SDL_Event *click, coord * c, typeColor gamer);
	bool clickToCoord(SDL_Event * click, coord * c);
	void printfCoord(coord * c);

	void gamerRand(typeColor * gamer);
	void gamerPrint(typeColor gamer);
	void gamerSwitch(typeColor * gamer);

#endif

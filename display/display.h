// Projet Yoté -- IATIC 3 ISTY
// See the authors in AUTHOR.md

#ifndef DISPLAY_H
	#define DISPLAY_H
	#include "../main.h"


	// Coordonnées de l'origine du plateau
	#define ORIGINE_PLATEAU_X 310
	#define ORIGINE_PLATEAU_Y 240

	/* Marge entre l'effigie du pion
	d'une réserve et le bord de la fenêtre */
	#define MARGE_RESERVE 20

	// Nombre maximum de caractère pour le fichier de règles
	#define TAILLE_MAX_REGLES 1000

	void initDisplay();
	void fatal(void* pt, char ms[]);
	void display();
	void displayBoard();
	void displayPawn(SDL_Surface* square, int x, int y);
	void displayTile(int x, int y);
	void displayTitle();
	void displayMenuButtons();
	void displayRules();
	void displayReserve();
	void displayReserveBox();

#endif

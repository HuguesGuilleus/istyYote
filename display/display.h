// Projet Yoté -- IATIC 3 ISTY
// See the authors in AUTHOR.md

#ifndef DISPLAY_H
	#define DISPLAY_H
	#include "../main.h"

	#define LARGEUR_FENETRE 1110
	#define HAUTEUR_FENETRE 800

	// Il était proposé de mettre cette valeur à 70,
	#define TAILLE_CASE 80

	// TODO: vérifier que le plateau est bien centré.
	#define ORIGINE_PLATEAU_X 360
	#define ORIGINE_PLATEAU_Y 240

	// TODO: Caller bien
	#define ORIGINE_RACE_HEAD_Y 20

	// TODO: Mettres des valeurs précises
	#define ORIGINE_RESERVE_ORC    80
	#define ORIGINE_RESERVE_DEAMON 840

	// Sprites
	typedef struct {
		SDL_Surface* spriteCase;
		SDL_Surface* spriteDemon;
		SDL_Surface* spriteOrc;
		SDL_Surface* spriteFond;
		SDL_Surface* spriteNuage;
	} typeSprites;

	// Déclaration de la variable globale permettant d'utiliser les sprites
	typeSprites sprites;

	void initDisplay();
	void fatal(void* pt, char ms[]);

	void display();
	void displayBoard(bool flip);
	void displayTile(int x, int y);
	void displayStatus(Uint32 color, int x, int y);
	void displayPawn(SDL_Surface* square, int x, int y);

	void displayReserve(int nbPion, SDL_Surface* img, int x);

	void displayRace();
	void displayRaceHead(SDL_Surface * img, int x);

#endif

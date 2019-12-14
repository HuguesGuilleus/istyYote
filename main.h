// Projet Yoté -- IATIC 3 ISTY
// See the authors in AUTHOR.md

#ifndef MAIN_H
	#define MAIN_H

	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <time.h>
	#include <SDL.h>
	#include <SDL_ttf.h>
	#include <unistd.h>

	//suggestion
	#define RESERVE 3
	#define PLATEAU 4
	#define JOUEUR1 1
	#define JOUEUR2 2

	// Type booléen
	typedef int bool;
	#define TRUE (1)
	#define FALSE (0)

	#define LARGEUR 5
	#define HAUTEUR 6

	// Coordonées d'une case du plateau (board).
	typedef struct {
		int x;
		int y;
	} coord;

	// La race d'un pion
	typedef enum {
		VIDE,
		ORC,
		DEMON,
	} raceJoueur;

	// Status d'une case:
	typedef enum {
		DEFAULT,
		SELECTED, // une case séléctionné
		ACCESSIBLE, // une casse accessible par un pion sur le plateau
		CAPTURE, // un pion adverse qui peut être pris
	} typeStatus;

	// une case du plateau
	typedef struct {
		raceJoueur race;
		typeStatus status ;
	} square;

	// La partie courante
	struct {
		// Race du joueur courant
		raceJoueur joueur;
		// Nombre de pion dans une réserve
		int reserveOrc;
		int reserveDemon;
	} currentParty ;

	// Le plateau, accès avec plateau[x][y] avec l'origine en haut à gauche
	// comme en SDL, SVG, canvas...
	square board[LARGEUR][HAUTEUR] ;

	#include "display/display.h"
	#include "modele/mod.h"

#endif

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


	// Type booléen
	typedef int bool;
	#define TRUE (1)
	#define FALSE (0)


	#define LARGEUR 6
	#define HAUTEUR 5

	#define LARGEUR_FENETRE 500
	#define HAUTEUR_FENETRE 500

	// Coordonées d'une case du plateau (board).
	typedef struct {
		int x;
		int y;
	} coord;

	// La couleur d'un pion
	typedef enum {
		EMPTY,
		BLACK,
		WHITE,
	} typeColor;

	// L'état d'une case:
	typedef enum {
		DEFAULT,
		SELECTED, // une case séléctionné
		ACCESSIBLE, // une casse accessible par un pion sur le plateau
		CAPTURE, // un pion adverse qui peut être pris
	} typeStatus;

	// une case du plateau
	typedef struct {
		typeColor color;
		typeStatus status ;
	} square;

	// Le plateau, accès avec plateau[x][y] avec l'origine en haut à gauche
	// comme en SDL, SVG, canvas
	square board[LARGEUR][HAUTEUR] ;

	#include "display/display.h"
	#include "modele/mod.h"

#endif

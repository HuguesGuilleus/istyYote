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


	// Type booléean
	typedef int bool;
	#define TRUE (1)
	#define FALSE (0)


	#define LARGEUR 6
	#define HAUTEUR 5

	// La couleur d'un pion
	typedef enum {
		EMPTY,
		BLACK,
		WHITE,
	} typeColor;

	// une case du plateau
	typedef struct {
		typeColor color;
	} square;

	// Le plateau, accès avec plateau[x][y] avec l'origine en haut à gauche
	// comme en SDL, SVG, canvas
	square board[LARGEUR][HAUTEUR] ;

	#include "display/display.h"
	#include "modele/mod.h"

#endif

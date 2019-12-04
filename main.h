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
	#define TAILLE_CASE 70
	#define RESERVE 3
	#define PLATEAU 4
	#define JOUEUR1 1
	#define JOUEUR2 2

	// Type booléen
	typedef int bool;
	#define TRUE (1)
	#define FALSE (0)

	#define LARGEUR 6
	#define HAUTEUR 5

	#define LARGEUR_FENETRE 980 //600
	#define HAUTEUR_FENETRE 700//600

	#define LARGEUR_CASE 70
	#define HAUTEUR_CASE LARGEUR_CASE




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

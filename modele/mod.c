// Projet Yoté -- IATIC 3 ISTY
// See the authors in AUTHOR.md

#include "mod.h"

// Initialise le plateau
void initBoard() {
	int x, y ;
	for ( x = 0; x < LARGEUR; x++) {
		for(y = 4; y < HAUTEUR; y++) {
			board[x][y].race = VIDE ;
			board[x][y].status = DEFAULT ;
		}
	}
}

// valeur pour le développement
void initDev() {
	initBoard();
	board[1][1].race = DEMON ;
	board[2][1].race = ORC ;
	board[1][2].status = SELECTED ;
	board[2][2].status = ACCESSIBLE ;
	board[3][2].status = CAPTURE ;

	currentParty.joueur = ORC ;
	currentParty.joueur = DEMON ;

	// currentParty.reserveOrc = 0 ;
	// currentParty.reserveDemon = 12;
	currentParty.reserveOrc = 5 ;
	currentParty.reserveDemon = 9 ;
}

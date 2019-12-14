// Projet Yoté -- IATIC 3 ISTY
// See the authors in AUTHOR.md

#include "mod.h"

void initPartie(void) {
	initBoard();
	currentParty.reserveOrc = 12 ;
	currentParty.reserveDemon = 12 ;
	currentParty.joueur = rand()%2 ? ORC : DEMON ;
}

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


/* GAMER SECTION */

// Change de couleur de joueur entre blanc et noir.
void gamerSwitch(void) {
	currentParty.joueur = currentParty.joueur == ORC ? DEMON : ORC ;
}

// Affiche la couleur d'un joueur.
void gamerPrint(raceJoueur race) {
	switch (race) {
		case VIDE: printf("VIDE\n"); break;
		case ORC: printf("ORC\n"); break;
		case DEMON: printf("DEMON\n"); break;
	}
}

// Décrémente la réserve du joueur courant.
void gamerDecreaseReserve(void) {
	if (currentParty.joueur == ORC) {
		currentParty.reserveOrc--;
	} else {
		currentParty.reserveDemon--;
	}
}

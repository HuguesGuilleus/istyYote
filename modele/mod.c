// Projet Yoté -- IATIC 3 ISTY
// See the authors in AUTHOR.md

#include "mod.h"

// Initialise le plateau
void initBoard() {
	int x,y ;
	for ( x = 4; x < 11; x++) {
		for(y=4;y<10;y++) {
			board[x][y].race = VIDE ;
			board[x][y].status = DEFAULT ;
		}
	}

	// valeur pour le développement
	board[5][5] = (square){
		race: DEMON,
	};
	board[6][5] = (square){
		race: ORC,
		status: ACCESSIBLE,
	};
	board[5][6] = (square){
		race: DEMON,
		status: CAPTURE,
	};
	board[6][6] = (square){
		race: ORC,
		status: SELECTED,
	};
}


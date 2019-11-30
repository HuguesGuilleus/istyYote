// Projet Yoté -- IATIC 3 ISTY
// See the authors in AUTHOR.md

#include "control.h"

// Joue une partie
void partie(void) {
	SDL_Event event;
	coord depart = {} ;
	typeColor gamer ;

	gamerRand(&gamer);
	printf("Couleur premier joueur: ");
	gamerPrint(gamer);

	display();

	while (TRUE) {
		SDL_WaitEvent(&event);
		switch (event.type) {
			case SDL_QUIT:
				return;
			case SDL_MOUSEBUTTONDOWN:
				// TODO: Faire un clic sur un bouton qui quitte la partie
				// TODO: Faire un placement de nouveau pion
				if (clickOnPiece(&event, &depart, gamer)) {
					// board[depart.x][depart.y].color = gamer ;
					board[depart.x][depart.y].status = SELECTED ;
					displayBoard(TRUE);
				}
				break;
		}
	}
}



// Retourne TRUE si le click est sur un pion du joueur courant.
// Si c'est le cas, la case sera enregistrée dans le pointeur de c.
bool clickOnPiece(SDL_Event *click, coord * c, typeColor gamer) {
	if (clickToCoord(click, c) == FALSE) {
		return FALSE ;
	}
	if (board[c->x][c->y].color == gamer) {
		return TRUE ;
	} else {
		return FALSE ;
	}
}

// Converti les coordondées d'un clic en coordonées du tableau.
// Si le clic n'est pas dans le tableau, return FALSE.
bool clickToCoord(SDL_Event * click, coord * c) {
	int x = click->button.x;
	int y = click->button.y;

	if (x> LARGEUR * 50 || y > HAUTEUR * 50) {
		return FALSE ;
	} else {
		c->x = x/50 ;
		c->y = y/50 ;
		return TRUE ;
	}
}

// Affiche les coordonées dans la console d'un clic.
void printCoord(coord * c) {
	if (c == NULL) {
		printf("(null)\n");
	} else {
		printf("(%d,%d)\n", c->x, c->y);
	}
}


/* GAMER SECTION */

// Tire aléatoirement un joueur blanc ou noir pour jouer.
void gamerRand(typeColor * gamer) {
	*gamer = rand()%2 ? WHITE : BLACK ;
}

// Affiche la couleur d'un joueur.
void gamerPrint(typeColor gamer) {
	switch (gamer) {
		case EMPTY: printf("EMPTY\n"); break;
		case WHITE: printf("WHITE\n"); break;
		case BLACK: printf("BLACK\n"); break;
	}
}

// Change de couleur de joueur entre blanc et noir.
void gamerSwitch(typeColor * gamer) {
	* gamer = *gamer == WHITE ? BLACK : WHITE ;
}

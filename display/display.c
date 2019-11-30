// Projet Yoté -- IATIC 3 ISTY
// See the authors in AUTHOR.md

#include "display.h"

// La surface de la fenêtre.
SDL_Surface *bg = NULL ;

// Une case vide.
SDL_Surface* squareBg   = NULL ;

// Un pion noir.
SDL_Surface* pieceBlack = NULL ;

// Un pion blacn.
SDL_Surface* pieceWhite = NULL ;

// Initilise la SDL et charge les images
void initDisplay() {
	SDL_Init(SDL_INIT_VIDEO);
	SDL_WM_SetCaption("ISTY - Yoté", NULL);

	bg = SDL_SetVideoMode(500, 500, 32, SDL_HWSURFACE);
	fatal(bg, "launch SDL");

	squareBg = SDL_LoadBMP("media/squareBg.bmp");
	fatal(squareBg, "load squareBg");

	pieceBlack = SDL_LoadBMP("media/pieceBlack.bmp");
	fatal(pieceBlack, "load pieceBlack");
	SDL_SetColorKey(pieceBlack,SDL_SRCCOLORKEY, SDL_MapRGB(pieceBlack->format,255,0,255));

	pieceWhite = SDL_LoadBMP("media/pieceWhite.bmp");
	fatal(pieceWhite, "Error load pieceWhite");
	SDL_SetColorKey(pieceWhite,SDL_SRCCOLORKEY, SDL_MapRGB(pieceWhite->format,255,0,255));
}

// Fatal if pt is NULL, print the message and the log of SDL
void fatal(void* pt, char ms[]) {
	if (pt == NULL) {
		fprintf(stderr, "Error %s: %s\n", ms, SDL_GetError());
		exit(1);
	}
}

// Affiche tout
void display() {
	displayBoard(FALSE);
	SDL_Flip(bg);
}

// Affiche que le plateau, si flip est vrai, alors
// le plateau est affiché.
void displayBoard(bool flip) {
	int x,y ;
	for ( x = 0; x < 6; x++) {
		for(y=0;y<5;y++) {
			displaySquare(squareBg, x, y);
			switch (board[x][y].color) {
				case EMPTY:
					break;
				case WHITE:
					displaySquare(pieceWhite, x, y);
					break;
				case BLACK:
					displaySquare(pieceBlack, x, y);
					break;
			}
		}
	}
	if (flip) {
		SDL_Flip(bg);
	}
}

// x and y is the coord of a square
void displaySquare(SDL_Surface* square, int x, int y) {
	// TODO: what in the return of SDL_BlitSurface ? error?
	SDL_BlitSurface(square, NULL, bg, &(SDL_Rect){
		x: x*50,
		y: y*50,
	});
}


void displayTitle() {
	SDL_Surface* titleSprite = SDL_LoadBMP("media/title.bmp");
	fatal(titleSprite, "load titleSprite");

	SDL_BlitSurface(titleSprite, NULL, bg, &(SDL_Rect){0,0});

	SDL_Flip(bg);
}

void displayMove(typeColor color, SQL_Square start, SQL_Square end) {
	/*
	int x,y ;
	for ( x = 0; x < 6; x++) {
		for(y=0;y<5;y++) {
			displaySquare(squareBg, x, y);
			switch (board[x][y].color) {
				case EMPTY:
					break;
				case WHITE:
					displaySquare(pieceWhite, x, y);
					break;
				case BLACK:
					displaySquare(pieceBlack, x, y);
					break;
			}
		}
	}
	*/

}
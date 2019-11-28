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
			switch (board[x][y].status) {
				// TODO: use the color format from the background
				case SELECTED:   displayStatus(0x00FF00, x, y); break;
				case ACCESSIBLE: displayStatus(0x0000FF, x, y); break;
				case CAPTURE:    displayStatus(0xFF0000, x, y); break;
			}
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

// Dessin un fond différent selon le fond
void displayStatus(Uint32 color, int x, int y) {
	int r = SDL_FillRect(bg, &(SDL_Rect){
		x: x*50,
		y: y*50,
		w: 50,
		h: 50,
	}, color);
	fatal(!r, "displayStatus()");
}

// x and y is the coord of a square
void displaySquare(SDL_Surface* square, int x, int y) {
	int r = SDL_BlitSurface(square, NULL, bg, &(SDL_Rect){
		x: x*50,
		y: y*50,
	});
	fatal(!r, "displaySquare()");
}

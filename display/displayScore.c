// Projet Yoté -- IATIC 3 ISTY
// See the authors in AUTHOR.md

#include "displayScore.h"

// La surface de la fenêtre.
SDL_Surface *fenetre ;

TTF_Font *police ;

void displayScoreInput(raceJoueur j) {
	SDL_Surface * text = NULL ;
	SDL_BlitSurface(sprites.spriteFond, NULL, fenetre, &(SDL_Rect){x:0,y:0});

	if (j == ORC) {
		text = TTF_RenderText_Solid(police, "Orc", (SDL_Color){0,0,0});
	} else {
		text = TTF_RenderText_Solid(police, "Demon", (SDL_Color){0,0,0});
	}
	SDL_BlitSurface(text, NULL, fenetre, &(SDL_Rect){
		x: (LARGEUR_FENETRE-text->w)/2,
		y: 100,
	});
	SDL_FreeSurface(text);
	text = NULL;

	SDL_Flip(fenetre);
}

// Affiche le texte en cours de saisit.
void displayScoreInputText(char* str) {
	SDL_Surface * text = NULL ;
	SDL_Rect pos = {};

	pos.h = TTF_FontHeight(police) ;
	pos.y = (HAUTEUR_FENETRE-pos.h)/2 ;
	pos.x = 0;
	pos.w = LARGEUR_FENETRE ;
	SDL_FillRect(fenetre, &pos, 0xD00000);

	text = TTF_RenderText_Solid(police, str, (SDL_Color){0,0,0});
	if (text != NULL) {
		pos.y = (HAUTEUR_FENETRE-text->h)/2 ;
		pos.x = (LARGEUR_FENETRE-text->w)/2 ;
		SDL_BlitSurface(text, NULL, fenetre, &pos);
	}

	SDL_Flip(fenetre);
	SDL_FreeSurface(text);
}

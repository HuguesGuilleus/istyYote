// Projet Yoté -- IATIC 3 ISTY
// See the authors in AUTHOR.md

#include "displayScore.h"

// variables également définie dans display.c
SDL_Surface *fenetre ;
TTF_Font * police ;

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

/* AFFICHAGE DES SCORES */

// Affiche les 15 derniers scores
void displayScores(void) {
	int i ;
	int begin = allScores.len > 15 ? allScores.len-15 : 0 ;
	SDL_Rect position = {};
	SDL_Surface* text = NULL;
	TTF_Font* police = TTF_OpenFont("fonts/VCR_OSD_MONO_1.001.ttf", 18);

	displayScoreAssets();

	position.y = sprites.spriteNuage->h + 50;
	for (i = begin; i < allScores.len; i++) {
		text = TTF_RenderUTF8_Blended(police, allScores.l[i].playerOrc, couleurNoire);
		position.x = (LARGEUR_FENETRE / 2) - text->w - 40;
		SDL_BlitSurface(text, NULL, fenetre, &position);
		SDL_FreeSurface(text);

		text = TTF_RenderUTF8_Blended(police, allScores.l[i].playerDemon, couleurNoire);
		position.x = (LARGEUR_FENETRE / 2) + 42;
		SDL_BlitSurface(text, NULL, fenetre, &position);
		SDL_FreeSurface(text);

		position.y += 3 ;

		text = TTF_RenderGlyph_Blended(police, scoreSaveStatus1(allScores.l[i].status), couleurNoire);
		position.x = (LARGEUR_FENETRE / 2) - text->w - 15;
		SDL_BlitSurface(text, NULL, fenetre, &position);
		SDL_FreeSurface(text);

		text = TTF_RenderGlyph_Blended(police, scoreSaveStatus2(allScores.l[i].status), couleurNoire);
		position.x = (LARGEUR_FENETRE / 2) + 17;
		SDL_BlitSurface(text, NULL, fenetre, &position);
		SDL_FreeSurface(text);

		position.y += 27;
	}

	SDL_Flip(fenetre);
	TTF_CloseFont(police);
}

void displayScoreAssets(void) {
	SDL_Surface* affichageTexte = NULL;
	TTF_Font* police = TTF_OpenFont("fonts/VCR_OSD_MONO_1.001.ttf", 50);

	// Fond, nuage et bouton
	eraseWindow();
	SDL_BlitSurface(sprites.spriteNuage, NULL, fenetre, &(SDL_Rect){
		x: 310,
		y: 3,
	});
	displayBackButton();

	// Titre
	affichageTexte = TTF_RenderUTF8_Blended(police, "Scores", couleurNoire);
	SDL_BlitSurface(affichageTexte, NULL, fenetre, &(SDL_Rect){
		x: (LARGEUR_FENETRE / 2) - (affichageTexte->w / 2),
		y:(sprites.spriteNuage->h / 2) - (affichageTexte->h / 4),
	});
	TTF_CloseFont(police);

	/* Dessine une ligne noire au milieu de la fenêtre
	pour séparer les pseudos des joueurs */
	SDL_FillRect(fenetre, &(SDL_Rect){
		x: (LARGEUR_FENETRE / 2),
		y: sprites.spriteNuage->h + 50,
		w: 2,
		h: 450
	}, 0x00000);
}

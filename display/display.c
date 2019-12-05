// Projet Yoté -- IATIC 3 ISTY
// See the authors in AUTHOR.md

#include "display.h"

// La surface de la fenêtre.
SDL_Surface *fenetre = NULL ;

// Initilise la SDL et charge les images
void initDisplay() {
	SDL_Init(SDL_INIT_VIDEO);
	SDL_WM_SetCaption("ISTY - Yoté", NULL);

	fenetre = SDL_SetVideoMode(LARGEUR_FENETRE, HAUTEUR_FENETRE, 32, SDL_HWSURFACE);
	fatal(fenetre, "launch SDL");
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
	SDL_Flip(fenetre);
}

// Affiche que le plateau, si flip est vrai, alors
// le plateau est affiché.
void displayBoard(bool flip) {
	int x,y ;

	SDL_Surface* spriteCase = SDL_LoadBMP("media/sprites/spriteCase.bmp");
	fatal(spriteCase, "load spriteCase");

	SDL_Surface* demon = SDL_LoadBMP("media/sprites/pions/demon.bmp");
	fatal(demon, "load demon");
	SDL_SetColorKey(demon, SDL_SRCCOLORKEY, SDL_MapRGB(demon->format,255,0,255));

	SDL_Surface* orc = SDL_LoadBMP("media/sprites/pions/orc.bmp");
	fatal(orc, "Error load orc");
	SDL_SetColorKey(orc, SDL_SRCCOLORKEY, SDL_MapRGB(orc->format,255,0,255));

	for ( x = 4; x < 10; x++) {
		for(y = 4; y < 9; y++) {
			displaySquare(spriteCase, x, y);
			/* switch (board[x][y].status) {
				// TODO: use the color format from the background
				spriteCase SELECTED:   displayStatus(0x00FF00, x, y); break;
				spriteCase ACCESSIBLE: displayStatus(0x0000FF, x, y); break;
				spriteCase CAPTURE:    displayStatus(0xFF0000, x, y); break;
			} */
			switch (board[x][y].race) {
				case VIDE:
					break;
				case ORC:
					displaySquare(orc, x, y);
					break;
				case DEMON:
					displaySquare(demon, x, y);
					break;
			}
		}
	}
	if (flip) {
		SDL_Flip(fenetre);
	}
}

// Dessin un fond différent selon le fond
void displayStatus(Uint32 color, int x, int y) {
	int r = SDL_FillRect(fenetre, &(SDL_Rect){
		x: x * LARGEUR_CASE,
		y: y * HAUTEUR_CASE,
		w: LARGEUR_CASE,
		h: HAUTEUR_CASE,
	}, color);
	fatal(!r, "displayStatus()");
}

// x and y is the coord of a square
void displaySquare(SDL_Surface* square, int x, int y) {
	int r = SDL_BlitSurface(square, NULL, fenetre, &(SDL_Rect){
		x: x*LARGEUR_CASE,
		y: y*HAUTEUR_CASE,
	});
	fatal(!r, "displaySquare()");
}


void displayTitle() {
	SDL_Surface* spriteTitre = SDL_LoadBMP("media/sprites/spriteTitre.bmp");
	fatal(spriteTitre, "load titleSprite");

	SDL_BlitSurface(spriteTitre, NULL, fenetre, &(SDL_Rect){0,0});

	SDL_Flip(fenetre);
}


// Affiche les boutons du menu du jeu
void displayMenuButtons() {
	SDL_Surface* boutonPVP = SDL_LoadBMP("media/sprites/boutons/boutonPVP.bmp");
	SDL_Surface* boutonIA = SDL_LoadBMP("media/sprites/boutons/boutonIA.bmp");
	SDL_Surface* boutonRegles = SDL_LoadBMP("media/sprites/boutons/boutonRegles.bmp");
	SDL_Surface* boutonResultats = SDL_LoadBMP("media/sprites/boutons/boutonResultats.bmp");
	SDL_Surface* boutonQuitter = SDL_LoadBMP("media/sprites/boutons/boutonQuitter.bmp");

	SDL_BlitSurface(boutonPVP, NULL, fenetre, &(SDL_Rect){50, 200});
	SDL_BlitSurface(boutonIA, NULL, fenetre, &(SDL_Rect){260, 200});
	SDL_BlitSurface(boutonRegles, NULL, fenetre, &(SDL_Rect){50, 280});
	SDL_BlitSurface(boutonResultats, NULL, fenetre, &(SDL_Rect){260, 280});
	SDL_BlitSurface(boutonQuitter, NULL, fenetre, &(SDL_Rect){50, 360});

	SDL_Flip(fenetre);
}

// Affiche les règles du jeu
void displayRules(char* regles) {
	int i = 0;

	// Initialisation de SDL_ttf
	TTF_Init();
	TTF_Font* police = TTF_OpenFont("media/LifeCraft.ttf", 30);

	SDL_Surface* texte = NULL;

	char* separateur = "_";
	// Découpe une chaîne de caractères en sous-chaînes, en fonction d'un ou plusieurs séparateurs
	const char* ligne = strtok(regles, separateur);

	while (ligne != NULL) {
		texte = TTF_RenderUTF8_Shaded(police, ligne, (SDL_Color){255, 255, 255}, (SDL_Color){0,0,0});
		SDL_BlitSurface(texte, NULL, fenetre, &(SDL_Rect){0, i});
		i += 30;
		ligne = strtok(NULL, separateur);
	}

	SDL_Flip(fenetre);

	// Clotûre de la police et de SDL_ttf
	TTF_CloseFont(police);
	TTF_Quit();
}


// Affiche les derniers scores
void displayScores(char* scores) {
	// Coordonnées x dans la fenêtre
	int i = 150;
	// Coordonnées y dans la fenêtre
	int j = 100;
	// Compteur
	int k = 0;

	TTF_Init();
	TTF_Font* police = NULL;
	SDL_Surface* texte = NULL;

	police = TTF_OpenFont("media/LifeCraft.ttf", 25);
	texte = TTF_RenderUTF8_Shaded(police, "REGLES DU JEU", (SDL_Color){255, 255, 255}, (SDL_Color){0,0,0});
	SDL_BlitSurface(texte, NULL, fenetre, &(SDL_Rect){(LARGEUR_FENETRE - texte->w)/2, 30});
	TTF_CloseFont(police);

	// La police est ouverte une seconde fois car on l'ouvre avec une taille différente
	police = TTF_OpenFont("media/LifeCraft.ttf", 20);
	const char* separateurs = "\n|";
	char* ligne = strtok(scores, separateurs);

	while (ligne != NULL) {
		texte = TTF_RenderUTF8_Shaded(police, ligne, (SDL_Color){255, 255, 255}, (SDL_Color){0,0,0});
		SDL_BlitSurface(texte, NULL, fenetre, &(SDL_Rect){i, j});
		k++;

		// Si k % 2 = 0 alors on est en train d'écrire le score du premier joueur d'une partie
		// Sinon on est en train d'écrire le score du 2e joueur
		if (k % 2 == 0) {
			i = 150;
			j+= 30;
		}
		else {
			i = 250;
		}

		ligne = strtok(NULL, separateurs);
	}

	SDL_Flip(fenetre);

	TTF_CloseFont(police);
	TTF_Quit();
}

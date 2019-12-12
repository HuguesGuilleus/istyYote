// Projet Yoté -- IATIC 3 ISTY
// See the authors in AUTHOR.md

#include "display.h"

// La surface de la fenêtre.
SDL_Surface *fenetre = NULL, *texte = NULL, *rectangle = NULL ;

SDL_Rect position;

TTF_Font *police = NULL;
SDL_Color couleurNoire = {0, 0, 0};


// Initilise la SDL et charge les images
void initDisplay() {
	
	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();
	
	fenetre = SDL_SetVideoMode(1040, 880, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
	SDL_WM_SetCaption("ISTY - Yoté", NULL);

	/* Chargement de la police */
	police = TTF_OpenFont("angelina.TTF", 65);
	/* Écriture du texte dans la SDL_Surface texte en mode Blended (optimal) */
	texte = TTF_RenderText_Blended(police, "JEU DE YOTE", couleurNoire);

	// Allocation de la surface
	rectangle = SDL_CreateRGBSurface(SDL_HWSURFACE, 1028, 180, 32, 0, 0, 0, 0);
	SDL_FillRect(fenetre, NULL, SDL_MapRGB(fenetre->format, 17, 206, 112));

	position.x = 5; // Les coordonnées de la surface seront (0, 0)
	position.y = 5;
	// Remplissage de la surface avec du blanc
	SDL_FillRect(rectangle, NULL, SDL_MapRGB(fenetre->format, 255, 255, 255)); 
	SDL_BlitSurface(rectangle, NULL, fenetre, &position); // Collage de la surface sur l'écran

	position.x = 350;
	position.y = 70;
	SDL_BlitSurface(texte, NULL, fenetre, &position); /* Blit du texte */
	

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
	sprites.spriteCase = SDL_LoadBMP("media/sprites/spriteCase.bmp");
	sprites.spriteDemon = SDL_LoadBMP("media/sprites/pions/demon.bmp");
	sprites.spriteOrc = SDL_LoadBMP("media/sprites/pions/orc.bmp");
	
	SDL_SetColorKey(sprites.spriteDemon, SDL_SRCCOLORKEY, SDL_MapRGB(sprites.spriteDemon->format,255,0,255));
	SDL_SetColorKey(sprites.spriteOrc, SDL_SRCCOLORKEY, SDL_MapRGB(sprites.spriteOrc->format,255,0,255));

	displayBoard(FALSE);
	SDL_Flip(fenetre);
}

// Affiche que le plateau, si flip est vrai, alors
// le plateau est affiché.
void displayBoard(bool flip) {
	int x,y ;
	for ( x = 4; x < 10; x++) {
		for(y = 4; y < 9; y++) {
			/* switch (board[x][y].status) {
				// TODO: use the color format from the background
				spriteCase SELECTED:   displayStatus(0x00FF00, x, y); break;
				spriteCase ACCESSIBLE: displayStatus(0x0000FF, x, y); break;
				spriteCase CAPTURE:    displayStatus(0xFF0000, x, y); break;
			} */
			displayTile(x, y);
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

// x et y sont les coordonnées pour le dessin du pion
void displayPawn(SDL_Surface* sprite, int x, int y) {
	int r = SDL_BlitSurface(sprite, NULL, fenetre, &(SDL_Rect){
		x: x * LARGEUR_CASE,
		y: y * HAUTEUR_CASE,
	});
	fatal(!r, "displayPawn()");
}

void displayTile(int x, int y) {
	SDL_BlitSurface(sprites.spriteCase, NULL, fenetre, &(SDL_Rect){
		x: x * LARGEUR_CASE,
		y: y * HAUTEUR_CASE
	});
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

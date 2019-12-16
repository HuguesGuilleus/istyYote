// Projet Yoté -- IATIC 3 ISTY
// See the authors in AUTHOR.md

#include "display.h"

// La surface de la fenêtre.
SDL_Surface * fenetre = NULL ;

// Var local?
SDL_Surface *rectangle = NULL ;

// Variable locale?
SDL_Rect position;

TTF_Font *police = NULL;

const SDL_Color couleurNoire = {0, 0, 0};


// Initilise la SDL et charge les images et les fontes
// TODO: Rework
void initDisplay() {
	SDL_Surface *texte = NULL ;

	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();
	fenetre = SDL_SetVideoMode(LARGEUR_FENETRE, HAUTEUR_FENETRE, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
	fatal(fenetre, "launch SDL");
	SDL_WM_SetCaption("ISTY - Yoté", NULL);

	// Chargement de la police
	police = TTF_OpenFont("VCR_OSD_MONO_1.001.ttf", 45);
	/* Écriture du texte dans la SDL_Surface texte en mode Blended (optimal) */
	texte = TTF_RenderText_Blended(police, "Tour des Orcs", couleurNoire);

	// Allocation de la surface
	rectangle = SDL_CreateRGBSurface(SDL_HWSURFACE, 500, 180, 32, 0, 0, 0, 0);
	SDL_FillRect(fenetre, NULL, SDL_MapRGB(fenetre->format, 17, 206, 112));

	//un fond jolie
	position.x = 0; // Les coordonnées de la surface seront (0, 0)
	position.y = 0;
	sprites.spriteFond = SDL_LoadBMP("media/sprites/fondjeu.bmp");
	SDL_BlitSurface(sprites.spriteFond, NULL, fenetre, &position);



	position.x = 310; // Les coordonnées de la surface seront (0, 0)
	position.y = 5;
	// Remplissage de la surface avec du blanc
	//SDL_FillRect(rectangle, NULL, SDL_MapRGB(fenetre->format, 255, 255, 255));
	//SDL_BlitSurface(rectangle, NULL, fenetre, &position); // Collage de la surface sur l'écran

	position.x = 310; // Les coordonnées de la surface seront (0, 0)
	position.y = 3;
	sprites.spriteNuage = SDL_LoadBMP("media/sprites/nuage.bmp");

	SDL_BlitSurface(sprites.spriteNuage, NULL, fenetre, &position);
	position.x = 364;
	position.y = 80;
	SDL_BlitSurface(texte, NULL, fenetre, &position); /* Blit du texte */


	sprites.spriteCase = SDL_LoadBMP("media/sprites/spriteCase.bmp");

	sprites.spriteDemon = SDL_LoadBMP("media/sprites/pions/demon.bmp");
	sprites.spriteOrc = SDL_LoadBMP("media/sprites/pions/orc.bmp");
	SDL_SetColorKey(sprites.spriteDemon, SDL_SRCCOLORKEY, SDL_MapRGB(sprites.spriteDemon->format,255,0,255));
	SDL_SetColorKey(sprites.spriteOrc, SDL_SRCCOLORKEY, SDL_MapRGB(sprites.spriteOrc->format,255,0,255));
}

// Si le pointeur pt est NULL, afficher le message ms et l'erreur de la SDL et
// arrête brutalement le programme. C'est une fonction utilitaire0.
void fatal(void* pt, char ms[]) {
	if (pt == NULL) {
		fprintf(stderr, "Error %s: %s\n", ms, SDL_GetError());
		exit(1);
	}
}

// Affiche tout
void display() {
	SDL_Rect pos ;
	// Fond et nuage
	SDL_BlitSurface(sprites.spriteFond, NULL, fenetre, &(SDL_Rect){x:0,y:0});
	pos = (SDL_Rect){
		x: 310,
		y: 3,
	};
	SDL_BlitSurface(sprites.spriteNuage, NULL, fenetre, &pos);

	displayReserve(currentParty.reserveOrc, sprites.spriteOrc, ORIGINE_RESERVE_ORC);
	displayReserve(currentParty.reserveDemon, sprites.spriteDemon, ORIGINE_RESERVE_DEAMON);
	displayRace();
	displayBoard(FALSE);

	SDL_Flip(fenetre);
}


/* AFFICHAGE DU PLATEAU */

// Affiche tout le plateau, si flip vaut vrai, alors la fenêtre est actualisée.
void displayBoard(bool flip) {
	int x, y ;
	for ( x = 0; x < LARGEUR; x++) {
		for ( y = 0; y < HAUTEUR; y++) {
			displayTile(x, y);
		}
	}
	if (flip) {
		SDL_Flip(fenetre);
	}
}

// Dessine une case du plateau (fond et pion), x et y sont les indices du
// plateau de jeu correspondant à la case dessinée.
void displayTile(int x, int y) {
	switch (board[x][y].status) {
		case SELECTED:   displayStatus(0x00FF00, x, y); break;
		case ACCESSIBLE: displayStatus(0x0000FF, x, y); break;
		case CAPTURE:    displayStatus(0xFF0000, x, y); break;
		default:
			displayPawn(sprites.spriteCase, x, y);
	}
	switch (board[x][y].race) {
		case ORC:
			displayPawn(sprites.spriteOrc, x, y);
			break;
		case DEMON:
			displayPawn(sprites.spriteDemon, x, y);
			break;
		default:
			;
	}
}

// Rempli avec une certaine couleur la case du plateau correpondant aux indices
// x et y. Cette fonction sert pour afficher le status de la case.
void displayStatus(Uint32 color, int x, int y) {
	int r = SDL_FillRect(fenetre, &(SDL_Rect){
		x: x * TAILLE_CASE + ORIGINE_PLATEAU_X,
		y: y * TAILLE_CASE + ORIGINE_PLATEAU_Y,
		w: TAILLE_CASE,
		h: TAILLE_CASE,
	}, color);
	fatal(!r, "displayStatus()");
}

// Dessine une image (fond, pion) sur le plateau de jeu. x et y sont les
// indices du plateau correspondant à la case dessinée.
void displayPawn(SDL_Surface* img, int x, int y) {
	int r = SDL_BlitSurface(img, NULL, fenetre, &(SDL_Rect){
		x: x * TAILLE_CASE + ORIGINE_PLATEAU_X,
		y: y * TAILLE_CASE + ORIGINE_PLATEAU_Y,
	});
	fatal(!r, "displayPawn()");
	SDL_Flip(fenetre);
}


/* AFFICHAGE DES RÉSERVES */

// Affiche la réserve d'un type de joueur. Prend la surface qui représente un
// pion à afficher (orc ou deamon). L'origine de la réserve sera: en ascisse
// l'agument x et en ordonée la macro constante ORIGINE_PLATEAU_Y.
//
// Si nbPion, le nombre de pion dans la réserve, est supérieur à 6, alors la
// fonction se réapellera avec un décallage sur x pour créé une autre colone.
void displayReserve(int nbPion, SDL_Surface* img, int x) {
	int y = ORIGINE_PLATEAU_Y;
	int r ;

	if (nbPion > 6) {
		displayReserve(nbPion-6, img, x+TAILLE_CASE);
		nbPion = 6 ;
	}

	for(; nbPion > 0 ; nbPion--) {
		SDL_BlitSurface(sprites.spriteCase, NULL, fenetre, &(SDL_Rect){
			x: x ,
			y: y ,
		});
		SDL_BlitSurface(img, NULL, fenetre, &(SDL_Rect){
			x: x ,
			y: y ,
		});
		y += TAILLE_CASE;
	}
}


/* AFFICHAGE RACE JOUEUR */

// Affiche la race du joueur et le nuage
void displayRace() {
	SDL_Surface * texte ;

	switch (currentParty.joueur) {
		case ORC:
			displayRaceHead(sprites.spriteOrc, ORIGINE_RESERVE_ORC);
			texte = TTF_RenderText_Solid(police, "ORC", couleurNoire);
			break;
		case DEMON:
			displayRaceHead(sprites.spriteDemon, ORIGINE_RESERVE_DEAMON);
			texte = TTF_RenderText_Solid(police, "DEMON", couleurNoire);
			break;
		default:;
	}

	SDL_BlitSurface(texte, NULL, fenetre, &(SDL_Rect){
		x: (LARGEUR_FENETRE-texte->w)/2,
		y: 75,
	});
	SDL_FreeSurface(texte);
}

// Affiche la tête coorespondant au joueur courant.
// TODO: Grossir la tête.
void displayRaceHead(SDL_Surface * img, int x) {
	SDL_BlitSurface(img, NULL, fenetre, &(SDL_Rect){
		x: x,
		y: ORIGINE_RACE_HEAD_Y,
	});
}


/* ZONE DE MENU */

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

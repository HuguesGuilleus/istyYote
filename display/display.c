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
	
	fenetre = SDL_SetVideoMode(1110, 800, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
	SDL_WM_SetCaption("ISTY - Yoté", NULL);

	/* Chargement de la police */
	police = TTF_OpenFont("fonts/VCR_OSD_MONO_1.001.ttf", 45);
	/* Écriture du texte dans la SDL_Surface texte en mode Blended (optimal) */

	// Allocation de la surface
	rectangle = SDL_CreateRGBSurface(SDL_HWSURFACE, 500, 180, 32, 0, 0, 0, 0);
	SDL_FillRect(fenetre, NULL, SDL_MapRGB(fenetre->format, 17, 206, 112));
	
	//un fond jolie
	position.x = 0; // Les coordonnées de la surface seront (0, 0)
	position.y = 0;
	sprites.spritefond = SDL_LoadBMP("media/sprites/fondjeu.bmp");
	SDL_BlitSurface(sprites.spritefond, NULL, fenetre, &position);


	//position.x = 310; // Les coordonnées de la surface seront (0, 0)
	//position.y = 5;
	// Remplissage de la surface avec du blanc
	//SDL_FillRect(rectangle, NULL, SDL_MapRGB(fenetre->format, 255, 255, 255)); 
	//SDL_BlitSurface(rectangle, NULL, fenetre, &position); // Collage de la surface sur l'écran
		

	sprites.spriteCase = SDL_LoadBMP("media/sprites/spriteCase.bmp");
	sprites.spriteDemon = SDL_LoadBMP("media/sprites/pions/demon.bmp");
	sprites.spriteOrc = SDL_LoadBMP("media/sprites/pions/orc.bmp");
	sprites.spritenuage = SDL_LoadBMP("media/sprites/nuage.bmp");
	SDL_SetColorKey(sprites.spriteDemon, SDL_SRCCOLORKEY, SDL_MapRGB(sprites.spriteDemon->format,255,0,255));
	SDL_SetColorKey(sprites.spriteOrc, SDL_SRCCOLORKEY, SDL_MapRGB(sprites.spriteOrc->format,255,0,255));

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
	//displayTitle();
	//displayMenuButtons();
	//displayRules();
	displayBoard(FALSE);

	SDL_Flip(fenetre);
}

// Affiche que le plateau
void displayBoard() {
	int x,y ;
	for ( x = 0; x < LARGEUR; x++) {
		for(y = 0; y < HAUTEUR; y++) {
			/* switch (board[x][y].status) {
				// TODO: use the color format from the background
				spriteCase SELECTED:   displayStatus(0x00FF00, x, y); break;
				spriteCase ACCESSIBLE: displayStatus(0x0000FF, x, y); break;
				spriteCase CAPTURE:    displayStatus(0xFF0000, x, y); break;
			} */
			displayTile(x, y);
		}
	}
}

// Affiche le tour du joueur courant
void displayRound(raceJoueur joueur) {
	char* texteTour = "Tour des ";
	char* nomJoueur = joueur == ORC ? "Orcs" : "Demons";
	strcat(texteTour, nomJoueur);
	texte = TTF_RenderText_Blended(police, texteTour, couleurNoire);

	position.x = 310;
	position.y = 3;
	//sprites.spritenuage = SDL_LoadBMP("media/sprites/nuage.bmp");
	SDL_BlitSurface(sprites.spritenuage, NULL, fenetre, &position);
	
	position.x = 364;
	position.y = 80;
	SDL_BlitSurface(texte, NULL, fenetre, &position);

	SDL_Flip(fenetre);
}

// Dessin un fond différent selon le fond
void displayStatus(Uint32 color, int x, int y) {
	int r = SDL_FillRect(fenetre, &(SDL_Rect){
		x: x * TAILLE_CASE,
		y: y * TAILLE_CASE,
		w: TAILLE_CASE,
		h: TAILLE_CASE,
	}, color);
	fatal(!r, "displayStatus()");
}


/* Dessine un pion sur le plateau de jeu
	x et y sont les coordonnées du pion */
void displayPawn(SDL_Surface* sprite, int x, int y) {
	int r = SDL_BlitSurface(sprite, NULL, fenetre, &(SDL_Rect){
		x: x * TAILLE_CASE + ORIGINE_PLATEAU_X,
		y: y * TAILLE_CASE + ORIGINE_PLATEAU_Y
	});

	// Pour les tests
	char* pion = (sprite == sprites.spriteOrc) ? "orc" : "demon";
	printf("Je dessine un %s en case (%d, %d)\n", pion, x, y);
	fatal(!r, "displayPawn()");
	SDL_Flip(fenetre);
}


/* Dessine une case sur le plateau de jeu
	x et y sont les coordonnées de la case.
	Permet également d'effacer un pion en redessinant la case par-dessus */
void displayTile(int x, int y) {
	SDL_BlitSurface(sprites.spriteCase, NULL, fenetre, &(SDL_Rect){
		x: x * TAILLE_CASE + ORIGINE_PLATEAU_X,
		y: y * TAILLE_CASE + ORIGINE_PLATEAU_Y
	});
}


/* Affiche la réserve: le sprite du pion, accompagné du
nombre de pions restant dans la réserve */
void displayReserve(int nbPions, SDL_Surface* sprite) {
	int x, y; // Coordonnées uitlisées pour dessiner la réserve
	char buffer[5]; // Buffer permettant d'afficher le nombre de pions en chaîne de caractères
	
	y = ORIGINE_PLATEAU_Y;
	sprintf(buffer, "%d", nbPions); // On met dans le buffer le nombre de pions de la réserve
	
	// Ouverture de la police
	TTF_Font* police = TTF_OpenFont("fonts/VCR_OSD_MONO_1.001.ttf", 30);
	SDL_Surface* texte = TTF_RenderText_Blended(police, buffer, couleurNoire);
	
	if (sprite == sprites.spriteOrc) {
		x = MARGE_RESERVE;
		SDL_BlitSurface(sprite, NULL, fenetre, &(SDL_Rect){x, y});

		x += sprite->w + 20;
		y += (sprite->h / 2) - (texte->h / 3);
		SDL_BlitSurface(texte, NULL, fenetre, &(SDL_Rect){x, y});
	}
	else{
		x = LARGEUR_FENETRE - sprite->w - MARGE_RESERVE;
		SDL_BlitSurface(sprite, NULL, fenetre, &(SDL_Rect){x, y});

		x -= 30;
		y += (sprite->h / 2) - (texte->h / 3);
		SDL_BlitSurface(texte, NULL, fenetre, &(SDL_Rect){x, y});
	}

	// Clôture de la police
	TTF_CloseFont(police);
}


void displayTitle() {
	int x,y;

	TTF_Font* police = TTF_OpenFont("fonts/VCR_OSD_MONO_1.001.ttf", 45);
	SDL_Surface* texte;
	
	// Titre
	texte = TTF_RenderText_Blended(police, "Yote", couleurNoire);
	x = (LARGEUR_FENETRE / 2) - (texte->w / 2);
	y = 10;
	SDL_BlitSurface(texte, NULL, fenetre, &(SDL_Rect){x,y});
	TTF_CloseFont(police);

	// Sous-titre
	y += 20 + texte->h; // On décale la coordonnées y pour écrire le sous-titre
	police = TTF_OpenFont("fonts/VCR_OSD_MONO_1.001.ttf", 35); // On réouvre la police avec une taille plus petite
	texte = TTF_RenderText_Blended(police, "Orcs vs Demons", couleurNoire);
	x = (LARGEUR_FENETRE / 2) - (texte->w / 2);
	SDL_BlitSurface(texte, NULL, fenetre, &(SDL_Rect){x,y});
	TTF_CloseFont(police);

	SDL_Flip(fenetre);
}


// Affiche les boutons du menu du jeu
void displayMenuButtons() {
	SDL_Surface* texte;
	SDL_Rect position;

	TTF_Font* police = TTF_OpenFont("fonts/VCR_OSD_MONO_1.001.ttf", 35);
	SDL_Surface* petitNuage = SDL_LoadBMP("media/sprites/petit_nuage.bmp");


	// Bouton Play
	position.x = (LARGEUR_FENETRE / 2) - petitNuage->w - 50;
	position.y = ORIGINE_PLATEAU_Y;
	SDL_BlitSurface(petitNuage, NULL, fenetre, &position);

	texte = TTF_RenderText_Blended(police, "Play", couleurNoire);
	SDL_BlitSurface(texte, NULL, fenetre, &(SDL_Rect){
		x: position.x + texte->w,
		y: position.y + texte->h + 5
	});


	// Bouton Quit
	position.y = ORIGINE_PLATEAU_Y + petitNuage->h + 50;
	SDL_BlitSurface(petitNuage, NULL, fenetre, &position);
	
	texte = TTF_RenderText_Blended(police, "Quit", couleurNoire);
	SDL_BlitSurface(texte, NULL, fenetre, &(SDL_Rect){
		x: position.x + texte->w,
		y: position.y + texte->h + 5
	});


	// Bouton Help
	position.x = (LARGEUR_FENETRE / 2) + 50;
	position.y = ORIGINE_PLATEAU_Y;
	SDL_BlitSurface(petitNuage, NULL, fenetre, &position);

	texte = TTF_RenderText_Blended(police, "Help", couleurNoire);
	SDL_BlitSurface(texte, NULL, fenetre, &(SDL_Rect){
		x: position.x + texte->w,
		y: position.y + texte->h + 5
	});


	// Bouton Scores
	position.y = ORIGINE_PLATEAU_Y + petitNuage->h + 50;
	SDL_BlitSurface(petitNuage, NULL, fenetre, &position);
	
	texte = TTF_RenderText_Blended(police, "Scores", couleurNoire);
	SDL_BlitSurface(texte, NULL, fenetre, &(SDL_Rect){
		x: position.x + (texte->w / 2),
		y: position.y + texte->h + 5
	});

	TTF_CloseFont(police);
	SDL_Flip(fenetre);
}


// Affiche les règles du jeu
void displayRules() {
	SDL_Rect position; // Position des différents élements de la fenêtre
	char texte[TAILLE_MAX_REGLES] = ""; // Tampon pour le fichier media/regles.txt

	TTF_Font* police = TTF_OpenFont("fonts/VCR_OSD_MONO_1.001.ttf", 50);
	SDL_Surface* affichageTexte = NULL;
	FILE* fichierRegles = NULL;
	
	// Titre
	position.x = 310;
	position.y = 3;
	SDL_BlitSurface(sprites.spritenuage, NULL, fenetre, &position);
	
	affichageTexte = TTF_RenderUTF8_Blended(police, "Règles", couleurNoire);
	position.x = (LARGEUR_FENETRE / 2) - (affichageTexte->w / 2);
	position.y = (sprites.spritenuage->h / 2) - (affichageTexte->h / 4);
	SDL_BlitSurface(affichageTexte, NULL, fenetre, &position);

	TTF_CloseFont(police);

	// Affichage des règles
	police = TTF_OpenFont("fonts/VCR_OSD_MONO_1.001.ttf", 18);
	position.y = sprites.spritenuage->h + 50;
	fichierRegles = fopen("media/regles.txt", "r");

	if (fichierRegles != NULL) {
		while (fgets(texte, TAILLE_MAX_REGLES, fichierRegles) != NULL) {
			texte[strlen(texte) - 1] = '\0'; // On supprime les "\n" en fin de lignes (permet aussi de supprimer les sauts de lignes);
			affichageTexte = TTF_RenderUTF8_Blended(police, texte, couleurNoire); // Affichage du texte avec encodage UTF8, pour prendre en compte les accents
			if (affichageTexte != 0) {
				position.x = (LARGEUR_FENETRE / 2) - (affichageTexte->w / 2);
				SDL_BlitSurface(affichageTexte, NULL, fenetre, &position);
			}
			position.y += 20;
		}
		fclose(fichierRegles);
	}

	SDL_Flip(fenetre);
	TTF_CloseFont(police);
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

	police = TTF_OpenFont("fonts/VCR_OSD_MONO_1.001.ttf", 25);
	texte = TTF_RenderUTF8_Shaded(police, "REGLES DU JEU", (SDL_Color){255, 255, 255}, (SDL_Color){0,0,0});
	SDL_BlitSurface(texte, NULL, fenetre, &(SDL_Rect){(LARGEUR_FENETRE - texte->w)/2, 30});
	TTF_CloseFont(police);

	// La police est ouverte une seconde fois car on l'ouvre avec une taille différente
	police = TTF_OpenFont("fonts/VCR_OSD_MONO_1.001.ttf", 20);
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

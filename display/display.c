// Projet Yoté -- IATIC 3 ISTY
// See the authors in AUTHOR.md

#include "display.h"

// La surface de la fenêtre.
SDL_Surface *fenetre = NULL, *texte = NULL, *rectangle = NULL ;
SDL_Rect position;

TTF_Font *police = NULL;

// Initilise la SDL et charge les images
void initDisplay() {
	couleurNoire = (SDL_Color){0, 0, 0};

	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();

	fenetre = SDL_SetVideoMode(1110, 800, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
	SDL_WM_SetCaption("ISTY - Yoté", NULL);

	/* Chargement de la police */
	police = TTF_OpenFont("fonts/VCR_OSD_MONO_1.001.ttf", 45);

	// Allocation de la surface
	rectangle = SDL_CreateRGBSurface(SDL_HWSURFACE, 500, 180, 32, 0, 0, 0, 0);
	SDL_FillRect(fenetre, NULL, SDL_MapRGB(fenetre->format, 17, 206, 112));

	// Un fond joli
	position.x = 0; // Les coordonnées de la surface seront (0, 0)
	position.y = 0;
	sprites.spriteFond = SDL_LoadBMP("media/sprites/fondjeu.bmp");
	SDL_BlitSurface(sprites.spriteFond, NULL, fenetre, &position);


	//position.x = 310; // Les coordonnées de la surface seront (0, 0)
	//position.y = 5;
	// Remplissage de la surface avec du blanc
	//SDL_FillRect(rectangle, NULL, SDL_MapRGB(fenetre->format, 255, 255, 255));
	//SDL_BlitSurface(rectangle, NULL, fenetre, &position); // Collage de la surface sur l'écran


	sprites.spriteCase = SDL_LoadBMP("media/sprites/spriteCase.bmp");
	sprites.spriteDemon = SDL_LoadBMP("media/sprites/pions/demon.bmp");
	sprites.spriteOrc = SDL_LoadBMP("media/sprites/pions/orc.bmp");
	sprites.spriteNuage = SDL_LoadBMP("media/sprites/nuage.bmp");
	sprites.spriteCarreBleu = SDL_LoadBMP("media/sprites/carre_bleu.bmp");
	sprites.spriteCaisse = SDL_LoadBMP("media/sprites/caisse.bmp");
	sprites.spriteBoutonRetour = SDL_LoadBMP("media/sprites/bouton_retour.bmp");
	SDL_SetColorKey(sprites.spriteDemon, SDL_SRCCOLORKEY, SDL_MapRGB(sprites.spriteDemon->format,255,0,255));
	SDL_SetColorKey(sprites.spriteOrc, SDL_SRCCOLORKEY, SDL_MapRGB(sprites.spriteOrc->format,255,0,255));
	SDL_SetColorKey(sprites.spriteBoutonRetour, SDL_SRCCOLORKEY, SDL_MapRGB(sprites.spriteOrc->format,255,0,255));

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
	eraseWindow();
	displayTitle();
	displayMenuButtons();
	SDL_Flip(fenetre);
}

// Affiche que le plateau
void displayBoard() {
	eraseWindow();
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

// efface le contenu de la fenêtre pour afficher un autre menu/une autre interface
void eraseWindow() {
	SDL_BlitSurface(sprites.spriteFond, NULL, fenetre, &(SDL_Rect){x:0,y:0});
}
// Affiche le tour du joueur courant
void displayRound(raceJoueur joueur) {
	SDL_Rect position;
	char* texteFinal;
	char* texteTour = "Tour des ";
	const char* nomJoueur = joueur == ORC ? "Orcs" : "Démons";
	TTF_Font* police = TTF_OpenFont("fonts/VCR_OSD_MONO_1.001.ttf", 40);

	// On alloue texteFinal
	texteFinal = (char*) malloc((strlen(texteTour) + strlen(nomJoueur) + 2));

	// On recopie la première partie de la chaîne dans texteFinal
	strcpy(texteFinal, texteTour);
	// On concatène texteFinal et nomJoueur
	strcat(texteFinal, nomJoueur);

	texte = TTF_RenderUTF8_Blended(police, texteFinal, couleurNoire);

	position.x = 310;
	position.y = 3;
	SDL_BlitSurface(sprites.spriteNuage, NULL, fenetre, &position);

	position.x = LARGEUR_FENETRE / 2 - (texte->w / 2);
	position.y = 80;
	SDL_BlitSurface(texte, NULL, fenetre, &position);

	SDL_Flip(fenetre);
	TTF_CloseFont(police);

	// On libère la mémoire occupée par texteFinal
	free(texteFinal);
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
	SDL_Flip(fenetre);
}


/* Affiche la réserve: le sprite du pion, accompagné du
nombre de pions restant dans la réserve */
void displayReserve(int nbPions, SDL_Surface* sprite) {
	int x, y; // Coordonnées uitlisées pour dessiner la réserve
	char buffer[5]; // Buffer permettant d'afficher le nombre de pions en chaîne de caractères

	displayReserveBox();
    
	y = ORIGINE_PLATEAU_Y;
	sprintf(buffer, "%d", nbPions); // On met dans le buffer le nombre de pions de la réserve

	// Ouverture de la police
	TTF_Font* police = TTF_OpenFont("fonts/VCR_OSD_MONO_1.001.ttf", 30);
	SDL_Surface* texte = TTF_RenderText_Blended(police, buffer, couleurNoire);

	if (sprite == sprites.spriteOrc) {
		x = MARGE_RESERVE;
		SDL_BlitSurface(sprite, NULL, fenetre, &(SDL_Rect){x, y});

		x += sprite->w + MARGE_RESERVE;
		y += (sprite->h / 2) - (texte->h / 3);
		// Carré bleu utilisé pour masquer le nombre précédent
		SDL_BlitSurface(sprites.spriteCarreBleu, NULL, fenetre, &(SDL_Rect){x, y});
		SDL_BlitSurface(texte, NULL, fenetre, &(SDL_Rect){x, y});
	}
	else{
		x = LARGEUR_FENETRE - sprite->w - MARGE_RESERVE;
		SDL_BlitSurface(sprite, NULL, fenetre, &(SDL_Rect){x, y});

		// Carré bleu utilisé pour masquer le nombre précédent
		x -= sprites.spriteCarreBleu->w + 20;
		y += (sprite->h / 2) - (texte->h / 3);
		SDL_BlitSurface(sprites.spriteCarreBleu, NULL, fenetre, &(SDL_Rect){x, y});

		x += sprites.spriteCarreBleu->w - texte->w;
		SDL_BlitSurface(texte, NULL, fenetre, &(SDL_Rect){x, y});
	}

	// Clôture de la police
	TTF_CloseFont(police);
	SDL_Flip(fenetre);
}

// Affiche le sprite de caisse dans les réserves
void displayReserveBox() {
	SDL_Rect position;
	
    SDL_FillRect(fenetre, &(SDL_Rect){
		x: (sprites.spriteCaisse->w / 2)-5,
		y: ORIGINE_PLATEAU_Y + TAILLE_CASE -5,
		w: 5*2+(sprites.spriteCaisse->w),
		h: 5*2+(sprites.spriteCaisse->h),
	}, SDL_MapRGB(fenetre->format, 148, 172, 206));
	
	SDL_FillRect(fenetre, &(SDL_Rect){
		x: LARGEUR_FENETRE - sprites.spriteCaisse->w - (sprites.spriteCaisse->w / 2) -5,
		y: ORIGINE_PLATEAU_Y + TAILLE_CASE -5,
		w: 5*2+(sprites.spriteCaisse->w),
		h: 5*2+(sprites.spriteCaisse->h),
	}, SDL_MapRGB(fenetre->format, 148, 172, 206));
	
	position.x = (sprites.spriteCaisse->w / 2);
	position.y = ORIGINE_PLATEAU_Y + TAILLE_CASE;
	SDL_BlitSurface(sprites.spriteCaisse, NULL, fenetre, &position);

	position.x = LARGEUR_FENETRE - sprites.spriteCaisse->w - (sprites.spriteCaisse->w / 2);
	SDL_BlitSurface(sprites.spriteCaisse, NULL, fenetre, &position);

	SDL_Flip(fenetre);
}

// Affiche le titre du jeu
void displayTitle() {
	SDL_Rect position;

	SDL_Surface* spriteTitre = SDL_LoadBMP("media/sprites/titre.bmp");
	position.x = (LARGEUR_FENETRE / 2) - (spriteTitre->w / 2);
	position.y = 20;
	SDL_BlitSurface(spriteTitre, NULL, fenetre, &position);

	SDL_Surface* edition = SDL_LoadBMP("media/sprites/orcVSdemon.bmp");
	position.x = (LARGEUR_FENETRE / 2) - (edition->w / 2);
	position.y = spriteTitre->h + 25;
	SDL_BlitSurface(edition, NULL, fenetre, &position);
	SDL_Flip(fenetre);
}


// Affiche les boutons du menu du jeu
void displayMenuButtons() {
	SDL_Surface* texte;
	SDL_Rect position;

	TTF_Font* police = TTF_OpenFont("fonts/VCR_OSD_MONO_1.001.ttf", 35);
	SDL_Surface* petitNuage = SDL_LoadBMP("media/sprites/petit_nuage.bmp");


	// Bouton Jouer
	position.x = (LARGEUR_FENETRE / 2) - petitNuage->w - 50;
	position.y = ORIGINE_PLATEAU_Y + 145;

	SDL_BlitSurface(petitNuage, NULL, fenetre, &position);
	texte = TTF_RenderText_Blended(police, "Jouer", couleurNoire);
	SDL_BlitSurface(texte, NULL, fenetre, &(SDL_Rect){
		x: position.x + (petitNuage->w  / 3),
		y: position.y + texte->h + 5
	});


	// Bouton Quitter
	position.y = ORIGINE_PLATEAU_Y + petitNuage->h + 205;
	SDL_BlitSurface(petitNuage, NULL, fenetre, &position);
	texte = TTF_RenderText_Blended(police, "Quitter", couleurNoire);
	SDL_BlitSurface(texte, NULL, fenetre, &(SDL_Rect){
		x: position.x + (petitNuage->w  / 4),
		y: position.y + texte->h + 5
	});


	// Bouton Aide
	position.x = (LARGEUR_FENETRE / 2) + 50;
	position.y = ORIGINE_PLATEAU_Y + 145;

	SDL_BlitSurface(petitNuage, NULL, fenetre, &position);
	texte = TTF_RenderText_Blended(police, "Aide", couleurNoire);
	SDL_BlitSurface(texte, NULL, fenetre, &(SDL_Rect){
		x: position.x + texte->w,
		y: position.y + texte->h + 5
	});


	// Bouton Scores
	position.y = ORIGINE_PLATEAU_Y + petitNuage->h + 205;
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
	eraseWindow();
	SDL_Rect position; // Position des différents élements de la fenêtre
	char texte[TAILLE_MAX_REGLES] = ""; // Tampon pour le fichier media/regles.txt

	TTF_Font* police = TTF_OpenFont("fonts/VCR_OSD_MONO_1.001.ttf", 50);
	SDL_Surface* affichageTexte = NULL;
	FILE* fichierRegles = NULL;

	// Titre
	position.x = 310;
	position.y = 3;
	SDL_BlitSurface(sprites.spriteNuage, NULL, fenetre, &position);

	affichageTexte = TTF_RenderUTF8_Blended(police, "Règles", couleurNoire);
	position.x = (LARGEUR_FENETRE / 2) - (affichageTexte->w / 2);
	position.y = (sprites.spriteNuage->h / 2) - (affichageTexte->h / 4);
	SDL_BlitSurface(affichageTexte, NULL, fenetre, &position);

	TTF_CloseFont(police);

	// Affichage des règles
	police = TTF_OpenFont("fonts/VCR_OSD_MONO_1.001.ttf", 18);
	position.y = sprites.spriteNuage->h + 50;
	fichierRegles = fopen("texts/regles.txt", "r");

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

	displayBackButton();
	SDL_Flip(fenetre);
	TTF_CloseFont(police);
}

// Affiche un bouton retour sur les pages de règles et de scores
void displayBackButton() {
	SDL_Rect position;
	SDL_Surface* affichageTexte = NULL;
	TTF_Font* police;

	police = TTF_OpenFont("fonts/VCR_OSD_MONO_1.001.ttf", 22);

	position.x = 20;
	position.y = HAUTEUR_FENETRE - 20 - sprites.spriteBoutonRetour->h;
	SDL_BlitSurface(sprites.spriteBoutonRetour, NULL, fenetre, &position);
	affichageTexte = TTF_RenderText_Blended(police, "Retour", couleurNoire);

	position.x += (sprites.spriteBoutonRetour->w / 3);
	position.y += (sprites.spriteBoutonRetour->h / 4);
	SDL_BlitSurface(affichageTexte, NULL, fenetre, &position);

	TTF_CloseFont(police);
}

void displayGamemodeChoice() {
	eraseWindow();
	SDL_Rect position;

	TTF_Font* police = TTF_OpenFont("fonts/VCR_OSD_MONO_1.001.ttf", 40);
	SDL_Surface* affichageTexte = TTF_RenderUTF8_Blended(police, "Mode de jeu", couleurNoire);
	SDL_Surface* petitNuage = SDL_LoadBMP("media/sprites/petit_nuage.bmp");

	// Titre
	position.x = 310;
	position.y = 3;
	SDL_BlitSurface(sprites.spriteNuage, NULL, fenetre, &position);
	position.x = (LARGEUR_FENETRE / 2) - (affichageTexte->w / 2);
	position.y = (sprites.spriteNuage->h / 2) - (affichageTexte->h / 4);
	SDL_BlitSurface(affichageTexte, NULL, fenetre, &position);
	TTF_CloseFont(police);

	// Boutons
	police = TTF_OpenFont("fonts/VCR_OSD_MONO_1.001.ttf", 35);
	position.x = (LARGEUR_FENETRE / 4) - (petitNuage->w / 2);
	position.y = (HAUTEUR_FENETRE / 2) - (int)(1.5 * petitNuage->h);
	SDL_BlitSurface(petitNuage, NULL, fenetre, &position);
	position.y = (HAUTEUR_FENETRE / 2) + (int)(1.5 * petitNuage->h);
	SDL_BlitSurface(petitNuage, NULL, fenetre, &position);

	// Texte bouton 1
	affichageTexte = TTF_RenderUTF8_Blended(police, "1", couleurNoire);
	position.x = (LARGEUR_FENETRE / 4) - (affichageTexte->w / 3);
	position.y = (HAUTEUR_FENETRE / 2) - (int)(1.5 * petitNuage->h) + (petitNuage->h / 2) - 5;
	SDL_BlitSurface(affichageTexte, NULL, fenetre, &position);

	affichageTexte = TTF_RenderUTF8_Blended(police, "Joueur contre Joueur", couleurNoire);
	SDL_BlitSurface(affichageTexte, NULL, fenetre, &(SDL_Rect) {
		x: position.x + 200,
		y: position.y
	});

	// Texte bouton 2
	affichageTexte = TTF_RenderUTF8_Blended(police, "2", couleurNoire);
	position.y = (HAUTEUR_FENETRE / 2) + (int)(1.5 * petitNuage->h) + (petitNuage->h / 2) - 5;
	SDL_BlitSurface(affichageTexte, NULL, fenetre, &position);

	affichageTexte = TTF_RenderUTF8_Blended(police, "Joueur contre IA", couleurNoire);
	SDL_BlitSurface(affichageTexte, NULL, fenetre, &(SDL_Rect) {
		x: position.x + 200,
		y: position.y
	});

	TTF_CloseFont(police);
	displayBackButton();
	SDL_Flip(fenetre);
}

void surbrillanceCaisse(raceJoueur race){	
	if(race== DEMON)
	{		
		surbrillanceCaisse2();	
	}
	else
	{
		surbrillanceCaisse1();
    }
}

void surbrillanceCaisse1()
{
	SDL_Rect position;

	position.x = (sprites.spriteCaisse->w / 2);
		

    SDL_FillRect(fenetre, &(SDL_Rect){
		x: (sprites.spriteCaisse->w / 2)-5,
		y: ORIGINE_PLATEAU_Y + TAILLE_CASE -5,
		w: 5*2+(sprites.spriteCaisse->w),
		h: 5*2+(sprites.spriteCaisse->h),
	}, SDL_MapRGB(fenetre->format, 17, 206, 112));
	
	
	position.x = (sprites.spriteCaisse->w / 2);
	position.y = ORIGINE_PLATEAU_Y + TAILLE_CASE;
	SDL_BlitSurface(sprites.spriteCaisse, NULL, fenetre, &position);


	SDL_Flip(fenetre);
}

void surbrillanceCaisse2(void) {
	SDL_Rect position;

	SDL_FillRect(fenetre, &(SDL_Rect){
		x: LARGEUR_FENETRE - sprites.spriteCaisse->w - (sprites.spriteCaisse->w / 2) -5,
		y: ORIGINE_PLATEAU_Y + TAILLE_CASE -5,
		w: 5*2+(sprites.spriteCaisse->w),
		h: 5*2+(sprites.spriteCaisse->h),
	}, SDL_MapRGB(fenetre->format, 17, 206, 112));

	position.x = LARGEUR_FENETRE - sprites.spriteCaisse->w - (sprites.spriteCaisse->w / 2);
	position.y = ORIGINE_PLATEAU_Y + TAILLE_CASE;
	SDL_BlitSurface(sprites.spriteCaisse, NULL, fenetre, &position);

	SDL_Flip(fenetre);
}

void displayWinner(raceJoueur race) {
	eraseWindow();

	SDL_Rect position;
	char* texte = race == ORC ? "L\'orc a gagné !" : "Le démon a gagné !";
	TTF_Font* police = TTF_OpenFont("fonts/VCR_OSD_MONO_1.001.ttf", 40);
	SDL_Surface* affichageTexte = TTF_RenderUTF8_Blended(police, texte, couleurNoire);
	SDL_Surface* sprite = race == ORC ? sprites.spriteOrc : sprites.spriteDemon;


	position.x = (LARGEUR_FENETRE / 2) - (affichageTexte->w / 2);
	position.y = (HAUTEUR_FENETRE / 3) - (affichageTexte->h / 2);
	SDL_BlitSurface(affichageTexte, NULL, fenetre, &position);

	position.x = (LARGEUR_FENETRE / 2) - (sprite->w / 2);
	position.y = (HAUTEUR_FENETRE / 2) - (sprite->h / 2);
	SDL_BlitSurface(sprite, NULL, fenetre, &position);

	TTF_CloseFont(police);
	SDL_Flip(fenetre);
}

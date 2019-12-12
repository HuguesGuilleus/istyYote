// Projet Yoté -- IATIC 3 ISTY
// See the authors in AUTHOR.md

#include "control.h"

// Joue une partie
void partie(void) {
	SDL_Event event;
	coord depart = {} ;
	raceJoueur gamer ;

	//gamerRand(&gamer);
	
	printf("Couleur premier joueur: ");
	gamerPrint(gamer);

	display();





	int joueur=ORC;
	int continuer=1;
	while(continuer==1){ 
		//recuepere l'action de jeu a effectuer
		int action;
		coord CordPion;

		printf("joueur:%d \n",joueur);

		action=ActionJoueur(joueur,&depart,&continuer);
		printf("continuer =%d\n",continuer);

		//action : placement
		if (action==RESERVE){
			CordPion=placement(joueur);
		}


		if(joueur==ORC){
			joueur=DEMON;
		}
		else{
			joueur=ORC;
		}

		affiche_plateau();
		printf("a l'autre\n");

	}
	printf("finiiii\n");
	return;


	display();
	affiche_plateau();
	printf("c'est cool\n");

	while (TRUE) {
		SDL_WaitEvent(&event);
		switch (event.type) {
			case SDL_QUIT:
				return;
			case SDL_MOUSEBUTTONDOWN:
				// TODO: Faire un clic sur un bouton qui quitte la partie
				// TODO: Faire un placement de nouveau pion
				if (clickOnPiece(&event, &depart, gamer)) {
					// board[depart.x][depart.y].race = gamer ;
					board[depart.x][depart.y].status = SELECTED ;
					displayBoard(TRUE);
				}
				break;
		}
	}
}



// Retourne TRUE si le click est sur un pion du joueur courant.
// Si c'est le cas, la case sera enregistrée dans le pointeur de c.
bool clickOnPiece(SDL_Event *click, coord * c, raceJoueur gamer) {
	if (clickToCoord(click, c) == FALSE) {
		return FALSE ;
	}
	if (board[c->x][c->y].race == gamer) {
		return TRUE ;
	} else {
		return FALSE ;
	}
}


// Converti les coordondées d'un clic en coordonées du tableau.
// Si le clic n'est pas dans le plateau, retourne FALSE.
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
void gamerRand(raceJoueur * gamer) {
	*gamer = rand()%2 ? ORC : DEMON ;
}

// Affiche la couleur d'un joueur.
void gamerPrint(raceJoueur gamer) {
	switch (gamer) {
		case VIDE: printf("VIDE\n"); break;
		case ORC: printf("ORC\n"); break;
		case DEMON: printf("DEMON\n"); break;
	}
}

// Change de couleur de joueur entre blanc et noir.
void gamerSwitch(raceJoueur * gamer) {
	* gamer = *gamer == ORC ? DEMON : ORC ;
}



//Détermine si le joueur veut placer un pion ou déplacer un pion en fonction du premier clic
//Retourne l'action
int ActionJoueur(raceJoueur joueur,coord *c,int *continuer){
	int Clic1;
	SDL_Event event;
	printf(" atoi %d\n",joueur);
	while (*continuer)
	{
    	SDL_WaitEvent(&event);
		switch(event.type)
		{
			case SDL_QUIT:
				*continuer=0;
				printf("fini");
				break;
			case SDL_MOUSEBUTTONUP:

				//On regarde si le clic 1 est dans le plateau ou la reserve
				printf(" atoi %d\n",joueur);
				Clic1= verifClic1(event.button.x,event.button.y,joueur);
				if (Clic1==RESERVE){
					//renvoie l'action a faire qui est : placement d'un pion
					return RESERVE;
				}
				else if(Clic1==PLATEAU){
					//renvoie l'action a faire qui est : deplacement ou capture
					//TO DO : attribuer la coordonner en fonction de la valeur de event.button.x et event.button.y
					return PLATEAU;
				}
		}
	}
}

//on verifie que le clic est autoriser
//renvoie l'action qui sera effectuer au clic2 ( placement ou deplacement/capture)
int verifClic1 (int x, int y, raceJoueur joueur){

	if(joueur==ORC){
		//reserve du joueur 1
		if((x>TAILLE_CASE)&&(x<3*TAILLE_CASE)&&(y>4*TAILLE_CASE)&&(y<9*TAILLE_CASE)){
			printf("reserve J1\n");
			return RESERVE;
		}
	}
	else if(joueur==DEMON){
		//reserve du joueur 1
		printf(" ta reserve demon\n");
		if((x>11*TAILLE_CASE)&&(x<13*TAILLE_CASE)&&(y>4*TAILLE_CASE)&&(y<9*TAILLE_CASE)){
			printf("reserve J2\n");
			return RESERVE;
		}
	}
	else if((x>4*TAILLE_CASE)&&(x<10*TAILLE_CASE)&&(y>4*TAILLE_CASE)&&(y<9*TAILLE_CASE)&&(board[x/TAILLE_CASE][y/TAILLE_CASE].race==joueur)){
		printf("plateau\n");
		return PLATEAU;
	}

}

bool verifClic2Placement(int x, int y)
{
	if((x>4*TAILLE_CASE)&&(x<10*TAILLE_CASE)&&(y>4*TAILLE_CASE)&&(y<9*TAILLE_CASE)&&(board[(x/TAILLE_CASE)-4][(y/TAILLE_CASE)-4].race==VIDE)){
		return TRUE;
	}
	else{
		return FALSE;
	}
}


coord placement(raceJoueur joueur)
{
	coord c;
	bool Clic2;
	int continuer=1;

	SDL_Event event;
	while (continuer)
	{
    	SDL_WaitEvent(&event);
		switch(event.type)
		{
			case SDL_QUIT:
				continuer=0;
				printf("fini");
				break;
			case SDL_MOUSEBUTTONUP:

				//On regarde si le clic 1 est dans le plateau ou la reserve
				Clic2= verifClic2Placement(event.button.x,event.button.y);
				if(Clic2==TRUE){
					printf("je place le pion en %d, %d \n",(event.button.x/TAILLE_CASE)-4,(event.button.y/TAILLE_CASE)-4);
					c.x=event.button.x;
					c.y=event.button.y;
					
					board[c.x/TAILLE_CASE-4][c.y/TAILLE_CASE-4].race=joueur;
					return c;
				}
		}
	}
	return c;
}

coord deplacement(raceJoueur joueur)
{
	coord c;
	bool Clic2;
	int continuer=1;

	SDL_Event event;
	while (continuer)
	{
    	SDL_WaitEvent(&event);
		switch(event.type)
		{
			case SDL_QUIT:
				continuer=0;
				printf("fini");
				break;
			case SDL_MOUSEBUTTONUP:

				//On regarde si le clic 1 est dans le plateau ou la reserve
				Clic2= verifClic2Placement(event.button.x,event.button.y);
				if(Clic2==TRUE){
					printf("je place le pion en %d, %d \n",(event.button.x/TAILLE_CASE)-4,(event.button.y/TAILLE_CASE)-4);
					c.x=event.button.x;
					c.y=event.button.y;
					
					board[c.x/TAILLE_CASE-4][c.y/TAILLE_CASE-4].race=joueur;
					return c;
				}
		}
	}
	return c;
}

void affiche_plateau(){
	int i,j;
	for(i=0;i<5;i++){
		for(j=0;j<6;j++){
			printf(" %d ",board[j][i].race);
		}
		printf("\n");
	}
	printf("finiiii\n");
}

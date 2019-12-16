// Projet Yoté -- IATIC 3 ISTY
// See the authors in AUTHOR.md

#include "control.h"

void partie2(void) {
	typeHandlers h = {} ;
	// Indique si l'utilisateur veut placer un pion.
	bool reserve = FALSE ;
	// Indique si une case est déjà séléctionnée.
	bool selected = FALSE ;
	// Le permier click
	coord firstClick ;

	// Le joueur courant séléctionne sa réserve pour poser un pion.
	void activeReserve(SDL_Event click) {
		reserve = TRUE ;
		selected = FALSE ;
		if (selected) {
			board[firstClick.x][firstClick.y].status = DEFAULT ;
			displayBoard(TRUE);
		}
	}
	handlersAdd(&h, SDL_MOUSEBUTTONDOWN, &filterReserve, &activeReserve);

	// Le joueur courant séléctionne un de ses pions pour un déplacement.
	void selectPiece(SDL_Event click) {
		reserve = FALSE ;
		if (selected) {
			board[firstClick.x][firstClick.y].status = DEFAULT ;
		}
		selected = TRUE ;
		firstClick = clickToCoord(click);
		board[firstClick.x][firstClick.y].status = SELECTED ;
		displayBoard(TRUE);
	}
	handlersAdd(&h, SDL_MOUSEBUTTONDOWN, &filterTile, &selectPiece);

	// Le joueur va déplacer un pion déjà présent ou bien poser un nouveau pion.
	void cliquer(SDL_Event click) {
		if (reserve) {
			firstClick = clickToCoord(click);
			board[firstClick.x][firstClick.y].race = currentParty.joueur ;
			gamerDecreaseReserve();
			gamerSwitch();
			display();
			reserve = FALSE ;
		} else if (selected) {
			board[firstClick.x][firstClick.y].race = VIDE ;
			board[firstClick.x][firstClick.y].status = DEFAULT ;
			firstClick = clickToCoord(click);
			board[firstClick.x][firstClick.y].race = currentParty.joueur ;
			gamerSwitch();
			selected = FALSE ;
			display();
		}
	}
	handlersAdd(&h, SDL_MOUSEBUTTONDOWN, &filterTileEmpty, &cliquer);

	handlersAdd(&h, SDL_QUIT, &filterTrue, &end);
	initPartie();
	display();
	handlersRun(&h);
	handlersFree(&h);
}



// Joue une partie
void partie(void) {
	SDL_Event event;
	coord depart = {} ;
	raceJoueur gamer ;

	// gamerRand(&gamer);

	printf("Couleur premier joueur: ");
	gamerPrint(gamer);

	initPartie();
	display();

	coord c1 = {};
	int joueur = ORC;
	int continuer=1;
	while(continuer==1){
		// recupère l'action de jeu à effectuer
		int action;
		coord CordPion;

		printf("joueur:\n");
		gamerPrint(joueur);

		action = ActionJoueur(joueur, &c1, &continuer);
		// action: placement
		if (action==RESERVE){
			printf("placement\n");
			CordPion=placement(joueur);
			// displayPawn(sprites.spriteOrc, CordPion.x, CordPion.y);
		}
		else if(action==PLATEAU){
			printf(" deplacement\n");
			CordPion=deplacement(joueur,c1);
		}

		// changement de joueur
		if(joueur==ORC){
			joueur = DEMON;
		} else{
			joueur = ORC;
		}

		displayBoard(TRUE);
		printf("a l'autre\n");
	}
	printf("finiiii\n");
	return ;
}

// Affiche les coordonées dans la console d'un clic.
void printCoord(coord * c) {
	if (c == NULL) {
		printf("(null)\n");
	} else {
		printf("(%d,%d)\n", c->x, c->y);
	}
}

//Détermine si le joueur veut placer un pion ou déplacer un pion en fonction du premier clic
//Retourne l'action
int ActionJoueur(raceJoueur joueur, coord *c, int *continuer) {
	int clic1;
	SDL_Event event;
	printf(" à toi %d\n", joueur);
	while (*continuer) {
		SDL_WaitEvent(&event);
		switch(event.type) {
			case SDL_QUIT:
				*continuer=0;
				printf("fini");
				break;
			case SDL_MOUSEBUTTONUP:
				// On regarde si le clic 1 est dans le plateau ou la reserve
				printf(" à toi %d\n", joueur);
				clic1 = verifClic1(event, joueur);
				if (clic1==RESERVE){
					//renvoie l'action à faire qui est: placement d'un pion
					return RESERVE;
				} else if(clic1==PLATEAU) {
					//renvoie l'action à faire qui est: deplacement ou capture
					*c = clickToCoord(event);
					return PLATEAU;
				}
		}
	}
}

// On verifie que le clic est autorisée.
// Renvoie l'action qui sera effectuer au clic2 ( placement ou deplacement/capture)
int verifClic1(SDL_Event event, raceJoueur joueur) {
	coord c ;
	if (filterReserveWithJoueur(event, joueur)) {
		return RESERVE ;
	}
	if (filterInBoard(event)) {
		c = clickToCoord(event);
		if (board[c.x][c.y].race == joueur) {
			return PLATEAU;
		}
	}
	return NOTHING;

	// printf("\n\n\n%d %d\n",x/TAILLE_CASE-5,y/TAILLE_CASE-4);
	// printf("tableua = %d   joeur == %d \n",board[x/TAILLE_CASE-5][y/TAILLE_CASE-4].race,joueur);
	// if(joueur==ORC){
	// 	//reserve du joueur orc
	// 	if((x>TAILLE_CASE)&&(x<3*TAILLE_CASE)&&(y>4*TAILLE_CASE)&&(y<9*TAILLE_CASE)){
	// 		printf("reserve J1\n");
	// 		return RESERVE;
	// 	}
	// }
	// else if(joueur==DEMON){
	// 	//reserve du joueur demon
	// 	if((x>13*TAILLE_CASE)&&(x<15*TAILLE_CASE)&&(y>4*TAILLE_CASE)&&(y<9*TAILLE_CASE)){
	// 		printf("reserve J2\n");
	// 		return RESERVE;
	// 	}
	// }
	//
	// if((x>5*TAILLE_CASE)&&(x<11*TAILLE_CASE)&&(y>4*TAILLE_CASE)&&(y<9*TAILLE_CASE)&&(board[x/TAILLE_CASE-5][y/TAILLE_CASE-4].race)==joueur){
	// 	printf("plateau\n");
	// 	return PLATEAU;
	// }
}

coord placement(raceJoueur joueur) {
	coord c;
	bool clic2;
	bool continuer = TRUE ;
	SDL_Event event;

	while (continuer) {
		SDL_WaitEvent(&event);
		switch(event.type) {
			case SDL_QUIT:
				continuer = FALSE ;
				printf("fini");
				break;
			case SDL_MOUSEBUTTONUP:
				// On regarde si le clic 2 est bien dans le plateau
				// clic2= verifClic2Placement(event.button.x,event.button.y);
				clic2 = filterTileEmpty(event);
				if(clic2==TRUE){
					c = clickToCoord(event);
					printf("je place le pion en %d, %d \n", c.x, c.y);
					board[c.x][c.y].race = joueur;
					return c;
				}
		}
	}
	return c;
}

bool verifClic2Placement(int x, int y) {
	if(
		(x>5*TAILLE_CASE)&&(x<11*TAILLE_CASE)&&(y>4*TAILLE_CASE)
		&&(y<9*TAILLE_CASE)
		&&(board[(x/TAILLE_CASE)-4][(y/TAILLE_CASE)-4].race==VIDE)){
		return TRUE;
	}
	else{
		return FALSE;
	}
}

coord deplacement(raceJoueur joueur, coord c1) {
	coord c2;
	bool clic2;
	bool continuer = TRUE;
	SDL_Event event;

	while (continuer) {
		SDL_WaitEvent(&event);
		switch(event.type) {
			case SDL_QUIT:
				continuer = FALSE ;
				break;
			case SDL_MOUSEBUTTONUP:
				if (!filterInBoard(event)) {
					printf("Click hors du plateau\n");
					break ;
				}
				c2 = clickToCoord(event);
				//On regarde si le clic 2 est bien dans le plateau, a une case de distance 1 et vide
				clic2=verifClic2Deplacement(c1, c2);
				if(clic2==TRUE){
					printf("je deplace le pion en (%d,%d)\n", c2.x, c2.y);
					board[c1.x][c1.y].race = VIDE;
					board[c2.x][c2.y].race = joueur;
					return c2;
				}
		}
	}
	return c2;
}

// On regarde si le clic 2 est bien dans le plateau, a une case de distance 1
// et vide.
bool verifClic2Deplacement(coord c1, coord c2) {
	int departX = c1.x ;
	int departY = c1.y ;
	int arriveX = c2.x ;
	int arriveY = c2.y ;

	printf("verifions que tu peux allez ici :    %d %d\n", arriveX, arriveY);
	printf("tu etais ici :    %d %d\n", departX, departY);
	// verifie que le clic est dans le plateau et que la case et vide
	if(board[arriveX][arriveY].race==VIDE){
		// verifie que la case est à une distance de 1 du pion
		if( ((arriveX==departX+1) && (arriveY==departY))
			|| ((arriveX==departX-1) && (arriveY==departY))
			|| ((arriveX==departX) && (arriveY==departY+1))
			|| ((arriveX==departX) && (arriveY==departY-1))
		){
			printf("c'est possible de se deplacer !\n");
			return TRUE;
		}
	}
	return FALSE;
}

void affiche_plateau(){
	int i,j;
	printf("\n");
	for(i=0;i<5;i++){
		for(j=0;j<6;j++){
			printf(" %d ",board[j][i].race);
		}
		printf("\n");
	}
	printf("\n");
}

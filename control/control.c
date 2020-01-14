// Projet Yoté -- IATIC 3 ISTY
// See the authors in AUTHOR.md

#include "control.h"

// Joue une partie
void partie(void) {
	// Saisit le nom des joueur.
	// À l'heure actuelle, cela ne sert à rien.
	//scoreParty infoJoueur = scoreInput();
	
	//affichage de la fenetre de jeu
	// display();

	initBoard();
	scoreParty infoJoueur = scoreInput();
	displayBoard();

	//declaration des variables
	coord c1 = {};
	coord CordPion1Cap,CordPion2Cap;
	raceJoueur raceDebut;
	Joueur joueurOrc,joueurDemon,joueur;
	int continuer=1;
	int capture=0;

	//initialisation des 2 joueurs
	joueurOrc.race=ORC;
	joueurOrc.reserve=12;
	joueurOrc.plateau=0;
	joueurOrc.cAnc.x=-1;
	joueurOrc.cAnc.y=-1;
	
	joueurDemon.race=DEMON;
	joueurDemon.reserve=12;
	joueurDemon.plateau=0;
	joueurDemon.cAnc.x=-1;
	joueurDemon.cAnc.y=-1;

	raceDebut=joueurAleatoir(raceDebut);

	if(raceDebut==ORC){
		joueur=joueurDemon;
		}
	else{
		joueur=joueurOrc;
	}
	//initialisation a des valeur hors du plateau
	joueur.cAnc.x=-1;
	joueur.cAnc.y=-1;

	//affichage des nombre de piosn contenue dans les reserves au debut de partie
	displayReserve(joueur.reserve,sprites.spriteOrc);
	displayReserve(joueur.reserve,sprites.spriteDemon);
	
	//affichage des 2 caisses de rerve
	displayReserveBox();

	while(continuer==1){

		int action;
		// CordPionNouv est les coord qu'on souhaite faire et CordPionAnc est les coord qu'on avait fait au tour precedent
		coord CordPionNouv, CordPionAnc; 

		//affiche le joueur courant
		displayRound(joueur.race);

		//recupère l'action de jeu à effectuer par le clic
		action=ActionJoueur(&joueur,&c1,&continuer);
		printf("debut de tour : anncienne position %d %d \n",joueur.cAnc.x,joueur.cAnc.y);
		//action : placement
		if (action==RESERVE){

			CordPionNouv=placement(&joueur);
			joueur.plateau=joueur.plateau +1;
			if(joueur.race==ORC){
				//affiche pion orc et diminu de 1 la reserve
				displayPawn(sprites.spriteOrc,CordPionNouv.x, CordPionNouv.y);
				// on met des valeur hors du plateau en ancien coup pour que quelque soit la case selectionné elle soit differente
				joueurOrc.plateau=joueur.plateau;
				joueurOrc.reserve=joueur.reserve;
			}

			else{
				//affiche pion demon et diminue de 1 la reserve
				displayPawn(sprites.spriteDemon,CordPionNouv.x, CordPionNouv.y);
				// on met des valeur hors du plateau en ancien coup pour que quelque soit la case selectionné elle soit differente
				joueurDemon.plateau=joueur.plateau;
				joueurDemon.reserve=joueur.reserve;
			}
			joueur.cAnc.x=-1;
			joueur.cAnc.y=-1;
			printf("nb plateau : %d \n",joueur.plateau);
		}
		
		//action : déplacement
		else if(action==PLATEAU){

			CordPionNouv=deplacement(joueur,c1,joueur.cAnc,&capture);

			printf(" capture : %d\n",capture);

			//si le joueur capture un pion dans son deplacement
			if (capture==1){
				
				displayTile(c1.x/TAILLE_CASE-4, c1.y/TAILLE_CASE-3);
				//si le joueur courant et un orc
				if(joueur.race==ORC){
					//affiche pion orc a la nouvelle position
					displayPawn(sprites.spriteOrc,CordPionNouv.x, CordPionNouv.y);
					//on diminue de un le nb de pion sur le plateau du joeur adverse
					joueurDemon.plateau=joueurDemon.plateau-1;
					printf("nb plateau trtr : %d \n",joueurDemon.plateau);
					//si le joueur a plus de pion sur le plateau un pion de la reserve et prix
					if (joueurDemon.plateau==0)
					{
						joueurDemon.reserve=joueurDemon.reserve-1;
						displayReserve(joueur.reserve,sprites.spriteDemon);
					}
					//sinon le joueur choisi un pion sur le plateau qui est capturer
					else{
						CordPion2Cap=capture2(joueur);
						displayTile(CordPion2Cap.x, CordPion2Cap.y);
						joueurDemon.plateau=joueurDemon.plateau-1;
					}
					//sauvegarde de la position de départ avant le mouvement
					//joueurOrc.cAnc=c1;
				}
				//si le joueur courant est un demon
				else{
					//affiche pion demon a la nouvelle position
					displayPawn(sprites.spriteDemon,CordPionNouv.x, CordPionNouv.y);
					joueurOrc.plateau=joueurOrc.plateau-1;
					if (joueurOrc.plateau==0)
					{
						joueurOrc.reserve=joueurOrc.reserve-1;
						displayReserve(joueur.reserve,sprites.spriteOrc);
					}
					else{
						CordPion2Cap=capture2(joueur);
						displayTile(CordPion2Cap.x, CordPion2Cap.y);
						joueurOrc.plateau=joueurOrc.plateau-1;
					}
					//sauvegarde de la position de départ avant le mouvement
					//joueurDemon.cAnc=c1;
				}
				//la capture est fini on remet donc capture a FAUX (0)
				capture =0;
			}
			//sinon deplacement normal
			else {
					//affiche une case a l'ancienne position du pion pour l'effacer
				displayTile(c1.x/TAILLE_CASE-4, c1.y/TAILLE_CASE-3);
				if(joueur.race==ORC){
					//affiche pion orc a la nouvelle position
					displayPawn(sprites.spriteOrc,CordPionNouv.x, CordPionNouv.y);
				}
				else{
					//affiche pion demon a la nouvelle position
					displayPawn(sprites.spriteDemon,CordPionNouv.x, CordPionNouv.y);
				}
				
			}
			//on enregistre la position de depart pour que le joueur ne puisse pas faire un coup inverse a celui ci au prochain tour
			joueur.cAnc.x=c1.x/TAILLE_CASE-4;
			joueur.cAnc.y=c1.y/TAILLE_CASE-3;
		}
		printf("fin de tour : anncienne position orc   %d %d \n",joueurOrc.cAnc.x,joueurOrc.cAnc.y);
		printf("fin de tour : anncienne position demon %d %d \n",joueurDemon.cAnc.x,joueurDemon.cAnc.y);
		//affichage de la réserve
		if(joueur.race==ORC){
			displayReserve(joueurOrc.reserve,sprites.spriteOrc);
			}
		else{
			displayReserve(joueurDemon.reserve,sprites.spriteDemon);
		}
		//changement de joueur
		if(joueur.race==ORC){
			joueurOrc.cAnc=joueur.cAnc;
			joueur=joueurDemon;
		
		}
		else{
			joueurDemon.cAnc=joueur.cAnc;
			joueur=joueurOrc;
		}
		if((joueur.plateau==0) && (joueur.reserve==0)){
			SDL_Delay(1000);
			printf("\n\n\n JOUEUR %d A PERDU\n", joueur.race);
			continuer=0;
		}
		
		//permet d'afficher le contenue du tableau du plateau dans le terminal
		//affiche_plateau();
	}
	display();
	menuClick();
	return;
}

raceJoueur joueurAleatoir(raceJoueur race) {
	return rand()%2 ? DEMON : ORC ;
}

//Détermine si le joueur veut placer un pion ou déplacer un pion en fonction du premier clic
//Retourne l'action
int ActionJoueur(Joueur* joueur,coord *c,int *continuer){
	int Clic1;
	SDL_Event event;

	while (*continuer)
	{
    	SDL_WaitEvent(&event);
		switch(event.type)
		{
			case SDL_QUIT:
				*continuer=0;
				break;
			case SDL_MOUSEBUTTONUP:

				//On regarde si le clic 1 est dans le plateau ou dans la reserve
				Clic1= verifClic1(event.button.x,event.button.y,*joueur);
				if ((Clic1==RESERVE)&&(joueur->reserve!=0)){
					//renvoie l'action a faire qui est : placement d'un pion

					return RESERVE;
				}
				else if(Clic1==PLATEAU){
					//renvoie l'action a faire qui est : deplacement (le joeur ne peut pas encore capturer)
					//donc deplacement se fera vers une case vide
					c->x=event.button.x;
					c->y=event.button.y;
					return PLATEAU;
				}
		}
	}
}

//on verifie que le clic est autoriser
//renvoie l'action qui sera effectué au clic2 (placement ou deplacement)
int verifClic1 (int x, int y, Joueur joueur){
	if(joueur.race==ORC){
		//si clic dans reserve lorsque le joueur est Orc
		if((x>62)&&(x<187)&&(y>320)&&(y<445)){
			return RESERVE;
		}
	}
	else if(joueur.race==DEMON){
		//si clic dans reserve lorsque le joueur est Demon
		if((x>923)&&(x<1048)&&(y>320)&&(y<445)){
			return RESERVE;
		}
	}

	if((x>4*TAILLE_CASE)&&(x<11*TAILLE_CASE)&&(y>3*TAILLE_CASE)&&(y<9*TAILLE_CASE)&&(board[x/TAILLE_CASE-4][y/TAILLE_CASE-3].race)==joueur.race){
		//test si le clic est sur une case avec un des pions du joueur dans le plateau
		return PLATEAU;
	}
}

//verifie que le clic 2 suite a un clic dans la reserve et correcte renvoie un booleen
bool verifClic2Placement(int x, int y)
{
	// la case selectionner doit être une case vide du plateau
	if((x>4*TAILLE_CASE)&&(x<10*TAILLE_CASE)&&(y>3*TAILLE_CASE)&&(y<8*TAILLE_CASE)&&(board[(x/TAILLE_CASE)-4][(y/TAILLE_CASE)-3].race==VIDE)){
		return TRUE;
	}
	else{
		return FALSE;
	}
}

//suite a un clic 1 de placement, on attend un second clic tant qu'il n'est pas valide ou
//que l'utilisateur n'appuit pas sur la croix on attend un clic
coord placement(Joueur* joueur)
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
				break;
			case SDL_MOUSEBUTTONUP:

				//On regarde si le clic 2 est bien dans le plateau
				Clic2= verifClic2Placement(event.button.x,event.button.y);
				if(Clic2==TRUE){

					c.x=(event.button.x/TAILLE_CASE)-4;
					c.y=(event.button.y/TAILLE_CASE)-3;
					// on place un pion dans la case selectionnée au clic 2
					board[c.x][c.y].race=joueur->race;
					//on diminue de 1 la reserve
					joueur->reserve =joueur->reserve-1;
					return c;
				}
		}
	}
	return c;
}

//verifie que le clic2 pour une action de deplacement est possible ou non
bool verifClic2Deplacement(int x,int y,coord c1,int * capture, Joueur joueur){
	//variables des cases d'arrivée et de depart pour le deplacement
	//on convertit un clic en case du plateau
	int arriveX = x/TAILLE_CASE-4;
	int arriveY = y/TAILLE_CASE-3;
	int departX=  c1.x/TAILLE_CASE-4;
	int departY= c1.y/TAILLE_CASE-3;

	//verifie que le clic est dans le plateau et que la case et vide
	if((x>4*TAILLE_CASE)&&(x<10*TAILLE_CASE)&&(y>3*TAILLE_CASE)&&(y<8*TAILLE_CASE)&&(board[arriveX][arriveY].race==VIDE)){
		//verifie que la case est a une distance de 1 du pion
		if( ((arriveX==departX+1) && (arriveY==departY)) || ((arriveX==departX-1) && (arriveY==departY))
		|| ((arriveX==departX) && (arriveY==departY+1)) || ((arriveX==departX) && (arriveY==departY-1)) ){
			return TRUE;
		}
	}
	else if ((x>4*TAILLE_CASE)&&(x<10*TAILLE_CASE)&&(y>3*TAILLE_CASE)&&(y<8*TAILLE_CASE)&&(board[arriveX][arriveY].race!=joueur.race)){
		*capture =1;
	}
	return FALSE;
}


//suite a un clic 1 de deplacement, on attend un second clic tant qu'il n'est pas valide ou
//que l'utilisateur n'appuit pas sur la croix on attend un clic
coord deplacement(Joueur joueur,coord c1, coord cAnc,int *capture){
	coord c2,cPionCap1;
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
				break;
			case SDL_MOUSEBUTTONUP:

				//On regarde si le clic 2 est bien dans le plateau et a une case de distance 1 et vide
				Clic2=verifClic2Deplacement(event.button.x,event.button.y,c1,capture,joueur);

				if ((*capture== 1)&&((event.button.x/TAILLE_CASE-4 != cAnc.x)&&(event.button.y/TAILLE_CASE-3 != cAnc.y))){
					c2.x=event.button.x/TAILLE_CASE-4;
					c2.y=event.button.y/TAILLE_CASE-3;
					cPionCap1.x=event.button.x/TAILLE_CASE-4;
					cPionCap1.y=event.button.y/TAILLE_CASE-3;
					if(c2.x==c1.x/TAILLE_CASE-4+1)
					{
						c2.x=c2.x+1;
					}
					else if(c2.x==c1.x/TAILLE_CASE-4-1)
					{
						c2.x=c2.x-1;
					}
					else if(c2.y==c1.y/TAILLE_CASE-3-1)
					{
						c2.y=c2.y-1;
					}
					else if ((c2.y==c1.y/TAILLE_CASE-3+1))
					{
						c2.y=c2.y+1;
					}

					//printf("c2 %d, %d \n",c2.x,c2.y);
					//printf("cc %d, %d \n",cPionCap.x,cPionCap.y);
					//printf("c1 %d, %d \n",c1.x/TAILLE_CASE-4,c1.y/TAILLE_CASE-3);
					if(board[c2.x][c2.y].race==VIDE){
						printf("tu capture\n") ;
						board[cPionCap1.x][cPionCap1.y].race=VIDE;
						board[c2.x][c2.y].race=joueur.race;
						board[c1.x/TAILLE_CASE-4][c1.y/TAILLE_CASE-3].race=VIDE;
						displayTile(cPionCap1.x, cPionCap1.y);
						
						//lancer capture pour que le joueur choisisse un autre pion du plateau a degager
						return c2;
					}
				}
				printf("\nanncienne position   %d %d \n",joueur.cAnc.x,joueur.cAnc.y);
				printf("nouvelle position    %d %d \n",event.button.x/TAILLE_CASE-4,event.button.y/TAILLE_CASE-3);
				if(Clic2==TRUE){
					if((event.button.x/TAILLE_CASE-4 != cAnc.x)&&(event.button.y/TAILLE_CASE-3 != cAnc.y)){
						//convertit le clic en case du tableau
						c2.x=event.button.x/TAILLE_CASE-4;
						c2.y=event.button.y/TAILLE_CASE-3;
						board[c1.x/TAILLE_CASE-4][c1.y/TAILLE_CASE-3].race=VIDE;
						board[c2.x][c2.y].race=joueur.race;
						return c2;
					}
				}
		}
	}
	return c2;
}


//verifie que le clic est sur la case d'un pion advers pour le deuxieme pion capturer
bool verifClicCapture2(int x, int y,Joueur joueur)
{
	// la case selectionner doit être une case vide du plateau
	if((x>4*TAILLE_CASE)&&(x<10*TAILLE_CASE)&&(y>3*TAILLE_CASE)&&(y<8*TAILLE_CASE)&&(board[(x/TAILLE_CASE)-4][(y/TAILLE_CASE)-3].race!=joueur.race)&&(board[(x/TAILLE_CASE)-4][(y/TAILLE_CASE)-3].race!=VIDE)){
		return TRUE;
	}
	else{
		return FALSE;
	}
}

//suite a la cazpture d'un pion le joueur en prend un autre au choix sur le plateau 
// precondition : il reste des pions au joueur adverse sur le plateau
coord capture2(Joueur joueur)
{
	coord cCapturer;
	bool Clic;
	int continuer=1;

	SDL_Event event;
	while (continuer)
	{
    	SDL_WaitEvent(&event);
		switch(event.type)
		{
			case SDL_QUIT:
				continuer=0;
				break;
			case SDL_MOUSEBUTTONUP:

				//On regarde si le clic est bien dans le plateau
				Clic= verifClicCapture2(event.button.x,event.button.y,joueur);
				if(Clic==TRUE){
					cCapturer.x=(event.button.x/TAILLE_CASE)-4;
					cCapturer.y=(event.button.y/TAILLE_CASE)-3;
					// on supprime le pion
					board[cCapturer.x][cCapturer.y].race=VIDE;
					return cCapturer;
				}
		}
	}
	return cCapturer;
}

void menuClick() {
	SDL_Event event;
	int isMainMenuDisplayed = 1;

	while(1) {
		SDL_WaitEvent(&event);
		switch(event.type) {
			case SDL_MOUSEBUTTONUP:
				
				// Si le joueur est dans le menu principal
				if (isMainMenuDisplayed) {
					// Clic sur "Jouer"
					if (event.button.x >= 255 && event.button.x <= 505 && event.button.y >= 240 && event.button.y <= 337) {
						isMainMenuDisplayed = !isMainMenuDisplayed;
						partie();
					}
					// Clic sur "Quitter"
					else if (event.button.x >= 255 && event.button.x <= 505 && event.button.y >= 387 && event.button.y <= 484) {
						end(event);
					}
					// Clic sur "Aide"
					else if (event.button.x >= 605 && event.button.x <= 855 && event.button.y >= 240 && event.button.y <= 337) {
						isMainMenuDisplayed = !isMainMenuDisplayed;
						displayRules();
					}
					// Clic sur "Scores"
					else if (event.button.x >= 605 && event.button.x <= 855 && event.button.y >= 387 && event.button.y <= 484) {
						isMainMenuDisplayed = !isMainMenuDisplayed;
						displayScores();
					}
				}
				// Le joueur est dans une autre interface comportant un menu
				else {
					// Clic sur le bouton "Retour" pour retourner vers le menu principal
					if (event.button.x >= 20 && event.button.x <= 148 && event.button.y >= 730 && event.button.y <= 780) {
						isMainMenuDisplayed = !isMainMenuDisplayed;
						display();
					}
				}
				break;
			case SDL_QUIT:
				end(event);
				break;
		}
	}
}

//fonction de test pour voir le plateau das le terminal ( 0 : VIDE, 1 : ORC, 2 : DEMON)
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

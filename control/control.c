// Projet Yoté -- IATIC 3 ISTY
// See the authors in AUTHOR.md

#include "control.h"

// Joue une partie
void partie(void) {
	// Saisit le nom des joueur.
	// À l'heure actuelle, cela ne sert à rien.
	scoreParty infoJoueur = scoreInput();

	//affichage de la fenetre de jeu
	display();

	//declaration des variables
	coord c1 = {};
	raceJoueur raceDebut;
	Joueur joueurOrc,joueurDemon,joueur;
	int continuer=1;

	//initialisation des 2 joueurs
	joueurOrc.race=ORC;
	joueurOrc.reserve=12;
	joueurDemon.race=DEMON;
	joueurDemon.reserve=12;

	raceDebut=joueurAleatoir(raceDebut);

	if(joueur.race==ORC){
		joueur=joueurDemon;
		}
	else{
		joueur=joueurOrc;
	}

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

		//action : placement
		if (action==RESERVE){
			CordPionNouv=placement(&joueur);

			if(joueur.race==ORC){
				//affiche pion orc et diminu de 1 la reserve
				displayPawn(sprites.spriteOrc,CordPionNouv.x, CordPionNouv.y);
				joueurOrc.reserve=joueur.reserve;
			}

			else{
				//affiche pion demon et diminue de 1 la reserve
				displayPawn(sprites.spriteDemon,CordPionNouv.x, CordPionNouv.y);
				joueurDemon.reserve=joueur.reserve;
			}
		}

		//action : déplacement
		else if(action==PLATEAU){
			CordPionNouv=deplacement(joueur,c1,CordPionAnc);

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
			CordPionAnc=CordPionNouv;
		}
		//affichage de la réserve
		if(joueur.race==ORC){
				displayReserve(joueurOrc.reserve,sprites.spriteOrc);
			}
		else{
			displayReserve(joueurDemon.reserve,sprites.spriteDemon);
		}
		//changement de joueur
		if(joueur.race==ORC){
			joueur=joueurDemon;
		}
		else{
			joueur=joueurOrc;
		}
		//permet d'afficher le contenue du tableau du plateau dans le terminal
		//affiche_plateau();
	}
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
bool verifClic2Deplacement(int x,int y,coord c1){
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
	return FALSE;
}

//suite a un clic 1 de deplacement, on attend un second clic tant qu'il n'est pas valide ou
//que l'utilisateur n'appuit pas sur la croix on attend un clic
coord deplacement(Joueur joueur,coord c1, coord cAnc){
	coord c2;
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

				Clic2=verifClic2Deplacement(event.button.x,event.button.y,c1);

				if(Clic2==TRUE){
					if((c2.x != cAnc.x)||(c2.y != cAnc.y)){
						//convertit le clic en case du tableau
						c2.x=event.button.x/TAILLE_CASE-4;
						c2.y=event.button.y/TAILLE_CASE-3;
						board[c2.x][c2.y].race=joueur.race;
						return c2;
					}
				}
		}
	}
	return c2;
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

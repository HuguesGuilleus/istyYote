// Projet Yoté -- IATIC 3 ISTY
// See the authors in AUTHOR.md

#include "control.h"

// Joue une partie
void partieJvsJ(void) {
	scoreParty infoJoueur = scoreInputJvsJ();

	//affichage de la fenetre de jeu
	// display();

	initBoard();
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

		}

		//action : déplacement
		else if(action==PLATEAU){

			CordPionNouv=deplacement(joueur,c1,joueur.cAnc,&capture,&CordPion1Cap);

			//si le joueur capture un pion dans son deplacement
			if (capture==1){
				displayTile(CordPion1Cap.x, CordPion1Cap.y);
				displayTile(c1.x/TAILLE_CASE-4, c1.y/TAILLE_CASE-3);
				//si le joueur courant et un orc
				if(joueur.race==ORC){
					//affiche pion orc a la nouvelle position
					displayPawn(sprites.spriteOrc,CordPionNouv.x, CordPionNouv.y);
					//on diminue de un le nb de pion sur le plateau du joeur adverse
					joueurDemon.plateau=joueurDemon.plateau-1;

					//si le joueur a plus de pion sur le plateau un pion de la reserve et prix
					if (joueurDemon.plateau==0)
					{
						joueurDemon.reserve=joueurDemon.reserve-1;
						displayReserve(joueur.reserve,sprites.spriteDemon);
					}
					//sinon le joueur choisi un pion sur le plateau qui est capturer
					else{
						printf("\n choisi un deuxime pion a capturer \n");
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
		//si le joueur a plus de pion c'est qu'il a perdu
		if((joueur.plateau==0) && (joueur.reserve==0)){
			if (joueur.race==ORC){
				displayWinner(DEMON);
			}
			else
			{
				displayWinner(ORC);
			}


			SDL_Delay(4000);

			continuer=0;
		}

		//permet d'afficher le contenue du tableau du plateau dans le terminal
		//affiche_plateau();
	}
	display();
	menuClick();
	return;
}

void partieJvsIA(void) {
	scoreParty infoJoueur = scoreInputJvsIA();

	//affichage de la fenetre de jeu
	// display();

	initBoard();
	displayBoard();

	//declaration des variables
	srand(time(NULL));

	coord c1 = {};
	coord CordPion1Cap,CordPion2Cap;
	int action;
	// CordPionNouv est les coord qu'on souhaite faire et CordPionAnc est les coord qu'on avait fait au tour precedent
	coord CordPionNouv, CordPionAnc;
	raceJoueur raceDebut;
	Joueur joueurOrc,joueurDemon,joueur;
	int continuer=1,i=0,j=0;
	int capture=0,XDemon=-1,YDemon=-1,XOrc=-1,YOrc=-1;
	coord TabPionIA[3];
	bool verif=FALSE;
	//on initialise les position du plateau au coordonner (-1,-1) pour que cela soit une case hors du tableau
	TabPionIA[0].x= -1; TabPionIA[0].y=-1;
	TabPionIA[1].x= -1; TabPionIA[1].y=-1;
	TabPionIA[2].x= -1; TabPionIA[2].y=-1;

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

		//affiche le joueur courant
		displayRound(joueur.race);
		printf("joueur : %d \n\n",joueur.race);
		if(joueur.race==ORC) // le joueur humain joue les orcs
		{
			//recupère l'action de jeu à effectuer par le clic
			action=ActionJoueur(&joueur,&c1,&continuer);

			if (action==RESERVE){

				CordPionNouv=placement(&joueur);
				joueur.plateau=joueur.plateau +1;

				//affiche pion orc et diminu de 1 la reserve
				displayPawn(sprites.spriteOrc,CordPionNouv.x, CordPionNouv.y);

				joueurOrc.plateau=joueur.plateau;
				joueurOrc.reserve=joueur.reserve;
				// on met des valeur hors du plateau en ancien coup pour que quelque soit la case selectionné elle soit differente
				joueur.cAnc.x=-1;
				joueur.cAnc.y=-1;
			}
			else if(action==PLATEAU){
			CordPionNouv=deplacement(joueur,c1,joueur.cAnc,&capture,&CordPion1Cap);

				//si le joueur capture un pion dans son deplacement
				if (capture==1){
					displayTile(CordPion1Cap.x, CordPion1Cap.y);
					displayTile(c1.x/TAILLE_CASE-4, c1.y/TAILLE_CASE-3);
					for(i=0;i<3;i++){
							if((CordPion1Cap.x==TabPionIA[i].x)&&(CordPion1Cap.y==TabPionIA[i].y)){
								TabPionIA[i].x=-1;
								TabPionIA[i].y=-1;
							}
						}
					//affiche pion orc a la nouvelle position
					displayPawn(sprites.spriteOrc,CordPionNouv.x, CordPionNouv.y);
					//on diminue de un le nb de pion sur le plateau du joeur adverse
					joueurDemon.plateau=joueurDemon.plateau-1;

					//si le joueur a plus de pion sur le plateau un pion de la reserve est pris
					if (joueurDemon.plateau==0)
					{
						joueurDemon.reserve=joueurDemon.reserve-1;
						displayReserve(joueur.reserve,sprites.spriteDemon);
					}
					//sinon le joueur choisi un pion sur le plateau qui est capturer
					else{
						printf("\n choisi un deuxime pion a capturer \n");
						CordPion2Cap=capture2(joueur);
						displayTile(CordPion2Cap.x, CordPion2Cap.y);
						joueurDemon.plateau=joueurDemon.plateau-1;
						for(i=0;i<3;i++){
							if((CordPion2Cap.x==TabPionIA[i].x)&&(CordPion2Cap.y==TabPionIA[i].y)){
								TabPionIA[i].x=-1;
								TabPionIA[i].y=-1;
							}
						}
					}
					//la capture est fini on remet donc capture a FAUX (0)
					capture =0;
				}


				//sinon deplacement normal
				else {
					//affiche une case a l'ancienne position du pion pour l'effacer
					displayTile(c1.x/TAILLE_CASE-4, c1.y/TAILLE_CASE-3);

					//affiche pion orc a la nouvelle position
					displayPawn(sprites.spriteOrc,CordPionNouv.x, CordPionNouv.y);
				}
				//on enregistre la position de depart pour que le joueur ne puisse pas faire un coup inverse a celui ci au prochain tour
				joueur.cAnc.x=c1.x/TAILLE_CASE-4;
				joueur.cAnc.y=c1.y/TAILLE_CASE-3;

			}
		}
		else if(joueur.race==DEMON){ // l'IA joue les Demons
			//si l'IA peut manger un pion adverse
			//alors l'IA mange un pion
			printf("\n\nc'est a moi\n\n");
			i=-1;
			do{
				i++;
				//printf("je veux te manger\n");
				printf("i=%d",i);
				XDemon=TabPionIA[i].x;
				XOrc=TabPionIA[i].x;
				YDemon=TabPionIA[i].y;
				YOrc=TabPionIA[i].y;
				printf("demon %d ",XDemon);

				printf(" demon %d !\n",YDemon);
				if((board[XDemon+1][YDemon].race == ORC)&&(board[XDemon+2][YDemon].race==VIDE)&&(XDemon+2<6)){
					XDemon=XDemon+2;
					XOrc=XOrc+1;
					capture=1;
					printf("je veux te manger a droite\n");
				}
				else if((board[XDemon-1][YDemon].race == ORC)&&(board[XDemon-2][YDemon].race==VIDE)&&(XDemon-2>=0)){
					XDemon=XDemon-2;
					XOrc=XOrc-1;
					capture=1;
					printf("je veux te manger a gauche\n");
				}
				else if((board[XDemon][YDemon+1].race == ORC)&&(board[XDemon][YDemon+2].race==VIDE)&&(YDemon+2<5)){
					YDemon=YDemon+2;
					YOrc=YOrc+1;
					capture=1;
					printf("je veux te manger en bas\n");
				}
				else if((board[XDemon][YDemon-1].race == ORC)&&(board[XDemon][YDemon-2].race==VIDE)&&(YDemon-2>=0)){
					YDemon=YDemon-2;
					YOrc=YOrc-1;
					capture=1;
					printf("je veux te manger en haut\n");
				}

			}while((capture!=1)&&(i!=2));
			if(capture==1){
				SDL_Delay(500);
				printf("ah ah capture =1 i =%d!\n",i);
				//le pion de l'ia saute par dessus et on le dessine al'arriver
				board[TabPionIA[i].x][TabPionIA[i].y].race=VIDE;

				//on efface l'ancienne position du joueur
				displayTile(TabPionIA[i].x,TabPionIA[i].y);

				//board[TabPionIA[i].x][TabPionIA[i].y].race=VIDE;
				TabPionIA[i].x=XDemon;
				printf("%d ",XDemon);
				TabPionIA[i].y=YDemon;
				printf("  %d !\n",YDemon);
				board[TabPionIA[i].x][TabPionIA[i].y].race=DEMON;
				//on dessine a la nouvelle position du joueur

				displayPawn(sprites.spriteDemon,TabPionIA[i].x, TabPionIA[i].y);

				//on efface le le pion orc
				printf("	j'ai manger l'oc qui etait en %d %d",XOrc,YOrc);
				board[XOrc][YOrc].race=VIDE;

				displayTile(XOrc,YOrc);

				//on retire un pion orc du compteur plateau du joueur orc
				joueurOrc.plateau=joueurOrc.plateau-1;

				//si il y a plus d'orc ur le plateau
				//alors mange un pion de la reserve
				if(joueurOrc.plateau==0){
					printf("reserve !\n");
					joueurOrc.reserve =joueurOrc.reserve-1;
				}
				//sinon
				//mange un pion orc au hasar sur le plateau
				else{
					printf("je capture un deuxuieme pion ! \n");
					SDL_Delay(500);
					i=-1;
					j=-1;
					verif=0;
					do{
						i++;
						do{
							j++;
							printf("i %d j %d\n",i ,j );
							if(board[i][j].race==ORC){
								verif=TRUE;
								printf("trouver !\n");
							}
						}while((j<4)&&(verif!=1));
						if(verif!=1){
							j=-1;
						}
					}while((i<5)&&(verif!=1));
					printf("je chope le 2 eme pion en i:%d j:%d \n",i,j);
					board[i][j].race=VIDE;
					joueurOrc.plateau=joueurOrc.plateau-1;
					displayTile(i,j);
				}
				capture=0;
			}
			//si un pion de l'IA est menacer
			//alors le pion fuit a l'opposer

			// a faire

			//si le nombre de pion de l'IA est <=3
			//alors placement aléatoire (au moins 1 case d'écart avec un pion adverse)
			else if((joueurDemon.plateau<3)&&(joueurDemon.reserve!=0)){
				SDL_Delay(500);
				do{
					XDemon=aleatoire(0,6);
					YDemon=aleatoire(0,5);

				}while(board[XDemon][YDemon].race!=VIDE);
				printf("placement en :\ni=%dj=%d\n",XDemon,YDemon);
				board[XDemon][YDemon].race=DEMON;
				i=0;
				while((TabPionIA[i].x!=-1)&&(TabPionIA[i].y!=-1)){
					i++;
				}
				TabPionIA[i].x=XDemon;
				TabPionIA[i].y=YDemon;
				displayPawn(sprites.spriteDemon,XDemon,YDemon);
				joueurDemon.plateau=joueurDemon.plateau+1;
				joueurDemon.reserve=joueurDemon.reserve-1;
				joueurDemon.cAnc.x=-1;
				joueurDemon.cAnc.y=-1;
			}
			//sinon deplacelemnt aléatoire
			else{
				SDL_Delay(500);
				do{
					i=aleatoire(0,3);
					XDemon=TabPionIA[i].x;
					YDemon=TabPionIA[i].y;

					//printf("le pion %d qui est en %d %d va se deplacer\n",i,XDemon,YDemon);
					printf(" ancienne pos avant recherceh de coup= %d %d\n",joueurDemon.cAnc.x,joueurDemon.cAnc.y);
					printf("je vais en %d %d et %d \n\n",XDemon+1,YDemon,verif);
					printf("je vais en %d %d et %d \n\n",XDemon-1,YDemon,verif);
					printf("je vais en %d %d et %d \n\n",XDemon,YDemon+1,verif);
					printf("je vais en %d %d et %d \n\n",XDemon,YDemon-1,verif);
					if((board[XDemon+1][YDemon].race==VIDE)&&(XDemon+1<6)&&(joueurDemon.cAnc.x!=XDemon+1)){
						joueurDemon.cAnc.x=XDemon;
						joueurDemon.cAnc.y=YDemon;
						XDemon=XDemon+1;
						verif=TRUE;
					}

					else if((board[XDemon-1][YDemon].race==VIDE)&&(XDemon-1>=0)&&(joueurDemon.cAnc.x!=XDemon-1)){
						joueurDemon.cAnc.x=XDemon;
						joueurDemon.cAnc.y=YDemon;
						XDemon=XDemon-1;

					joueurDemon.cAnc.y=YDemon;
						verif=TRUE;
					}

					else if((board[XDemon][YDemon+1].race==VIDE)&&(YDemon+1<5)&&(joueurDemon.cAnc.y!=YDemon+1)){
						joueurDemon.cAnc.x=XDemon;
						joueurDemon.cAnc.y=YDemon;
						YDemon=YDemon+1;
						verif=TRUE;
					}

					else if((board[XDemon][YDemon-1].race==VIDE)&&(YDemon-1>=0)&&(joueurDemon.cAnc.y!=YDemon-1))
					{
						joueurDemon.cAnc.x=XDemon;
						joueurDemon.cAnc.y=YDemon;
						YDemon=YDemon-1;
						verif=TRUE;
					}
				}while (verif!=TRUE);

				verif=FALSE;
				printf("nouvelle ancienne pos = %d %d\n",joueurDemon.cAnc.x,joueurDemon.cAnc.y);
				printf("je vais en %d %d\n\n",XDemon,YDemon);



				displayTile(TabPionIA[i].x,TabPionIA[i].y);

				board[TabPionIA[i].x][TabPionIA[i].y].race=VIDE;

				TabPionIA[i].x=XDemon;
				TabPionIA[i].y=YDemon;

				board[TabPionIA[i].x][TabPionIA[i].y].race=DEMON;
				displayPawn(sprites.spriteDemon,XDemon,YDemon);
			}
			printf("j'ai fini mon tour \n \n\n");
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
			joueurOrc.cAnc=joueur.cAnc;
			joueur=joueurDemon;
		}
		else{

			joueur=joueurOrc;
		}
		//si le joueur a plus de pion c'est qu'il a perdu
		if((joueur.plateau==0) && (joueur.reserve==0)){
			SDL_Delay(1000);
			printf("\n\n\n\n\n JOUEUR %d A PERDU\n\n\n\n", joueur.race);
			continuer=0;
		}

		//permet d'afficher le contenue du tableau du plateau dans le terminal
		affiche_plateau();
	}
	initBoard();
	display();
	menuClick();

}

raceJoueur joueurAleatoir(raceJoueur race) {
	return rand()%2 ? DEMON : ORC ;
}
int aleatoire(int a,int b){


	return rand()%(b-a)+a;
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
	printf("%d,%d\n",x,y);
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
coord deplacement(Joueur joueur,coord c1, coord cAnc,int *capture,coord *cPionCap1){
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
				Clic2=verifClic2Deplacement(event.button.x,event.button.y,c1,capture,joueur);

				if ((*capture== 1)&&((event.button.x/TAILLE_CASE-4 != cAnc.x)&&(event.button.y/TAILLE_CASE-3 != cAnc.y))){
					c2.x=event.button.x/TAILLE_CASE-4;
					c2.y=event.button.y/TAILLE_CASE-3;
					cPionCap1->x=event.button.x/TAILLE_CASE-4;
					cPionCap1->y=event.button.y/TAILLE_CASE-3;
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
					printf("hello\n");
					//printf("c2 %d, %d \n",c2.x,c2.y);
					//printf("cc %d, %d \n",cPionCap.x,cPionCap.y);
					//printf("c1 %d, %d \n",c1.x/TAILLE_CASE-4,c1.y/TAILLE_CASE-3);
					if((board[c2.x][c2.y].race==VIDE)&&(c2.x<6)&&(c2.x>=0)&&(c2.y>=0)&&(c2.y<5)){
						printf("tu capture\n") ;
						board[cPionCap1->x][cPionCap1->y].race=VIDE;
						board[c2.x][c2.y].race=joueur.race;
						board[c1.x/TAILLE_CASE-4][c1.y/TAILLE_CASE-3].race=VIDE;


						//lancer capture pour que le joueur choisisse un autre pion du plateau a degager
						return c2;
					}
					printf("bye\n");
				}
				printf("pas toucher\n");
				*capture=0;
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
	typeMenu menu = HOME;

	while(1) {
		SDL_WaitEvent(&event);
		switch(event.type) {
			case SDL_MOUSEBUTTONUP:

				// Si le joueur est dans le menu principal
				if (menu == HOME) {
					// Clic sur "Jouer"
					if (event.button.x >= 255 && event.button.x <= 505 && event.button.y >= 385 && event.button.y <= 482) {
						menu = GAMEMODE;
						// On affiche le menu de choix de mode de jeu
						displayGamemodeChoice();
					}
					// Clic sur "Quitter"
					else if (event.button.x >= 255 && event.button.x <= 505 && event.button.y >= 542 && event.button.y <= 639) {
						// On quitte le programme
						end(event);
					}
					// Clic sur "Aide"
					else if (event.button.x >= 605 && event.button.x <= 855 && event.button.y >= 385 && event.button.y <= 482) {
						menu = OTHER;
						// Affichage des règles
						displayRules();
					}
					// Clic sur "Scores"
					else if (event.button.x >= 605 && event.button.x <= 855 && event.button.y >= 542 && event.button.y <= 639) {
						menu = OTHER;
						// Affchage des scores
						displayScores();
					}
				}
				else if (menu == GAMEMODE) {
					if (event.button.x >= 152 && event.button.x <= 402) {
						if (event.button.y >= 255 && event.button.y <= 352) {
							partieJvsJ();
						}
						else if (event.button.y >= 545 && event.button.y <= 642) {
						partieJvsIA();
						}
					}
				}

				// Si le menu affiché n'est pas le menu d'accueil
				if (menu != HOME) {
					// Clic sur le bouton retour en bas à gauche de l'écran
					if (event.button.x >= 20 && event.button.x <= 148 && event.button.y >= 730 && event.button.y <= 780) {
						menu = HOME;
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

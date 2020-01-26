// Projet Yoté -- IATIC 3 ISTY
// See the authors in AUTHOR.md

#include "control.h"

// Jouer une partie joueur contre joueur
void partieJvsJ(void) {
	scoreParty infoJoueur = scoreInputJvsJ();

	//affichage de la fenetre de jeu
	initBoard();
	displayBoard();

	//declaration des variables
	coord c1 = {};
	coord CordPion1Cap,CordPion2Cap;
	raceJoueur raceDebut;
	Joueur joueurOrc,joueurDemon,joueur;
	int continuer=1;
	int compteur=0;
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

	//choix aléatoire du joueur qui commence
	raceDebut=joueurAleatoir(raceDebut);

	//la race du joueur courant est initialisée a la race choisi aléatoirement
	if(raceDebut==ORC){
		joueur=joueurDemon;
		}
	else{
		joueur=joueurOrc;
	}
	//initialisation a des valeurs hors du plateau
	joueur.cAnc.x=-1;
	joueur.cAnc.y=-1;

	//affichage du nombre de pions contenus dans les reserves au debut de partie
	displayReserve(joueur.reserve,sprites.spriteOrc);
	displayReserve(joueur.reserve,sprites.spriteDemon);

	//affichage des 2 sprites de caisses de la reserve
	displayReserveBox();

	while(continuer==1){

		int action;
		// CordPionNouv est les coord du déplacement que l'on souhaite faire et CordPionAnc est les coord qu'on avait fait au tour precedent
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
				//affiche pion orc et diminue de 1 la reserve
				displayPawn(sprites.spriteOrc,CordPionNouv.x, CordPionNouv.y);
				// on met des valeurs hors du plateau en ancien coup pour que quelque soit la case selectionnée elle en soit differente
				joueurOrc.plateau=joueur.plateau;
				joueurOrc.reserve=joueur.reserve;
			}

			else{
				//affiche pion demon et diminue de 1 la reserve
				displayPawn(sprites.spriteDemon,CordPionNouv.x, CordPionNouv.y);
				// on met des valeurs hors du plateau en ancien coup pour que quelque soit la case selectionnée elle soit differente
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
					//on diminue de 1 le nb de pion sur le plateau du joueur adverse
					joueurDemon.plateau=joueurDemon.plateau-1;

					//si le joueur a plus de pion sur le plateau un pion de la reserve est pris
					if (joueurDemon.plateau==0)
					{
						joueurDemon.reserve=joueurDemon.reserve-1;
						displayReserve(joueur.reserve,sprites.spriteDemon);
					}
					//sinon le joueur choisi un pion sur le plateau qui est capturé
					else{

						CordPion2Cap=capture2(joueur);
						displayTile(CordPion2Cap.x, CordPion2Cap.y);
						joueurDemon.plateau=joueurDemon.plateau-1;
					}

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

				}
				//la capture est finie on remet donc capture a FAUX (0)
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

		//affichage de la réserve des 2 joueurs

			displayReserve(joueurOrc.reserve,sprites.spriteOrc);

			displayReserve(joueurDemon.reserve,sprites.spriteDemon);

		//changement de joueur
		if(joueur.race==ORC){
			joueurOrc.cAnc=joueur.cAnc;
			joueur=joueurDemon;

		}
		else{
			joueurDemon.cAnc=joueur.cAnc;
			joueur=joueurOrc;
		}
		
		//On fait la condition lorsqu'il reste plus que 2 pions chacun
		if((joueurDemon.plateau==2) && ( joueurDemon.reserve==0) && (joueurOrc.plateau==2) && (joueurOrc.reserve==0)){
			
			compteur++;
		}
		
		//si le joueur n'a plus de pion sur le plateau et dans sa reserve il perd la partie
		if((joueur.plateau==0) && (joueur.reserve==0)){
			if (joueur.race==ORC){
				infoJoueur.status = PLAYER_LOSE ;
				displayWinner(DEMON);
			} else {
				infoJoueur.status = PLAYER_WIN ;
				displayWinner(ORC);
			}
			//sauvegarde des scores
			scoreAppend(&allScores, infoJoueur);
			scoreSave(allScores);
			//pause de 3 sec sur l'ecran qui affiche le nom du gagnant
			SDL_Delay(3000);
			continuer=0;
		}
		//Si 10 tours se sont écoulés depuis la condition d'égalité, il y a égalité
		else if(compteur==10)
		{	//La partie est nulle
			infoJoueur.status = PLAYER_NULL;
			//On ajoute le score à la liste et on enregistre dans le fichier
			scoreAppend(&allScores, infoJoueur);
			scoreSave(allScores);
			
			
			SDL_Delay(3000);
			continuer=0;
			
		}


	}
	display();
	menuClick();
	return;
}

// jouer une partie joueur contre IA
void partieJvsIA(void) {
	scoreParty infoJoueur = scoreInputJvsIA();

	//affichage de la fenetre de jeu
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
	int continuer=1,i=0,j=0, compteur=0;
	int capture=0,XDemon=-1,YDemon=-1,XOrc=-1,YOrc=-1;
	coord TabPionIA[3];
	bool verif=FALSE;
	//on initialise les position du plateau aux coordonnées (-1,-1) pour que cela soit une case hors du tableau
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

	//choix aléatoire du joueur qui commence
	raceDebut=joueurAleatoir(raceDebut);

	//la race du joueur courant est initialisée à la race choisi aléatoirement
	if(raceDebut==ORC){
		joueur=joueurDemon;
		}
	else{
		joueur=joueurOrc;
	}
	//initialisation a des valeurs hors du plateau
	joueur.cAnc.x=-1;
	joueur.cAnc.y=-1;

	//affichage des nombres de pions contenus dans les reserves au debut de partie
	displayReserve(joueur.reserve,sprites.spriteOrc);
	displayReserve(joueur.reserve,sprites.spriteDemon);

	//affichage des 2 sprites de caisses de la reserve
	displayReserveBox();

	while(continuer==1){

		//affiche la race du joueur courant
		displayRound(joueur.race);
		if(joueur.race==ORC) // le joueur humain joue les orcs
		{
			//recupère l'action de jeu a effectuer par le clic
			action=ActionJoueur(&joueur,&c1,&continuer);

			if (action==RESERVE){

				CordPionNouv=placement(&joueur);
				joueur.plateau=joueur.plateau +1;

				//affiche pion orc et diminue de 1 la reserve
				displayPawn(sprites.spriteOrc,CordPionNouv.x, CordPionNouv.y);

				joueurOrc.plateau=joueur.plateau;
				joueurOrc.reserve=joueur.reserve;
				// on met des valeur hors du plateau en ancien coup pour que quelque soit la case selectionnée elle soit differente
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
					//sinon le joueur choisi un pion sur le plateau qui est capturé
					else{

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
					//la capture est finie on remet donc capture a FAUX (0)
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
			i=-1;
			//on test pour chaque pion de l'IA si ils ont un orc a capturer
			do{
				i++;

				XDemon=TabPionIA[i].x;
				XOrc=TabPionIA[i].x;
				YDemon=TabPionIA[i].y;
				YOrc=TabPionIA[i].y;
				
				if((board[XDemon+1][YDemon].race == ORC)&&(board[XDemon+2][YDemon].race==VIDE)&&(XDemon+2<6)){
					XDemon=XDemon+2;
					XOrc=XOrc+1;
					capture=1;
				}
				else if((board[XDemon-1][YDemon].race == ORC)&&(board[XDemon-2][YDemon].race==VIDE)&&(XDemon-2>=0)){
					XDemon=XDemon-2;
					XOrc=XOrc-1;
					capture=1;
				}
				else if((board[XDemon][YDemon+1].race == ORC)&&(board[XDemon][YDemon+2].race==VIDE)&&(YDemon+2<5)){
					YDemon=YDemon+2;
					YOrc=YOrc+1;
					capture=1;
				}
				else if((board[XDemon][YDemon-1].race == ORC)&&(board[XDemon][YDemon-2].race==VIDE)&&(YDemon-2>=0)){
					YDemon=YDemon-2;
					YOrc=YOrc-1;
					capture=1;
				}
			
			}while((capture!=1)&&(i!=2));

			//si elle peut en capturer un elle le fait
			if(capture==1){
				//pour rendre sont deplacement plus "realiste" on met des delay courts
				//qui permettent de decouper son tour lors des affichages
				SDL_Delay(500);
				//le pion de l'ia saute par dessus et on le dessine a l'arrivée
				board[TabPionIA[i].x][TabPionIA[i].y].race=VIDE;

				//on efface l'ancienne position du joueur
				displayTile(TabPionIA[i].x,TabPionIA[i].y);

				//board[TabPionIA[i].x][TabPionIA[i].y].race=VIDE;
				TabPionIA[i].x=XDemon;
				TabPionIA[i].y=YDemon;
				board[TabPionIA[i].x][TabPionIA[i].y].race=DEMON;
				//on dessine a la nouvelle position du joueur

				displayPawn(sprites.spriteDemon,TabPionIA[i].x, TabPionIA[i].y);

				//on efface le pion orc
				board[XOrc][YOrc].race=VIDE;

				displayTile(XOrc,YOrc);

				//on retire un pion orc du compteur plateau du joueur orc
				joueurOrc.plateau=joueurOrc.plateau-1;

				//si il y a plus d'orc sur le plateau
				//alors mange un pion de la reserve
				if(joueurOrc.plateau==0){
					joueurOrc.reserve =joueurOrc.reserve-1;
				}
				//sinon
				//mange un pion orc au hasard sur le plateau
				else{
					SDL_Delay(500);
					i=-1;
					j=-1;
					verif=0;
					//on balaie le plateau jusqu a tomber sur un pion orc
					do{
						i++;
						do{
							j++;
							if(board[i][j].race==ORC){
								verif=TRUE;
							}
						}while((j<4)&&(verif!=1));
						if(verif!=1){
							j=-1;
						}
					}while((i<5)&&(verif!=1));
					//le pion orc trouvé en position (i,j) est alors capturé
					board[i][j].race=VIDE;
					joueurOrc.plateau=joueurOrc.plateau-1;
					displayTile(i,j);
				}
				capture=0;
			}

			//si le nombre de pion de l'IA est <=3
			//alors placement aléatoire 
			else if((joueurDemon.plateau<3)&&(joueurDemon.reserve!=0)){
				SDL_Delay(500);
				// l'IA tente de se placer de maniere aleatoire sur le plateau on cherche des coordonnées 
				// tant que la case correspondante n'est pas vide
				do{
					XDemon=aleatoire(0,6);
					YDemon=aleatoire(0,5);

				}while(board[XDemon][YDemon].race!=VIDE);
				//on place le pion ans le plateau
				board[XDemon][YDemon].race=DEMON;
				i=0;
				//pour remplir le tableau contenant ses coordoner on cherche une case qui ne contenait
				//pas encore de coordoner (elles sont initialisée a (-1,-1) si pas de coordonées sauvegardées)
				while((TabPionIA[i].x!=-1)&&(TabPionIA[i].y!=-1)){
					i++;
				}
				// on remplit le tableau des coordonées des pions de l'IA
				TabPionIA[i].x=XDemon;
				TabPionIA[i].y=YDemon;
				// on affiche un demon a cette position
				displayPawn(sprites.spriteDemon,XDemon,YDemon);
				// on augmente son compteur de pions sur le plateau et diminue celui de la reserve
				joueurDemon.plateau=joueurDemon.plateau+1;
				joueurDemon.reserve=joueurDemon.reserve-1;
				// on enregistre son ancienne position a une case hors du plateau
				joueurDemon.cAnc.x=-1;
				joueurDemon.cAnc.y=-1;
			}
			//sinon deplacement aléatoire
			else{
				SDL_Delay(500);
				do{
					i=aleatoire(0,3);
					XDemon=TabPionIA[i].x;
					YDemon=TabPionIA[i].y;
					// test si le pion peut se deplacer
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
					//on recommence jusqu a ce qu'un de ses pions puisse bouger
				}while (verif!=TRUE);
				//on reinistialise verif a faux
				verif=FALSE;
				displayTile(TabPionIA[i].x,TabPionIA[i].y);
				//la case ou se trouvait le pion de l'IA devient vide
				board[TabPionIA[i].x][TabPionIA[i].y].race=VIDE;
				// on remplit le tableau contenant les coordonner des pion de l'ia
				TabPionIA[i].x=XDemon;
				TabPionIA[i].y=YDemon;
				//la case ou le pion de l'IA s'est deplacée est remplie
				board[TabPionIA[i].x][TabPionIA[i].y].race=DEMON;
				displayPawn(sprites.spriteDemon,XDemon,YDemon);
			}
		}
		//affichage de la réserve

			displayReserve(joueurOrc.reserve,sprites.spriteOrc);
			displayReserve(joueurDemon.reserve,sprites.spriteDemon);

		//changement de joueur
		if(joueur.race==ORC){
			joueurOrc.cAnc=joueur.cAnc;
			joueur=joueurDemon;
		}
		else{

			joueur=joueurOrc;
			
			
		}
		//On fait la condition lorsqu'il reste plus que 2 pions chacun
		if((joueurDemon.plateau==2) && ( joueurDemon.reserve==0) && (joueurOrc.plateau==2) && (joueurOrc.reserve==0)){
			
			compteur++;
			
		}
		
		//si le joueur n'a plus de pion sur le plateau et dans sa reserve c'est qu'il a perdu
		if((joueur.plateau==0) && (joueur.reserve==0)){
			if (joueur.race==ORC){
				infoJoueur.status = PLAYER_LOSE ;
				displayWinner(DEMON);
			} else {
				infoJoueur.status = PLAYER_WIN ;
				displayWinner(ORC);
			}
			//sauvegarde des scores
			scoreAppend(&allScores, infoJoueur);
			scoreSave(allScores);
			//pause de 3 sec sur l'ecran qui affiche le nom du gagnant
			SDL_Delay(3000);
			continuer=0;
		}
		
		else if((compteur==10))
		{
			
			//La partie est nulle
			infoJoueur.status = PLAYER_NULL;
			//On ajoute le score à la liste et on enregistre dans le fichier
			scoreAppend(&allScores, infoJoueur);
			scoreSave(allScores);
			
			
			
			SDL_Delay(3000);
			continuer=0;
			
		}

	}
	initBoard();
	display();
	menuClick();

}

//fonction qui choisi aleéatoirement une des 2 races
raceJoueur joueurAleatoir(raceJoueur race) {
	return rand()%2 ? DEMON : ORC ;
}
//fonction qui renvoie un chiffre aléatoire entre a et b 
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
		//on attend un clic de l'utilisateur
    	SDL_WaitEvent(&event);
		switch(event.type)
		{

			case SDL_QUIT:
				// si la croix rouge est cliquée
				*continuer=0;
				break;
			case SDL_MOUSEBUTTONUP:

				//On regarde si le clic 1 est dans le plateau ou dans la reserve
				Clic1= verifClic1(event.button.x,event.button.y,*joueur);
				if ((Clic1==RESERVE)&&(joueur->reserve!=0)){
					//renvoie l'action a faire qui est : placement d'un pion
                    surbrillanceCaisse(joueur->race);
					return RESERVE;
				}
				else if(Clic1==PLATEAU){
					//renvoie l'action a faire qui est : deplacement (le joueur ne peut pas encore capturer)
					//donc deplacement se fera vers une case vide
					c->x=event.button.x;
					c->y=event.button.y;
					return PLATEAU;
				}
		}
	}
}

//on verifie que le clic est autorisé
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
		displayFocus(x/TAILLE_CASE-4, y/TAILLE_CASE-3);
		return PLATEAU;
	}
}

//verifie que le clic 2 suite a un clic dans la reserve est correct et puis renvoie un booleen
bool verifClic2Placement(int x, int y)
{
	// la case selectionnée doit être une case vide du plateau
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
	//on attend un clic de l'utilisateur
	SDL_Event event;
	while (continuer)
	{
    	SDL_WaitEvent(&event);
		switch(event.type)
		{
			case SDL_QUIT:
				// si la croix rouge est cliquée
				continuer=0;
				break;
			case SDL_MOUSEBUTTONUP:

				//On regarde si le clic 2 est bien dans le plateau
				Clic2= verifClic2Placement(event.button.x,event.button.y);
				if(Clic2==TRUE){
					// on transforme le clic en indice du tableau
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
	//on converti un clic en case du plateau
	// on transforme les clics en indice du tableau
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
		//action de capture engagée
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
				// si la croix rouge est cliquée
				continuer=0;
				break;
			case SDL_MOUSEBUTTONUP:

				//On regarde si le clic 2 est bien dans le plateau et a une case de distance 1 et vide
				Clic2=verifClic2Deplacement(event.button.x,event.button.y,c1,capture,joueur);
				// si action de capture engagée et que la case est pas l'ancienne emplacement on regarde si la capture est possible
				if ((*capture== 1)&&((event.button.x/TAILLE_CASE-4 != cAnc.x)||(event.button.y/TAILLE_CASE-3 != cAnc.y))){
					// on transforme les clics en indice du tableau
					c2.x=event.button.x/TAILLE_CASE-4;
					c2.y=event.button.y/TAILLE_CASE-3;
					cPionCap1->x=event.button.x/TAILLE_CASE-4;
					cPionCap1->y=event.button.y/TAILLE_CASE-3;
					// on verifie que le pion qui saute atterrit sur une case vide. Si elle n'est pas vide la capture ne se fera pas
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
					if((board[c2.x][c2.y].race==VIDE)&&(c2.x<6)&&(c2.x>=0)&&(c2.y>=0)&&(c2.y<5)){
						// la case du pion capturée devient vide
						board[cPionCap1->x][cPionCap1->y].race=VIDE;
						// la case d'atterissage du pion qui capture est remplie
						board[c2.x][c2.y].race=joueur.race;
						// la case ou se trouvait le pion avant de capturer devient vide
						board[c1.x/TAILLE_CASE-4][c1.y/TAILLE_CASE-3].race=VIDE;
						return c2;
					}

				}
				//si la capture n'a pas ete possible on reinitialise a faux la variable capture
				*capture=0;
				if(Clic2==TRUE){
					if((event.button.x/TAILLE_CASE-4 != cAnc.x)||(event.button.y/TAILLE_CASE-3 != cAnc.y)){
						//convertit le clic en case du tableau
						c2.x=event.button.x/TAILLE_CASE-4;
						c2.y=event.button.y/TAILLE_CASE-3;
						// on met la caes de l'ancienne position a case vide
						board[c1.x/TAILLE_CASE-4][c1.y/TAILLE_CASE-3].race=VIDE;
						// on met la case de la nouvelle position a la race du joueur
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

//suite a la capture d'un pion le joueur en prend un autre au choix sur le plateau
// precondition : il reste des pions au joueur adverse sur le plateau
coord capture2(Joueur joueur)
{
	coord cCapturer;
	bool Clic;
	int continuer=1;

	SDL_Event event;
	while (continuer)
	{
		//on attend un clic du joueur
    	SDL_WaitEvent(&event);
		switch(event.type)
		{
			case SDL_QUIT:
				// si la croix rouge est cliquée
				continuer=0;
				break;
			case SDL_MOUSEBUTTONUP:

				//On regarde si le clic est bien dans le plateau
				Clic= verifClicCapture2(event.button.x,event.button.y,joueur);
				if(Clic==TRUE){
					//on convertit le clic en indice du plateau
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



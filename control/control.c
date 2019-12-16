// Projet Yoté -- IATIC 3 ISTY
// See the authors in AUTHOR.md

#include "control.h"

// Joue une partie
void partie(void) {
	
	display();
	
	coord c1 = {};
	Joueur joueurOrc,joueurDemon,joueur;
	joueurOrc.race=ORC;
	joueurOrc.reserve=12;
	joueurDemon.race=DEMON;
	joueurDemon.reserve=12;

	int continuer=1;
	joueur=joueurOrc;
	displayReserve(joueur.reserve,sprites.spriteOrc);
	displayReserve(joueur.reserve,sprites.spriteDemon);
	while(continuer==1){ 
		//recuepere l'action de jeu a effectuer
		int action;
		coord CordPion;

		printf("joueur : %d \n",joueur.race);
		//displayRound(joueur.race);
		
		
		action=ActionJoueur(&joueur,&c1,&continuer);

		//action : placement
		if (action==RESERVE){
			printf("placement\n");
			CordPion=placement(&joueur);
			if(joueur.race==ORC){
				displayPawn(sprites.spriteOrc,CordPion.x, CordPion.y);

			printf("reserve : %d \n",joueur.reserve);
				joueurOrc.reserve=joueur.reserve;
			}
			else{
				displayPawn(sprites.spriteDemon,CordPion.x, CordPion.y);

		printf("reserve : %d \n",joueur.reserve);
				joueurDemon.reserve=joueur.reserve;
			}
		}
		else if(action==PLATEAU){
			printf(" deplacement\n");
			CordPion=deplacement(joueur,c1);
			printf("cord : %d %d \n",c1.x, c1.y);
			displayTile(c1.x/TAILLE_CASE-4, c1.y/TAILLE_CASE-3);
			if(joueur.race==ORC){
				displayPawn(sprites.spriteOrc,CordPion.x, CordPion.y);
			}
			else{
				displayPawn(sprites.spriteDemon,CordPion.x, CordPion.y);
			}
		}
		//affichage de la reserve
		if(joueur.race==ORC){
				displayReserve(joueurOrc.reserve,sprites.spriteOrc);
			}
		else{
			displayReserve(joueurDemon.reserve,sprites.spriteDemon);
		}
		//changemlent de joueur
		if(joueur.race==ORC){
			joueur=joueurDemon;
		}
		else{
			joueur=joueurOrc;
		}
		affiche_plateau();
	}
	return;
}

Joueur joueurAleatoir(Joueur joueur) {
	joueur.race= rand()%2 ? ORC : DEMON ;
	return joueur;
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

				//On regarde si le clic 1 est dans le plateau ou la reserve
				Clic1= verifClic1(event.button.x,event.button.y,*joueur);
				if ((Clic1==RESERVE)&&(joueur->reserve!=0)){
					//renvoie l'action a faire qui est : placement d'un pion

					return RESERVE;
				}
				else if(Clic1==PLATEAU){
					//renvoie l'action a faire qui est : deplacement ou capture
					c->x=event.button.x;
					c->y=event.button.y;
					return PLATEAU;
				}
		}
	}
}

//on verifie que le clic est autoriser
//renvoie l'action qui sera effectuer au clic2 ( placement ou deplacement/capture)
int verifClic1 (int x, int y, Joueur joueur){
	printf("\n\n\n%d %d\n",x/TAILLE_CASE-5,y/TAILLE_CASE-4);
	printf("tableua = %d   joeur == %d \n",board[x/TAILLE_CASE-5][y/TAILLE_CASE-4].race,joueur);
	if(joueur.race==ORC){
		//reserve du joueur orc
		if((x>TAILLE_CASE)&&(x<3*TAILLE_CASE)&&(y>4*TAILLE_CASE)&&(y<9*TAILLE_CASE)){
			printf("reserve J1\n");
			return RESERVE;
		}
	}
	else if(joueur.race==DEMON){
		//reserve du joueur demon
		if((x>12*TAILLE_CASE)&&(x<14*TAILLE_CASE)&&(y>4*TAILLE_CASE)&&(y<9*TAILLE_CASE)){
			printf("reserve J2\n");
			return RESERVE;
		}
	}
	
	if((x>4*TAILLE_CASE)&&(x<11*TAILLE_CASE)&&(y>3*TAILLE_CASE)&&(y<9*TAILLE_CASE)&&(board[x/TAILLE_CASE-4][y/TAILLE_CASE-3].race)==joueur.race){
		printf("plateau\n");
		return PLATEAU;
	}
	

}

bool verifClic2Placement(int x, int y)
{
	if((x>4*TAILLE_CASE)&&(x<10*TAILLE_CASE)&&(y>3*TAILLE_CASE)&&(y<8*TAILLE_CASE)&&(board[(x/TAILLE_CASE)-4][(y/TAILLE_CASE)-4].race==VIDE)){
		return TRUE;
	}
	else{
		return FALSE;
	}
}


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
				printf("fini");
				break;
			case SDL_MOUSEBUTTONUP:

				//On regarde si le clic 2 est bien dans le plateau
				Clic2= verifClic2Placement(event.button.x,event.button.y);
				if(Clic2==TRUE){
					printf("je place le pion en %d, %d \n",(event.button.x/TAILLE_CASE)-4,(event.button.y/TAILLE_CASE)-3);
					c.x=(event.button.x/TAILLE_CASE)-4;
					c.y=(event.button.y/TAILLE_CASE)-3;
					
					board[c.x][c.y].race=joueur->race;
					joueur->reserve =joueur->reserve-1;
					return c;
				}
		}
	}
	return c;
}

bool verifClic2Deplacement(int x,int y,coord c1){
	
	int arriveX = x/TAILLE_CASE-4;
	int arriveY = y/TAILLE_CASE-3;
	int departX=  c1.x/TAILLE_CASE-4;
	int departY= c1.y/TAILLE_CASE-3;
	printf("verifions que tu peux allez ici :    %d %d\n",arriveX,arriveY);
	printf("tu etais ici :    %d %d\n", departX, departY);
	//verifie que le clic est dans le plateau et que la case et vide
	if((x>4*TAILLE_CASE)&&(x<10*TAILLE_CASE)&&(y>3*TAILLE_CASE)&&(y<8*TAILLE_CASE)&&(board[arriveX][arriveY].race==VIDE)){
		//verifie que la case est a une distance de 1 du pion
		if( ((arriveX==departX+1) && (arriveY==departY)) || ((arriveX==departX-1) && (arriveY==departY)) || ((arriveX==departX) && (arriveY==departY+1)) || ((arriveX==departX) && (arriveY==departY-1)) ){
			printf("c'esr possible de se deplacer ! \n");
			return TRUE;
		}
		
	}
	return FALSE;
}

coord deplacement(Joueur joueur,coord c1){
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
				printf("tu dois te deplace\n");
				Clic2=verifClic2Deplacement(event.button.x,event.button.y,c1);
				
				if(Clic2==TRUE){
					printf("je deplace le pion en %d, %d \n",(event.button.x/TAILLE_CASE)-4,(event.button.y/TAILLE_CASE)-3);
					c2.x=event.button.x/TAILLE_CASE-4;
					c2.y=event.button.y/TAILLE_CASE-3;
					printf("je deplace le pion de %d, %d \n",c1.x/TAILLE_CASE-4,c1.y/TAILLE_CASE-3);
					printf("en %d, %d \n",c2.x,c2.y);
					board[c1.x/TAILLE_CASE-4][c1.y/TAILLE_CASE-3].race=VIDE;
					board[c2.x][c2.y].race=joueur.race;
					return c2;
				}
		}
	}
	return c2;
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

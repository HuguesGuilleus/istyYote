#ifndef CONTROL_H
	#define CONTROL_H

	#include "../main.h"


	void partieJvsJ(void);
	void partieJvsIA(void);
	raceJoueur joueurAleatoir(raceJoueur race);

	int ActionJoueur(Joueur* joueur,coord *c,int *continuer);

	int verifClic1 (int x, int y, Joueur joueur);

	coord placement(Joueur * joueur);
	bool verifClic2Placement(int x, int y);
	
	coord deplacement(Joueur joueur,coord c1, coord cAnc, int *capture,coord*CordPion1Cap);
	bool verifClic2Deplacement(int x,int y,coord c1,int * capture, Joueur joueur);
	coord capture2(Joueur joueur);
	bool verifClicCapture2(int x, int y,Joueur joueur);

	void menuClick();
	
	int aleatoire(int a,int b);

#endif

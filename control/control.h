#ifndef CONTROL_H
	#define CONTROL_H

	#include "../main.h"


	void partie(void);
	raceJoueur joueurAleatoir(raceJoueur race);

	int ActionJoueur(Joueur* joueur,coord *c,int *continuer);

	int verifClic1 (int x, int y, Joueur joueur);

	coord placement(Joueur * joueur);
	bool verifClic2Placement(int x, int y);
	
	coord deplacement(Joueur joueur,coord c1, coord cAnc, int *capture);
	bool verifClic2Deplacement(int x,int y,coord c1,int * capture, Joueur joueur);
	coord capture2(Joueur joueur);
	bool verifClicCapture2(int x, int y,Joueur joueur);
	coord capture1(Joueur joueur,coord c2,coord c1);

	void affiche_plateau();

#endif

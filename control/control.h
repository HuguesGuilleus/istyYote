#ifndef CONTROL_H
	#define CONTROL_H

	#include "../main.h"


	void partie(void);
	Joueur joueurAleatoir(Joueur joueur);

	int ActionJoueur(Joueur* joueur,coord *c,int *continuer);

	int verifClic1 (int x, int y, Joueur joueur);

	coord placement(Joueur * joueur);
	bool verifClic2Placement(int x, int y);
	
	coord deplacement(Joueur joueur,coord c1);
	bool verifClic2Deplacement(int x,int y,coord c1);

	void affiche_plateau();

#endif

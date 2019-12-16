// Projet Yoté -- IATIC 3 ISTY
// See the authors in AUTHOR.md

#include "event.h"

// L'objectif du système ci-dessous est de facilier une approche évènemencielle
// des évènements générer par les joueurs.

// L'idée est de récupérer les évènements, de les filtrer puis d'éxécuter des
// fonctions qui leur correpondent; ces fonctions se noment CallBack ou fonction
// de rappel. Cela est réalisé par la fonction handlersRun()

// Le filtrage: on fait un filtre par rapport au type de l'évènement
// (par exemple: SDL_MOUSEBUTTONDOWN) puis de manière plus précise grâce à une
// fonction de filtre (par exemple filterInBoard), ces fonction prennent en
// paramètre un évènement SDL puis renvoie un booléen: vrai si le callBack final
// doit être apellé avec cet évènement, sinon faux.

// On ajoute des callbacks à un typeHandlers* (en gros c'est un tableau
// dynamique) avec handlersAdd; puis on lance notre liste de callBack avec
// handlersRun; enfin on libère la mémoire avec handlersFree.

void handlersAdd(typeHandlers * h, SDL_EventType e, typeFilter * f, typeActuator * a) {
	int i ;
	handler * new = NULL ;
	if (h->list == NULL || h->len == h->cap) {
		if (h->cap) {
			h->cap *= 2 ;
		} else {
			h->cap = 5 ;
		}
		new = (handler*)malloc(h->cap*sizeof(handler));
		for (i = 0; i < h->len; i++) {
			new[i] = h->list[i] ;
		}
		if (h->list) {
			free(h->list);
		}
		h->list = new ;
	}
	h->list[h->len] = (handler){
		event: e,
		filter: f,
		actuator: a,
	};
	h->len++;
}

void handlersFree(typeHandlers * h) {
	if (h == NULL) {
		return ;
	}
	if (h->list != NULL) {
		free(h->list);
		h->list = NULL;
	}
	h->len = 0 ;
	h->cap = 0 ;
}

void handlersRun(typeHandlers * h) {
	SDL_Event event;
	int i;

	while (SDL_PollEvent(&event));

	while (!h->stop) {
		if (SDL_PollEvent(&event)) {
			for (i = 0; i < h->len; i++) {
				if (h->list[i].event == event.type && h->list[i].filter(event)) {
					h->list[i].actuator(event) ;
				}
			}
		} else {
			usleep(2);
		}
	}
}


/* FILTER */

// It return allwas TRUE.
bool filterTrue(SDL_Event _) {
	return TRUE ;
}

// Le click est sur le plateau
bool filterInBoard(SDL_Event event) {
	int x = event.button.x ;
	int y = event.button.y ;
	return ORIGINE_PLATEAU_X < x && x < ORIGINE_PLATEAU_X+TAILLE_CASE*LARGEUR
		&& ORIGINE_PLATEAU_Y < y && y < ORIGINE_PLATEAU_Y+TAILLE_CASE*HAUTEUR ;
}

// Le click est en dehors du plateau
bool filterOutBoard(SDL_Event event) {
	int x = event.button.x ;
	int y = event.button.y ;
	return x < ORIGINE_PLATEAU_X || ORIGINE_PLATEAU_X+TAILLE_CASE*LARGEUR < x
		|| y < ORIGINE_PLATEAU_Y || ORIGINE_PLATEAU_Y+TAILLE_CASE*HAUTEUR < y ;
}

// Le click est sur le plateau et sur une case vide.
bool filterTileEmpty(SDL_Event event) {
	coord c;

	if (!filterInBoard(event)) {
		return FALSE ;
	}

	c = clickToCoord(event);
	return board[c.x][c.y].race == VIDE ;
}

// Le click est le plateau et sur une piece du joueur courant.
bool filterTile(SDL_Event event) {
	coord c;

	if (!filterInBoard(event)) {
		return FALSE ;
	}

	c = clickToCoord(event);
	return board[c.x][c.y].race == currentParty.joueur ;
}

// Renvoie les coordonées de la case cliqué. Le click doit être sur le plateau.
coord clickToCoord(SDL_Event click) {
	return (coord){
		x: (click.button.x - ORIGINE_PLATEAU_X)/TAILLE_CASE,
		y: (click.button.y - ORIGINE_PLATEAU_Y)/TAILLE_CASE,
	};
}

// Renvoie TRUE si le joueur courant a des pions dans la réserve et que le click
// est dans la zone maxiale de la réserve.
bool filterReserve(SDL_Event event) {
	int minX ;
	if (currentParty.joueur == ORC) {
		minX = ORIGINE_RESERVE_ORC ;
		if (currentParty.reserveOrc == 0) {
			return FALSE ;
		}
	} else {
		minX = ORIGINE_RESERVE_DEAMON ;
		if (currentParty.reserveDemon == 0) {
			return FALSE ;
		}
	}

	int x = event.button.x ;
	int y = event.button.y ;
	return minX < x && x < minX+TAILLE_CASE*2
		&& ORIGINE_PLATEAU_Y < y && y < ORIGINE_PLATEAU_Y+TAILLE_CASE*HAUTEUR ;
}

// Renvoie TRUE si le joueur clic sur sa réserve.
bool filterReserveWithJoueur(SDL_Event event, raceJoueur joueur) {
	int minX ;
	if (joueur == ORC) {
		minX = ORIGINE_RESERVE_ORC ;
	} else {
		minX = ORIGINE_RESERVE_DEAMON ;
	}

	int x = event.button.x ;
	int y = event.button.y ;
	return minX < x && x < minX+TAILLE_CASE*2
		&& ORIGINE_PLATEAU_Y < y && y < ORIGINE_PLATEAU_Y+TAILLE_CASE*HAUTEUR ;
}

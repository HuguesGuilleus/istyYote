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

/* FILTRE D'ÉVÈNEMENT DU CLAVIER */

bool filterKeyReturn(SDL_Event key) {
	return key.key.keysym.sym == SDLK_RETURN ;
}
bool filterKeyBackspace(SDL_Event key) {
	return key.key.keysym.sym == SDLK_BACKSPACE ;
}
bool filterKeyLetter(SDL_Event key) {
	char k = key.key.keysym.sym ;
	return ('A' <= k && k <= 'Z') || ('a' <= k && k <= 'z');
}

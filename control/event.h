// Projet Yoté -- IATIC 3 ISTY
// See the authors in AUTHOR.md

#ifndef CONTROL_EVENT
	#define CONTROL_EVENT 1

	#include "../main.h"

	typedef void (*typeFilter(SDL_Event))(bool) ;
	typedef void (*typeActuator(SDL_Event))(void) ;

	typedef struct {
		SDL_EventType event;
		typeFilter * filter;
		typeActuator * actuator;
	} handler ;

	typedef struct {
		handler * list ;
		int len ;
		int cap ;
		bool stop ;
	} typeHandlers ;


	void handlersAdd(typeHandlers * h, SDL_EventType e, typeFilter * f, typeActuator * a);
	void handlersRun(typeHandlers * h);

	bool filterInBoard(SDL_Event event);
	bool filterOutBoard(SDL_Event event);
	bool filterTileEmpty(SDL_Event event);
	bool filterTile(SDL_Event event);
	bool filterReserve(SDL_Event event);
	bool filterTrue(SDL_Event event);
	coord clickToCoord(SDL_Event click);

	bool filterReserveWithJoueur(SDL_Event event, raceJoueur joueur);

	bool filterKeyReturn(SDL_Event key);
	bool filterKeyBackspace(SDL_Event key);
	bool filterKeyLetter(SDL_Event key);

#endif

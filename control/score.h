// Projet Yoté -- IATIC 3 ISTY
// See the authors in AUTHOR.md

#ifndef CONTROL_SCORE_H
	#define CONTROL_SCORE_H 1

	#include "../main.h"

	#define STRING_BUFFER	15

	typedef enum {
		PLAYER_WIN,
		PLAYER_LOSE,
		PLAYER_NULL,
	} scoreStatus ;

	// Le status d'une partie.
	typedef struct {
		// Le nom du premier joueur
		char * playerOrc ;
		// Le nom du deuxième joueur.
		char * playerDemon ;
		// Le status du premier joueur.
		scoreStatus status ;
	} scoreParty ;

	typedef struct {
		// La liste en elle-même
		scoreParty* l;
		// Le nombre d'élément de la liste
		int len ;
		// La capacité totale de la liste
		int cap;
	} scorePartyList ;

	scoreParty scoreInput(void);
	char* scoreInputOncePlayer(raceJoueur j);
	void scoreSave(scorePartyList list);
	char scoreSaveStatus1(scoreStatus status);
	char scoreSaveStatus2(scoreStatus status);
	scorePartyList scoreRecover(void);
	scoreStatus scoreParseStatus(char c);
	void scoreAppend(scorePartyList * list, scoreParty party);
	void scoreFree(scorePartyList list);
	char *cpStr(char * src);

#endif

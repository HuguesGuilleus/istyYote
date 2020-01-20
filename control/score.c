// Projet Yoté -- IATIC 3 ISTY
// See the authors in AUTHOR.md

#include "score.h"

/* SAISIT DES SCORES PAR LES JOUEUR */

// Saisit le nom des deux joueurs humain.
scoreParty scoreInputJvsJ(void) {
	return (scoreParty){
		playerOrc: scoreInputOncePlayer(ORC),
		playerDemon: scoreInputOncePlayer(DEMON),
	};
}

// Saisit le nom d'un joueur humain et de l'IA.
scoreParty scoreInputJvsIA(void) {
	return (scoreParty){
		playerOrc: scoreInputOncePlayer(ORC),
		playerDemon: cpStr("IA"),
	};
}

// Saisit un seul nom de joueur.
char* scoreInputOncePlayer(raceJoueur j) {
	typeHandlers h = {} ;
	char name[STRING_BUFFER] = "";
	int len = 0;

	displayScoreInput(j);
	displayScoreInputText("<Entrez votre nom de joueur>");

	// Fin de la saisit par retour ou clic sur un bouton.
	void keyReturn(SDL_Event _) {
		if (len>0) {
			h.stop = TRUE ;
		}
	};
	handlersAdd(&h, SDL_KEYDOWN, &filterKeyReturn, &keyReturn);

	// Efface le dernier caractère
	void keyBack(SDL_Event _) {
		if (len>0) {
			len--;
			name[len] = '\0' ;
			if (len != 0) {
				displayScoreInputText(name);
			} else {
				displayScoreInputText("<Entrez votre nom de joueur>");
			}
		}
	};
	handlersAdd(&h, SDL_KEYDOWN, &filterKeyBackspace, &keyBack);

	// Saisit une lettre
	void keyLetter(SDL_Event key) {
		if (len<STRING_BUFFER-1) {
			name[len] = key.key.keysym.sym ;
			len++;
			name[len] = '\0' ;
			displayScoreInputText(name);
		}
	};
	handlersAdd(&h, SDL_KEYDOWN, &filterKeyLetter, &keyLetter);

	handlersAdd(&h, SDL_QUIT, &filterTrue, &end);
	handlersRun(&h);
	handlersFree(&h);

	return cpStr(name);
}


/* SAUVEGARDE DES SCORES */

// Sauvegarde les scores dans le fichier score.txt
void scoreSave(scorePartyList list) {
	FILE* file = NULL ;
	int i;
	file = fopen("score.txt", "w");
	if (file == NULL) {
		printf("Erreur lors de la sauvegarde des scores\n");
		return ;
	}
	for (i = 0; i < list.len; i++) {
		fprintf(file, "%s %c | %s %c\n",
			list.l[i].playerOrc,
			scoreSaveStatus1(list.l[i].status),
			list.l[i].playerDemon,
			scoreSaveStatus2(list.l[i].status)
		);
	}
	fclose(file);
}

// Retourne la lettre du status de la partie du premier joueur.
char scoreSaveStatus1(scoreStatus status) {
	switch (status) {
		case PLAYER_WIN:  return 'G';
		case PLAYER_LOSE: return 'P';
		case PLAYER_NULL: return 'N';
	}
}

// Retourne la lettre du status de la partie du deuxième joueur.
char scoreSaveStatus2(scoreStatus status) {
	switch (status) {
		case PLAYER_WIN:  return 'P';
		case PLAYER_LOSE: return 'G';
		case PLAYER_NULL: return 'N';
	}
}


/* RÉCUPÉRATION DES SCORES */

// Lit le fichier score.txt
scorePartyList scoreRecover(void) {
	int i;
	FILE* file = NULL;
	// Des buffer pour la lecture des chaîne de caractère.
	char playerOrc[STRING_BUFFER] = "", playerDemon[STRING_BUFFER] = "" ;
	// Des pointeurs vers les chaîne de caractère alloué dynamiquement.
	char *p1 = NULL, *p2 = NULL ;
	// chaîne de caractère pour le status.
	char status, _ ;
	// La liste qui sera renvoyée.
	scorePartyList list = {};

	file = fopen("score.txt", "r");
	if (file == NULL) {
		printf("Erreur lors de l'ouverture du fichier score.txt\n");
		return (scorePartyList){};
	}
	while (4 == fscanf(file, "%s %c | %s %c\n", playerOrc, &status, playerDemon, &_ )) {
		scoreAppend(&list, (scoreParty){
			playerOrc: cpStr(playerOrc),
			playerDemon: cpStr(playerDemon),
			status: scoreParseStatus(status),
		});
	}
	fclose(file);
	return list;
}

// Interprètre depuis un fichier de score le status de la partie.
scoreStatus scoreParseStatus(char c) {
	switch (c) {
		case 'G': return PLAYER_WIN;
		case 'P': return PLAYER_LOSE;
		case 'N': return PLAYER_NULL;
		default:
			printf("scoreStatus inconnu: '%c'\n", c);
			return PLAYER_NULL;
	}
}


/* ZONE DE MANIPULATION MÉMOIRE */

// Ajoute un élément à la liste de parties. La liste peut être vide ou pleine,
// l'allocation sera automatique.
void scoreAppend(scorePartyList * list, scoreParty party) {
	scoreParty * new = NULL ;
	int i;
	if (list->l == NULL) {
		list->len = 0;
		list->cap = 5;
		list->l = (scoreParty*)malloc(list->cap*sizeof(scoreParty));
	} else if (list->len == list->cap) {
		list->cap *= 2 ;
		new = (scoreParty*)malloc(list->cap*sizeof(scoreParty));
		for (i = 0; i < list->len; i++) {
			new[i] = list->l[i] ;
		}
		free(list->l);
		list->l = new ;
	}
	list->l[list->len] = party ;
	list->len++;
}

// Libère la mémoire (nom des joueurs et la liste l) de list.
void scoreFree(scorePartyList list) {
	int i ;
	for (i = 0; i < list.len; i++) {
		free(list.l[i].playerOrc);
		free(list.l[i].playerDemon);
	}
	free(list.l);
}

// Copie la chaîne de caractère src vers un espace mémoire alloué.
// La chaîne devra être désalouée par la suite.
char *cpStr(char * src) {
	int len = strlen(src)+1 ;
	char * out = (char*)malloc(len*sizeof(char));
	strcpy(out, src);
	return out ;
}

// Écrit dans la console une liste de score. Fonction pour le développment.
void scorePrint(scorePartyList list) {
	int i ;
	printf("List: (%d/%d)\n", list.len, list.cap);
	if (list.l == NULL) {
		printf("  (NULL)\n");
	} else {
		for (i = 0; i < list.len; i++) {
			switch (list.l[i].status) {
				case PLAYER_WIN:
					printf("  %10s (win)  | %-10s\n", list.l[i].playerOrc, list.l[i].playerDemon );
					break;
				case PLAYER_LOSE:
					printf("  %10s (lose) | %-10s\n", list.l[i].playerOrc, list.l[i].playerDemon );
					break;
				case PLAYER_NULL:
					printf("  %10s (null) | %-10s\n", list.l[i].playerOrc, list.l[i].playerDemon );
					break;
			}
		}
	}
}

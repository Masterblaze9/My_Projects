#ifndef PROJETOFINALED_AUXLIST_H
#define PROJETOFINALED_AUXLIST_H

#include "../Trie/TrieStruct.h"

typedef struct AuxListContent {
    char word[MAX_WORD_LENGTH];
    int counter;
} AuxListContent, *PtAuxListContent;

typedef struct AuxListNode {
    PtAuxListContent info;
    List *files;
    struct AuxListNode *next;
} AuxListNode, *PtAuxListNode;

typedef struct AuxList {
    AuxListNode *start;
    int count;
} AuxList, *PtAuxList;

//!Functions
PtAuxListNode createAuxListNode(char *word, char *fileName);

PtAuxList createAuxList();

int compareWords(PtAuxListContent x, PtAuxListContent y);

PtAuxListNode searchForSameElementsAuxList(PtAuxList auxList, char *word);

void addToAuxList(PtAuxList auxList, PtAuxListNode auxListNode);

void rankingAuxList(PtAuxList auxList, StringList stringList, int tokens);

void readFilesAux(PtAuxList auxList, char *fileName);

void showAuxList(PtAuxList auxList);

void getAuxListWordsStats(PtAuxList auxList);

void destroyAuxList(PtAuxList auxList);

#endif //PROJETOFINALED_AUXLIST_H

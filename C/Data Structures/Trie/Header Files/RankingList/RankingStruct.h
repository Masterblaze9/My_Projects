#ifndef PROJETOFINALED_RANKINGSTRUCT_H
#define PROJETOFINALED_RANKINGSTRUCT_H

#include "../Trie/TrieStruct.h"

typedef struct RankingListContent {
    char fileName[MAX_WORD_LENGTH];
    int min;
    int max;
    float ranking;
} RankingListContent, *PtRankingListContent;

typedef struct RankingListNode {
    PtRankingListContent info;
    struct RankingListNode *next;
} RankingListNode, *PtRankingListNode;

typedef struct RankingList {
    RankingListNode *start;
    int count;
} RankingList, *PtRankingList;

//! Functions
PtRankingListNode createRankingListNode(char *fileName, int min, int max);

PtRankingList createRankingList();

PtRankingListNode searchForSameRankingNode(PtRankingList rankingList, char *word);

void addToRankingList(PtRankingList rankinglist, PtRankingListNode rankingListNode);

void destroyRankingList(PtRankingList rankinglist);

#endif //PROJETOFINALED_RANKINGSTRUCT_H

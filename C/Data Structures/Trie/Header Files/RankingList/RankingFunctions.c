#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "RankingStruct.h"

PtRankingListNode createRankingListNode(char *fileName, int min, int max) {
    PtRankingListNode rankingListNode = (PtRankingListNode) malloc(sizeof(RankingListNode));
    if (!rankingListNode) return NULL;

    rankingListNode->info = (PtRankingListContent) malloc(sizeof(RankingListContent));
    if (!rankingListNode->info) return NULL;

    strcpy(rankingListNode->info->fileName, fileName);
    rankingListNode->info->min = min;
    rankingListNode->info->max = max;

    rankingListNode->next = NULL;
    return rankingListNode;
}

PtRankingList createRankingList() {
    PtRankingList rankingList = (PtRankingList) malloc(sizeof(RankingList));
    if (!rankingList) return NULL;

    rankingList->start = NULL;
    rankingList->count = 0;
    return rankingList;
}

PtRankingListNode searchForSameRankingNode(PtRankingList rankingList, char *word) {
    if (!rankingList) return NULL;

    PtRankingListNode aux = rankingList->start;
    while (aux) {
        if (strcmp(aux->info->fileName, word) == 0) return aux;
        aux = aux->next;
    }
    return NULL;
}

void addToRankingList(PtRankingList rankinglist, PtRankingListNode rankingListNode) {
    if (!rankinglist) return;

    if (!rankinglist->start) rankinglist->start = rankingListNode;
    else {
        PtRankingListNode aux = rankinglist->start;
        while (aux->next) aux = aux->next;

        aux->next = rankingListNode;
    }
    rankinglist->count++;
}

void destroyRankingList(PtRankingList rankinglist) {
    if (!rankinglist) return;

    PtRankingListNode rankingListNode = rankinglist->start;
    while (rankingListNode) {
        free(rankingListNode->info);
        PtRankingListNode aux = rankingListNode->next;
        free(rankingListNode);
        rankingListNode = aux;
    }
    free(rankinglist);
}

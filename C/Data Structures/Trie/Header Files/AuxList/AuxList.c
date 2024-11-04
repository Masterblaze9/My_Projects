#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "AuxList.h"
#include "../RankingList/RankingStruct.h"

PtAuxListNode createAuxListNode(char *word, char *fileName) {
    PtAuxListNode auxListNode = (PtAuxListNode) malloc(sizeof(AuxListNode));
    if (!auxListNode) return NULL;

    auxListNode->info = (PtAuxListContent) malloc(sizeof(AuxListContent));
    if (!auxListNode->info) return NULL;

    strcpy(auxListNode->info->word, word);
    auxListNode->info->counter = 1;
    auxListNode->files = (PtList) malloc(sizeof(List));
    if (!auxListNode->files) return NULL;

    auxListNode->files = createList();
    auxListNode->next = NULL;
    return auxListNode;
}

PtAuxList createAuxList() {
    PtAuxList auxList = (PtAuxList) malloc(sizeof(AuxList));
    if (!auxList) return NULL;

    auxList->start = NULL;
    auxList->count = 0;
    return auxList;
}

int compareWords(PtAuxListContent x, PtAuxListContent y) {
    return strcmp(x->word, y->word);
}

PtAuxListNode searchForSameElementsAuxList(PtAuxList auxList, char *word) {
    if (!auxList) return NULL;

    PtAuxListNode aux = auxList->start;
    while (aux) {
        if (strcmp(aux->info->word, word) == 0) return aux;
        aux = aux->next;
    }
    return NULL;
}

void addToAuxList(PtAuxList auxList, PtAuxListNode auxListNode) {
    if (!auxList) return;

    int pos = 1;
    PtAuxListNode aux;
    PtAuxListNode prev = NULL;
    if (!auxList->start) auxList->start = auxListNode;
    else {
        aux = auxList->start;
        while (pos == 1) {
            if (compareWords(aux->info, auxListNode->info) > 0) pos = 2;
            else if (!aux->next) pos = 0;
            else {
                prev = aux;
                aux = aux->next;
            }
        }

        if (aux == auxList->start && pos == 2) { //! Insert in the beginning
            auxListNode->next = auxList->start;
            auxList->start = auxListNode;
        } else if (pos == 2) { //! Insert in the middle
            prev->next = auxListNode;
            auxListNode->next = aux;
        } else aux->next = auxListNode;
    }
    auxList->count++;
}

void rankingAuxList(PtAuxList auxList, StringList stringList, int tokens) {
    if (!auxList || !stringList) return;

    int i;
    PtAuxListNode aux = NULL;
    PtRankingListNode rankNode;
    PtRankingList rankingList = createRankingList();
    if (!rankingList) return;

    int first = 1;
    int isValid = 1;
    for (i = 0; i < tokens; i++) {
        aux = searchForSameElementsAuxList(auxList, stringList[i]);
        if (aux && first) { //! 1º Token
            first = 0;
            PtListNode auxFiles = aux->files->start;
            while (auxFiles) {
                rankNode = createRankingListNode(auxFiles->info->fileName, auxFiles->info->counter,
                                                 auxFiles->info->counter);
                addToRankingList(rankingList, rankNode);
                auxFiles = auxFiles->next;
            }
            isValid = 1;
        } else if (aux) {
            //! Following Tokens (2, 3...)
            rankNode = rankingList->start;
            while (rankNode) {
                //! Search
                PtListNode ptAux = searchForSameFileByFileName(aux->files, rankNode->info->fileName);
                if (ptAux) {
                    if (rankNode->info->min > ptAux->info->counter)
                        rankNode->info->min = ptAux->info->counter;
                    else if (rankNode->info->max < ptAux->info->counter)
                        rankNode->info->max = ptAux->info->counter;
                } else rankNode->info->min = 0;
                rankNode = rankNode->next;
            }
            isValid = 1;
        } else isValid = 0;
    }

    if (isValid) {
        //! Calculate Ranking
        rankNode = rankingList->start;
        while (rankNode) {
            if (rankNode->info->min > 0)
                rankNode->info->ranking = (float) (rankNode->info->min + log10(rankNode->info->max));
            else rankNode->info->ranking = 0;
            rankNode = rankNode->next;
        }

        PtRankingListNode node1, node2;
        PtRankingListContent nodeAux;
        int changed = 1;
        while (changed) {
            node1 = rankingList->start;
            node2 = node1->next;
            changed = 0;
            while (node2) {
                if (node2->info->ranking > node1->info->ranking) {
                    changed = 1;
                    nodeAux = node2->info;
                    node2->info = node1->info;
                    node1->info = nodeAux;
                }
                node1 = node2;
                node2 = node2->next;
            }
        }

        rankNode = rankingList->start;
        while (rankNode) {
            if (rankNode->info->ranking > 0)
                printf("\n[%s] %d %d -> RANKING: %.2f", rankNode->info->fileName, rankNode->info->min,
                       rankNode->info->max,
                       rankNode->info->ranking);
            rankNode = rankNode->next;
        }
        printf("\n--------------------------------");
        destroyRankingList(rankingList);
    }
}

void readFilesAux(PtAuxList auxList, char *fileName) {
    if (!auxList) return;

    int i;
    FILE *f = fopen(fileName, "r");
    if (!f) return;

    int count = 0;
    fscanf(f, "%d", &count);

    char word[MAX_WORD_LENGTH];
    PtAuxListNode auxListNode;
    PtListNode listNode;
    for (i = 0; i < count; i++) {
        char file[MAX_WORD_LENGTH];
        fscanf(f, "%s", file);

        FILE *fileToRead = fopen(file, "r");
        if (!fileToRead) return;

        while (!feof(fileToRead)) {
            fscanf(fileToRead, "%s", word);

            auxListNode = searchForSameElementsAuxList(auxList, word);
            if (auxListNode) auxListNode->info->counter++;
            else {
                auxListNode = createAuxListNode(word, fileName);
                if (!auxListNode) printf("\nSomething went wrong!");
                else addToAuxList(auxList, auxListNode);
            }

            listNode = searchForSameFile(auxListNode->files, i + 1);
            if (listNode) listNode->info->counter++;
            else {
                listNode = createListNode(i + 1, file);
                if (!listNode) printf("\nSomething went wrong!");
                else addFileToListNode(auxListNode->files, listNode);
            }
        }
        fclose(fileToRead);
    }

    //! Read Queries
    int countQueries = 0;
    int countTokens = 0;
    int j;
    char wordToken[MAX_WORD_LENGTH];
    fscanf(f, "%d", &countQueries);

    for (i = 0; i < countQueries; i++) {
        fscanf(f, "%d", &countTokens);
        StringList queries = (StringList) malloc(countTokens * sizeof(MyString));
        if (!queries) return;

        for (j = 0; j < countTokens; j++) {
            fscanf(f, "%s", wordToken);
            queries[j] = (MyString) malloc((strlen(wordToken) + 1) * sizeof(char));
            if (!queries[j]) return;

            strcpy(queries[j], wordToken);
        }
        rankingAuxList(auxList, queries, countTokens);

        //! Free StringList
        for (j = 0; j < countTokens; j++) free(queries[j]);
        free(queries);
    }
    fclose(f);
}

void showAuxList(PtAuxList auxList) {
    if (!auxList) return;

    PtAuxListNode aux = auxList->start;
    PtListNode auxListNode;
    printf("\n");
    while (aux) {
        printf("[%s] - [Total: %d]\n", aux->info->word, aux->info->counter);
        auxListNode = aux->files->start;
        while (auxListNode) {
            printf("\tFILE: [%d] - [%s] (%d)\n", auxListNode->info->idDoc, auxListNode->info->fileName,
                   auxListNode->info->counter);
            auxListNode = auxListNode->next;
        }
        printf("\n");
        aux = aux->next;
    }
}

void getAuxListWordsStats(PtAuxList auxList) {
    if (!auxList) return;

    PtAuxListNode auxListNode = auxList->start;
    int min = MAX_WORD_LENGTH, max = 0;
    float total = 0;
    while (auxListNode) {
        total += strlen(auxListNode->info->word);
        if (strlen(auxListNode->info->word) > max) max = strlen(auxListNode->info->word);
        if (strlen(auxListNode->info->word) < min) min = strlen(auxListNode->info->word);
        auxListNode = auxListNode->next;
    }

    printf("\n[%d]", auxList->count);
    printf("\n[%d]", min);
    printf("\n[%d]", max);
    printf("\n[%.2f]", (total / (float) auxList->count));
}

void destroyAuxList(PtAuxList auxList) {
    if (!auxList) return;

    PtAuxListNode auxListNode = auxList->start;
    while (auxListNode) {
        free(auxListNode->info);
        destroyList(auxListNode->files);
        PtAuxListNode aux = auxListNode->next;
        free(auxListNode);
        auxListNode = aux;
    }
    free(auxList);
}

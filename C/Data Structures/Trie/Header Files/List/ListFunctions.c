#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ListStruct.h"

PtListNode createListNode(int idDoc, char *fileName) {
    PtListNode listNode = (PtListNode) malloc(sizeof(ListNode));
    if (!listNode) return NULL;

    listNode->info = (PtListContent) malloc(sizeof(ListContent));
    if (!listNode->info) return NULL;

    listNode->info->idDoc = idDoc;
    strcpy(listNode->info->fileName, fileName);
    listNode->info->counter = 1;

    listNode->next = NULL;

    return listNode;
}

PtList createList() {
    PtList list = (PtList) malloc(sizeof(List));
    if (!list) return NULL;

    list->start = NULL;
    list->count = 0;
    return list;
}

PtListNode searchForSameFile(PtList list, int idDoc) {
    if (!list) return NULL;

    PtListNode aux = list->start;
    while (aux) {
        if (aux->info->idDoc == idDoc) return aux;
        aux = aux->next;
    }
    return NULL;
}

PtListNode searchForSameFileByFileName(PtList list, char *fileName) {
    if (!list) return NULL;

    PtListNode aux = list->start;
    while (aux) {
        if (strcmp(aux->info->fileName, fileName) == 0) return aux;
        aux = aux->next;
    }
    return NULL;
}

void addFileToListNode(PtList list, PtListNode listNode) {
    if (!list) return;

    PtListNode auxListNode;
    if (!list->start) list->start = listNode;
    else {
        auxListNode = list->start;
        while (auxListNode->next) auxListNode = auxListNode->next;
        auxListNode->next = listNode;
    }
    list->count++;
}

void writeFilesList(PtList list) {
    if (!list) return;

    PtListNode aux = list->start;
    while (aux) {
        printf("\tFILE: [%d] - [%s] (%d)\n", aux->info->idDoc, aux->info->fileName,
               aux->info->counter);
        aux = aux->next;
    }
}

size_t calculateListRAM(PtList list) {
    if (!list) return 0;

    return sizeof(list) + list->count * sizeof(ListNode);
}

void destroyList(PtList list) {
    if (!list) return;

    PtListNode listNode = list->start;
    while (listNode) {
        free(listNode->info);
        PtListNode aux = listNode->next;
        free(listNode);
        listNode = aux;
    }
    free(list);
}

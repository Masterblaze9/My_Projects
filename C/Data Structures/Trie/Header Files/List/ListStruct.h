#ifndef PROJETOFINALED_LISTSTRUCT_H
#define PROJETOFINALED_LISTSTRUCT_H
#define MAX_WORD_LENGTH 100

typedef struct ListContent {
    int idDoc;
    char fileName[MAX_WORD_LENGTH];
    int counter;
} ListContent, *PtListContent;

typedef struct ListNode {
    PtListContent info;
    struct ListNode *next;
} ListNode, *PtListNode;

typedef struct List {
    ListNode *start;
    int count;
} List, *PtList;

//! Functions
PtListNode createListNode(int idDoc, char *fileName);

PtList createList();

PtListNode searchForSameFile(PtList list, int idDoc);

PtListNode searchForSameFileByFileName(PtList list, char *fileName);

void addFileToListNode(PtList list, PtListNode listNode);

void writeFilesList(PtList list);

size_t calculateListRAM(PtList list);

void destroyList(PtList list);

#endif //PROJETOFINALED_LISTSTRUCT_H

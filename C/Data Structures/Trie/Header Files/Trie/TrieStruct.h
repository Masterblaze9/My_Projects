#ifndef PROJETOFINALED_TRIESTRUCT_H
#define PROJETOFINALED_TRIESTRUCT_H
#define MAX_CHAR 26
#define FALSE 0
#define TRUE 1

#include "../List/ListStruct.h"

typedef struct TrieNode {
    int wordExists;
    int wordCount;
    struct TrieNode *vector[MAX_CHAR];
    //struct TrieNode **vector[MAX_CHAR];
    List *files;
} TrieNode, *PtTrieNode;

typedef struct Files {
    char *fileName;
    int count;
} Files;

typedef struct Trie {
    TrieNode *start;
    int numberOfFiles;
    int count;
    Files *filesArray;
} Trie, *PtTrie;

typedef char *MyString;
typedef MyString *StringList;

//! Functions
PtTrieNode createTrieNode();

PtTrie createTrie();

int compareElementsREC(PtTrieNode trieNode, char *word);

int compareElements(PtTrieNode trieNode, char *word);

PtTrieNode searchForSameElementsTrieREC(PtTrieNode trieNode, char *token, char *word);

PtTrieNode searchForSameElementsTrie(PtTrie trie, char *token);

void rankingTrie(PtTrie trie, StringList stringList, int tokens);

void readFiles(PtTrie trie, char *fileName);

int hashing(char *word);

PtTrieNode addNodeToTrie(PtTrieNode trieNode, char *word, int idDoc, char *file);

void addWordToTrie(PtTrie trie, char *word, int idDoc, char *file);

void showTrieNodesREC(PtTrie trie, PtTrieNode trieNode, char *word);

void showTrie(PtTrie trie);

void getWordsStatsREC(PtTrie trie, PtTrieNode trieNode, char *word, int *min, int *max, float *total, int *totalWords);

void getWordsStats(PtTrie trie);

int searchForWordInTrieREC(PtTrieNode trieNode, char *userWord, char *word);

void searchForWordInTrie(PtTrie trie, char *userWord);

void wordWithHighestAppearanceREC(PtTrieNode trieNode, char *word, char *maxWord, int *times);

void wordWithHighestAppearance(PtTrie trie);

void fileWithHigherAppearanceREC(PtTrieNode trieNode, Files *filesArray);

void fileWithHighestAppearance(PtTrie trie);

int countNodesInLevelREC(PtTrieNode trieNode, int currentLvl, int level);

void countNodesInLevel(PtTrie trie, int level);

int countWordsWithCertainLengthREC(PtTrieNode trieNode, int currentLvl, int length);

void countWordsWithCertainLength(PtTrie trie, int length);

int countWordsUnderCertainLengthREC(PtTrieNode trieNode, int currentLvl, int length);

void countWordsUnderCertainLength(PtTrie trie, int length);

size_t calculateRAM_REC(PtTrieNode trieNode);

void calculateRAM(PtTrie trie);

void destroyTrieREC(PtTrieNode trieNode);

void destroyTrie(PtTrie trie);

#endif //PROJETOFINALED_TRIESTRUCT_H

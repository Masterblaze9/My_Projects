#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "TrieStruct.h"
#include "../Utils/Utils.h"
#include "../RankingList/RankingStruct.h"

PtTrieNode createTrieNode() {
    //notImplemented(__FUNCTION__);
    PtTrieNode trieNode = (PtTrieNode) malloc(sizeof(TrieNode));
    if (!trieNode) return NULL;

    trieNode->wordExists = FALSE;
    trieNode->wordCount = 0;
    for (int i = 0; i < MAX_CHAR; i++)
        trieNode->vector[i] = NULL;
    trieNode->files = NULL;

    return trieNode;
}

PtTrie createTrie() {
    //notImplemented(__FUNCTION__);
    PtTrie trie = (PtTrie) malloc(sizeof(Trie));
    if (!trie) return NULL;

    trie->start = createTrieNode();
    trie->numberOfFiles = 0;
    trie->count = 0;
    trie->filesArray = NULL;
    return trie;
}

int compareElementsREC(PtTrieNode trieNode, char *word) {
    if (!trieNode) return -1;
    if (*word == '\0') return trieNode->wordExists;

    char aux = *word;
    int index = aux - 'a'; //posicao do vetor
    if (trieNode->vector[index] == NULL)
        return 0;
    return compareElementsREC(trieNode->vector[index], word + 1);
}

int compareElements(PtTrieNode trieNode, char *word) {
    if (!trieNode) return -1;

    return compareElementsREC(trieNode, word);
}

PtTrieNode searchForSameElementsTrieREC(PtTrieNode trieNode, char *token, char *word) {
    if (!trieNode) return NULL;
    if (*token == '\0' && trieNode->wordExists) return trieNode;

    if (trieNode->wordExists) {
        if (strcmp(token, word) == 0) return trieNode;
    }

    int pos = *token - 'a';
    if (trieNode->vector[pos]) {
        int index = strlen(word);
        word[pos] = 'a' + index;
        word[pos + 1] = '\0';
        return searchForSameElementsTrieREC(trieNode->vector[pos], token + 1, word);
    } else return NULL;
}

PtTrieNode searchForSameElementsTrie(PtTrie trie, char *token) {
    if (!trie) return NULL;
    char word[MAX_WORD_LENGTH];
    word[0] = '\0';

    return searchForSameElementsTrieREC(trie->start, token, word);
}

void rankingTrie(PtTrie trie, StringList stringList, int tokens) {
    if (!trie || !stringList) return;

    int i;
    PtTrieNode aux = NULL;
    PtRankingListNode rankNode;
    PtRankingList rankingList = createRankingList();
    if (!rankingList) return;

    int first = 1;
    int isValid = 1;
    printf("\nQUERY: [");
    for (i = 0; i < tokens; i++) {
        aux = searchForSameElementsTrie(trie, stringList[i]);

        printf("%s ", stringList[i]);



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
    printf("]\n");

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

void readFiles(PtTrie trie, char *fileName) {
    if (!trie) return;

    int i;
    FILE *f = fopen(fileName, "r");
    if (!f) return;

    int numberOfFiles = 0;
    fscanf(f, "%d", &numberOfFiles);
    trie->numberOfFiles = numberOfFiles;
    trie->filesArray = (Files *) malloc(trie->numberOfFiles * sizeof(Files));
    if (!trie->filesArray) return;

    char word[MAX_WORD_LENGTH];
    for (i = 0; i < numberOfFiles; i++) {
        char file[MAX_WORD_LENGTH];
        fscanf(f, "%s", file);
        trie->filesArray[i].fileName = (char *) malloc((strlen(file) + 1) * sizeof(char));
        if (!trie->filesArray[i].fileName) return;
        strcpy(trie->filesArray[i].fileName, file);
        trie->filesArray->count = 0;

        FILE *fileToRead = fopen(file, "r");
        if (!fileToRead) return;

        while (!feof(fileToRead)) {
            fscanf(fileToRead, "%s", word);
            strcpy(word, toLower(word));
            addWordToTrie(trie, word, i + 1, file);
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
        rankingTrie(trie, queries, countTokens);

        //! Free StringList
        for (j = 0; j < countTokens; j++) free(queries[j]);
        free(queries);
    }
    fclose(f);
}

int hashing(char *word) {
    return word[0] - 'a';
}

PtTrieNode addNodeToTrie(PtTrieNode trieNode, char *word, int idDoc, char *file) {
    if (!trieNode) return NULL;
    PtListNode listNode;

    if (*word == '\0') {
        trieNode->wordExists = TRUE;
        trieNode->wordCount++;
        if (!trieNode->files)
            trieNode->files = createList();
        listNode = searchForSameFile(trieNode->files, idDoc);
        if (listNode) listNode->info->counter++;
        else {
            listNode = createListNode(idDoc, file);
            if (!listNode) printf("\nSomething went wrong!");
            else addFileToListNode(trieNode->files, listNode);
        }
    } else {
        int pos = hashing(word);
        if (!trieNode->vector[pos]) trieNode->vector[pos] = createTrieNode();
        trieNode->vector[pos] = addNodeToTrie(trieNode->vector[pos], word + 1, idDoc, file);
    }
    return trieNode;
}

void addWordToTrie(PtTrie trie, char *word, int idDoc, char *file) {
    if (!trie) return;

    trie->start = addNodeToTrie(trie->start, word, idDoc, file);
}

void showTrieNodesREC(PtTrie trie, PtTrieNode trieNode, char *word) {
    if (!trieNode) return;

    if (trieNode->wordExists) {
        trie->count++;
        printf("\n[%s]\n", word);
        writeFilesList(trieNode->files);
    }
    for (int i = 0; i < MAX_CHAR; i++) {
        if (trieNode->vector[i]) {
            int pos = strlen(word);
            word[pos] = 'a' + i;
            word[pos + 1] = '\0';
            showTrieNodesREC(trie, trieNode->vector[i], word);
            word[strlen(word) - 1] = '\0';
        }
    }
}

void showTrie(PtTrie trie) {
    if (!trie) return;

    char word[MAX_WORD_LENGTH];
    word[0] = '\0';
    showTrieNodesREC(trie, trie->start, word);
}

void getWordsStatsREC(PtTrie trie, PtTrieNode trieNode, char *word, int *min, int *max, float *total, int *totalWords) {
    if (!trieNode) return;

    if (trieNode->wordExists) {
        trie->count++;
        *totalWords += trieNode->wordCount;
        *total += strlen(word);
        if (strlen(word) > *max) *max = strlen(word);
        if (strlen(word) < *min) *min = strlen(word);
    }
    for (int i = 0; i < MAX_CHAR; i++) {
        if (trieNode->vector[i]) {
            int pos = strlen(word);
            word[pos] = 'a' + i;
            word[pos + 1] = '\0';
            getWordsStatsREC(trie, trieNode->vector[i], word, min, max, total, totalWords);
            word[strlen(word) - 1] = '\0';
        }
    }
}

void getWordsStats(PtTrie trie) {
    if (!trie) return;

    int min = MAX_WORD_LENGTH, max = 0, totalWords = 0;
    float total = 0;
    char word[MAX_WORD_LENGTH];
    word[0] = '\0';
    getWordsStatsREC(trie, trie->start, word, &min, &max, &total, &totalWords);

    printf("\n\nTotal de palavras(sem ser repetidas): [%d]", trie->count);
    printf("\nNumero de caracteres da menor palavra: [%d]", min);
    printf("\nNumero de caracteres da maior palavra: [%d]", max);
    printf("\nTamanho medio de caracteres: [%.2f]", (total / (float) trie->count));
    printf("\nTotal de palavras (com repetidas): [%d]", totalWords);
}

int searchForWordInTrieREC(PtTrieNode trieNode, char *userWord, char *word) {
    if (!trieNode) return FALSE;
    if (*userWord == '\0' && trieNode->wordExists) return TRUE;

    if (trieNode->wordExists) {
        if (strcmp(userWord, word) == 0) return TRUE;
    }

    int pos = hashing(userWord);
    if (trieNode->vector[pos]) {
        int index = strlen(word);
        word[pos] = 'a' + index;
        word[pos + 1] = '\0';
        return searchForWordInTrieREC(trieNode->vector[pos], userWord + 1, word);
    } else return FALSE;
}

void searchForWordInTrie(PtTrie trie, char *userWord) {
    if (!trie) return;

    char word[MAX_WORD_LENGTH];
    word[0] = '\0';

    int i = searchForWordInTrieREC(trie->start, userWord, word);
    if (i == 1) printf("\n%s esta na Trie!", userWord);
    else printf("\n%s nao esta na Trie!", userWord);
}

void wordWithHighestAppearanceREC(PtTrieNode trieNode, char *word, char *maxWord, int *times) {
    if (!trieNode) return;

    if (trieNode->wordExists) {
        if (*times < trieNode->wordCount) {
            *times = trieNode->wordCount;
            strcpy(maxWord, word);
        }
    }
    for (int i = 0; i < MAX_CHAR; i++) {
        if (trieNode->vector[i]) {
            int pos = strlen(word);
            word[pos] = 'a' + i;
            word[pos + 1] = '\0';
            wordWithHighestAppearanceREC(trieNode->vector[i], word, maxWord, times);
            word[strlen(word) - 1] = '\0';
        }
    }
}

void wordWithHighestAppearance(PtTrie trie) {
    if (!trie) return;

    char word[MAX_WORD_LENGTH];
    word[0] = '\0';

    char maxWord[MAX_WORD_LENGTH];
    maxWord[0] = '\0';
    int times = 0;
    wordWithHighestAppearanceREC(trie->start, word, maxWord, &times);
    printf("\nPalavra: %s aparece %d veze(s)", maxWord, times);
}

void fileWithHigherAppearanceREC(PtTrieNode trieNode, Files *filesArray) {
    if (!trieNode) return;

    if (trieNode->wordExists) {
        PtListNode listNode = trieNode->files->start;
        if (!listNode) return;

        while (listNode) {
            filesArray[listNode->info->idDoc - 1].count += listNode->info->counter;
            listNode = listNode->next;
        }
    }
    for (int i = 0; i < MAX_CHAR; i++) {
        if (trieNode->vector[i]) {
            fileWithHigherAppearanceREC(trieNode->vector[i], filesArray);
        }
    }
}

void fileWithHighestAppearance(PtTrie trie) {
    if (!trie) return;

    fileWithHigherAppearanceREC(trie->start, trie->filesArray);
    int idMax = 0, value = 0;
    for (int i = 0; i < trie->numberOfFiles; i++) {
        if (trie->filesArray[i].count > value) {
            value = trie->filesArray[i].count;
            idMax = i;
        }
    }
    printf("Ficheiro: [%s] - Contagem: [%d]", trie->filesArray[idMax].fileName, trie->filesArray[idMax].count);
}

int countNodesInLevelREC(PtTrieNode trieNode, int currentLvl, int level) {
    if (!trieNode) return 0;
    if (currentLvl > level) return 0;
    if (currentLvl == level) return 1;

    int count = 0;
    for (int i = 0; i < MAX_CHAR; i++) {
        if (trieNode->vector[i])
            count += countNodesInLevelREC(trieNode->vector[i], currentLvl + 1, level);
    }
    return count;
}

void countNodesInLevel(PtTrie trie, int level) {
    if (!trie) return;

    printf("\nNumero de Nos: %d", countNodesInLevelREC(trie->start, 0, level));
}

int countWordsWithCertainLengthREC(PtTrieNode trieNode, int currentLvl, int length) {
    if (!trieNode) return 0;
    if (currentLvl == length) {
        if (trieNode->wordExists) return 1;
        else return 0;
    }

    int count = 0;
    for (int i = 0; i < MAX_CHAR; i++) {
        if (trieNode->vector[i])
            count += countWordsWithCertainLengthREC(trieNode->vector[i], currentLvl + 1, length);
    }
    return count;
}

void countWordsWithCertainLength(PtTrie trie, int length) {
    if (!trie) return;

    printf("\nNumero de Palavras com %d de tamanho: %d", length,
           countWordsWithCertainLengthREC(trie->start, 0, length));
}

int countWordsUnderCertainLengthREC(PtTrieNode trieNode, int currentLvl, int length) {
    if (!trieNode) return 0;
    if (currentLvl == length) {
        if (trieNode->wordExists) return 1;
        else return 0;
    }

    int count = 0;
    if (currentLvl < length) {
        if (trieNode->wordExists) count = 1;
    }
    for (int i = 0; i < MAX_CHAR; i++) {
        if (trieNode->vector[i])
            count += countWordsUnderCertainLengthREC(trieNode->vector[i], currentLvl + 1, length);
    }
    return count;
}

void countWordsUnderCertainLength(PtTrie trie, int length) {
    if (!trie) return;

    printf("\nNumero de Palavras abaixo de %d de tamanho: %d", length,
           countWordsUnderCertainLengthREC(trie->start, 0, length - 1));
}

size_t calculateRAM_REC(PtTrieNode trieNode) {
    if (!trieNode) return 0;

    size_t sum = sizeof(TrieNode) + calculateListRAM(trieNode->files);
    for (int i = 0; i < MAX_CHAR; i++) {
        if (trieNode->vector[i]) {
            sum += calculateRAM_REC(trieNode->vector[i]);
        }
    }
    return sum;
}

void calculateRAM(PtTrie trie) {
    if (!trie) return;

    printf("\n%d", sizeof(Trie) + calculateRAM_REC(trie->start));
}

void destroyTrieREC(PtTrieNode trieNode) {
    if (!trieNode) return;

    for (int i = 0; i < MAX_CHAR; i++) {
        if (trieNode->vector[i]) destroyTrieREC(trieNode->vector[i]);
    }

    if (trieNode->files) destroyList(trieNode->files);
    free(trieNode);
}

void destroyTrie(PtTrie trie) {
    //notImplemented(__FUNCTION__);
    if (!trie) return;

    destroyTrieREC(trie->start);
    free(trie);
}

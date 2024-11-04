#include <stdio.h>
#include <locale.h>
#include <time.h>
#include "Header Files/Trie/TrieStruct.h"
#include "Header Files/AuxList/AuxList.h"

#define TEST_FILE "Demo.txt"
#define INPUT_FILE "TesteCompleto.txt"

/* André Miguel (19889) | Rafael Monteiro (19905) | Rui Simões (19482) | Sérgio Baptista (14606) */
int menu() {
    printf("\n");
    printf("1 - Lista \n");
    printf("2 - Trie \n");
    printf("0 - Sair\n");
    int opcao;
    printf("Qual a opcao:");
    scanf("%d", &opcao);
    return opcao;
}

int subMenu() {
    printf("\n");
    printf("1 - Palavra que ocorre mais vezes \n");
    printf("2 - Documento que contem mais palavras \n");
    printf("3 - Contar nr de Nos num dado nivel \n");
    printf("4 - Numero de palavras com determinado numero de caracteres \n");
    printf("5 - Numero de palavras abaixo de um determinado numero de caracteres \n");
    printf("6 - Verificar se palavra existe \n");
    printf("7 - Memoria que existe \n");
    printf("8 - Destruir Trie \n");
    printf("0 - Sair\n");
    int opcao;
    printf("Qual a opcao:");
    scanf("%d", &opcao);
    return opcao;
}

int main() {
    setlocale(LC_ALL, "");
    double time_spent;
    clock_t begin;
    clock_t end;

    char pal[MAX_WORD_LENGTH];
    int level, length;

    int option;
    do {
        AuxList *auxList = createAuxList();
        Trie *trie = createTrie();
        option = menu();
        switch (option) {
            case 1:
                time_spent = 0.0;
                begin = clock();

                readFilesAux(auxList, TEST_FILE);
                //showAuxList(auxList);
                getAuxListWordsStats(auxList);
                destroyAuxList(auxList);
                end = clock();
                time_spent += (double) (end - begin) / CLOCKS_PER_SEC;
                printf("\n\nThe elapsed time is %f seconds\n", time_spent);
                break;
            case 2 :
                time_spent = 0.0;
                begin = clock();

                readFiles(trie, INPUT_FILE);
                //showTrie(trie);
                getWordsStats(trie);
                end = clock();
                time_spent += (double) (end - begin) / CLOCKS_PER_SEC;
                printf("\n\nO tempo de execucao foi de %f segundos\n", time_spent);

                int subOption;
                do {
                    subOption = subMenu();
                    switch (subOption) {
                        case 1 :
                            wordWithHighestAppearance(trie);
                            break;
                        case 2 :
                            fileWithHighestAppearance(trie);
                            break;
                        case 3 :
                            printf("\nNivel: ");
                            scanf("%d", &level);
                            countNodesInLevel(trie, level);
                            break;
                        case 4 :
                            printf("\nTamanho: ");
                            scanf("%d", &length);
                            countWordsWithCertainLength(trie, length);
                            break;
                        case 5 :
                            printf("\nTamanho: ");
                            scanf("%d", &length);
                            countWordsUnderCertainLength(trie, length);
                            break;
                        case 6:
                            printf("\nPalavra: ");
                            fflush(stdin);
                            gets(pal);
                            searchForWordInTrie(trie, pal);
                            break;
                        case 7:
                            calculateRAM(trie);
                            break;
                        case 8:
                            destroyTrie(trie);
                            subOption = 0;
                            break;
                    }
                } while (subOption != 0);
                break;
        };
    } while (option != 0);

    return 0;
}
cmake_minimum_required(VERSION 3.19)
project(ProjetoFinalED C)

set(CMAKE_C_STANDARD 99)

add_executable(ProjetoFinalED main.c "Header Files/Trie/TrieFunctions.c" "Header Files/Trie/TrieStruct.h" "Header Files/AuxList/AuxList.h" "Header Files/List/ListStruct.h" "Header Files/Utils/Utils.h" "Header Files/AuxList/AuxList.c" "Header Files/List/ListFunctions.c" "Header Files/Utils/Utils.c" "Header Files/RankingList/RankingStruct.h" "Header Files/RankingList/RankingFunctions.c")
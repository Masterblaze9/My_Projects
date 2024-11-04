#include <stdio.h>
#include <time.h>
#include <ctype.h>

void notImplemented(char *function) { printf("\nFunction %s not implemented yet", function); }

void wait(int mlSeconds) {
    clock_t endwait;
    endwait = clock() + mlSeconds;
    while (clock() < endwait);
}

void waitSeconds(int seconds) {
    clock_t endwait;
    endwait = clock() + seconds * CLOCKS_PER_SEC;
    while (clock() < endwait);
}

char *toLower(char *s) {
    for (char *w = s; *w; w++) *w = tolower(*w);
    return s;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "utils.h"

void printContents(char *path, int setOption1, int setOption2) {
    int lineNumber = 1;

    FILE *ptr = fopen(path, "r");
    if (ptr == NULL) {
        printf("%s", strerror(errno));
        return;
    }
    char ch;
    ch = fgetc(ptr);
    if (setOption2) printf("%5d  ", lineNumber++);
    while (ch != EOF) {
        if (setOption1 && ch == '\n') printf("$");
        printf("%c", ch);
        char chCopy = ch;
        ch = fgetc(ptr);
        if (setOption2 && chCopy == '\n' && ch != EOF) printf("%5d  ", lineNumber++);
    }
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        printf("too few arguments\n");
        return 0;
    }

    // Define options
    char option1 = 'E';
    char option2 = 'n';
    int setOption1 = 0;
    int setOption2 = 0;

    // Define files array
    char **files = getFilesArray();
    int numFiles = 0;


    // Extract arguments and options
    extractArguments(files, argv, argc, option1, option2, &setOption1, &setOption2, &numFiles);

    getAbsolutePath(files, numFiles);

    for (int i = 0; i < numFiles; ++i) {
        printContents(files[i], setOption1, setOption2);
    }
    printf("\n");
    return 0;
}

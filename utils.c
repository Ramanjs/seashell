#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

char** tokeniseString(char* string, int *args, char token) {
    char** tokenisedString = (char**)malloc(100 * sizeof(char*));
    for (int i = 0; i < 100; ++i) {
        tokenisedString[i] = (char*)malloc(100 * sizeof(char));
        tokenisedString[i][0] = '\0';
    }
    int index = 0;
    for (int i = 0; i < strlen(string); ++i) {
        if (string[i] != token) {
            strncat(tokenisedString[index], &string[i], 1);
        } else {
            index++;
            (*args)++;
        }
    }
    index++;
    (*args)++;
    tokenisedString[index] = (char *)NULL;
    return tokenisedString;
}

char **getFilesArray() {
    char **files = malloc(10 * sizeof(char*));
    for (int i = 0; i < 10; ++i) {
        files[i] = malloc(100 * sizeof(char));
        files[i][0] = '\0';
    }
    return files;
}

void extractArguments(char **files, char *argv[], int argc, char option1, char option2, int *setOption1, int *setOption2, int *numFiles) {
    // Extract arguments and options
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            for (int j = 0; j < strlen(argv[i]); j++) {
                if (argv[i][j] == option1) *setOption1 = 1;
                if (argv[i][j] == option2) *setOption2 = 1;
            }
        } else {
            strcat(files[*numFiles], argv[i]);
            (*numFiles)++;
        }
    }
}

void getAbsolutePath(char **files, int numFiles) {
    // prefix every file name with PWD
    for (int i = 0; i < numFiles; ++i) {
        char temp[100];
        strcpy(temp, files[i]);
        strcpy(files[i], getenv("PWD"));
        strcat(files[i], "/");
        strcat(files[i], temp);
    }
}

int isDir(char *path) {
    struct stat attrib;
    return stat(path, &attrib) == 0 && S_ISDIR(attrib.st_mode);
}

int isLink(char *path) {
    struct stat attrib;
    return lstat(path, &attrib) == 0 && S_ISLNK(attrib.st_mode);
}

int isFile(char *path) {
    struct stat attrib;
    return lstat(path, &attrib) == 0 && S_ISREG(attrib.st_mode);
}

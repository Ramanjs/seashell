#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>

#include "utils.h"

void createDirectory(char *path, int setOption1, int setOption2) {
    char* pwd;
    pwd = getenv("PWD");

    if (setOption1) {
        int depth = 0;
        char **dirTree = tokeniseString(path, &depth, '/');

        for (int i = 0; i < depth; i++) {
            strcat(pwd, "/");
            strcat(pwd, dirTree[i]);
            if (!isDir(pwd)) {
                int status = mkdir(pwd, 0777);
                if (status != 0) {
                    printf("%s\n", strerror(errno));
                } else if (setOption2) {
                    printf("Created directory %s\n", pwd);
                }
            }
        }
    } else {
        strcat(pwd, "/");
        strcat(pwd, path);
        int status = mkdir(pwd, 0777);
        if (status != 0) {
            printf("%s\n", strerror(errno));
        } else if (setOption2) {
            printf("Created directory %s\n", pwd);
        }
    }
}

int main(int argc, char *argv[]) {

    // Define options
    char option1 = 'p';
    char option2 = 'v';
    int setOption1 = 0;
    int setOption2 = 0;


    // Define files array
    char **files = getFilesArray();
    int numFiles = 0;

    extractArguments(files, argv, argc, option1, option2, &setOption1, &setOption2, &numFiles);

    for (int i = 0; i < numFiles; ++i) {
        createDirectory(files[i], setOption1, setOption2);
    }

    return 0;
}

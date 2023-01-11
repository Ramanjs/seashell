#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <pthread.h>
#include <limits.h>

#include "utils.h"

int isExternalCommand(char *command) {
    int result = 0;
    if (strcmp(command, "lsss") == 0) result = 1;
    if (strcmp(command, "catss") == 0) result = 1;
    if (strcmp(command, "datess") == 0) result = 1;
    if (strcmp(command, "rmss") == 0) result = 1;
    if (strcmp(command, "mkdirss") == 0) result = 1;
    return result;
}

void *worker(void *arg) {
    char *command = arg;
    char temp[longStringLength];
    getcwd(temp, longStringLength);
    chdir(getenv("PWD"));

    system(command);

    chdir(temp);

    return NULL;
}

int getEscapeCharacter(char ch) {
    if (ch == 92) return 92;  // backslash '\\'
    if (ch == 'a') return 7;  // alert '\a'
    if (ch == 'b') return 8;  // backspace '\b'
    if (ch == 'e') return 27; // escape '\e'
    if (ch == 'f') return 12; // form feed '\f'
    if (ch == 'n') return 10; // new line '\n'
    if (ch == 'r') return 13; // carriage return '\r'
    if (ch == 't') return 9;  // horizontal tab '\t'
    if (ch == 'v') return 11; // vertical tab '\v'
    return 0;
}

void echo(char **tokenisedCommand, int args) {
    char option1 = 'n';
    char option2 = 'e';
    int setOption1 = 0;
    int setOption2 = 0;

    char **strings = getFilesArray();
    int numStrings = 0;

    extractArguments(strings, tokenisedCommand, args, option1, option2, &setOption1, &setOption2, &numStrings);

    char stringBuilder[longStringLength];
    stringBuilder[0] = '\0';
    for (int i = 0; i < numStrings; ++i) {
        if (setOption2) {
            char escapeString[wordLength];
            int index = 0;
            for (int j = 0; j < strlen(strings[i]); ++j) {
                char ch = strings[i][j];
                if (ch == 92 && j + 1 < strlen(strings[i])) {
                    ch = getEscapeCharacter(strings[i][j + 1]);
                    j++;
                }
                escapeString[index++] = ch;
            }
            if (i != 0) strcat(stringBuilder, " ");
            strcat(stringBuilder, escapeString);
        } else {
            if (i != 0) strcat(stringBuilder, " ");
            strcat(stringBuilder, strings[i]);
        }
    }
    printf("%s", stringBuilder);
    if (!setOption1) printf("\n");
}

void cd(char **tokenisedCommand, int args) {
    char option1 = 'L';
    char option2 = 'P';
    int setOption1 = 1;
    int setOption2 = 0;

    // Define files array
    char **files = getFilesArray();
    int numFiles = 0;

    extractArguments(files, tokenisedCommand, args, option1, option2, &setOption1, &setOption2, &numFiles);

    if (numFiles > 1) {
        printf("Too many arguments\n");
        return;
    }

    if (numFiles == 0) {
        setenv("PWD", getenv("HOME"), 1); // set PWD to HOME
        return;
    }

    char* cwd;
    cwd = getenv("PWD");

    if (strcmp(files[0], "../") == 0 || strcmp(files[0], "..") == 0) {
        if (strlen(cwd) == 0) {
            return;
        }

        int lastSlashIndex = strlen(cwd) - 1;
        while (cwd[lastSlashIndex] != '/') {
            lastSlashIndex--;
        }
        char substr[wordLength];
        memcpy(substr, &cwd[0], lastSlashIndex);
        substr[lastSlashIndex] = '\0';
        setenv("PWD", substr, 1);
        return;
    }

    char newPath[longStringLength];
    strcpy(newPath, cwd);
    strcat(newPath, "/");
    strcat(newPath, files[0]);

    if (!isLink(newPath) && !isDir(newPath)) {
        printf("Not a directory\n");
        return;
    }

    if (setOption2 || !isLink(newPath)) {
        char *temp;
        temp = realpath(newPath, NULL);
        if (temp == NULL) {
            printf("No such file or directory\n");
            return;
        }
        strcpy(newPath, temp);
    }
    setenv("PWD", newPath, 1);
}

void pwd(char **tokenisedCommand, int args) {
    char option1 = 'L';
    char option2 = 'P';
    int setOption1 = 1;
    int setOption2 = 0;

    // Define files array
    char **files = getFilesArray();
    int numFiles = 0;

    extractArguments(files, tokenisedCommand, args, option1, option2, &setOption1, &setOption2, &numFiles);

    if (numFiles > 0) {
        printf("Too many arguments\n");
        return;
    }

    char* cwd;
    cwd = getenv("PWD");

    if (setOption2) {
        cwd = realpath(cwd, NULL);
    }
    printf("%s\n", cwd);
}

int main() {
    // export pwd to path
    char path[longStringLength];
    getcwd(path, longStringLength);
    strcat(path, "/");

    char* PATHVAR = getenv("PATH");
    char newPath[longStringLength];
    strcpy(newPath, path);
    strcat(newPath, ":");
    strcat(newPath, PATHVAR);
    setenv("PATH", newPath, 1);

    printf("Welcome to SeaShell!\n");

    char* command;

    while (1) {
        command = readline(">> ");
        if (strlen(command) > 0) {
            int args = 0;
            char **tokenisedCommand = tokeniseString(command, &args, ' ');
            if (strcmp("pwd", tokenisedCommand[0]) == 0) {
                pwd(tokenisedCommand, args);
            } else if (strcmp("cd", tokenisedCommand[0]) == 0) {
                cd(tokenisedCommand, args);
            } else if (strcmp("echo", tokenisedCommand[0]) == 0) {
                echo(tokenisedCommand, args);
            } else if (isExternalCommand(tokenisedCommand[0])) {
                if (strcmp(tokenisedCommand[args - 1], "&t") == 0) {
                    char externalCommand[longStringLength];
                    externalCommand[0] = '\0';
                    for (int i = 0; i < args - 1; ++i) {
                        strcat(externalCommand, tokenisedCommand[i]);
                        strcat(externalCommand, " ");
                    }
                    pthread_t newThread;
                    pthread_create(&newThread, NULL, &worker, externalCommand);
                    pthread_join(newThread, NULL);
                } else {
                    int rc = fork();
                    if (rc == 0) {
                        char buffer[longStringLength];
                        strcpy(buffer, path);
                        strcat(buffer, tokenisedCommand[0]);
                        if (execv(buffer, &tokenisedCommand[0]) == -1) {
                            printf("%s\n", strerror(errno));
                        }
                        return 0;
                    } else {
                        int wstatus;
                        waitpid(rc, &wstatus, 0);
                    }
                }
            } else if (strcmp("exit", tokenisedCommand[0]) == 0) {
                printf("Thanks for visiting SeaShell!\n");
                break;
            } else {
                printf("%s: command not found\n", tokenisedCommand[0]);
            }
        }
        free(command);
    }
    return 0;
}

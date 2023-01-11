#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <errno.h>

#include "utils.h"

int main(int argc, char *argv[]) {

    // Define options
    char option1 = 'R';
    char option2 = 'u';
    int setOption1 = 0;
    int setOption2 = 0;

    // Define files array
    char **files = getFilesArray();
    int numFiles = 0;

    extractArguments(files, argv, argc, option1, option2, &setOption1, &setOption2, &numFiles);

    if (numFiles > 1) {
        printf("Too many arguments\n");
        exit(1);
    }

    time_t rTime = time(NULL);

    if (rTime == -1) {
        printf("Failed to get time\n");
        exit(1);
    }

    struct tm *ftime;

    char time[dateTimeLength];

    if (setOption2) {
        ftime = gmtime(&rTime);
        strftime(time, sizeof(time), "%A %d %B %Y %I:%M:%S %p UTC", ftime);
        printf("%s\n", time);
    } else if (setOption1) {
        ftime = localtime(&rTime);
        strftime(time, sizeof(time), "%a, %d %b %Y %T %z", ftime);
        printf("%s\n", time);
    } else {
        ftime = localtime(&rTime);
        if (numFiles == 1) {
            strftime(time, sizeof(time), files[0], ftime);
        } else {
            int status = strftime(time, sizeof(time), "%A %d %B %Y %I:%M:%S %p %Z", ftime);
            if (status == 0) {
                printf("%s\n", strerror(errno));
                exit(1);
            }
        }
        printf("%s\n", time);
    }

    exit(0);
}

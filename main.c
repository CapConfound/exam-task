#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
    char fileString[200];
    FILE *file;
    int startPoints[100];
    int endPoints[100];

    char *remainder;
    remainder = malloc(sizeof(char) * 200);

    printf("Hello, World!\n");

    if (!(file = fopen("./network1.txt", "r"))) {
        puts("ur done, c ya!");
        return 0;
    }

    puts("startf");

    while (fgets(fileString, 200, file)) {
        printf("%s", fileString);
        remainder = strncpy(remainder, fileString+1, strlen(fileString) - 4);

        printf("remainder %s\n", remainder);
    }
    puts("endf");


    return 0;
}

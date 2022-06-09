#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define true 1
#define false 0


FILE *MYFILE;
const int DEBUG = true;
const int LINES = 2;
const int SWITCH_N = 11;
const int HOSTS_N = 12;
const int VERTICLES = SWITCH_N + HOSTS_N;


int open_file(char filename[])
{
    if (!(MYFILE = fopen(filename, "r"))) {
        puts("An error occured. File not found.");
        puts("current directory");
        system("pwd");
        return 0;
    }

    return 1;
}

int numPlaces (int n) {
    if (n < 0) return numPlaces ((n == INT_MIN) ? INT_MAX: -n);
    if (n < 10) return 1;
    return 1 + numPlaces (n / 10);
}


char *strshift(char *string)
{
    if (!string) return 0;
    char *result = NULL;
    char *pattern = "[%u, %u], ";
    char match[200];
    char pin = 0;
    int digit = 0;
    int shift = 0;

    if (*string != '[') return NULL;

    digit = atoi(string+1);
    //string += numPlaces(digit);



    int actual_length = strlen(string) - shift;

    result = malloc(sizeof(char) * actual_length);
    if (!result) return 0;
    for (int i = 0; i < actual_length; i++) {
        result[i] = string[i+shift];
    }
    result[actual_length+1] = '\0';

    return result;
}

int main() {
    char filename[] = "../network1.txt"; // название файла
    char fileString[200];
    char *remainder; // остаток от строки
    char v_arr[LINES][200]; // массив с обоими строками
    char *format;
    int i = 0;
    int graph[VERTICLES][VERTICLES];
    size_t str_size; // полезный размер строчки

    if (!open_file(filename)) return 0;

    remainder = malloc(sizeof(char) * 200);
    format = malloc(sizeof(char) * 4 );

    i = 0;
    while (fgets(fileString, 200, MYFILE)) {
        str_size = strlen(fileString) - 3;
//        malloc(sizeof(char) * str_size);

        // отформатировал строку
//        malloc(sizeof(char) * str_size);
        remainder = strncpy(remainder, fileString + 1, str_size);
        *(remainder + strlen(remainder)) = '\0';
        char h = *(remainder + strlen(remainder));
        if (DEBUG) {
            printf("remainder \n%s\n", remainder);
        }
        str_size = strlen(remainder);
        strcpy(v_arr[i], remainder);
        i++;
        free(remainder);
    }

    if (DEBUG) {
        i = 0;
        while (i < LINES) {
            printf("v_arr[%d] %s\n", i, v_arr[i]);
            i++;
        }
    }

    // указал формат
    format = strcpy(format, "[%u,");


    int v_from = 0, v_to = 0;

    for (i = 0; i < VERTICLES; i++) {
        for (int j = 0; j < VERTICLES; j++) {
            graph[i][j] = 0;
        }
    }

    for (i = 0; i < VERTICLES; i++) {

        sscanf(v_arr[0], format, &v_from);
        sscanf(v_arr[1], format, &v_to);

        graph[v_from-1][v_to-1] = graph[v_to-1][v_from-1] = 1;

        strcpy(v_arr[0], strshift(v_arr[0]));
        strcpy(v_arr[1], strshift(v_arr[1]));
    }

    for (i = 0; i < VERTICLES; i++) {
        for (int j = 0; j < VERTICLES; j++) {
            printf(" %d ", graph[i][j]);

        }
        printf("\n");
    }




    puts("endf");


    return 0;
}

//
// Created by ilya1 on 10.06.2022.
//

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>



#define true 1
#define false 0


FILE* MYFILE;
errno_t err;
const int DEBUG = false;
const int LINES = 2;
const int SWITCH_N = 11;
const int HOSTS_N = 12;
const int VERTICES = SWITCH_N + HOSTS_N;


int open_file(char *filename)
{
    if (fopen_s(&MYFILE, filename, "r")) {
        puts("An error occured. File not found.");
        puts("current directory");
        system("pwd");
        return 0;
    }

    return 1;
}

int numPlaces(int n) {
    if (n < 0) return numPlaces((n == INT_MIN) ? INT_MAX : -n);
    if (n < 10) return 1;
    return 1 + numPlaces(n / 10);
}


char* strshift(char* string)
{
    if (!string) return 0;
    char* result = NULL;
    char pattern[] = "[%u, %u], ";
    char* temp;
    char match[200];
    char pin = 0;
    int digit = 0;
    int shift = 1;

    if (!string) return NULL;

    if (*string != '[' || !(string + shift)) return NULL;

    digit = atoi(string + shift);
    shift += numPlaces(digit) + 2;


    digit = atoi(string + shift);
    shift += numPlaces(digit) + 1;

    if (string[shift-1] == '\0') {
        //printf("%c", check);
    }
    else {
        shift += 2;
    }

    int actual_length = strlen(string) - shift;

    if (actual_length == 0) {
        return NULL;
    }
    result = (char *)malloc(sizeof(char) * actual_length);
    if (!result) return NULL;
    for (int i = 0; i < actual_length; i++) {
        result[i] = string[i + shift];
    }
    result[actual_length + 1] = '\0';

    return result;
}

int main() {
    char filename[] = "network1.txt"; // название файла
    char fileString[200];
    char remainder[200]; // остаток от строки
    char v_arr[LINES][200]; // массив с обоими строками
    char format[] = "[%u,";
    int i = 0;
    int graph[VERTICES][VERTICES];
    size_t str_size; // полезный размер строчки

    if (!open_file(filename)) return 0;

    //    remainder = malloc(sizeof(char) * 200);


    i = 0;
    while (fgets(fileString, 200, MYFILE)) {
        str_size = strlen(fileString) - 3;

        // отформатировал строку
        strncpy_s(remainder, fileString + 1, str_size);
        remainder[str_size] = '\0';
        char h = remainder[strlen(remainder)];
        if (DEBUG) {
            printf("remainder \n%s\n", remainder);
        }
        str_size = strlen(remainder);
        strcpy_s(v_arr[i], remainder);
        i++;
        remainder[0] = '\0';
    }

    if (DEBUG) {
        i = 0;
        while (i < LINES) {
            printf("v_arr[%d] %s\n", i, v_arr[i]);
            i++;
        }
    }

    // указал формат

    int v_from = 0, v_to = 0;

    for (i = 0; i < VERTICES; i++) {
        for (int j = 0; j < VERTICES; j++) {
            graph[i][j] = 0;
        }
    }

    char* temp = NULL;
    int j = 11;
    for (i = 0; *v_arr[0] != 0 && i < VERTICES; i++) {

        sscanf(v_arr[0], format, &v_from);
        sscanf(v_arr[1], format, &v_to);

        v_from = v_from == 0 ? j++ : v_from;

        graph[v_from - 1][v_to - 1] = graph[v_to - 1][v_from - 1] = 1;

        if ((temp = strshift(v_arr[0])) != NULL) {
            strcpy_s(v_arr[0], temp);
        }

        if ((temp = strshift(v_arr[1])) != NULL) {
            strcpy_s(v_arr[1], temp);
        }

    }

    for (i = 0; i < VERTICES; i++) {
        for (int j = 0; j < VERTICES; j++) {
            printf(" %d ", graph[i][j]);
        }
        printf("\n");
    }

    return 0;
}

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>


#define true 1
#define false 0
#define VERTICES MAX


FILE* MYFILE;
errno_t err;
const int DEBUG = false;
const int LINES = 2;
const int SWITCH_N = 11;
const int HOSTS_N = 12;
const int VERTICES = SWITCH_N + HOSTS_N;
int G[VERTICES][VERTICES], visited[VERTICES], n;

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

void dijkstra(int G[VERTICES][VERTICES], int n, int startnode)
{

    int cost[VERTICES][VERTICES], distance[VERTICES], pred[VERTICES];
    int visited[VERTICES], count, mindistance, nextnode = 0, i, j;
    //pred[] stores the predecessor of each node
    //count gives the number of nodes seen so far
    //create the cost matrix
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            if (G[i][j] == 0)
                cost[i][j] = INFINITY;
            else
                cost[i][j] = G[i][j];
    //initialize pred[],distance[] and visited[]
    for (i = 0; i < n; i++)
    {
        distance[i] = cost[startnode][i];
        pred[i] = startnode;
        visited[i] = 0;
    }
    distance[startnode] = 0;
    visited[startnode] = 1;
    count = 1;
    while (count < n - 1)
    {
        mindistance = INFINITY;
        //nextnode gives the node at minimum distance
        for (i = 0; i < n; i++)
            if (distance[i] < mindistance && !visited[i])
            {
                mindistance = distance[i];
                nextnode = i;
            }
        //check if a better path exists through nextnode
        visited[nextnode] = 1;
        for (i = 0; i < n; i++)
            if (!visited[i])
                if (mindistance + cost[nextnode][i] < distance[i])
                {
                    distance[i] = mindistance + cost[nextnode][i];
                    pred[i] = nextnode;
                }
        count++;
    }

    //print the path and distance of each node
    for (i = 0; i < n; i++)
        if (i != startnode)
        {
            printf("\nDistance of node%d=%d", i, distance[i]);
            printf("\nPath=%d", i);
            j = i;
            do
            {
                j = pred[j];
                printf("<-%d", j);
            } while (j != startnode);
        }
}

void DFS(int i)
{
    int j;
    printf("\n%d", i);
    visited[i] = 1;
    for (j = 0; j < n; j++)
        if (!visited[j] && G[i][j] == 1)
            DFS(j);
}

int main()
{
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
    int j = 12;
    for (i = 0; *v_arr[0] != 0 && i < VERTICES; i++) {

        sscanf(v_arr[0], format, &v_from);
        sscanf(v_arr[1], format, &v_to);

        v_from = v_from == 0 ? j++ : v_from;

        graph[v_from - 1][v_to - 1] = graph[v_to - 1][v_from - 1] = 1;

        printf("Вершины - %d - %d\n", v_from, v_to);

        if ((temp = strshift(v_arr[0])) != NULL) {
            strcpy_s(v_arr[0], temp);
        }

        if ((temp = strshift(v_arr[1])) != NULL) {
            strcpy_s(v_arr[1], temp);
        }

    }

    puts("матрица смежности\n");
    printf("     ");
    for (int i = 0; i < VERTICES; i++) {

        printf("%d ", i+1);
        if (i < 9) printf(" ", i);
    }

    putchar('\n');
    for (int i = 0; i < VERTICES*1.6; i++) {
        printf("--");
    }



    putchar('\n');
    for (i = 0; i < VERTICES; i++) {
        if (i < 9) printf(" ", i);
        printf("%d| ", i+1);
        for (int j = 0; j < VERTICES; j++) {

            printf(" %d ", graph[i][j]);
        }
        printf("\n");
    }


    // че делать будем?


    dijkstra(graph, VERTICES, 2);


    puts("");

    return 0;
}


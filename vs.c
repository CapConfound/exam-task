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
int G[VERTICES][VERTICES], visited[VERTICES];
int q[20], n, i, j, f = 0, r = -1;

typedef struct node {
    int vertex;
    struct node* next;
};

typedef struct Graph {
    int numVertices;
    int* visited;

    // Нам нужен int** для хранения двумерного массива.
    // Аналогично, нам нужна структура node** для хранения массива связанных списков.
    struct node** adjLists;
};

void DFS(struct Graph* graph, int vertex) {
    struct node* adjList = graph->adjLists[vertex];
    struct node* temp = adjList;

    graph->visited[vertex] = 1;
    printf("Visited %d \n", vertex);

    while (temp != NULL) {
        int connectedVertex = temp->vertex;

        if (graph->visited[connectedVertex] == 0) {
            DFS(graph, connectedVertex);
        }
        temp = temp->next;
    }

    for (int i = 0; i < VERTICES; i++) {
        printf(" %d", i+1);
    }

    printf("\n");

    for (int i = 0; i < VERTICES; i++) {
        if (i > 9) printf(" ");
        printf(" %d", graph->visited[i]);
    }
    printf("\n");
}

struct node* createNode(int v) {
    struct node* newNode = (node *)malloc(sizeof(struct node));
    newNode->vertex = v;
    newNode->next = NULL;
    return newNode;
}

// Создание графа
struct Graph* createGraph(int vertices) {
    struct Graph* graph = (Graph *)malloc(sizeof(struct Graph));
    graph->numVertices = vertices;

    graph->adjLists = (node **)malloc(vertices * sizeof(struct node*));

    graph->visited = (int *)malloc(vertices * sizeof(int));

    int i;
    for (i = 0; i < vertices; i++) {
        graph->adjLists[i] = NULL;
        graph->visited[i] = 0;
    }
    return graph;
}

// Добавление ребра
void addEdge(struct Graph* graph, int src, int dest) {
    // Проводим ребро от начальной вершины ребра графа к конечной вершине ребра графа
    struct node* newNode = createNode(dest);
    newNode->next = graph->adjLists[src];
    graph->adjLists[src] = newNode;

    // Проводим ребро из конечной вершины ребра графа в начальную вершину ребра графа
    newNode = createNode(src);
    newNode->next = graph->adjLists[dest];
    graph->adjLists[dest] = newNode;
}

// Выводим граф
void printGraph(struct Graph* graph) {
    int v;
    for (v = 0; v < graph->numVertices; v++) {
        struct node* temp = graph->adjLists[v];
        printf("\n Adjacency list of vertex %d\n ", v);
        while (temp) {
            printf("%d -> ", temp->vertex);
            temp = temp->next;
        }
        printf("\n");
    }
}







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
    Graph* graph1 = createGraph(VERTICES);

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
        addEdge(graph1, v_from, v_to);

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


    puts("");


    printGraph(graph1);

    DFS(graph1, 1);


    return 0;
}

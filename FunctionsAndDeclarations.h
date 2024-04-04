#include <stdbool.h>
#include <stdlib.h>
#ifndef def FUNC_DEC
#define FUNC_DEC

#define ERR 1
#define OK 0

typedef struct node {
    struct node* up, *right, *left, *upRight, *downLeft, *down, *downRight, *upLeft;
    int type;
    struct node* parent;
    int xpos, ypos;
    // struct for nodes in graph
    int f_cost, g_cost, h_cost;
}Node;

Node* map[10][10];
Node* bus;
char busians[10];
int busians_index = 0;


typedef struct Stack {
    int top;
    int size;
    Node* path[70];
}stack;

typedef struct Task {
    int cost;
    stack* path_stack;
    int starting_x, starting_y;
    int ending_x, ending_y;
    bool pickup_mode;
    char name;
}task;

typedef struct min_heap_priority {
    int heap_size;
    task* heap_data[70];
}heap_priority;

typedef struct min_heap {
    int heap_size;
    Node* heap_data[70];
}heap;

int rear = -1, front = -1;
int temp_queue_size = 0;
task* temp_queue[70];

task* temporaryHeapStorage[70];

struct min_heap_priority* initialize_priority();

void heapify_priority(heap_priority* heap, task* T, int element_index);

void insert_priority(heap_priority* heap, task* T);

task* pop_heap_priority(heap_priority* heap);

int getGHCost(Node* current, Node* end);

int checkFull(stack* path_stack);

void push(Node* current, stack* path_stack);

int checkEmpty(stack* path_stack);

Node* pop(stack* path_stack);

int getFCost(Node* current, Node* neighbour, Node* target, int check);

bool checkArray(Node* neighbour, Node* array[], int size);

struct min_heap* initialize();

void heapify(heap* heap, Node* node, int node_index);

void insert(heap* heap, Node* node);

Node* pop_heap(heap* heap);

bool Astar(Node* start, Node* target, task* T);

task* initialize_task(int xi, int yi, int xf, int yf, char name);

int path_cost (task* T);

char searchQueue(heap_priority* priorityQueue, int j, int k);

void print_map_user (heap_priority* priorityQueue);

void print_map_computer ();

Node* createNode (int i, int j);

void random1 ();

void random2 ();

Node* path_finding_initialization ();

task* add_route (int xi, int yi, int xf, int yf, heap_priority* queue, char name);

bool QIsFull();

bool QIsEmpty();

bool enqueue(task* T);

task* dequeue(task* t);

void Simulate(heap_priority* priorityQueue);

void takeStep(stack* path_stack);



#endif
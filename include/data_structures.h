#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include <stdbool.h>
#include <stdlib.h>

#define ERR 1
#define OK 0

// A grid node stores its neighbors, map position, terrain type, and A* costs.
typedef struct node {
    struct node* up;
    struct node* right;
    struct node* left;
    struct node* upRight;
    struct node* downLeft;
    struct node* down;
    struct node* downRight;
    struct node* upLeft;
    int type;
    struct node* parent;
    int xpos;
    int ypos;
    int f_cost;
    int g_cost;
    int h_cost;
} Node;

typedef struct Stack {
    int top;
    int size;
    Node* path[70];
} stack;

// A task represents one pickup and dropoff request for the bus.
typedef struct Task {
    int cost;
    stack* path_stack;
    int starting_x;
    int starting_y;
    int ending_x;
    int ending_y;
    bool pickup_mode;
    char name;
} task;

typedef struct min_heap_priority {
    int heap_size;
    task* heap_data[70];
} heap_priority;

// The A* open list is also stored as a min-heap, but over nodes instead of tasks.
typedef struct min_heap {
    int heap_size;
    Node* heap_data[70];
} heap;

extern Node* map[10][10];
extern Node* bus;
extern int rear;
extern int front;
extern int temp_queue_size;
extern task* temp_queue[70];

// Task-priority heap helpers.
struct min_heap_priority* initialize_priority(void);
void heapify_priority(heap_priority* heap, task* T, int element_index);
void insert_priority(heap_priority* heap, task* T);
task* pop_heap_priority(heap_priority* heap);

int checkFull(stack* path_stack);
void push(Node* current, stack* path_stack);
int checkEmpty(stack* path_stack);
Node* pop(stack* path_stack);
bool checkArray(Node* neighbour, Node* array[], int size);

// Node heap helpers used by the A* frontier.
struct min_heap* initialize(void);
void heapify(heap* heap, Node* node, int node_index);
void insert(heap* heap, Node* node);
Node* pop_heap(heap* heap);

// Temporary queue helpers used when task costs are recalculated after a step.
bool QIsFull(void);
bool QIsEmpty(void);
bool enqueue(task* T);
task* dequeue(task* t);

#endif


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <string.h>
//#include "FunctionsAndDeclarations.h"
#include "Functions.c"

//#define ERR 1
//#define OK 0

//typedef struct node {
//    struct node* up, *right, *left, *upRight, *downLeft, *down, *downRight, *upLeft;
//    int type;
//    struct node* parent;
//    int xpos, ypos;
//    // struct for nodes in graph
//    int f_cost, g_cost, h_cost;
//}Node;
//
//Node* map[10][10];
//Node* bus;
//char busians[10];
//int busians_index = 0;
//
//typedef struct Stack {
//    int top;
//    int size;
//    Node* path[70];
//}stack;
//
//typedef struct Task {
//    int cost;
//    stack* path_stack;
//    int starting_x, starting_y;
//    int ending_x, ending_y;
//    bool pickup_mode;
//    char name;
//}task;
//
//typedef struct min_heap_priority {
//    int heap_size;
//    task* heap_data[70];
//}heap_priority;
//
//typedef struct min_heap {
//    int heap_size;
//    Node* heap_data[70];
//}heap;
//
//struct min_heap_priority* initialize_priority() {
//    // Initiallize the heap, call it open
//    heap_priority* new = (heap_priority*) malloc(sizeof(struct min_heap_priority));
//    new->heap_size = 0;
//    return new;
//}
//
//task* temporaryHeapStorage[70];
//int rear = -1, front = -1;
//int temp_queue_size = 0;
//task* temp_queue[70];
//
//void heapify_priority(heap_priority* heap, task* T, int element_index) {
//    while (element_index > 0) {
//        int parent_index = (element_index - 1) / 2;
//        // Check if cost of element to be inserted is greater than cost of it's parent
//        // If it's true, heap property is satisfied and node is placed
//        if (T->cost > heap->heap_data[parent_index]->cost) {
//            heap->heap_data[element_index] = T;
//            return;
//        }
//        // Brings the parent down to where the node was inserted
//        // Then switch index for next loop
//        heap->heap_data[element_index] = heap->heap_data[parent_index];
//        element_index = parent_index;
//    }
//    // When the node_index hits 0 it reaches the root, therefore insert at root
//    heap->heap_data[0] = T;
//    return;
//}
//
//// Insert in heap function = push_heap
//void insert_priority(heap_priority* heap, task* T) {
//    // Check if heap is full
//    if (heap->heap_size == 70) return;
//    // Recording the index to pass into heapify and increase heap_size by 1
//    int element_index = heap->heap_size;
//    heap->heap_size = heap->heap_size + 1;
//    return heapify_priority(heap, T, element_index);
//}
//
//task* pop_heap_priority(heap_priority* heap) {
//    // Setting variables
//    task* temp;
//    int r_child = 2;
//    int l_child = 1;
//    int empty_slot = 0;
//    // Decrease heap size
//    heap->heap_size = heap->heap_size - 1;
//    temp = heap->heap_data[heap->heap_size];
//    // Record the min node of the heap
//    task* answer = heap->heap_data[0];
//    // Setting children
//    // Checking left child
//    while (l_child < heap->heap_size){
//        // Checking if right child exists
//        // If this statement hits there is both a right and left child
//        if (r_child < heap->heap_size) {
//            // If the cost of the right child is less, move slot to right child
//            if (heap->heap_data[r_child]->cost < heap->heap_data[l_child]->cost) {
//                heap->heap_data[empty_slot] = heap->heap_data[r_child];
//                empty_slot = r_child;
//            }
//                // If the cost of the left child is less, move slot to left child
//            else {
//                heap->heap_data[empty_slot] = heap->heap_data[l_child];
//                empty_slot = l_child;
//            }
//        }
//        else {
//            heap->heap_data[empty_slot] = heap->heap_data[l_child];
//            empty_slot = l_child;
//        }
//        // Increment left and right child down the heap
//        l_child = 2 * empty_slot + 1;
//        r_child = 2 * empty_slot + 2;
//    }
//    heapify_priority(heap, temp, empty_slot);
//    return answer;
//}
//
//int getGHCost(Node* current, Node* end){
//    int x1 = current->xpos, y1 = current->ypos, x2 = end->xpos, y2 = end->ypos;
//    int euclidean = 10 * sqrt(pow((x2-x1), 2) + pow((y2-y1),2));
//    return euclidean;
//}
//
//int checkFull(stack* path_stack) {
//    if (path_stack->top == path_stack->size) return 1;
//    return 0;
//}
//
//void push(Node* current, stack* path_stack){
//    if (checkFull(path_stack)) return;
//    path_stack->path[++path_stack->top] = current;
//}
//
//int checkEmpty(stack* path_stack) {
//    if (path_stack->top == -1) return 1;
//    return 0;
//}
//
//Node* pop(stack* path_stack){
//    if (checkEmpty(path_stack)) return NULL;
//    return path_stack->path[path_stack->top--];
//}
//
//int getFCost(Node* current, Node* neighbour, Node* target, int check){
//    int g_cost = getGHCost(current, neighbour) + current->g_cost;
//    int h_cost = getGHCost(neighbour, target);
//    if (!check) {
//        neighbour->g_cost = g_cost;
//        neighbour->h_cost = h_cost;
//    }
//    return g_cost + h_cost;
//}
//
//bool checkArray(Node* neighbour, Node* array[], int size){
//    for(int i = 0; i < size; i++){
//        if(array[i]==NULL)return false;
////        if(array[i]->xpos == neighbour->xpos && array[i]->ypos == neighbour->ypos) return true;
//        if(array[i] == neighbour) return true;
//        //printf("6");
//    }
//    return false;
//}
//
//struct min_heap* initialize() {
//    // Initiallize the heap, call it open
//    heap* new_heap = (heap*) malloc(sizeof(struct min_heap));
//    new_heap->heap_size = 0;
//    return new_heap;
//}
//
//void heapify(heap* heap, Node* node, int node_index) {
//    while (node_index > 0) {
//        int parent_index = (node_index - 1) / 2;
//        // Check if node to be f cost of node to be inserted is greater than f cost of it's parent
//        // If it's true, heap property is satisfied and node is placed
//        if (node->f_cost > heap->heap_data[parent_index]->f_cost) {
//            heap->heap_data[node_index] = node;
//            return;
//        } else if (node->f_cost == heap->heap_data[parent_index]->f_cost){
//            // Tie breaker for f cost is lowest g cost moves up
//            // If g cost is the same just insert in heap anyways
//            if (node->g_cost >= heap->heap_data[parent_index]->g_cost) {
//                heap->heap_data[node_index] = node;
//                return;
//            }
//        }
//
//        // When false, brings the parent down to where the node was inserted
//        // Then switch index for next loop
//        heap->heap_data[node_index] = heap->heap_data[parent_index];
//        node_index = parent_index;
//
//    }
//    // When the node_index hits 0 it reaches the root, therefore insert at root
//    heap->heap_data[0] = node;
//    return;
//}
//
//// Insert in heap function = push_heap
//void insert(heap* heap, Node* node) {
//    // Check if heap is full
//    if (heap->heap_size == 70) return;
//    // Recording the index to pass into heapify and increase heap_size by 1
//    int node_index = heap->heap_size;
//    heap->heap_size = heap->heap_size + 1;
//    return heapify(heap, node, node_index);
//}
//
//Node* pop_heap(heap* heap) {
//    // Setting variables
//    Node* temp;
//    int r_child = 2;
//    int l_child = 1;
//    int empty_slot = 0;
//    // Decrease heap size
//    heap->heap_size = heap->heap_size - 1;
//    temp = heap->heap_data[heap->heap_size];
//    // Record the min node of the heap
//    Node* answer = heap->heap_data[0];
//    // Setting children
//    // Checking left child
//    while (l_child < heap->heap_size){
//        // Checking if right child exists
//        // If this statement hits there is both a right and left child
//        if (r_child < heap->heap_size) {
//            // If the f cost of the right child is less, move slot to right child
//            if (heap->heap_data[r_child]->f_cost < heap->heap_data[l_child]->f_cost) {
//                heap->heap_data[empty_slot] = heap->heap_data[r_child];
//                empty_slot = r_child;
//            }
//                // If the f cost of the left child is less, move slot to left child
//            else if (heap->heap_data[r_child]->f_cost > heap->heap_data[l_child]->f_cost) {
//                heap->heap_data[empty_slot] = heap->heap_data[l_child];
//                empty_slot = l_child;
//            }
//                // If f cost are equal, g cost is tie breaker
//                // If g costs are the same, move slot to left child
//            else if (heap->heap_data[r_child]->f_cost == heap->heap_data[l_child]->f_cost) {
//                if (heap->heap_data[r_child]->g_cost < heap->heap_data[l_child]->g_cost) {
//                    heap->heap_data[empty_slot] = heap->heap_data[r_child];
//                    empty_slot = r_child;
//                } else {
//                    heap->heap_data[empty_slot] = heap->heap_data[l_child];
//                    empty_slot = l_child;
//                }
//            }
//        }
//        else {
//            heap->heap_data[empty_slot] = heap->heap_data[l_child];
//            empty_slot = l_child;
//        }
//        // Increment left and right child down the heap
//        l_child = 2 * empty_slot + 1;
//        r_child = 2 * empty_slot + 2;
//    }
//    heapify(heap, temp, empty_slot);
//    return answer;
//}
//
//bool Astar(Node* start, Node* target, task* T) {
//    Node* closed[100];
//    for(int i = 0; i<100; i++) {
//        closed[i] = NULL;
//    }
//    heap* open = initialize();
//    int count1 = -1;
//    insert(open,start);
//    Node* current, *neighbour;
//
//    while (1){
//        current = pop_heap(open);
//        //removecurrent(current);
//        if (count1==70) {
//            printf("Not possible\n");
//            return ERR;
//        }
//        closed[++count1] = current;
//        //printf("row = %d, col = %d\n", current->xpos,current->ypos);
//
//        if (current == target){
//            //pop all, set size equal to negative one before start pushing
//            //printf("1");
//            T->path_stack->top = -1;
//            while(current->xpos != start->xpos || current->ypos != start->ypos){
//                push(current, T->path_stack);
//                current = current->parent;
//            }
//            return OK;
//        }
//        int countStuck = 0;
//        for (int i = 0; i<8; i++) {
//            switch (i) {
//                case 0:
//                    neighbour = current->right;
//                    break;
//                case 1:
//                    neighbour = current->downRight;
//                    break;
//                case 2:
//                    neighbour = current->down;
//                    break;
//                case 3:
//                    neighbour = current->downLeft;
//                    break;
//                case 4:
//                    neighbour = current->left;
//                    break;
//                case 5:
//                    neighbour = current->upLeft;
//                    break;
//                case 6:
//                    neighbour = current->up;
//                    break;
//                case 7:
//                    neighbour = current->upRight;
//                    break;
//            }
//
//            if (neighbour==NULL || neighbour->type == 1 || checkArray(neighbour, closed, 100)) {
//                countStuck++;
//                if(countStuck == 8 && open->heap_size == 0) {
//                    printf("Not Possible\n");
//                    return ERR;
//                }
//                continue;
//            }
//
//
//            if (neighbour->f_cost > getFCost(current, neighbour, target, 1) || !checkArray(neighbour, open->heap_data, open->heap_size)) {
//
//                neighbour->f_cost = getFCost(current, neighbour, target, 0);
//                neighbour->parent = current;
//
//                if (!checkArray(neighbour, open->heap_data, open->heap_size)) insert(open,neighbour);
//            }
//        }
//    }
//}
//
//task* initialize_task(int xi, int yi, int xf, int yf, char name) {
//    if (map[xi][yi]->type == 1 || map[xf][yf]->type == 1) {
//        printf("INVALID POSITION\n");
//        return NULL;
//    }
//
//    // Initiallize task structure
//    // First allocate memory for stack structure
//    task* new = (task*) malloc(sizeof(struct Task));
//    // Allocate memory for stack structure in task
//    stack* new_stack = (stack*) malloc(sizeof(struct Stack));
//    // Initialize the stack
//    new->path_stack = new_stack;
//    new->path_stack->top = -1;
//    new->path_stack->size = 70;
//    // Initialize the rest of the task
//    new->starting_x = xi;
//    new->starting_y = yi;
//    new->ending_x = xf;
//    new->ending_y = yf;
//    new->name = name;
//    return new;
//}
//
//int path_cost (task* T) {
//    int path_cost = 0;
//    // Looping through stack array
//    for (int i = 0; i < T->path_stack->top; i++) {
//        // If straight relation add 10 to path cost
//        if (T->path_stack->path[i]->down == T->path_stack->path[i+1] || T->path_stack->path[i]->up == T->path_stack->path[i+1]
//            || T->path_stack->path[i]->left == T->path_stack->path[i+1] || T->path_stack->path[i]->right == T->path_stack->path[i+1]) {
//            path_cost += 10;
//        }
//            // If diagonal relation add 14 to path cost
//        else {
//            path_cost += 14;
//        }
//    }
//    return path_cost;
//}
//
//char searchQueue(heap_priority* priorityQueue, int j, int k) {
//    for (int i = 0; i < priorityQueue->heap_size; i++) {
//        if (priorityQueue->heap_data[i]->starting_x == j && priorityQueue->heap_data[i]->starting_y == k && priorityQueue->heap_data[i]->pickup_mode) return priorityQueue->heap_data[i]->name;
//        else if (priorityQueue->heap_data[i]->ending_x == j && priorityQueue->heap_data[i]->ending_y == k && !priorityQueue->heap_data[i]->pickup_mode) return priorityQueue->heap_data[i]->name;
//    }
//    return '!';
//}
//
//void print_map_user (heap_priority* priorityQueue) {
//    for (int i = 9; i > -1; i--) {
//        for (int j = 0; j < 10; j++) {
//            //printf("(%d %d) ", j, i);
//            if (map[j][i]->type == 1) printf("%c ", 254);
//            else if (map[j][i]->type == 0) printf("%c ", 95);
//            else if (map[j][i]->type == 2) {
//                char name = searchQueue(priorityQueue, j, i);
//                printf("%c ", name);
//            }
//            else printf("%d ", map[j][i]->type);
//        }
//        printf("\n");
//    }
//}
//
//void print_map_computer () {
//    for (int i = 0; i < 10; i++) {
//        for (int j = 0; j < 10; j++) {
//            if (map[j][i]->type == 1) printf("%c ", 254);
//            else if (map[j][i]->type == 0) printf("%c ", 95);
//            else printf("%d ", map[i][j]->type);
//        }
//        printf("\n");
//    }
//}
//
//
//Node* createNode (int i, int j) {
//    // Memory allocation
//    Node* new_node = (Node *) malloc(sizeof(Node));
//    if (new_node == NULL) printf("Memory allocation failed");
//    // Set coordinates
//    new_node->xpos = i;
//    new_node->ypos = j;
//    // Type 0 = roads
//    new_node->type = 0;
//    // Set A* parent pointers to NULL
//    new_node->parent = NULL;
//    // Set all connections to null, to deal with edge cases
//    new_node->up = NULL;
//    new_node->down = NULL;
//    new_node->right = NULL;
//    new_node->left = NULL;
//    new_node->upLeft = NULL;
//    new_node->upRight = NULL;
//    new_node->downLeft = NULL;
//    new_node->downRight = NULL;
//    // Set costs
//    new_node->f_cost = 0;
//    new_node->g_cost = 0;
//    new_node->h_cost = 0;
//    return new_node;
//}
//
//void random1 () {
//    // Set varaibles
//    int count = 0;
//    int trap = 0;
//    bool trapped = false;
//    // Initialize the random function
//    time_t t;
//    srand((unsigned) time(&t));
//    while (count < 30) {
//        // Each iteration calculate a new random x and y value
//        int x = rand() % 10;
//        int y = rand() % 10;
//        if (map[x][y]->type == 0) {
//            // Making sure the bus is not trapped in it's starting node
//            if ((x == 1 && y == 1) || (x == 1 && y == 0) || (x == 0 && y == 1)) trap++;
//            if (trap == 3) trapped = true;
//            // If (0,0) is not trapped place blockage
//            if (!(trapped)) {
//                map[x][y]->type = 1;
//                count++;
//            }
//            // If trapped, reset the trapped boolean
//            if (trap == 3) {
//                trap--;
//                trapped = false;
//            }
//        }
//    }
//}
//
//void random2 () {
//    // Set varaibles
//    int count = 0;
//    int trap = 0;
//    bool corner = false;
//    // Initialize the random function
//    time_t t;
//    srand((unsigned) time(&t));
//    while (count < 30) {
//        // Each iteration calculate a new random x and y value
//        int x = rand() % 10;
//        int y = rand() % 10;
//        if (map[x][y]->type == 0) {
//            // All possible cases where a corner appears, making sure to not access a NULL pointer
//            if (x == 0) {
//                if (y == 0) {
//                    if ((map[x][y]->up->type == map[x][y]->up->right->type == 1) || (map[x][y]->right->type == map[x][y]->right->up->type == 1)) {
//                        corner = true;
//                    }
//                } else if (y == 9) {
//                    if ((map[x][y]->right->type == map[x][y]->right->down->type == 1) || (map[x][y]->down->type == map[x][y]->down->right->type == 1)) {
//                        corner = true;
//                    }
//                } else {
//                    if ((map[x][y]->up->type == map[x][y]->up->right->type == 1) || (map[x][y]->right->type == map[x][y]->right->up->type == 1) ||
//                        (map[x][y]->right->type == map[x][y]->right->down->type == 1) || (map[x][y]->down->type == map[x][y]->down->right->type == 1)) {
//                        corner = true;
//                    }
//                }
//            } else if (x == 9) {
//                if (y == 0) {
//                    if ((map[x][y]->left->type == map[x][y]->left->up->type == 1) || (map[x][y]->up->type == map[x][y]->up->left->type == 1)) {
//                        corner = true;
//                    }
//                } else if (y == 9) {
//                    if ((map[x][y]->left->type == map[x][y]->left->down->type == 1) || (map[x][y]->down->type == map[x][y]->down->left->type == 1)) {
//                        corner = true;
//                    }
//                } else {
//                    if ( (map[x][y]->up->type == map[x][y]->up->left->type == 1) || (map[x][y]->left->type == map[x][y]->left->up->type == 1) ||
//                         (map[x][y]->left->type == map[x][y]->left->down->type == 1) || (map[x][y]->down->type == map[x][y]->down->left->type == 1)) {
//                        corner = true;
//                    }
//                }
//            } else if (y == 0) {
//                if ((map[x][y]->up->type == map[x][y]->up->right->type == 1) || (map[x][y]->up->type == map[x][y]->up->left->type == 1) ||
//                    (map[x][y]->left->type == map[x][y]->left->up->type == 1) || (map[x][y]->right->type == map[x][y]->right->up->type == 1)) {
//                    corner = true;
//                }
//            } else if (y == 9) {
//                if ((map[x][y]->left->type == map[x][y]->left->down->type == 1) || (map[x][y]->right->type == map[x][y]->right->down->type == 1) ||
//                    (map[x][y]->down->type == map[x][y]->down->right->type == 1) || (map[x][y]->down->type == map[x][y]->down->left->type == 1)) {
//                    corner = true;
//                }
//            } else {
//                if (((map[x][y]->up->type && map[x][y]->up->right->type) == 1) || ((map[x][y]->up->type && map[x][y]->up->left->type) == 1) ||
//                    ((map[x][y]->left->type && map[x][y]->left->up->type) == 1) || ((map[x][y]->left->type && map[x][y]->left->down->type) == 1) ||
//                    ((map[x][y]->right->type && map[x][y]->right->up->type) == 1) || ((map[x][y]->right->type && map[x][y]->right->down->type) == 1) ||
//                    ((map[x][y]->down->type && map[x][y]->down->right->type) == 1) || ((map[x][y]->down->type && map[x][y]->down->left->type) == 1)) {
//                    corner = true;
//                }
//            }
//            // If no corner appears place blockage
//            if (!(corner)) {
//                map[x][y]->type = 1;
//                count++;
//            }
//            corner = false;
//        }
//    }
//}
//
//
//
//
//
//
//
//// MAIN IMPORTANT FUNCTIONS HERE
//
//Node* path_finding_initialization () {
//    // Create the grid of nodes
//    for (int i = 0; i < 10; i++) {
//        for (int j = 0; j < 10; j++) {
//            map[i][j] = createNode(i,j);
//        }
//    }
//    // Link the nodes
//    for (int i = 0; i < 10; i++) {
//        for (int j = 0; j < 10; j++) {
//            // Link up
//            if (i > 0) map[i][j]->up = map[i - 1][j];
//            // Link down
//            if (i < 9) map[i][j]->down = map[i + 1][j];
//            // Link left
//            if (j > 0) map[i][j]->left = map[i][j - 1];
//            // Link right
//            if (j < 9) map[i][j]->right = map[i][j + 1];
//            // Link upLeft
//            if (i > 0 && j > 0) map[i][j]->upLeft = map[i - 1][j - 1];
//            // Link downLeft
//            if (i < 9 && j > 0) map[i][j]->downLeft = map[i + 1][j - 1];
//            // Link upRight
//            if (i > 0 && j < 9) map[i][j]->upRight = map[i - 1][j + 1];
//            // Link downRight
//            if (i < 9 && j < 9) {
//                map[i][j]->downRight = map[i + 1][j + 1];
//            }
//        }
//    }
//    // Setup bus
//    map[0][0]->type = 3;
//    bus = map[0][0];
//    // Call randomization function
//    random1();
//    return bus;
//}
//
//task* add_route (int xi, int yi, int xf, int yf, heap_priority* queue, char name) {
//    // Allocate and initialize a task structure
//    task* add_task = initialize_task(xi, yi, xf, yf, name);
//
//    if (add_task == NULL) return NULL;
//    // Set the task into pickup mode
//    add_task->pickup_mode = true;
//    // Call A* algorithm
//    //Node* destination = map[xi][yi];
//    if (Astar(bus, map[add_task->starting_x][add_task->starting_y], add_task) == ERR) return NULL;
//    // Calculate tasks cost from new path from A*
//    add_task->cost = path_cost(add_task);
//    // Add the new task structure to the queue
//    insert_priority(queue, add_task);
//    busians[busians_index] = name;
//    busians_index++;
//    map[xi][yi]->type = 2;
//    return add_task;
//}
//
//bool QIsFull(){
//    return temp_queue_size == 70;
//}
//
//bool QIsEmpty(){
//    return temp_queue_size == 0;
//}
//
//bool enqueue(task* T){
//    if(QIsFull()) return ERR;
//    rear = (++rear) % 70;
//    temp_queue[rear] = T;
//    temp_queue_size++;
//    if(temp_queue_size == 1){
//        front = rear;
//    }
//    return OK;
//}
//
//task* dequeue(task* t){
//    if(QIsEmpty()) return NULL;
//    t = temp_queue[front];
//    front = ++front % 70;
//    temp_queue_size--;
//    if(QIsEmpty(temp_queue)) rear = front = -1;
//    return t;
//}
//
//void Simulate(heap_priority* priorityQueue){
////    if((bus->xpos == priorityQueue->heap_data[0]->ending_x) && (bus->ypos == priorityQueue->heap_data[0]->ending_y)) pop_heap_priority(priorityQueue);
////
////    if((bus->xpos == priorityQueue->heap_data[0]->starting_x) && (bus->ypos == priorityQueue->heap_data[0]->starting_y)) {
////        priorityQueue->heap_data[0]->pickup_mode = false;
////        Astar(bus, map[priorityQueue->heap_data[0]->ending_x][priorityQueue->heap_data[0]->ending_y], priorityQueue->heap_data[0]);
////    }
//
//    //take step
//    //insert_priority, pop_heap_priority
//    //fetch the position to go to next to follow the current path
//
//    if(priorityQueue->heap_size == 0){
//        printf("Enter a route to follow first");
//        return;
//    }
//    Node* newPosition = pop(priorityQueue->heap_data[0]->path_stack);
//    //Move the bus to those new positions
//    int currentX = newPosition->xpos;
//    int currentY = newPosition->ypos;
//    map[currentX][currentY]->type = 3;
//    map[bus->xpos][bus->ypos]->type = 0;
//    bus = map[currentX][currentY];
//    int i= 0;
//
//    if((bus->xpos == priorityQueue->heap_data[0]->ending_x) && (bus->ypos == priorityQueue->heap_data[0]->ending_y)) pop_heap_priority(priorityQueue);
//
//    if((bus->xpos == priorityQueue->heap_data[0]->starting_x) && (bus->ypos == priorityQueue->heap_data[0]->starting_y)) {
//        priorityQueue->heap_data[0]->pickup_mode = false;
//        map[priorityQueue->heap_data[0]->ending_x][priorityQueue->heap_data[0]->ending_y]->type = 2;
//        //Astar(bus, map[priorityQueue->heap_data[0]->ending_x][priorityQueue->heap_data[0]->ending_y], priorityQueue->heap_data[0]);
//    }
//
//    //update the path, cost of each path
//    while(i < priorityQueue->heap_size){
//
//        Node* destination;
//        if(priorityQueue->heap_data[i]->pickup_mode) destination = map[priorityQueue->heap_data[i]->starting_x][priorityQueue->heap_data[i]->starting_y];
//        else destination = map[priorityQueue->heap_data[i]->ending_x][priorityQueue->heap_data[i]->ending_y];
//
//        //Update path
//        Astar(bus, destination, priorityQueue->heap_data[i]);
//
//        // Update Cost
//        priorityQueue->heap_data[i]->cost = path_cost(priorityQueue->heap_data[i]);
//
//        //Update Current Coordinates
//        // priorityQueue->heap_data[i]->starting_x = bus->posx;
//        // priorityQueue->heap_data[i]->starting_y = bus->posy;
//        //Add to the storage Queue
//        enqueue(priorityQueue->heap_data[i]);
//        i++;
//    }
//
//    //clear the current heap
//    priorityQueue->heap_size = 0;
//
//    //insert everything back into the heap in their sorted positions
//    while(!QIsEmpty()){
//        task* temp;
//        temp = dequeue(temp);
//        insert_priority(priorityQueue, temp);
//    }
//}
//
//// MAIN IMPORTANT FUNCTIONS ^^^^
//
//void takeStep(stack* path_stack){
//    Node* current = pop(path_stack);
//    map[current->xpos][current->ypos]->type = 3;
//    map[bus->xpos][bus->ypos]->type = 0;
//    bus->xpos = current->xpos;
//    bus->ypos = current->ypos;
//}

int main () {
    path_finding_initialization();

//    map[0][9]->type = 4;
//    map[9][0]->type = 5;
//    map[9][9]->type = 6;
//    map[4][4]->type = 7;

//    map[0][2]->type = 1;
//    map[1][2]->type = 1;
//    map[2][2]->type = 1;
//    map[2][1]->type = 1;
//    map[2][0]->type = 1;

    heap_priority* queue;

    print_map_user(queue);
    //print_map_computer();

    int input = 0;
    int inputXStart = 0, inputYStart = 0, inputXEnd = 0, inputYEnd = 0;
    char inputName;

    queue = initialize_priority();

    while(1) {
        printf("1 for add route, 2 for simulate: ");
        scanf("%d", &input);
        printf("\n");
        if (input == 1) {
            printf("starting coordinates, x, y: ");
            scanf("%d %d", &inputXStart, &inputYStart);
            printf("starting coordinates, x, y: ");
            scanf("%d %d", &inputXEnd, &inputYEnd);
            printf("name: ");
            getchar();
            scanf("%c", &inputName);
            add_route(inputXStart, inputYStart, inputXEnd, inputYEnd, queue, inputName);
        }
        else if (input == 2) {
            Simulate(queue);
            print_map_user(queue);
            //print_map_computer();
        }
        input = 0;
    }

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            free(map[i][j]);
        }
    }
}
//abs(inputYStart-9)


// Sebastien's Code
// Final Main

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include<windows.h>
#include <string.h>
#include"FunctionsAndDeclarations.h"


/*
Functions to store tasks in the heap
 ***********************************
 */

// Structure definition for the min_heap to sort the tasks based on cost
struct min_heap_priority* initialize_priority() {
    // Initiallize the heap, call it open
    heap_priority* new = (heap_priority*) malloc(sizeof(struct min_heap_priority));
    // initialize the new heap to have nothing in it
    new->heap_size = 0;
    return new;
}

// function to resort the heap of tasks
void heapify_priority(heap_priority* heap, task* T, int element_index) {
    while (element_index > 0) {
        int parent_index = (element_index - 1) / 2;
        // Check if cost of element to be inserted is greater than cost of it's parent
        // If it's true, heap property is satisfied and node is placed
        if (T->cost > heap->heap_data[parent_index]->cost) {
            heap->heap_data[element_index] = T;
            return;
        }
        // Brings the parent down to where the node was inserted
        // Then switch index for next loop
        heap->heap_data[element_index] = heap->heap_data[parent_index];
        element_index = parent_index;
    }
    // When the node_index hits 0 it reaches the root, therefore insert at root
    heap->heap_data[0] = T;
    return;
}

// Insert in heap function = push_heap
void insert_priority(heap_priority* heap, task* T) {
    // Check if heap is full
    if (heap->heap_size == 70) return;
    // Recording the index to pass into heapify and increase heap_size by 1
    int element_index = heap->heap_size;
    heap->heap_size = heap->heap_size + 1;
    return heapify_priority(heap, T, element_index);
}

task* pop_heap_priority(heap_priority* heap) {
    // Setting variables
    task* temp;
    int r_child = 2;
    int l_child = 1;
    int empty_slot = 0;
    // Decrease heap size
    heap->heap_size = heap->heap_size - 1;
    temp = heap->heap_data[heap->heap_size];
    // Record the min node of the heap
    task* answer = heap->heap_data[0];
    // Setting children
    // Checking left child
    while (l_child < heap->heap_size){
        // Checking if right child exists
        // If this statement hits there is both a right and left child
        if (r_child < heap->heap_size) {
            // If the cost of the right child is less, move slot to right child
            if (heap->heap_data[r_child]->cost < heap->heap_data[l_child]->cost) {
                heap->heap_data[empty_slot] = heap->heap_data[r_child];
                empty_slot = r_child;
            }
                // If the cost of the left child is less, move slot to left child
            else {
                heap->heap_data[empty_slot] = heap->heap_data[l_child];
                empty_slot = l_child;
            }
        }
        else {
            heap->heap_data[empty_slot] = heap->heap_data[l_child];
            empty_slot = l_child;
        }
        // Increment left and right child down the heap
        l_child = 2 * empty_slot + 1;
        r_child = 2 * empty_slot + 2;
    }
    heapify_priority(heap, temp, empty_slot);
    return answer;
}


/*
Functions that compute and store information relating to the Astar algorithm
 ***************************************************************************
 */

// Function to compute either the Gcost or Hcost of a node with respect to the current Node depending on which end target is passed in
//The end is the target for Hcost, and the neighbor node for Gcost
int getGHCost(Node* current, Node* end){
    int x1 = current->xpos, y1 = current->ypos, x2 = end->xpos, y2 = end->ypos;
    //Find the euclidean distance between two nodes and return that distance
    int euclidean = 10 * sqrt(pow((x2-x1), 2) + pow((y2-y1),2));
    return euclidean;
}

// Function to if the stack storing the path determined by Astar is full
int checkFull(stack* path_stack) {
    // returns one if full, 0 if not
    if (path_stack->top == path_stack->size) return 1;
    return 0;
}

// Adds a node to the top of a stack
void push(Node* current, stack* path_stack){
    // check if stack is full before the node is added
    if (checkFull(path_stack)) return;
    // add the node to the incremented top of the stack
    path_stack->path[++path_stack->top] = current;
}

// Check if the stack storing the path determined by Astar is empty
int checkEmpty(stack* path_stack) {
    if (path_stack->top == -1) return 1;
    return 0;
}

// Remove the top node from  a stack
Node* pop(stack* path_stack){
    // check if there is nothing to pop first, if there is something to pop, then return the element at the top and shrink the stack
    if (checkEmpty(path_stack)) return NULL;
    return path_stack->path[path_stack->top--];
}

// Gets the Fcost of a node, neighbor, with respect to current and a target node
int getFCost(Node* current, Node* neighbour, Node* target, int check){
    // Calculate the G cost of the neighbour node by passing in current as the start and neighbour as the ending node
    int g_cost = getGHCost(current, neighbour) + current->g_cost;
    // Calculate the G cost of the neighbour node by passing in current as the start and the final target as the ending node
    int h_cost = getGHCost(neighbour, target);

    //Check if the current fcost of the neighbour is less than the previous fcost it was set to when it was origionally in the frontier, the open list
    // If it is make the new calculated g and h cost the new g and h cost of the node
    //If it is,
    if (!check) {
        neighbour->g_cost = g_cost;
        neighbour->h_cost = h_cost;
    }
    // return the fcost
    return g_cost + h_cost;
}

//linearly search through the array passed going as far as the size passed and checking if the neighbour passed is in it
bool checkArray(Node* neighbour, Node* array[], int size){
    for(int i = 0; i < size; i++){
        if(array[i]==NULL)return false;
//        if(array[i]->xpos == neighbour->xpos && array[i]->ypos == neighbour->ypos) return true;
        //neighbour is in it
        if(array[i] == neighbour) return true;
        //printf("6");
    }
    //neighbour is not in it
    return false;
}

//initializes min_heap for the open list in Astar
struct min_heap* initialize() {
    //allocates heap memory
    heap* new_heap = (heap*) malloc(sizeof(struct min_heap));
    //sets the size of the heap to 0
    new_heap->heap_size = 0;
    //return the initilized heap
    return new_heap;
}

//moves nodes around the heap until the correct node is at the first index of the heap
void heapify(heap* heap, Node* node, int node_index) {
    while (node_index > 0) {
        int parent_index = (node_index - 1) / 2;
        // Check if node to be f cost of node to be inserted is greater than f cost of it's parent
        // If it's true, heap property is satisfied and node is placed
        if (node->f_cost > heap->heap_data[parent_index]->f_cost) {
            heap->heap_data[node_index] = node;
            return;
        } else if (node->f_cost == heap->heap_data[parent_index]->f_cost){
            // Tie breaker for f cost is lowest g cost moves up
            // If g cost is the same just insert in heap anyways
            if (node->g_cost >= heap->heap_data[parent_index]->g_cost) {
                heap->heap_data[node_index] = node;
                return;
            }
        }

        // When false, brings the parent down to where the node was inserted
        // Then switch index for next loop
        heap->heap_data[node_index] = heap->heap_data[parent_index];
        node_index = parent_index;

    }
    // When the node_index hits 0 it reaches the root, therefore insert at root
    heap->heap_data[0] = node;
    return;
}

// Insert in heap function = push_heap
void insert(heap* heap, Node* node) {
    // Check if heap is full
    if (heap->heap_size == 70) return;
    // Recording the index to pass into heapify and increase heap_size by 1
    int node_index = heap->heap_size;
    heap->heap_size = heap->heap_size + 1;
    return heapify(heap, node, node_index);
}

Node* pop_heap(heap* heap) {
    // Setting variables
    Node* temp;
    int r_child = 2;
    int l_child = 1;
    int empty_slot = 0;
    // Decrease heap size
    heap->heap_size = heap->heap_size - 1;
    temp = heap->heap_data[heap->heap_size];
    // Record the min node of the heap
    Node* answer = heap->heap_data[0];
    // Setting children
    // Checking left child
    while (l_child < heap->heap_size){
        // Checking if right child exists
        // If this statement hits there is both a right and left child
        if (r_child < heap->heap_size) {
            // If the f cost of the right child is less, move slot to right child
            if (heap->heap_data[r_child]->f_cost < heap->heap_data[l_child]->f_cost) {
                heap->heap_data[empty_slot] = heap->heap_data[r_child];
                empty_slot = r_child;
            }
                // If the f cost of the left child is less, move slot to left child
            else if (heap->heap_data[r_child]->f_cost > heap->heap_data[l_child]->f_cost) {
                heap->heap_data[empty_slot] = heap->heap_data[l_child];
                empty_slot = l_child;
            }
                // If f cost are equal, g cost is tie breaker
                // If g costs are the same, move slot to left child
            else if (heap->heap_data[r_child]->f_cost == heap->heap_data[l_child]->f_cost) {
                if (heap->heap_data[r_child]->g_cost < heap->heap_data[l_child]->g_cost) {
                    heap->heap_data[empty_slot] = heap->heap_data[r_child];
                    empty_slot = r_child;
                } else {
                    heap->heap_data[empty_slot] = heap->heap_data[l_child];
                    empty_slot = l_child;
                }
            }
        }
        else {
            heap->heap_data[empty_slot] = heap->heap_data[l_child];
            empty_slot = l_child;
        }
        // Increment left and right child down the heap
        l_child = 2 * empty_slot + 1;
        r_child = 2 * empty_slot + 2;
    }
    heapify(heap, temp, empty_slot);
    return answer;
}

//is passed the starting node, the target node and the task for that target and finds the most optimal path to that target
bool Astar(Node* start, Node* target, task* T) {
    //initalizes closed list and sets all nodes to NULL
    Node* closed[100];
    for(int i = 0; i<100; i++) {
        closed[i] = NULL;
    }
    //initializes the open heap
    heap* open = initialize();
    //initialize and set count1 to -1 which is what index we are at for the closed list
    int count1 = -1;
    //insert the starting point into the open list
    insert(open,start);
    //initialize the current and neighbour pointers
    Node* current, *neighbour;

    //infinite loop until path has been found or no path is possible
    while (1){
        //get lowest fcost node out of heap and set it to the current node
        current = pop_heap(open);
        //removecurrent(current);
        //if all nodes have been checked and put into close list than path is not possible and return error
        if (count1==70) {
            printf("Not possible\n");
            return ERR;
        }
        //add current node to closed list
        closed[++count1] = current;
        //printf("row = %d, col = %d\n", current->xpos,current->ypos);

        //if the current node is the target node the path ahs been found
        if (current == target){
            //pop all, set size equal to negative one before start pushing
            //printf("1");
            //set stack of task to empty
            T->path_stack->top = -1;
            //loop backwards through parent pointer and push each node onto a stack to store the path and stop when
            // current equals start
            while(current->xpos != start->xpos || current->ypos != start->ypos){
                push(current, T->path_stack);
                current = current->parent;
            }
            return OK;
        }
        //initialize stuck counter to check if all nodes around are closed or blocked
        int countStuck = 0;

        //loop through each neighbour of current
        for (int i = 0; i<8; i++) {
            switch (i) {
                case 0:
                    neighbour = current->right;
                    break;
                case 1:
                    neighbour = current->downRight;
                    break;
                case 2:
                    neighbour = current->down;
                    break;
                case 3:
                    neighbour = current->downLeft;
                    break;
                case 4:
                    neighbour = current->left;
                    break;
                case 5:
                    neighbour = current->upLeft;
                    break;
                case 6:
                    neighbour = current->up;
                    break;
                case 7:
                    neighbour = current->upRight;
                    break;
            }

            //if neighbour is closed or NULL or a wall continue to the next neighbour
            if (neighbour==NULL || neighbour->type == 1 || checkArray(neighbour, closed, 100)) {
                //increment countStuck
                countStuck++;
                //if all neighbours around current are closed or blocked and there are no more nodes to search that
                // means the path is not possible and return error
                if(countStuck == 8 && open->heap_size == 0) {
                    printf("Not Possible\n");
                    return ERR;
                }
                continue;
            }

            //if neighbours new fcost on the current path it is going is less than the old path or the neighbour is not in the
            // open list which means has not been searched than set fcost to that neighbour and set parent pointer to current
            if (neighbour->f_cost > getFCost(current, neighbour, target, 1) || !checkArray(neighbour, open->heap_data, open->heap_size)) {

                //get and store fcost in neighbour
                neighbour->f_cost = getFCost(current, neighbour, target, 0);
                //set parent pointer to current
                neighbour->parent = current;

                //if neighbour is not in the open list put it in the open list
                if (!checkArray(neighbour, open->heap_data, open->heap_size)) insert(open,neighbour);
            }
        }
    }
}

//initialize a task with a given x y pickup point and x y dropoff point and a name
task* initialize_task(int xi, int yi, int xf, int yf, char name) {
    //if the pickup or dropoff point is a wall than error
    if (map[xi][yi]->type == 1 || map[xf][yf]->type == 1) {
        printf("INVALID POSITION\n");
        return NULL;
    }

    // Initiallize task structure
    // First allocate memory for stack structure
    task* new = (task*) malloc(sizeof(struct Task));
    // Allocate memory for stack structure in task
    stack* new_stack = (stack*) malloc(sizeof(struct Stack));
    // Initialize the stack
    new->path_stack = new_stack;
    new->path_stack->top = -1;
    new->path_stack->size = 70;
    // Initialize the rest of the task
    new->starting_x = xi;
    new->starting_y = yi;
    new->ending_x = xf;
    new->ending_y = yf;
    new->name = name;
    return new;
}

int path_cost (task* T) {
    int path_cost = 0;
    // Looping through stack array
    for (int i = 0; i < T->path_stack->top; i++) {
        // If straight relation add 10 to path cost
        if (T->path_stack->path[i]->down == T->path_stack->path[i+1] || T->path_stack->path[i]->up == T->path_stack->path[i+1]
            || T->path_stack->path[i]->left == T->path_stack->path[i+1] || T->path_stack->path[i]->right == T->path_stack->path[i+1]) {
            path_cost += 10;
        }
            // If diagonal relation add 14 to path cost
        else {
            path_cost += 14;
        }
    }
    return path_cost;
}

//search the task queue for a given x y and return the name of that task to be printed to the map
char searchQueue(heap_priority* priorityQueue, int j, int k) {
    for (int i = 0; i < priorityQueue->heap_size; i++) {
        //if the tasks x and the tasks y equal to the current x y being printed and the task is in correct mode than return the name
        if (priorityQueue->heap_data[i]->starting_x == j && priorityQueue->heap_data[i]->starting_y == k && priorityQueue->heap_data[i]->pickup_mode) return priorityQueue->heap_data[i]->name;
        //else if the tasks x and y match the current x y and the other mode is correct than return name
        else if (priorityQueue->heap_data[i]->ending_x == j && priorityQueue->heap_data[i]->ending_y == k && !priorityQueue->heap_data[i]->pickup_mode) return priorityQueue->heap_data[i]->name;
    }
    return '!';
}

//print the map in x y axis form with 0 0 being bottom left and 9 9 being top right
void print_map_user (heap_priority* priorityQueue) {
    for (int i = 9; i > -1; i--) {
        for (int j = 0; j < 10; j++) {
            //printf("(%d %d) ", j, i);
            //if x y point is a wall print a block
            if (map[j][i]->type == 1) printf("%c ", 254);
            //if x y is a road print a underscore
            else if (map[j][i]->type == 0) printf("%c ", 95);
            //if x y is a task search for task and print its name
            else if (map[j][i]->type == 2) {
                char name = searchQueue(priorityQueue, j, i);
                printf("%c ", name);
            }
            //else print type which will always be the bus which is 3
            else printf("%d ", map[j][i]->type);
        }
        printf("\n");
    }
}

//print the map in array mode with 0 0 being top left and 9 9 being bottom right
void print_map_computer () {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            //if x y point is a wall print a block
            if (map[j][i]->type == 1) printf("%c ", 254);
                //if x y is a road print a underscore
            else if (map[j][i]->type == 0) printf("%c ", 95);
                //else print type
            else printf("%d ", map[i][j]->type);
        }
        printf("\n");
    }
}

//initialize a node with all correct default states
Node* createNode (int i, int j) {
    // Memory allocation
    Node* new_node = (Node *) malloc(sizeof(Node));
    if (new_node == NULL) printf("Memory allocation failed");
    // Set coordinates
    new_node->xpos = i;
    new_node->ypos = j;
    // Type 0 = roads
    new_node->type = 0;
    // Set A* parent pointers to NULL
    new_node->parent = NULL;
    // Set all connections to null, to deal with edge cases
    new_node->up = NULL;
    new_node->down = NULL;
    new_node->right = NULL;
    new_node->left = NULL;
    new_node->upLeft = NULL;
    new_node->upRight = NULL;
    new_node->downLeft = NULL;
    new_node->downRight = NULL;
    // Set costs
    new_node->f_cost = 0;
    new_node->g_cost = 0;
    new_node->h_cost = 0;
    return new_node;
}

//randomly generate walls at random x y points and put them in the map
void random1 () {
    // Set varaibles
    int count = 0;
    int trap = 0;
    bool trapped = false;
    // Initialize the random function
    time_t t;
    srand((unsigned) time(&t));
    while (count < 30) {
        // Each iteration calculate a new random x and y value
        int x = rand() % 10;
        int y = rand() % 10;
        if (map[x][y]->type == 0) {
            // Making sure the bus is not trapped in it's starting node
            if ((x == 1 && y == 1) || (x == 1 && y == 0) || (x == 0 && y == 1)) trap++;
            if (trap == 3) trapped = true;
            // If (0,0) is not trapped place blockage
            if (!(trapped)) {
                map[x][y]->type = 1;
                count++;
            }
            // If trapped, reset the trapped boolean
            if (trap == 3) {
                trap--;
                trapped = false;
            }
        }
    }
}

//different random wall generator with different check cases
void random2 () {
    // Set varaibles
    int count = 0;
    int trap = 0;
    bool corner = false;
    // Initialize the random function
    time_t t;
    srand((unsigned) time(&t));
    while (count < 30) {
        // Each iteration calculate a new random x and y value
        int x = rand() % 10;
        int y = rand() % 10;
        if (map[x][y]->type == 0) {
            // All possible cases where a corner appears, making sure to not access a NULL pointer
            if (x == 0) {
                if (y == 0) {
                    if ((map[x][y]->up->type == map[x][y]->up->right->type == 1) || (map[x][y]->right->type == map[x][y]->right->up->type == 1)) {
                        corner = true;
                    }
                } else if (y == 9) {
                    if ((map[x][y]->right->type == map[x][y]->right->down->type == 1) || (map[x][y]->down->type == map[x][y]->down->right->type == 1)) {
                        corner = true;
                    }
                } else {
                    if ((map[x][y]->up->type == map[x][y]->up->right->type == 1) || (map[x][y]->right->type == map[x][y]->right->up->type == 1) ||
                        (map[x][y]->right->type == map[x][y]->right->down->type == 1) || (map[x][y]->down->type == map[x][y]->down->right->type == 1)) {
                        corner = true;
                    }
                }
            } else if (x == 9) {
                if (y == 0) {
                    if ((map[x][y]->left->type == map[x][y]->left->up->type == 1) || (map[x][y]->up->type == map[x][y]->up->left->type == 1)) {
                        corner = true;
                    }
                } else if (y == 9) {
                    if ((map[x][y]->left->type == map[x][y]->left->down->type == 1) || (map[x][y]->down->type == map[x][y]->down->left->type == 1)) {
                        corner = true;
                    }
                } else {
                    if ( (map[x][y]->up->type == map[x][y]->up->left->type == 1) || (map[x][y]->left->type == map[x][y]->left->up->type == 1) ||
                         (map[x][y]->left->type == map[x][y]->left->down->type == 1) || (map[x][y]->down->type == map[x][y]->down->left->type == 1)) {
                        corner = true;
                    }
                }
            } else if (y == 0) {
                if ((map[x][y]->up->type == map[x][y]->up->right->type == 1) || (map[x][y]->up->type == map[x][y]->up->left->type == 1) ||
                    (map[x][y]->left->type == map[x][y]->left->up->type == 1) || (map[x][y]->right->type == map[x][y]->right->up->type == 1)) {
                    corner = true;
                }
            } else if (y == 9) {
                if ((map[x][y]->left->type == map[x][y]->left->down->type == 1) || (map[x][y]->right->type == map[x][y]->right->down->type == 1) ||
                    (map[x][y]->down->type == map[x][y]->down->right->type == 1) || (map[x][y]->down->type == map[x][y]->down->left->type == 1)) {
                    corner = true;
                }
            } else {
                if (((map[x][y]->up->type && map[x][y]->up->right->type) == 1) || ((map[x][y]->up->type && map[x][y]->up->left->type) == 1) ||
                    ((map[x][y]->left->type && map[x][y]->left->up->type) == 1) || ((map[x][y]->left->type && map[x][y]->left->down->type) == 1) ||
                    ((map[x][y]->right->type && map[x][y]->right->up->type) == 1) || ((map[x][y]->right->type && map[x][y]->right->down->type) == 1) ||
                    ((map[x][y]->down->type && map[x][y]->down->right->type) == 1) || ((map[x][y]->down->type && map[x][y]->down->left->type) == 1)) {
                    corner = true;
                }
            }
            // If no corner appears place blockage
            if (!(corner)) {
                map[x][y]->type = 1;
                count++;
            }
            corner = false;
        }
    }
}

//initialize the map
Node* path_finding_initialization () {
    // Create the grid of nodes
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            //create nodes at all points in the map
            map[i][j] = createNode(i,j);
        }
    }
    // Link the nodes
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            // Link up
            if (i > 0) map[i][j]->up = map[i - 1][j];
            // Link down
            if (i < 9) map[i][j]->down = map[i + 1][j];
            // Link left
            if (j > 0) map[i][j]->left = map[i][j - 1];
            // Link right
            if (j < 9) map[i][j]->right = map[i][j + 1];
            // Link upLeft
            if (i > 0 && j > 0) map[i][j]->upLeft = map[i - 1][j - 1];
            // Link downLeft
            if (i < 9 && j > 0) map[i][j]->downLeft = map[i + 1][j - 1];
            // Link upRight
            if (i > 0 && j < 9) map[i][j]->upRight = map[i - 1][j + 1];
            // Link downRight
            if (i < 9 && j < 9) {
                map[i][j]->downRight = map[i + 1][j + 1];
            }
        }
    }
    // Setup bus
    map[0][0]->type = 3;
    bus = map[0][0];
    // Call randomization function
    random1();
    return bus;
}

//initialize, find path, and add task to priority queue given a pickup point, dropoff point, the task queue and a name
task* add_route (int xi, int yi, int xf, int yf, heap_priority* queue, char name) {
    // Allocate and initialize a task structure
    task* add_task = initialize_task(xi, yi, xf, yf, name);

    //if task was put at a wall than return NULL
    if (add_task == NULL) return NULL;
    // Set the task into pickup mode
    add_task->pickup_mode = true;
    // Call A* algorithm
    //Node* destination = map[xi][yi];
    if (Astar(bus, map[add_task->starting_x][add_task->starting_y], add_task) == ERR) return NULL;
    // Calculate tasks cost from new path from A*
    add_task->cost = path_cost(add_task);
    // Add the new task structure to the queue
    insert_priority(queue, add_task);
    busians[busians_index] = name;
    busians_index++;
    map[xi][yi]->type = 2;
    return add_task;
}

//is the queue full
bool QIsFull(){
    return temp_queue_size == 70;
}

//is the queue empty
bool QIsEmpty(){
    return temp_queue_size == 0;
}

//add task to a temporary queue
bool enqueue(task* T){
    //if queue is full error
    if(QIsFull()) return ERR;
    //increment rear
    rear = (++rear) % 70;
    //add task to the queue at the rear
    temp_queue[rear] = T;
    //increment the size
    temp_queue_size++;
    //if the queue is size 1 than front and rear will point to the same task
    if(temp_queue_size == 1){
        front = rear;
    }
    return OK;
}

//take a task off the queue
task* dequeue(task* t){
    //if queue is empty then return null
    if(QIsEmpty()) return NULL;
    //store task taken from queue into a temp return pointer
    t = temp_queue[front];
    //increment front
    front = ++front % 70;
    //deincrement size
    temp_queue_size--;
    //if the queue is now empty then set both rear and front to -1
    if(QIsEmpty(temp_queue)) rear = front = -1;
    //return the task taken from the queue
    return t;
}


void Simulate(heap_priority* priorityQueue){
//    if((bus->xpos == priorityQueue->heap_data[0]->ending_x) && (bus->ypos == priorityQueue->heap_data[0]->ending_y)) pop_heap_priority(priorityQueue);
//
//    if((bus->xpos == priorityQueue->heap_data[0]->starting_x) && (bus->ypos == priorityQueue->heap_data[0]->starting_y)) {
//        priorityQueue->heap_data[0]->pickup_mode = false;
//        Astar(bus, map[priorityQueue->heap_data[0]->ending_x][priorityQueue->heap_data[0]->ending_y], priorityQueue->heap_data[0]);
//    }

    // if the size of the heap is 0, it means they are trying to follow route that doesn't exist, in this case, return
    if(priorityQueue->heap_size == 0){
        printf("Enter a route to follow first");
        return;
    }
    // create a node to store the new position which is the node stored at the top of the stack in the task at front of heap
    Node* newPosition = pop(priorityQueue->heap_data[0]->path_stack);
    //Move the bus to the new position
    int currentX = newPosition->xpos;
    int currentY = newPosition->ypos;
    map[currentX][currentY]->type = 3;
    map[bus->xpos][bus->ypos]->type = 0;
    bus = map[currentX][currentY];
    int i= 0;

    //if the bus reaches a drop off position, then the task is complete and can be popped from the stack
    if((bus->xpos == priorityQueue->heap_data[0]->ending_x) && (bus->ypos == priorityQueue->heap_data[0]->ending_y)) pop_heap_priority(priorityQueue);

    // if the bus reaches a pickup positon
    if((bus->xpos == priorityQueue->heap_data[0]->starting_x) && (bus->ypos == priorityQueue->heap_data[0]->starting_y)) {
        // change it to dropoff mode
        priorityQueue->heap_data[0]->pickup_mode = false;
        // set the end position to be identified as a task for the printout
        map[priorityQueue->heap_data[0]->ending_x][priorityQueue->heap_data[0]->ending_y]->type = 2;
        //Astar(bus, map[priorityQueue->heap_data[0]->ending_x][priorityQueue->heap_data[0]->ending_y], priorityQueue->heap_data[0]);
    }

    //update the path, cost of each task in the priority queue
    while(i < priorityQueue->heap_size){

        // declare a destination to be passed into Astar
        Node* destination;

        // if the current task is in pickup mode set the destination to be the pick-up location
        if(priorityQueue->heap_data[i]->pickup_mode) destination = map[priorityQueue->heap_data[i]->starting_x][priorityQueue->heap_data[i]->starting_y];
            // if the current task is in drop off, mode set the destination to be the drop off location
        else destination = map[priorityQueue->heap_data[i]->ending_x][priorityQueue->heap_data[i]->ending_y];

        //Update path
        Astar(bus, destination, priorityQueue->heap_data[i]);

        // Update Cost
        priorityQueue->heap_data[i]->cost = path_cost(priorityQueue->heap_data[i]);

        //Update Current Coordinates
        // priorityQueue->heap_data[i]->starting_x = bus->posx;
        // priorityQueue->heap_data[i]->starting_y = bus->posy;

        //Add to the temporary storage Queue
        enqueue(priorityQueue->heap_data[i]);
        i++;
    }

    //clear the current heap
    priorityQueue->heap_size = 0;

    //insert everything back into the heap in their sorted positions after the cost is recomputed
    while(!QIsEmpty()){
        task* temp;
        temp = dequeue(temp);
        insert_priority(priorityQueue, temp);
    }
}

//primitive version of simulate just to move the bus across the map through the path from Astar
void takeStep(stack* path_stack){
    //pop the node off the stack given
    Node* current = pop(path_stack);
    //set the x y to type bus
    map[current->xpos][current->ypos]->type = 3;
    //set the node where the bus was to road
    map[bus->xpos][bus->ypos]->type = 0;
    //set bus to the x y position
    bus->xpos = current->xpos;
    bus->ypos = current->ypos;
}


//abs(inputYStart-9)


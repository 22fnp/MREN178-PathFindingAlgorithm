#include "pathfinding.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int getGHCost(Node* current, Node* end) {
    int x1 = current->xpos;
    int y1 = current->ypos;
    int x2 = end->xpos;
    int y2 = end->ypos;
    int euclidean = 10 * sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
    return euclidean;
}

int getFCost(Node* current, Node* neighbour, Node* target, int check) {
    // G tracks cost from the current route so far, H estimates distance to the target.
    int g_cost = getGHCost(current, neighbour) + current->g_cost;
    int h_cost = getGHCost(neighbour, target);

    if (!check) {
        neighbour->g_cost = g_cost;
        neighbour->h_cost = h_cost;
    }

    return g_cost + h_cost;
}

bool Astar(Node* start, Node* target, task* T) {
    // Closed stores nodes we have already finalized for this search.
    Node* closed[100];
    for (int i = 0; i < 100; i++) {
        closed[i] = NULL;
    }

    // Open is the frontier of candidate nodes ordered by best known F cost.
    heap* open = initialize();
    int count1 = -1;
    insert(open, start);

    Node* current;
    Node* neighbour;

    while (1) {
        current = pop_heap(open);
        if (count1 == 70) {
            printf("Not possible\n");
            return ERR;
        }

        closed[++count1] = current;
        if (current == target) {
            // Reset the stored route before rebuilding it from parent pointers.
            T->path_stack->top = -1;
            while (current->xpos != start->xpos || current->ypos != start->ypos) {
                push(current, T->path_stack);
                current = current->parent;
            }
            return OK;
        }

        int countStuck = 0;
        for (int i = 0; i < 8; i++) {
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
                default:
                    neighbour = current->upRight;
                    break;
            }

            // Ignore invalid, blocked, or already-closed neighbors.
            if (neighbour == NULL || neighbour->type == 1 || checkArray(neighbour, closed, 100)) {
                countStuck++;
                if (countStuck == 8 && open->heap_size == 0) {
                    printf("Not Possible\n");
                    return ERR;
                }
                continue;
            }

            // Update a neighbor when we find a cheaper route or encounter it for the first time.
            if (neighbour->f_cost > getFCost(current, neighbour, target, 1)
                || !checkArray(neighbour, open->heap_data, open->heap_size)) {
                neighbour->f_cost = getFCost(current, neighbour, target, 0);
                neighbour->parent = current;

                if (!checkArray(neighbour, open->heap_data, open->heap_size)) {
                    insert(open, neighbour);
                }
            }
        }
    }
}

int path_cost(task* T) {
    int total_cost = 0;
    // The path is stored node-by-node, so cost is reconstructed from consecutive moves.
    for (int i = 0; i < T->path_stack->top; i++) {
        if (T->path_stack->path[i]->down == T->path_stack->path[i + 1]
            || T->path_stack->path[i]->up == T->path_stack->path[i + 1]
            || T->path_stack->path[i]->left == T->path_stack->path[i + 1]
            || T->path_stack->path[i]->right == T->path_stack->path[i + 1]) {
            total_cost += 10;
        } else {
            total_cost += 14;
        }
    }

    return total_cost;
}

Node* createNode(int i, int j) {
    Node* new_node = (Node*) malloc(sizeof(Node));
    if (new_node == NULL) {
        printf("Memory allocation failed");
    }

    new_node->xpos = i;
    new_node->ypos = j;
    new_node->type = 0;
    new_node->parent = NULL;
    new_node->up = NULL;
    new_node->down = NULL;
    new_node->right = NULL;
    new_node->left = NULL;
    new_node->upLeft = NULL;
    new_node->upRight = NULL;
    new_node->downLeft = NULL;
    new_node->downRight = NULL;
    new_node->f_cost = 0;
    new_node->g_cost = 0;
    new_node->h_cost = 0;
    return new_node;
}

void random1(void) {
    int count = 0;
    int trap = 0;
    bool trapped = false;

    time_t t;
    srand((unsigned) time(&t));
    while (count < 30) {
        int x = rand() % 10;
        int y = rand() % 10;

        if (map[x][y]->type == 0) {
            // Avoid trapping the bus completely at the origin during random obstacle placement.
            if ((x == 1 && y == 1) || (x == 1 && y == 0) || (x == 0 && y == 1)) {
                trap++;
            }
            if (trap == 3) {
                trapped = true;
            }

            if (!trapped) {
                map[x][y]->type = 1;
                count++;
            }

            if (trap == 3) {
                trap--;
                trapped = false;
            }
        }
    }
}

Node* path_finding_initialization(void) {
    // First create all nodes, then wire them together as a graph.
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            map[i][j] = createNode(i, j);
        }
    }

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if (i > 0) {
                map[i][j]->up = map[i - 1][j];
            }
            if (i < 9) {
                map[i][j]->down = map[i + 1][j];
            }
            if (j > 0) {
                map[i][j]->left = map[i][j - 1];
            }
            if (j < 9) {
                map[i][j]->right = map[i][j + 1];
            }
            if (i > 0 && j > 0) {
                map[i][j]->upLeft = map[i - 1][j - 1];
            }
            if (i < 9 && j > 0) {
                map[i][j]->downLeft = map[i + 1][j - 1];
            }
            if (i > 0 && j < 9) {
                map[i][j]->upRight = map[i - 1][j + 1];
            }
            if (i < 9 && j < 9) {
                map[i][j]->downRight = map[i + 1][j + 1];
            }
        }
    }

    // The bus always starts at the lower-left corner from the user's perspective.
    map[0][0]->type = 3;
    bus = map[0][0];
    random1();
    return bus;
}

bool is_within_bounds(int x, int y) {
    return x >= 0 && x < 10 && y >= 0 && y < 10;
}

bool is_obstacle(int x, int y) {
    if (!is_within_bounds(x, y)) {
        return false;
    }

    return map[x][y]->type == 1;
}

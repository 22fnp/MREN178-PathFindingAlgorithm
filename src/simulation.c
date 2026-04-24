#include "simulation.h"

#include "pathfinding.h"

#include <stdio.h>
#include <stdlib.h>

task* initialize_task(int xi, int yi, int xf, int yf, char name) {
    if (!is_within_bounds(xi, yi) || !is_within_bounds(xf, yf)) {
        printf("INVALID POSITION\n");
        return NULL;
    }

    // Routes cannot start or end on blocked cells.
    if (map[xi][yi]->type == 1 || map[xf][yf]->type == 1) {
        printf("INVALID POSITION\n");
        return NULL;
    }

    task* new_task = (task*) malloc(sizeof(struct Task));
    stack* new_stack = (stack*) malloc(sizeof(struct Stack));

    new_task->path_stack = new_stack;
    new_task->path_stack->top = -1;
    new_task->path_stack->size = 70;
    new_task->starting_x = xi;
    new_task->starting_y = yi;
    new_task->ending_x = xf;
    new_task->ending_y = yf;
    new_task->name = name;
    return new_task;
}

char searchQueue(heap_priority* priorityQueue, int j, int k) {
    // Show the pickup label before pickup, then the dropoff label after pickup.
    for (int i = 0; i < priorityQueue->heap_size; i++) {
        if (priorityQueue->heap_data[i]->starting_x == j
            && priorityQueue->heap_data[i]->starting_y == k
            && priorityQueue->heap_data[i]->pickup_mode) {
            return priorityQueue->heap_data[i]->name;
        } else if (priorityQueue->heap_data[i]->ending_x == j
                   && priorityQueue->heap_data[i]->ending_y == k
                   && !priorityQueue->heap_data[i]->pickup_mode) {
            return priorityQueue->heap_data[i]->name;
        }
    }

    return '!';
}

void print_map_user(heap_priority* priorityQueue) {
    printf("\n    X  0 1 2 3 4 5 6 7 8 9\n");
    printf("   +---------------------+\n");
    for (int i = 9; i > -1; i--) {
        printf("Y %d | ", i);
        for (int j = 0; j < 10; j++) {
            if (map[j][i]->type == 1) {
                printf("%c ", 254);
            } else if (map[j][i]->type == 0) {
                printf("%c ", 95);
            } else if (map[j][i]->type == 2) {
                char name = searchQueue(priorityQueue, j, i);
                printf("%c ", name);
            } else {
                printf("%d ", map[j][i]->type);
            }
        }
        printf("|\n");
    }
    printf("   +---------------------+\n");
    printf("Legend: _ = road, %c = obstacle, 3 = bus, A-Z = task label\n", 254);
}

void print_map_computer(void) {
    // This view keeps array order for debugging internal indexing.
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if (map[j][i]->type == 1) {
                printf("%c ", 254);
            } else if (map[j][i]->type == 0) {
                printf("%c ", 95);
            } else {
                printf("%d ", map[i][j]->type);
            }
        }
        printf("\n");
    }
}

task* add_route(int xi, int yi, int xf, int yf, heap_priority* queue, char name) {
    task* add_task = initialize_task(xi, yi, xf, yf, name);
    if (add_task == NULL) {
        return NULL;
    }

    // New tasks always begin by routing toward their pickup location.
    add_task->pickup_mode = true;
    if (Astar(bus, map[add_task->starting_x][add_task->starting_y], add_task) == ERR) {
        return NULL;
    }

    add_task->cost = path_cost(add_task);
    insert_priority(queue, add_task);
    map[xi][yi]->type = 2;
    return add_task;
}

void Simulate(heap_priority* priorityQueue) {
    if (priorityQueue->heap_size == 0) {
        printf("Enter a route to follow first.\n");
        return;
    }

    // Advance one step along the current best route.
    Node* newPosition = pop(priorityQueue->heap_data[0]->path_stack);
    int currentX = newPosition->xpos;
    int currentY = newPosition->ypos;
    map[currentX][currentY]->type = 3;
    map[bus->xpos][bus->ypos]->type = 0;
    bus = map[currentX][currentY];

    int i = 0;
    if ((bus->xpos == priorityQueue->heap_data[0]->ending_x)
        && (bus->ypos == priorityQueue->heap_data[0]->ending_y)) {
        pop_heap_priority(priorityQueue);
    }

    // Once the bus reaches pickup, the same task starts routing toward dropoff.
    if ((bus->xpos == priorityQueue->heap_data[0]->starting_x)
        && (bus->ypos == priorityQueue->heap_data[0]->starting_y)) {
        priorityQueue->heap_data[0]->pickup_mode = false;
        map[priorityQueue->heap_data[0]->ending_x][priorityQueue->heap_data[0]->ending_y]->type = 2;
    }

    while (i < priorityQueue->heap_size) {
        Node* destination;
        if (priorityQueue->heap_data[i]->pickup_mode) {
            destination = map[priorityQueue->heap_data[i]->starting_x][priorityQueue->heap_data[i]->starting_y];
        } else {
            destination = map[priorityQueue->heap_data[i]->ending_x][priorityQueue->heap_data[i]->ending_y];
        }

        // Every step can change which task is cheapest, so paths are recomputed and re-ranked.
        Astar(bus, destination, priorityQueue->heap_data[i]);
        priorityQueue->heap_data[i]->cost = path_cost(priorityQueue->heap_data[i]);
        enqueue(priorityQueue->heap_data[i]);
        i++;
    }

    priorityQueue->heap_size = 0;
    while (!QIsEmpty()) {
        task* temp;
        temp = dequeue(temp);
        insert_priority(priorityQueue, temp);
    }
}

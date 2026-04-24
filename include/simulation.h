#ifndef SIMULATION_H
#define SIMULATION_H

#include "data_structures.h"

// Route creation, map rendering, and simulation helpers.
task* initialize_task(int xi, int yi, int xf, int yf, char name);
char searchQueue(heap_priority* priorityQueue, int j, int k);
void print_map_user(heap_priority* priorityQueue);
void print_map_computer(void);
task* add_route(int xi, int yi, int xf, int yf, heap_priority* queue, char name);
void Simulate(heap_priority* priorityQueue);

#endif

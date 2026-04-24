#ifndef PATHFINDING_H
#define PATHFINDING_H

#include "data_structures.h"

// Heuristic and pathfinding helpers.
int getGHCost(Node* current, Node* end);
int getFCost(Node* current, Node* neighbour, Node* target, int check);
bool Astar(Node* start, Node* target, task* T);
int path_cost(task* T);

// Grid construction helpers.
Node* createNode(int i, int j);
void random1(void);
Node* path_finding_initialization(void);
bool is_within_bounds(int x, int y);
bool is_obstacle(int x, int y);

#endif

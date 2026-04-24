#include "data_structures.h"

#include <stdio.h>

Node* map[10][10];
Node* bus;
int rear = -1;
int front = -1;
int temp_queue_size = 0;
task* temp_queue[70];

struct min_heap_priority* initialize_priority(void) {
    // Tasks with lower path cost rise to the front of this heap.
    heap_priority* new_heap = (heap_priority*) malloc(sizeof(struct min_heap_priority));
    new_heap->heap_size = 0;
    return new_heap;
}

void heapify_priority(heap_priority* heap, task* T, int element_index) {
    while (element_index > 0) {
        int parent_index = (element_index - 1) / 2;
        // Stop once the heap ordering is satisfied.
        if (T->cost > heap->heap_data[parent_index]->cost) {
            heap->heap_data[element_index] = T;
            return;
        }

        heap->heap_data[element_index] = heap->heap_data[parent_index];
        element_index = parent_index;
    }

    heap->heap_data[0] = T;
}

void insert_priority(heap_priority* heap, task* T) {
    if (heap->heap_size == 70) {
        return;
    }

    int element_index = heap->heap_size;
    heap->heap_size = heap->heap_size + 1;
    heapify_priority(heap, T, element_index);
}

task* pop_heap_priority(heap_priority* heap) {
    task* temp;
    int r_child = 2;
    int l_child = 1;
    int empty_slot = 0;

    heap->heap_size = heap->heap_size - 1;
    temp = heap->heap_data[heap->heap_size];

    task* answer = heap->heap_data[0];
    while (l_child < heap->heap_size) {
        if (r_child < heap->heap_size) {
            if (heap->heap_data[r_child]->cost < heap->heap_data[l_child]->cost) {
                heap->heap_data[empty_slot] = heap->heap_data[r_child];
                empty_slot = r_child;
            } else {
                heap->heap_data[empty_slot] = heap->heap_data[l_child];
                empty_slot = l_child;
            }
        } else {
            heap->heap_data[empty_slot] = heap->heap_data[l_child];
            empty_slot = l_child;
        }

        l_child = 2 * empty_slot + 1;
        r_child = 2 * empty_slot + 2;
    }

    heapify_priority(heap, temp, empty_slot);
    return answer;
}

int checkFull(stack* path_stack) {
    if (path_stack->top == path_stack->size) {
        return 1;
    }

    return 0;
}

void push(Node* current, stack* path_stack) {
    if (checkFull(path_stack)) {
        return;
    }

    path_stack->path[++path_stack->top] = current;
}

int checkEmpty(stack* path_stack) {
    if (path_stack->top == -1) {
        return 1;
    }

    return 0;
}

Node* pop(stack* path_stack) {
    if (checkEmpty(path_stack)) {
        return NULL;
    }

    return path_stack->path[path_stack->top--];
}

bool checkArray(Node* neighbour, Node* array[], int size) {
    // This project uses linear scans for membership checks in small fixed-size arrays.
    for (int i = 0; i < size; i++) {
        if (array[i] == NULL) {
            return false;
        }

        if (array[i] == neighbour) {
            return true;
        }
    }

    return false;
}

struct min_heap* initialize(void) {
    // This heap stores frontier nodes for A* ordered by F cost.
    heap* new_heap = (heap*) malloc(sizeof(struct min_heap));
    new_heap->heap_size = 0;
    return new_heap;
}

void heapify(heap* heap, Node* node, int node_index) {
    while (node_index > 0) {
        int parent_index = (node_index - 1) / 2;
        // Lower F cost wins; G cost is used as the tie breaker from the original code.
        if (node->f_cost > heap->heap_data[parent_index]->f_cost) {
            heap->heap_data[node_index] = node;
            return;
        } else if (node->f_cost == heap->heap_data[parent_index]->f_cost) {
            if (node->g_cost >= heap->heap_data[parent_index]->g_cost) {
                heap->heap_data[node_index] = node;
                return;
            }
        }

        heap->heap_data[node_index] = heap->heap_data[parent_index];
        node_index = parent_index;
    }

    heap->heap_data[0] = node;
}

void insert(heap* heap, Node* node) {
    if (heap->heap_size == 70) {
        return;
    }

    int node_index = heap->heap_size;
    heap->heap_size = heap->heap_size + 1;
    heapify(heap, node, node_index);
}

Node* pop_heap(heap* heap) {
    Node* temp;
    int r_child = 2;
    int l_child = 1;
    int empty_slot = 0;

    heap->heap_size = heap->heap_size - 1;
    temp = heap->heap_data[heap->heap_size];

    Node* answer = heap->heap_data[0];
    while (l_child < heap->heap_size) {
        if (r_child < heap->heap_size) {
            if (heap->heap_data[r_child]->f_cost < heap->heap_data[l_child]->f_cost) {
                heap->heap_data[empty_slot] = heap->heap_data[r_child];
                empty_slot = r_child;
            } else if (heap->heap_data[r_child]->f_cost > heap->heap_data[l_child]->f_cost) {
                heap->heap_data[empty_slot] = heap->heap_data[l_child];
                empty_slot = l_child;
            } else if (heap->heap_data[r_child]->g_cost < heap->heap_data[l_child]->g_cost) {
                heap->heap_data[empty_slot] = heap->heap_data[r_child];
                empty_slot = r_child;
            } else {
                heap->heap_data[empty_slot] = heap->heap_data[l_child];
                empty_slot = l_child;
            }
        } else {
            heap->heap_data[empty_slot] = heap->heap_data[l_child];
            empty_slot = l_child;
        }

        l_child = 2 * empty_slot + 1;
        r_child = 2 * empty_slot + 2;
    }

    heapify(heap, temp, empty_slot);
    return answer;
}

bool QIsFull(void) {
    return temp_queue_size == 70;
}

bool QIsEmpty(void) {
    return temp_queue_size == 0;
}

bool enqueue(task* T) {
    if (QIsFull()) {
        return ERR;
    }

    // The temporary queue is a circular buffer used while rebuilding task priorities.
    rear = (++rear) % 70;
    temp_queue[rear] = T;
    temp_queue_size++;
    if (temp_queue_size == 1) {
        front = rear;
    }

    return OK;
}

task* dequeue(task* t) {
    if (QIsEmpty()) {
        return NULL;
    }

    t = temp_queue[front];
    front = ++front % 70;
    temp_queue_size--;
    if (QIsEmpty()) {
        rear = -1;
        front = -1;
    }

    return t;
}

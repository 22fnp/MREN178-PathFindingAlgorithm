#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "pathfinding.h"

int main(void) {
    path_finding_initialization();

    assert(bus == map[0][0]);
    assert(map[0][0]->type == 3);
    assert(map[0][0]->right != NULL);
    assert(map[0][0]->down != NULL);

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            free(map[i][j]);
        }
    }

    printf("test_pathfinding passed\n");
    return 0;
}

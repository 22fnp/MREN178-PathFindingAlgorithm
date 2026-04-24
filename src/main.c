#include <stdio.h>
#include <stdlib.h>

#include "pathfinding.h"
#include "simulation.h"

static void clear_input_buffer(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
    }
}

static int read_menu_choice(void) {
    int input = 0;

    printf("\nMenu:\n");
    printf("  1 - Add route\n");
    printf("  2 - Simulate one step\n");
    printf("  0 - Quit\n");
    printf("Choose an option: ");

    if (scanf("%d", &input) != 1) {
        clear_input_buffer();
        return -1;
    }

    return input;
}

static int read_coordinate_pair(const char* prompt, int* x, int* y) {
    printf("%s", prompt);
    if (scanf("%d %d", x, y) != 2) {
        clear_input_buffer();
        return 0;
    }

    return 1;
}

static int read_route_name(char* inputName) {
    printf("Enter route label (single character): ");
    clear_input_buffer();
    if (scanf("%c", inputName) != 1) {
        clear_input_buffer();
        return 0;
    }

    return 1;
}

static int validate_route_point(const char* label, int x, int y) {
    if (!is_within_bounds(x, y)) {
        printf("%s coordinates must stay within 0 to 9.\n", label);
        return 0;
    }

    if (is_obstacle(x, y)) {
        printf("%s coordinates (%d, %d) are blocked by an obstacle.\n", label, x, y);
        return 0;
    }

    return 1;
}

int main(void) {
    path_finding_initialization();

    heap_priority* queue = initialize_priority();
    // Show the user the coordinate system immediately before the first menu prompt.
    print_map_user(queue);

    while (1) {
        int input = read_menu_choice();
        if (input == -1) {
            printf("Invalid menu input.\n");
            continue;
        }

        if (input == 0) {
            break;
        }

        if (input == 1) {
            int inputXStart = 0;
            int inputYStart = 0;
            int inputXEnd = 0;
            int inputYEnd = 0;
            char inputName;

            if (!read_coordinate_pair("Enter pickup coordinates (x y): ", &inputXStart, &inputYStart)) {
                printf("Invalid pickup coordinates.\n");
                continue;
            }

            if (!read_coordinate_pair("Enter dropoff coordinates (x y): ", &inputXEnd, &inputYEnd)) {
                printf("Invalid dropoff coordinates.\n");
                continue;
            }

            if (!validate_route_point("Pickup", inputXStart, inputYStart)
                || !validate_route_point("Dropoff", inputXEnd, inputYEnd)) {
                continue;
            }

            if (!read_route_name(&inputName)) {
                printf("Invalid route label.\n");
                continue;
            }

            add_route(inputXStart, inputYStart, inputXEnd, inputYEnd, queue, inputName);
            print_map_user(queue);
        } else if (input == 2) {
            Simulate(queue);
            print_map_user(queue);
        } else {
            printf("Unknown option. Use 0, 1, or 2.\n");
        }
    }

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            free(map[i][j]);
        }
    }

    return 0;
}

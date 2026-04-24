# Group15PathFinder

This repository contains a small C project that simulates a bus moving on a `10 x 10` grid while servicing pickup and dropoff tasks. The grid is modeled as a graph, obstacles are generated randomly, routes are planned with A* pathfinding, and active tasks are prioritized with a min-heap based on path cost.

The recent cleanup focused on organization and modularity without changing the core behavior of the original program.

## Repository Layout

```text
.
|-- CMakeLists.txt
|-- README.md
|-- src/
|   |-- main.c
|   |-- pathfinding.c
|   |-- simulation.c
|   `-- data_structures.c
|-- include/
|   |-- pathfinding.h
|   |-- simulation.h
|   `-- data_structures.h
`-- tests/
    `-- test_pathfinding.c
```

## What The Program Does

At startup the program:

1. Creates a `10 x 10` grid of `Node` objects.
2. Connects each node to up to 8 neighbors.
3. Places the bus at `(0, 0)`.
4. Randomly marks 30 cells as blocked.
5. Enters an interactive loop where the user can:
   - add a route
   - simulate one movement step
   - quit the program

Each route stores:

- a pickup coordinate
- a dropoff coordinate
- a single-character label
- a stored path stack
- a computed path cost
- a mode indicating whether the task is heading toward pickup or dropoff

The bus always follows the task with the smallest current route cost. After each simulation step, all active task paths are recomputed and the task heap is rebuilt.

## File-By-File Summary

### `src/main.c`

This is the entry point and console driver for the program.

Responsibilities:

- initializes the map and the task queue
- prints the initial map
- presents the menu loop
- reads and validates user input
- calls `add_route(...)` for new tasks
- calls `Simulate(...)` to move the bus one step
- frees the map nodes before exiting

Notable cleanup changes:

- the queue is now initialized before first use
- the program now has a quit option
- prompts are clearer
- coordinate input is validated before indexing the map

### `src/pathfinding.c`

This file contains the pathfinding and grid-construction logic.

Key responsibilities:

- computes movement and heuristic costs with `getGHCost(...)`
- computes `f`, `g`, and `h` values with `getFCost(...)`
- performs A* search in `Astar(...)`
- computes full task path costs in `path_cost(...)`
- allocates grid nodes with `createNode(...)`
- randomly generates blockages with `random1()`
- builds and links the `10 x 10` node grid in `path_finding_initialization()`
- exposes `is_within_bounds(...)` for coordinate checks

This file is where the graph and A* behavior live.

### `src/simulation.c`

This file contains route/task management and map-printing behavior.

Key responsibilities:

- allocates tasks in `initialize_task(...)`
- prints task markers with `searchQueue(...)`
- prints the map for users in `print_map_user(...)`
- prints the map in array order in `print_map_computer(...)`
- creates and queues new routes in `add_route(...)`
- advances the simulation and reprioritizes tasks in `Simulate(...)`

This file is where the higher-level scheduling behavior lives.

### `src/data_structures.c`

This file owns the shared global state and the low-level data-structure helpers.

Key responsibilities:

- defines global storage for:
  - `map`
  - `bus`
  - temporary queue state used while rebuilding priorities
- implements the task min-heap:
  - `initialize_priority(...)`
  - `heapify_priority(...)`
  - `insert_priority(...)`
  - `pop_heap_priority(...)`
- implements stack helpers:
  - `checkFull(...)`
  - `push(...)`
  - `checkEmpty(...)`
  - `pop(...)`
- implements A* open-list heap helpers:
  - `initialize(...)`
  - `heapify(...)`
  - `insert(...)`
  - `pop_heap(...)`
- implements the temporary queue used during reprioritization:
  - `QIsFull(...)`
  - `QIsEmpty(...)`
  - `enqueue(...)`
  - `dequeue(...)`

This file now centralizes the shared state that previously lived in the old header.

### `include/data_structures.h`

This header defines the shared structs, constants, global declarations, and low-level helper prototypes.

It contains:

- `ERR` and `OK`
- `Node`
- `stack`
- `task`
- `heap_priority`
- `heap`
- `extern` declarations for the shared global state
- prototypes for heap, stack, and temporary queue helpers

This replaces the old header pattern where globals were defined directly in the header.

### `include/pathfinding.h`

This header exposes the pathfinding and grid setup API:

- `getGHCost(...)`
- `getFCost(...)`
- `Astar(...)`
- `path_cost(...)`
- `createNode(...)`
- `random1()`
- `path_finding_initialization()`
- `is_within_bounds(...)`
- `is_obstacle(...)`

### `include/simulation.h`

This header exposes the simulation and route-management API:

- `initialize_task(...)`
- `searchQueue(...)`
- `print_map_user(...)`
- `print_map_computer(...)`
- `add_route(...)`
- `Simulate(...)`

### `tests/test_pathfinding.c`

This is a small smoke test for the reorganized project.

It currently verifies that:

- initialization succeeds
- the bus starts at `(0, 0)`
- the starting node is marked correctly
- the grid links are present for immediate neighbors

It is intentionally small, but it gives the repository a starting point for future tests.

### `CMakeLists.txt`

This file now builds the project using normal separate compilation.

It:

- includes headers from `include/`
- builds the main executable from the `src/` files
- builds a small `test_pathfinding` executable from `tests/`
- links the math library where needed
- registers the smoke test with CTest

## Build Instructions

These commands are intended to work for anyone who clones the repository, as long as they have CMake and a C compiler installed. The generated `build/` directory is local output and should not be committed.

### Configure

```sh
cmake -S . -B build
```

### Build

```sh
cmake --build build
```

### Run The Main Program

On Windows:

```powershell
.\build\Group15PathFinder.exe
```

On macOS or Linux:

```sh
./build/Group15PathFinder
```

### Run The Smoke Test

On Windows:

```powershell
.\build\test_pathfinding.exe
```

On macOS or Linux:

```sh
./build/test_pathfinding
```

Or with CTest:

```sh
ctest --test-dir build --output-on-failure
```

## Generated Build Files

If you see files such as `build.ninja`, `.ninja_log`, `CTestTestfile.cmake`, or files under `build/CMakeFiles/`, those are generated by CMake and Ninja during local builds.

- `build.ninja` and `rules.ninja` tell the Ninja build tool how to compile the project.
- `.ninja_log` and `.ninja_deps` store incremental-build bookkeeping.
- `CTestTestfile.cmake` is generated by CMake so `ctest` knows what tests exist.
- `build/CMakeFiles/` contains internal compiler and dependency metadata.

These files are relevant to a local build, but they are not source files and should generally not be tracked in the repository. A fresh clone should be able to regenerate them by running the configure and build commands above.

## How The Program Flows

1. `main()` calls `path_finding_initialization()`.
2. The node grid is created and linked.
3. The bus is placed at `(0, 0)`.
4. Random blockages are generated.
5. The user adds routes through the menu.
6. `add_route(...)` computes a path from the bus to the pickup point and inserts the task into the priority heap.
7. `Simulate(...)` moves the bus by one step along the current best route.
8. After each step, task paths and costs are recomputed and the heap is rebuilt.

## Cleanup Results

The repository organization was improved in the following ways:

- the old direct `#include "Functions.c"` build pattern was removed
- the broken header guard was replaced with normal header guards in the new headers
- shared global state was moved out of headers and into one implementation file
- dead code and unused items were removed:
  - the giant commented-out duplicate block in the old entry file
  - `random2`
  - `takeStep`
  - `temporaryHeapStorage`
  - `busians`
  - `windows.h`
- the build now uses normal multi-file compilation
- the entry-point UI now has clearer prompts, validation, and a quit path
- small inline comments were added where they help explain intent

## Suggested Future Works

- add unit tests for route insertion, A* success cases, and blocked-route cases
- add cleanup for dynamically allocated tasks, stacks, and heaps before program exit
- separate map rendering from simulation logic even further if the project keeps growing
- add deterministic map-generation support for reproducible debugging
- add a non-interactive mode for automated demonstrations or grading

## Summary

This repository now reflects a more standard C project layout while preserving the original core behavior: graph creation, A* pathfinding, task prioritization, and step-by-step bus simulation. The code is easier to navigate, safer to build, and more approachable for new contributors than the original single-entry-file arrangement.

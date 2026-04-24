# Group15PathFinder

This repository contains a small C program that simulates a bus moving on a `10 x 10` grid while servicing pickup and dropoff tasks. The grid is modeled as a graph, obstacles are placed randomly, routes are planned with an A* search, and pending tasks are prioritized by estimated path cost.

The project is useful as a class-style prototype of:

- graph modeling with linked neighbors
- A* pathfinding on a grid with diagonal movement
- a task priority queue implemented as a min-heap
- a simple text-based simulation loop

## What The Program Does

At startup the program:

1. Creates a `10 x 10` grid of `Node` objects.
2. Connects each node to up to 8 neighbors.
3. Places the bus at `(0, 0)`.
4. Randomly marks 30 cells as blocked.
5. Enters an interactive loop where the user can:
   - add a route
   - advance the simulation by one step

Each route has:

- a pickup coordinate
- a dropoff coordinate
- a single-character name
- a computed path
- a computed cost
- a mode indicating whether the bus is traveling to pickup or to dropoff

The bus always follows the task with the lowest current cost. After each simulation step, the code recomputes task paths and reorders the task heap.

## Repository Layout

- [178FinalProject.c](<c:\Users\jssau\MREN178-PathFindingAlgorithm\178FinalProject.c>) is the entry point and current interactive driver.
- [Functions.c](<c:\Users\jssau\MREN178-PathFindingAlgorithm\Functions.c>) contains almost all real program logic.
- [FunctionsAndDeclarations.h](<c:\Users\jssau\MREN178-PathFindingAlgorithm\FunctionsAndDeclarations.h>) declares types, globals, constants, and function prototypes.
- [CMakeLists.txt](<c:\Users\jssau\MREN178-PathFindingAlgorithm\CMakeLists.txt>) contains a minimal CMake target definition.

## File-By-File Summary

### `178FinalProject.c`

This file is the current executable entry point. Its main responsibilities are:

- initializing the map by calling `path_finding_initialization()`
- creating the task priority queue
- displaying the current map
- reading user input from the console
- calling `add_route(...)` when the user enters a new request
- calling `Simulate(...)` to move the bus one step at a time
- freeing the allocated grid nodes at program shutdown

Important details:

- It includes `Functions.c` directly instead of including only the header. That is unusual in C projects, but it is how this repository currently builds.
- A very large block of commented-out code at the top duplicates structs and functions that already exist in `Functions.c`. It appears to be an older in-file copy kept around during development.
- `print_map_user(queue);` is called before `queue` is initialized. That is undefined behavior and should be fixed before treating the program as stable.
- The main loop runs forever and currently has no quit option.
- The second prompt says `"starting coordinates"` twice; the second one is really the destination prompt.

Practical takeaway:

This file is mostly UI and orchestration code, but it currently contains a lot of historical clutter that makes it harder to understand than it needs to be.

### `Functions.c`

This is the core implementation file. It contains nearly all of the project logic.

Major areas in this file:

#### 1. Task priority queue

These functions implement a min-heap of `task*` ordered by `task->cost`:

- `initialize_priority()`
- `heapify_priority(...)`
- `insert_priority(...)`
- `pop_heap_priority(...)`

This heap decides which route the bus should service next.

#### 2. A* support utilities

These helpers support pathfinding:

- `getGHCost(...)` computes a Euclidean-distance-based heuristic/cost estimate
- `checkFull(...)`, `push(...)`, `checkEmpty(...)`, `pop(...)` manage the path stack
- `getFCost(...)` updates or evaluates `g`, `h`, and `f` costs
- `checkArray(...)` searches a node array linearly

#### 3. Open-list heap for A*

These functions implement a second min-heap, this time for `Node*`:

- `initialize()`
- `heapify(...)`
- `insert(...)`
- `pop_heap(...)`

This heap is the A* frontier, ordered by `f_cost` with `g_cost` as a tiebreaker.

#### 4. Pathfinding

- `Astar(...)` searches from a start node to a target node and stores the resulting path in the task's stack.

Behavior:

- blocked cells (`type == 1`) are avoided
- 8-directional movement is allowed
- each found path is stored by following parent pointers backward from target to start

#### 5. Task creation and cost calculation

- `initialize_task(...)` allocates and initializes a route
- `path_cost(...)` converts a stored path into a numeric movement cost

Straight moves cost `10`; diagonal moves cost `14`.

#### 6. Map display helpers

- `searchQueue(...)` finds a task marker to print
- `print_map_user(...)` prints the map with `(0,0)` at bottom-left
- `print_map_computer(...)` prints the map in array order

Cell types are effectively:

- `0` = open road
- `1` = wall/blockage
- `2` = active task marker
- `3` = bus

#### 7. Grid and obstacle generation

- `createNode(...)` allocates one node
- `random1()` places random walls while trying not to completely trap the bus at startup
- `random2()` appears to be an alternate wall generator intended to avoid corner traps
- `path_finding_initialization()` builds the graph, links neighbors, places the bus, and generates walls

#### 8. Simulation

- `add_route(...)` creates a task, computes its initial path from the current bus position to the pickup point, and inserts it into the priority queue
- `QIsFull()`, `QIsEmpty()`, `enqueue(...)`, `dequeue(...)` manage a temporary circular queue used while rebuilding task priorities
- `Simulate(...)` advances the bus by one node, updates task modes, recomputes paths, and rebuilds the task heap
- `takeStep(...)` is a simpler movement helper that looks like an earlier simulation utility

Practical takeaway:

This file contains the actual system behavior: data structures, pathfinding, route scheduling, and simulation updates.

### `FunctionsAndDeclarations.h`

This header centralizes shared declarations for the program.

It contains:

- `ERR` and `OK` macros
- the definitions of `Node`, `stack`, `task`, `heap_priority`, and `heap`
- global state declarations for:
  - `map`
  - `bus`
  - `busians`
  - `busians_index`
  - temporary queue state
- function prototypes for all major functions in `Functions.c`

Important detail:

- The include guard is malformed: `#ifndef def FUNC_DEC` is not valid style and produces a compiler warning. It should be replaced with a normal pattern such as:

```c
#ifndef FUNC_DEC
#define FUNC_DEC
...
#endif
```

- The header defines global variables directly. In a cleaner multi-file C layout, these would usually be declared with `extern` in the header and defined once in a `.c` file.

Practical takeaway:

The header currently acts as both an interface file and a storage location for global state, which works poorly as the project grows.

### `CMakeLists.txt`

This is a minimal build definition:

- requires CMake `3.26`
- creates a C11 project named `Group15PathFinder`
- builds a single executable from `178FinalProject.c`

Important detail:

- It does not list `Functions.c` as a separate compilation unit.
- The build succeeds only because `178FinalProject.c` directly includes `Functions.c`.

Practical takeaway:

The current CMake file reflects the project's current workaround-based layout, not a clean modular layout.

## Current Build Behavior

The repository currently compiles with a direct command like:

```powershell
gcc 178FinalProject.c -o build\app.exe -lm
```

That works because:

- `178FinalProject.c` includes `Functions.c`
- `Functions.c` includes `FunctionsAndDeclarations.h`

Current warning observed during compilation:

- `FunctionsAndDeclarations.h` has an invalid include-guard directive and emits a preprocessor warning

## How The Program Flows

High-level control flow:

1. `main()` calls `path_finding_initialization()`
2. The map and node graph are built
3. The user adds tasks with pickup and dropoff points
4. `add_route(...)` computes a path to the pickup point and inserts the task into the priority queue
5. `Simulate(...)` moves the bus one step along the current best task
6. After each step, task paths and costs are recomputed and the priority queue is rebuilt

## Known Issues And Rough Edges

These are worth knowing before further development:

- `178FinalProject.c` includes `Functions.c` directly instead of using separate compilation.
- `queue` is used before initialization in `main()`.
- The header defines globals instead of declaring them with `extern`.
- There is a large amount of commented-out duplicate code in `178FinalProject.c`.
- `random2()` appears unused.
- `takeStep()` appears unused by the main flow.
- `temporaryHeapStorage` is declared but appears unused.
- `busians` and `busians_index` are written to, but do not appear to affect runtime behavior.
- `windows.h` is included in `Functions.c` but does not appear to be used.
- Memory allocated for tasks, stacks, heaps, and temporary structures is not fully cleaned up.
- The simulation loop has no quit option.
- Input validation is minimal.

## Suggested Cleanup Roadmap

If the goal is to make the repository understandable to new contributors, the most valuable cleanup sequence is:

1. Fix correctness and build hygiene.
2. Remove commented-out and unused code.
3. Separate interface, implementation, and program entry point cleanly.
4. Add usage documentation and inline comments only where they help.
5. Add basic tests for map creation, route creation, and A* pathfinding.

Recommended concrete changes:

- Change `178FinalProject.c` to include `FunctionsAndDeclarations.h` instead of `Functions.c`.
- Update `CMakeLists.txt` to compile both `178FinalProject.c` and `Functions.c`.
- Fix the include guard in the header.
- Move global variable definitions into one `.c` file and use `extern` declarations in the header.
- Remove the giant commented-out duplicate block in `178FinalProject.c`.
- Remove unused variables and functions after confirming they are truly dead.
- Rename files to clearer names such as:
  - `main.c`
  - `pathfinding.c`
  - `pathfinding.h`
  - `simulation.c`
  - `simulation.h`
- Add a quit option to the interactive loop.
- Validate user coordinates before indexing into `map`.

## Suggested Future Structure

A cleaner version of the repository could look like:

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

## Summary

This repository already contains a real working idea: a grid graph, A* pathfinding, route prioritization, and step-by-step simulation. The main challenge is not missing functionality, but organization. Most of the cleanup work should focus on removing duplicated history, separating files by responsibility, fixing a few correctness issues, and documenting the intended architecture clearly.

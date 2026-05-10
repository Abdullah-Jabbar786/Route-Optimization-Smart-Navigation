# Route Optimization and Smart Navigation

A C++ simulation project using Raylib that demonstrates pathfinding algorithms and data structures in a virtual city environment. The application allows users to navigate through various zones (Main Zone, Hotel, Bank, Amusement Park, Food Court, Parking Lot) and find the most efficient routes using Dijkstra's and A* algorithms.

## Features

- **Interactive Navigation**: Navigate through a Main Zone and enter various sub-zones.
- **Pathfinding Algorithms**: Visualizes shortest paths using Dijkstra's Algorithm and A* Search Algorithm.
- **Categorized Search**: A `BuildingReport` system uses a Tree data structure to categorize and search for specific buildings or amenities.
- **Travel History**: Tracks your journey using a Stack data structure, allowing you to review your path, total distance, and time taken at the end.
- **Vehicle Selection**: Choose from different characters/vehicles (Hat, Cart, Car) with unique visual styles.
- **Teleportation**: Seamless transitions between different zones.

## Tech Stack

- **Language**: C++
- **Graphics Library**: Raylib
- **Data Structures**:
  - Graphs (Adjacency List) for map representation.
  - Trees for building categorization and reporting.
  - Stacks for tracking travel history.
  - Priority Queues for pathfinding algorithms.
  - Hash Maps (Unordered Maps) for node lookups.

## Project Structure

- `src/`: Contains all source code.
  - `main.cpp`: Entry point, main loop, and UI state management.
  - `mainZone.cpp/h`: Manages the central hub and connects to other zones.
  - `graph.cpp/h`: Graph data structure implementation.
  - `dijkstra.cpp/h`: Dijkstra's shortest path algorithm.
  - `astar.cpp/h`: A* search algorithm implementation.
  - `building_report.cpp/h`: Tree-based reporting system.
  - `[zone].cpp/h`: Specific logic for sub-zones (Hotel, Bank, etc.).

## Getting Started

### Prerequisites

- C++ Compiler (GCC/MinGW recommended)
- VS Code (Recommended IDE)
- Raylib installed and configured (The project uses a Raylib template structure).

### Building and Running

1. **Open the Project**: Open the `main.code-workspace` file in VS Code.
2. **Compile**: Press `F5` to compile and run the project using the configured task.
3. **Controls**:
   - **Mouse**: Click to select characters, buttons, and navigation targets.
   - **Keyboard**: Type in the search bar for the Building Report.

## Usage

1. **Selection**: Start by selecting a character/vehicle.
2. **Main Zone**: Search for a destination using the search bar or explore.
3. **Sub-Zones**: Enter buildings like the Hotel or Amusement Park to explore their specific internal maps.
4. **Building Report**: Access the report from the loading screen to search for buildings and see their categories.
5. **End Journey**: Go to the Parking Lot and exit to view your full travel history and stats.

## Contributors

- **AHM** - Developer

## License

All rights reserved.

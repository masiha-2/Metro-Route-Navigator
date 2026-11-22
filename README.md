# Metro Route Navigator (DSA Project in C++)

A modular, menu-driven **Metro Route Navigation System** built in C++ using core Data Structures and Algorithms from scratch. The application models a multi-line metro rail network as a **Weighted Undirected Graph** and computes optimal routes based on **shortest distance**, **fastest travel time**, and **minimum number of stops** — while supporting real-time track maintenance (dynamic route blocking & automatic rerouting) and file persistence.

---

## Key Features

- **Custom Min-Heap (Built from Scratch):** Implemented `MinHeap<T>` using heapify-up and heapify-down operations without relying on `std::priority_queue`.
- **Shortest Distance & Fastest Time (Dijkstra's Algorithm):** Computes optimal paths in $O((V + E) \log V)$ using the custom Min-Heap.
- **Minimum Stops Finder (BFS):** Uses Breadth-First Search with a Queue to find routes with the fewest station halts in $O(V + E)$.
- **Side-by-Side Route Comparison:** Compare Shortest Distance, Fastest Time, and Minimum Stops for any station pair in one view.
- **Interchange Alerts & Fare Estimator:** Detects metro line switches automatically, adds realistic interchange buffer time, and calculates slab-based INR fares.
- **Dynamic Maintenance Mode (Live Rerouting):** Block or unblock any direct track at runtime; Dijkstra and BFS instantly recalculate alternate paths.
- **Persistent File Storage (`fstream`):** Loads and saves stations (`data/stations.txt`) and routes (`data/routes.txt`) automatically.

---

## Data Structures & Complexity Analysis

| Module / Operation | Data Structure / Algorithm | Time Complexity | Space Complexity |
| :--- | :--- | :---: | :---: |
| Network Representation | Weighted Graph (Adjacency List) | $O(1)$ edge insert | $O(V + E)$ |
| Priority Queue Engine | Custom Binary Min-Heap | $O(\log V)$ push/pop | $O(V)$ |
| Shortest Distance / Time | Dijkstra's Algorithm | $O((V + E) \log V)$ | $O(V)$ |
| Minimum Stops Route | Breadth-First Search (BFS) | $O(V + E)$ | $O(V)$ |
| Station Lookup (Name / ID) | Hash Map (`unordered_map`) | $O(1)$ average | $O(V)$ |
| Path Reconstruction | Parent Map + Stack | $O(V)$ | $O(V)$ |
| Block / Unblock Track | Edge Active Status Flag | $O(\text{deg}(u))$ | $O(1)$ |

---

## Project Structure

```text
Metro-Route-Navigator-DSA/
├── include/
│   ├── MinHeap.h        # Custom Min-Heap template implementation
│   └── Graph.h          # Station, Edge, PathResult structs & MetroGraph class
├── src/
│   ├── Graph.cpp        # Graph loader, Dijkstra, BFS, fare & admin logic
│   └── main.cpp         # Interactive CLI menu and user interface
├── data/
│   ├── stations.txt     # Persistent station records (16 metro stations)
│   └── routes.txt       # Persistent weighted edges (distance, time, status)
├── .gitignore
└── README.md
```

---

## How to Compile and Run

Make sure you have `g++` (C++17 or newer) installed, and run these commands from the project root directory:

### Windows (MinGW / VS Code Terminal)
```bash
g++ -std=c++17 src/main.cpp src/Graph.cpp -o metro_navigator.exe
./metro_navigator.exe
```

### Linux / macOS
```bash
g++ -std=c++17 src/main.cpp src/Graph.cpp -o metro_navigator
./metro_navigator
```

---

## Sample Output

```text
==========================================================
  ROUTE SUMMARY : SHORTEST DISTANCE (Dijkstra)
==========================================================
  (START) [1] Rajiv Chowk (Blue Line)
      >>> [INTERCHANGE ALERT] Switch from Blue Line to Yellow Line (+5 min buffer) <<<
     |    [11] Central Secretariat (Yellow Line)
      >>> [INTERCHANGE ALERT] Switch from Yellow Line to Violet Line (+5 min buffer) <<<
     |    [15] Lajpat Nagar (Violet Line)
  ( END ) [16] Nehru Place (Violet Line)
----------------------------------------------------------
  Total Distance   : 10.50 km
  Estimated Time   : 32 mins
  Total Stops      : 3
  Line Interchanges: 2
  Estimated Fare   : Rs. 30
==========================================================
```

---

## Author

- **Masihullah** — B.Tech Computer Science & Engineering (DSA End-Semester Project)

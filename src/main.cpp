#include "../include/Graph.h"
#include <iostream>
#include <string>
#include <limits>

void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void printBanner() {
    std::cout << "\n==========================================================\n";
    std::cout << "        METRO ROUTE NAVIGATOR  |  DSA ENGINE (C++)\n";
    std::cout << "    Weighted Graph + Custom Min-Heap + Dijkstra + BFS\n";
    std::cout << "==========================================================\n";
}

void printMenu() {
    std::cout << "\n-------------------- MAIN MENU ---------------------------\n";
    std::cout << "  1. View All Metro Stations\n";
    std::cout << "  2. View Complete Network Map (Adjacency List)\n";
    std::cout << "  3. Find Shortest Distance Route  (Dijkstra + Min-Heap)\n";
    std::cout << "  4. Find Fastest Time Route       (Dijkstra + Min-Heap)\n";
    std::cout << "  5. Find Minimum Stops Route      (BFS + Queue)\n";
    std::cout << "  6. Compare All 3 Route Options Side-by-Side\n";
    std::cout << "  7. Admin Mode: Block / Unblock Route (Maintenance)\n";
    std::cout << "  8. Admin Mode: Add New Station or Route\n";
    std::cout << "  0. Exit\n";
    std::cout << "----------------------------------------------------------\n";
    std::cout << "Enter choice: ";
}

bool promptSourceDest(const MetroGraph& metro, int& srcId, int& destId) {
    std::string srcInput, destInput;
    std::cout << "Enter Source Station (ID or Name): ";
    std::getline(std::cin, srcInput);
    srcId = metro.findStationIdByInput(srcInput);

    if (srcId == -1) {
        std::cout << "[!] Source station not found. Check Option 1 for station list.\n";
        return false;
    }

    std::cout << "Enter Destination Station (ID or Name): ";
    std::getline(std::cin, destInput);
    destId = metro.findStationIdByInput(destInput);

    if (destId == -1) {
        std::cout << "[!] Destination station not found. Check Option 1 for station list.\n";
        return false;
    }
    return true;
}

int main() {
    MetroGraph metro("data/stations.txt", "data/routes.txt");

    if (!metro.loadData()) {
        std::cerr << "[Fatal] Failed to load network data. Run the program from the project root folder.\n";
        return 1;
    }

    printBanner();

    int choice = -1;
    while (true) {
        printMenu();
        if (!(std::cin >> choice)) {
            clearInput();
            std::cout << "[!] Invalid input. Please enter a number between 0 and 8.\n";
            continue;
        }
        clearInput();

        if (choice == 0) {
            std::cout << "\nThank you for using Metro Route Navigator. Safe travels!\n";
            break;
        }

        switch (choice) {
            case 1:
                metro.displayAllStations();
                break;

            case 2:
                metro.displayNetworkMap();
                break;

            case 3: {
                int src, dest;
                if (promptSourceDest(metro, src, dest)) {
                    PathResult res = metro.findShortestPathDijkstra(src, dest, false);
                    metro.printPathResult(res, "SHORTEST DISTANCE (Dijkstra)");
                }
                break;
            }

            case 4: {
                int src, dest;
                if (promptSourceDest(metro, src, dest)) {
                    PathResult res = metro.findShortestPathDijkstra(src, dest, true);
                    metro.printPathResult(res, "FASTEST TIME (Dijkstra)");
                }
                break;
            }

            case 5: {
                int src, dest;
                if (promptSourceDest(metro, src, dest)) {
                    PathResult res = metro.findMinimumStopsBFS(src, dest);
                    metro.printPathResult(res, "MINIMUM STOPS (BFS)");
                }
                break;
            }

            case 6: {
                int src, dest;
                if (promptSourceDest(metro, src, dest)) {
                    metro.printPathResult(metro.findShortestPathDijkstra(src, dest, false), "1. SHORTEST DISTANCE (Dijkstra)");
                    metro.printPathResult(metro.findShortestPathDijkstra(src, dest, true),  "2. FASTEST TIME (Dijkstra)");
                    metro.printPathResult(metro.findMinimumStopsBFS(src, dest),             "3. MINIMUM STOPS (BFS)");
                }
                break;
            }

            case 7: {
                metro.displayBlockedRoutes();
                std::cout << "  1. Block a Route (Simulate Maintenance)\n";
                std::cout << "  2. Unblock a Route (Restore Service)\n";
                std::cout << "Select action: ";
                int action;
                if (!(std::cin >> action)) {
                    clearInput();
                    break;
                }
                clearInput();

                int u, v;
                if (promptSourceDest(metro, u, v)) {
                    bool targetState = (action == 2);
                    if (metro.setRouteStatus(u, v, targetState)) {
                        std::cout << "[OK] Route between " << metro.getStation(u).name
                                  << " and " << metro.getStation(v).name
                                  << (targetState ? " is now OPERATIONAL.\n" : " is now BLOCKED.\n");
                    } else {
                        std::cout << "[!] No direct track exists between these two stations.\n";
                    }
                }
                break;
            }

            case 8: {
                std::cout << "  1. Add New Station\n";
                std::cout << "  2. Add / Update Route Between Existing Stations\n";
                std::cout << "Select action: ";
                int action;
                if (!(std::cin >> action)) {
                    clearInput();
                    break;
                }
                clearInput();

                if (action == 1) {
                    std::string name, line;
                    std::cout << "Enter New Station Name: ";
                    std::getline(std::cin, name);
                    std::cout << "Enter Metro Line (e.g., Blue Line, Yellow Line): ";
                    std::getline(std::cin, line);
                    int newId = -1;
                    if (metro.addStation(name, line, newId)) {
                        std::cout << "[OK] Added Station '" << name << "' with ID " << newId << " (saved to file).\n";
                    } else {
                        std::cout << "[!] Could not add station (duplicate or empty name).\n";
                    }
                } else if (action == 2) {
                    int u, v, t;
                    double d;
                    if (promptSourceDest(metro, u, v)) {
                        std::cout << "Enter Distance (km): ";
                        std::cin >> d;
                        std::cout << "Enter Travel Time (minutes): ";
                        std::cin >> t;
                        clearInput();
                        if (metro.addRoute(u, v, d, t)) {
                            std::cout << "[OK] Route saved successfully.\n";
                        } else {
                            std::cout << "[!] Invalid route parameters.\n";
                        }
                    }
                }
                break;
            }

            default:
                std::cout << "[!] Invalid option. Choose between 0 and 8.\n";
        }
    }

    return 0;
}

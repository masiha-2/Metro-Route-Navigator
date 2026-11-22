#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>
#include <unordered_map>

struct Station {
    int id;
    std::string name;
    std::string line;
};

struct Edge {
    int dest;
    double distance; // in kilometers
    int timeMinutes; // in minutes
    bool isActive;   // false when blocked for maintenance
};

struct PathResult {
    bool found;
    std::vector<int> stationIds;
    double totalDistance;
    int totalTime;
    int totalStops;
    int interchanges;
    int fareINR;
};

class MetroGraph {
private:
    std::unordered_map<int, Station> stations;
    std::unordered_map<std::string, int> nameToId; // lowercase name -> station ID
    std::unordered_map<int, std::vector<Edge>> adjList;

    std::string stationsFile;
    std::string routesFile;

    static std::string toLower(const std::string& str);
    static std::string trim(const std::string& str);
    int calculateFare(double distanceKm) const;
    void computeRouteStats(PathResult& result) const;

public:
    MetroGraph(const std::string& sFile, const std::string& rFile);

    bool loadData();
    bool saveStations() const;
    bool saveRoutes() const;

    // Station & Route Queries
    void displayAllStations() const;
    void displayNetworkMap() const;
    int findStationIdByInput(const std::string& input) const;
    bool stationExists(int id) const;
    Station getStation(int id) const;

    // Core DSA Pathfinding Algorithms
    PathResult findShortestPathDijkstra(int srcId, int destId, bool optimizeByTime = false) const;
    PathResult findMinimumStopsBFS(int srcId, int destId) const;

    // Dynamic Network Administration (Maintenance & Expansion)
    bool setRouteStatus(int u, int v, bool active);
    bool addStation(const std::string& name, const std::string& line, int& outNewId);
    bool addRoute(int u, int v, double distance, int timeMinutes);
    void displayBlockedRoutes() const;

    // Pretty Route Printer
    void printPathResult(const PathResult& result, const std::string& modeTitle) const;
};

#endif // GRAPH_H

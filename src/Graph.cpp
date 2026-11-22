#include "../include/Graph.h"
#include "../include/MinHeap.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <queue>
#include <stack>
#include <cctype>
#include <set>

MetroGraph::MetroGraph(const std::string& sFile, const std::string& rFile)
    : stationsFile(sFile), routesFile(rFile) {}

std::string MetroGraph::toLower(const std::string& str) {
    std::string res = str;
    for (char& c : res) {
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }
    return res;
}

std::string MetroGraph::trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, last - first + 1);
}

int MetroGraph::calculateFare(double distanceKm) const {
    if (distanceKm <= 2.0) return 10;
    if (distanceKm <= 5.0) return 20;
    if (distanceKm <= 12.0) return 30;
    if (distanceKm <= 21.0) return 40;
    if (distanceKm <= 32.0) return 50;
    return 60;
}

bool MetroGraph::loadData() {
    stations.clear();
    nameToId.clear();
    adjList.clear();

    std::ifstream sIn(stationsFile);
    if (!sIn.is_open()) {
        std::cerr << "[Error] Could not open stations file: " << stationsFile << "\n";
        return false;
    }

    std::string line;
    while (std::getline(sIn, line)) {
        line = trim(line);
        if (line.empty() || line[0] == '#') continue;

        std::stringstream ss(line);
        std::string idStr, name, metroLine;
        if (std::getline(ss, idStr, ',') &&
            std::getline(ss, name, ',') &&
            std::getline(ss, metroLine, ',')) {
            int id = std::stoi(trim(idStr));
            name = trim(name);
            metroLine = trim(metroLine);
            stations[id] = {id, name, metroLine};
            nameToId[toLower(name)] = id;
            adjList[id] = {};
        }
    }
    sIn.close();

    std::ifstream rIn(routesFile);
    if (!rIn.is_open()) {
        std::cerr << "[Error] Could not open routes file: " << routesFile << "\n";
        return false;
    }

    while (std::getline(rIn, line)) {
        line = trim(line);
        if (line.empty() || line[0] == '#') continue;

        std::stringstream ss(line);
        std::string uStr, vStr, distStr, timeStr, activeStr;
        if (std::getline(ss, uStr, ',') &&
            std::getline(ss, vStr, ',') &&
            std::getline(ss, distStr, ',') &&
            std::getline(ss, timeStr, ',')) {
            int u = std::stoi(trim(uStr));
            int v = std::stoi(trim(vStr));
            double dist = std::stod(trim(distStr));
            int t = std::stoi(trim(timeStr));
            bool active = true;
            if (std::getline(ss, activeStr, ',')) {
                active = (std::stoi(trim(activeStr)) != 0);
            }
            if (stations.count(u) && stations.count(v)) {
                adjList[u].push_back({v, dist, t, active});
                adjList[v].push_back({u, dist, t, active});
            }
        }
    }
    rIn.close();
    return true;
}

bool MetroGraph::saveStations() const {
    std::ofstream out(stationsFile);
    if (!out.is_open()) return false;

    out << "# StationID,StationName,Line\n";
    std::vector<int> ids;
    for (const auto& kv : stations) ids.push_back(kv.first);
    std::sort(ids.begin(), ids.end());

    for (int id : ids) {
        const Station& s = stations.at(id);
        out << s.id << "," << s.name << "," << s.line << "\n";
    }
    out.close();
    return true;
}

bool MetroGraph::saveRoutes() const {
    std::ofstream out(routesFile);
    if (!out.is_open()) return false;

    out << "# SourceID,DestID,DistanceKm,TimeMin,IsActive\n";
    std::set<std::pair<int, int>> written;

    std::vector<int> ids;
    for (const auto& kv : adjList) ids.push_back(kv.first);
    std::sort(ids.begin(), ids.end());

    for (int u : ids) {
        for (const Edge& e : adjList.at(u)) {
            int a = std::min(u, e.dest);
            int b = std::max(u, e.dest);
            if (!written.count({a, b})) {
                written.insert({a, b});
                out << a << "," << b << "," << e.distance << "," << e.timeMinutes << ","
                    << (e.isActive ? 1 : 0) << "\n";
            }
        }
    }
    out.close();
    return true;
}

void MetroGraph::displayAllStations() const {
    std::vector<int> ids;
    for (const auto& kv : stations) ids.push_back(kv.first);
    std::sort(ids.begin(), ids.end());

    std::cout << "\n==========================================================\n";
    std::cout << "  " << std::left << std::setw(6) << "ID"
              << std::setw(28) << "Station Name"
              << "Metro Line\n";
    std::cout << "==========================================================\n";
    for (int id : ids) {
        const Station& s = stations.at(id);
        std::cout << "  " << std::left << std::setw(6) << s.id
                  << std::setw(28) << s.name
                  << s.line << "\n";
    }
    std::cout << "==========================================================\n";
}

void MetroGraph::displayNetworkMap() const {
    std::vector<int> ids;
    for (const auto& kv : stations) ids.push_back(kv.first);
    std::sort(ids.begin(), ids.end());

    std::cout << "\n================== METRO ADJACENCY MAP ===================\n";
    for (int id : ids) {
        const Station& s = stations.at(id);
        std::cout << "[" << s.id << "] " << s.name << " (" << s.line << ")\n";
        for (const Edge& e : adjList.at(id)) {
            const Station& nb = stations.at(e.dest);
            std::cout << "    --> [" << nb.id << "] " << std::left << std::setw(22) << nb.name
                      << " | " << std::fixed << std::setprecision(1) << e.distance << " km"
                      << " | " << e.timeMinutes << " min"
                      << (e.isActive ? "" : "  [BLOCKED]") << "\n";
        }
    }
    std::cout << "==========================================================\n";
}

int MetroGraph::findStationIdByInput(const std::string& input) const {
    std::string cleaned = trim(input);
    if (cleaned.empty()) return -1;

    // Check if input is numeric ID
    bool allDigits = true;
    for (char c : cleaned) {
        if (!std::isdigit(static_cast<unsigned char>(c))) {
            allDigits = false;
            break;
        }
    }
    if (allDigits) {
        int id = std::stoi(cleaned);
        return stations.count(id) ? id : -1;
    }

    // Exact case-insensitive match
    std::string lower = toLower(cleaned);
    auto it = nameToId.find(lower);
    if (it != nameToId.end()) return it->second;

    // Partial prefix/substring match for user convenience
    for (const auto& kv : nameToId) {
        if (kv.first.find(lower) != std::string::npos) {
            return kv.second;
        }
    }
    return -1;
}

bool MetroGraph::stationExists(int id) const {
    return stations.count(id) > 0;
}

Station MetroGraph::getStation(int id) const {
    return stations.at(id);
}

void MetroGraph::computeRouteStats(PathResult& result) const {
    result.totalDistance = 0.0;
    result.totalTime = 0;
    result.totalStops = static_cast<int>(result.stationIds.size()) - 1;
    result.interchanges = 0;

    if (result.stationIds.empty()) return;

    std::string activeLine = stations.at(result.stationIds[0]).line;
    for (size_t i = 0; i + 1 < result.stationIds.size(); ++i) {
        int u = result.stationIds[i];
        int v = result.stationIds[i + 1];

        for (const Edge& e : adjList.at(u)) {
            if (e.dest == v && e.isActive) {
                result.totalDistance += e.distance;
                result.totalTime += e.timeMinutes;
                break;
            }
        }

        const std::string& nextLine = stations.at(v).line;
        if (nextLine != activeLine) {
            result.interchanges++;
            result.totalTime += 5; // 5-minute interchange walking buffer
            activeLine = nextLine;
        }
    }

    result.fareINR = calculateFare(result.totalDistance);
}

PathResult MetroGraph::findShortestPathDijkstra(int srcId, int destId, bool optimizeByTime) const {
    PathResult result{false, {}, 0.0, 0, 0, 0, 0};
    if (!stations.count(srcId) || !stations.count(destId)) return result;

    if (srcId == destId) {
        result.found = true;
        result.stationIds = {srcId};
        result.fareINR = 10;
        return result;
    }

    const double INF = std::numeric_limits<double>::infinity();
    std::unordered_map<int, double> dist;
    std::unordered_map<int, int> parent;
    std::unordered_map<int, bool> visited;

    for (const auto& kv : stations) {
        dist[kv.first] = INF;
        parent[kv.first] = -1;
        visited[kv.first] = false;
    }

    // Custom MinHeap stores pair<cost, stationId>
    MinHeap<std::pair<double, int>> pq;
    dist[srcId] = 0.0;
    pq.push({0.0, srcId});

    while (!pq.empty()) {
        auto current = pq.top();
        pq.pop();

        int u = current.second;
        if (visited[u]) continue;
        visited[u] = true;

        if (u == destId) break;

        for (const Edge& e : adjList.at(u)) {
            if (!e.isActive) continue; // Skip blocked maintenance routes

            double weight = optimizeByTime ? static_cast<double>(e.timeMinutes) : e.distance;
            if (!visited[e.dest] && dist[u] + weight < dist[e.dest]) {
                dist[e.dest] = dist[u] + weight;
                parent[e.dest] = u;
                pq.push({dist[e.dest], e.dest});
            }
        }
    }

    if (dist[destId] == INF) return result;

    // Reconstruct path using Stack
    std::stack<int> pathStack;
    for (int curr = destId; curr != -1; curr = parent[curr]) {
        pathStack.push(curr);
    }
    while (!pathStack.empty()) {
        result.stationIds.push_back(pathStack.top());
        pathStack.pop();
    }

    result.found = true;
    computeRouteStats(result);
    return result;
}

PathResult MetroGraph::findMinimumStopsBFS(int srcId, int destId) const {
    PathResult result{false, {}, 0.0, 0, 0, 0, 0};
    if (!stations.count(srcId) || !stations.count(destId)) return result;

    if (srcId == destId) {
        result.found = true;
        result.stationIds = {srcId};
        result.fareINR = 10;
        return result;
    }

    std::unordered_map<int, bool> visited;
    std::unordered_map<int, int> parent;
    std::queue<int> q;

    q.push(srcId);
    visited[srcId] = true;
    parent[srcId] = -1;

    bool reached = false;
    while (!q.empty()) {
        int u = q.front();
        q.pop();

        if (u == destId) {
            reached = true;
            break;
        }

        for (const Edge& e : adjList.at(u)) {
            if (!e.isActive) continue;
            if (!visited[e.dest]) {
                visited[e.dest] = true;
                parent[e.dest] = u;
                q.push(e.dest);
            }
        }
    }

    if (!reached) return result;

    std::stack<int> pathStack;
    for (int curr = destId; curr != -1; curr = parent[curr]) {
        pathStack.push(curr);
    }
    while (!pathStack.empty()) {
        result.stationIds.push_back(pathStack.top());
        pathStack.pop();
    }

    result.found = true;
    computeRouteStats(result);
    return result;
}

bool MetroGraph::setRouteStatus(int u, int v, bool active) {
    if (!adjList.count(u) || !adjList.count(v)) return false;

    bool found = false;
    for (Edge& e : adjList[u]) {
        if (e.dest == v) {
            e.isActive = active;
            found = true;
        }
    }
    for (Edge& e : adjList[v]) {
        if (e.dest == u) {
            e.isActive = active;
        }
    }
    if (found) saveRoutes();
    return found;
}

bool MetroGraph::addStation(const std::string& name, const std::string& line, int& outNewId) {
    std::string cleanName = trim(name);
    std::string cleanLine = trim(line);
    if (cleanName.empty() || cleanLine.empty()) return false;
    if (nameToId.count(toLower(cleanName))) return false;

    int maxId = 0;
    for (const auto& kv : stations) {
        if (kv.first > maxId) maxId = kv.first;
    }
    outNewId = maxId + 1;

    stations[outNewId] = {outNewId, cleanName, cleanLine};
    nameToId[toLower(cleanName)] = outNewId;
    adjList[outNewId] = {};

    saveStations();
    return true;
}

bool MetroGraph::addRoute(int u, int v, double distance, int timeMinutes) {
    if (!stations.count(u) || !stations.count(v) || u == v || distance <= 0 || timeMinutes <= 0) {
        return false;
    }

    // Update if route already exists
    for (Edge& e : adjList[u]) {
        if (e.dest == v) {
            e.distance = distance;
            e.timeMinutes = timeMinutes;
            e.isActive = true;
            for (Edge& rev : adjList[v]) {
                if (rev.dest == u) {
                    rev.distance = distance;
                    rev.timeMinutes = timeMinutes;
                    rev.isActive = true;
                }
            }
            saveRoutes();
            return true;
        }
    }

    adjList[u].push_back({v, distance, timeMinutes, true});
    adjList[v].push_back({u, distance, timeMinutes, true});
    saveRoutes();
    return true;
}

void MetroGraph::displayBlockedRoutes() const {
    std::set<std::pair<int, int>> seen;
    bool anyBlocked = false;

    std::cout << "\n--- Currently Blocked Routes (Under Maintenance) ---\n";
    for (const auto& kv : adjList) {
        int u = kv.first;
        for (const Edge& e : kv.second) {
            if (!e.isActive) {
                int a = std::min(u, e.dest);
                int b = std::max(u, e.dest);
                if (!seen.count({a, b})) {
                    seen.insert({a, b});
                    std::cout << "  [X] " << stations.at(a).name << " (ID " << a << ") <---> "
                              << stations.at(b).name << " (ID " << b << ")\n";
                    anyBlocked = true;
                }
            }
        }
    }
    if (!anyBlocked) {
        std::cout << "  All metro routes are currently operational.\n";
    }
    std::cout << "----------------------------------------------------\n";
}

void MetroGraph::printPathResult(const PathResult& result, const std::string& modeTitle) const {
    std::cout << "\n==========================================================\n";
    std::cout << "  ROUTE SUMMARY : " << modeTitle << "\n";
    std::cout << "==========================================================\n";

    if (!result.found) {
        std::cout << "  [!] No active route available between these stations.\n";
        std::cout << "      (Check if connecting tracks are blocked in Admin Mode)\n";
        std::cout << "==========================================================\n";
        return;
    }

    std::string activeLine = stations.at(result.stationIds[0]).line;

    for (size_t i = 0; i < result.stationIds.size(); ++i) {
        int currId = result.stationIds[i];
        const Station& s = stations.at(currId);

        if (i > 0 && s.line != activeLine) {
            std::cout << "      >>> [INTERCHANGE ALERT] Switch from " << activeLine
                      << " to " << s.line << " (+5 min buffer) <<<\n";
            activeLine = s.line;
        }

        if (i == 0) {
            std::cout << "  (START) ";
        } else if (i == result.stationIds.size() - 1) {
            std::cout << "  ( END ) ";
        } else {
            std::cout << "     |    ";
        }

        std::cout << "[" << s.id << "] " << s.name << " (" << s.line << ")\n";
    }

    std::cout << "----------------------------------------------------------\n";
    std::cout << "  Total Distance   : " << std::fixed << std::setprecision(2) << result.totalDistance << " km\n";
    std::cout << "  Estimated Time   : " << result.totalTime << " mins\n";
    std::cout << "  Total Stops      : " << result.totalStops << "\n";
    std::cout << "  Line Interchanges: " << result.interchanges << "\n";
    std::cout << "  Estimated Fare   : Rs. " << result.fareINR << "\n";
    std::cout << "==========================================================\n";
}

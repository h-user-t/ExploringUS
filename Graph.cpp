//
// Created by hunte on 11/13/2023.
//

#include "Graph.h"
#include "PriorityQueue.h"
#include "Utility.h"
#include <cmath>
#include <sstream>
#include <fstream>

using namespace std;

Graph::~Graph() {
    for (auto intersection : intersections)
        delete intersection;
}

void Graph::addIntersection(int id, double longitude, double latitude, double distanceFromPlace, string state, string city) {
    auto* newIntersection = new Intersection(id, longitude, latitude, distanceFromPlace, state, city);
    intersections.push_back(newIntersection);
}

void Graph::addEdge(string roadName, string roadType, double length, Intersection* start, Intersection* end) {
    string direction = dirfromto(start->longitude, start->latitude, end->longitude, end->latitude);
    Edge* newEdge = new Edge(roadName, roadType, length, start, end, direction);
//    cout << "Adding edge: " << roadName << " between " << start->id << " and " << end->id << ", length: " << length << endl;

    start->edges.push_back(newEdge);
    end->edges.push_back(newEdge);
}


void Graph::loadIntersections(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Unable to open file: " << filename << endl;
        return;
    }

    string line;
    double longitude, latitude, distance;
    string state, city;
    int lineCount = 0;

    while (getline(file, line)) {
        istringstream iss(line);
        if (!(iss >> longitude >> latitude >> distance >> state)) {
            cerr << "Error reading file at line " << lineCount + 1 << endl;
            break;
        }

        // Get the rest of the line for city
        getline(iss, city);
        city.erase(0, city.find_first_not_of(" \n\r\t"));
        city.erase(city.find_last_not_of(" \n\r\t") + 1);

        addIntersection(lineCount++, longitude, latitude, distance, state, city);
    }

    cout << "Total intersections loaded: " << intersections.size() << endl;
    file.close();
}
void Graph::loadConnections(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Unable to open file: " << filename << endl;
        return;
    }

    string roadName, roadType;
    int startId, endId;
    double length;

    while (file >> roadName >> roadType >> startId >> endId >> length) {
        // Debugging output
      //  cout << "Read connection: " << roadName << " (" << roadType << ") from " << startId << " to " << endId << ", length: " << length << endl;

        if (isValidIntersection(startId) && isValidIntersection(endId)) {
            Intersection* startIntersection = intersections[startId];
            Intersection* endIntersection = intersections[endId];
            addEdge(roadName, roadType, length, startIntersection, endIntersection);
        } else {
            cerr << "Invalid intersection IDs: " << startId << ", " << endId << endl;
        }
    }

    file.close();
}

Intersection* Graph::findIntersectionById(int id) {
    if (id >= 0 && id < intersections.size()) {
        return intersections[id];
    }
    return nullptr; // Return null if not found or out-of-bounds
}

vector<int> Graph::dijkstra(int sourceId, int destinationId) {
    PriorityQueue pq;
    vector<double> distances(intersections.size(), numeric_limits<double>::max());
    vector<int> predecessors(intersections.size(), -1);
    vector<bool> visited(intersections.size(), false);

    pq.push(sourceId, 0);
    distances[sourceId] = 0;

    while (!pq.isEmpty()) {
        Node currentNode = pq.top();
        pq.pop();

        int currentId = currentNode.id;
        if (visited[currentId]) continue;
        visited[currentId] = true;

        if (currentId == destinationId) {
            break; // Destination reached
        }

        for (Edge* edge : intersections[currentId]->edges) {
            Intersection* neighbor = (edge->start->id == currentId) ? edge->end : edge->start;
            double newDist = distances[currentId] + edge->length;

            if (newDist < distances[neighbor->id]) {
                distances[neighbor->id] = newDist;
                predecessors[neighbor->id] = currentId;
                pq.push(neighbor->id, newDist);
            }
        }
    }

    // Reconstruct the shortest path
    vector<int> path;
    for (int at = destinationId; at != -1; at = predecessors[at]) {
        path.push_back(at);
    }

          int n = path.size();
    for (int i = 0; i < n / 2; ++i) {
        swap(path[i], path[n - 1 - i]);
    }

   
    return path;
}

Edge* Graph::findConnectingEdge(int fromId, int toId) {
    Intersection* fromIntersection = findIntersectionById(fromId);
    Intersection* toIntersection = findIntersectionById(toId);

    if (!fromIntersection || !toIntersection) {
        cerr << "One or both intersections not found." << endl;
        return nullptr;
    }

    for (Edge* edge : fromIntersection->edges) {
        if ((edge->start == fromIntersection && edge->end == toIntersection) ||
            (edge->start == toIntersection && edge->end == fromIntersection)) {
            return edge;
        }
    }

    // If no edge is found, check the edges of the toIntersection
    for (Edge* edge : toIntersection->edges) {
        if ((edge->start == fromIntersection && edge->end == toIntersection) ||
            (edge->start == toIntersection && edge->end == fromIntersection)) {
            return edge;
        }
    }

    return nullptr; // Edge not found
}

bool Graph::isValidIntersection(int id) {
    return id >= 0 && id < static_cast<int>(intersections.size());
}

string* Graph::getNearestPlace(int id) {
    if (id >= 0 && id < intersections.size()) {
        return intersections[id]->nearestPlace;
    }
    return nullptr; // Return null if not found or out-of-bounds
}

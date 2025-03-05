#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct Edge;
struct Intersection;

struct Intersection {
    int id;
    double longitude;
    double latitude;
    double distanceFromPlace;
    string nearestPlace[2];
    vector<Edge*> edges;

    Intersection(int id, double longitude, double latitude, double distanceFromPlace, string state, string city)
            : id(id), longitude(longitude), latitude(latitude), distanceFromPlace(distanceFromPlace) {
        nearestPlace[0] = state;
        nearestPlace[1] = city;
    }
};

struct Edge {
    string roadName;
    string roadType;
    double length;
    Intersection* start;
    Intersection* end;
    string direction;

    Edge(string roadName, string roadType, double length, Intersection* start, Intersection* end, string direction)
            : roadName(roadName), roadType(roadType), length(length), start(start), end(end), direction(direction) {
    }
};

class Graph {
    vector<Intersection*> intersections;

public:
    ~Graph();
    void addIntersection(int id, double longitude, double latitude, double distanceFromPlace, string state, string city);
    void addEdge(string roadName, string roadType, double length, Intersection* start, Intersection* end);
    void loadIntersections(const string& filename);
    void loadConnections(const string& filename);
    Intersection* findIntersectionById(int id);
    Edge* findConnectingEdge(int fromId, int toId);
    vector<int> dijkstra(int sourceId, int destinationId);
    bool isValidIntersection(int id);
    string* getNearestPlace(int id);
};

#endif // GRAPH_H

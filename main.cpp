#include <iostream>
#include <vector>
#include <string>
#include "HashTable.h"
#include "Graph.h"
#include "Utility.h"

using namespace std;

int main() {
    Graph map;
    HashTable places(10);

     places.loadFromFile("/home/www/class/een318/named-places.txt");
    places.loadStateDataFromFile("/home/www/class/een318/states.txt");
    cout << "Loading intersections...\n";
     map.loadIntersections("/home/www/class/een318/intersections.txt");
    cout << "Loading connections...\n";
    map.loadConnections("/home/www/class/een318/connections.txt");
    cout << "Finished...\n";

    string placeName, stateAbbreviation, placeName2, stateAbbreviation2;
    Intersection *startIntersection = nullptr, *endIntersection = nullptr;
    int startId = -1, endId = -1;

    // Get starting place
    while (startIntersection == nullptr) {
        cout << "Starting place: ";
        getline(cin, placeName);
        if (!places.placeExists(placeName)) {
            cout << "Invalid starting place. Please try again." << endl;
            continue;
        }
        while (startIntersection == nullptr) {
            places.listStatesAbbreviation(placeName);
            cout << "State for starting place: ";
            getline(cin, stateAbbreviation);

            places.getPlaceInfo(placeName, stateAbbreviation);
            startId = places.getRoadIntersection(placeName, stateAbbreviation);
            startIntersection = map.findIntersectionById(startId);

            if (startIntersection == nullptr) {
                cout << "Invalid state for starting place. Please try again." << endl;
                continue;
            }
        }
    }

    // Get destination
    while (endIntersection == nullptr) {
        cout << "Destination: ";
        getline(cin, placeName2);
        if (!places.placeExists(placeName2)) {
            cout << "Invalid destination. Please try again." << endl;
            continue;
        }
        while (endIntersection == nullptr) {
            places.listStatesAbbreviation(placeName2);
            cout << "State for destination: ";
            getline(cin, stateAbbreviation2);

            places.getPlaceInfo(placeName2, stateAbbreviation2);
            endId = places.getRoadIntersection(placeName2, stateAbbreviation2);
            endIntersection = map.findIntersectionById(endId);

            if (endIntersection == nullptr) {
                cout << "Invalid state for destination. Please try again." << endl;
                continue;
            }
        }
    }

        cout<<endl<<endl;
  // Running Dijkstra's algorithm
vector<int> path = map.dijkstra(startId, endId);
if (!path.empty() && path.front() == startId) {
    string currentRoad = "";
    double totalDistance = 0.0, roadDistance = 0.0;
    bool isFirstRoad = true;

    // Print starting intersection details
    Intersection* startIntersection = map.findIntersectionById(startId);
    Intersection* endIntersection = map.findIntersectionById(endId);
    NamedPlace* toPlace = places.getPlaceInfo(startIntersection->nearestPlace[1], startIntersection->nearestPlace[0]);
    cout << "Starting point is intersection #" << startId << ", "
         << startIntersection->distanceFromPlace << " miles " 
         << dirfromto(toPlace->longitude, toPlace->latitude, startIntersection->longitude, startIntersection->latitude) 
         << " of " << startIntersection->nearestPlace[1] << ", " << startIntersection->nearestPlace[0] << endl;

    for (size_t i = 0; i < path.size() - 1; ++i) {
        int fromId = path[i];
        int nextId = path[i + 1];  // Update nextId for each iteration

        Intersection* fromIntersection = map.findIntersectionById(fromId);
        Intersection* toIntersection = map.findIntersectionById(nextId);

        NamedPlace* fromPlace = places.getPlaceInfo(fromIntersection->nearestPlace[1], fromIntersection->nearestPlace[0]);
        toPlace = places.getPlaceInfo(toIntersection->nearestPlace[1], toIntersection->nearestPlace[0]);

        if (!toPlace) {
            cerr << "Missing place info for intersection " << nextId << endl;
            continue;
        }

        Edge* edge = map.findConnectingEdge(fromId, nextId);
        if (!edge) {
            cerr << "Missing connection between intersections " << fromId << " and " << nextId << endl;
            continue;
        }

        string roadName = (edge->roadName == "?") ? "unnamed road" : edge->roadName;

        if (roadName != currentRoad) {
            if (!isFirstRoad) {
                cout << "Then " << currentRoad << " "
                     << dirfromto(fromIntersection->longitude, fromIntersection->latitude, toIntersection->longitude, toIntersection->latitude)
                     << " " << roadDistance << " miles to intersection #" << fromId << ", "
                     << " miles " << dirfromto(toPlace->longitude, toPlace->latitude, fromIntersection->longitude, fromIntersection->latitude)
 << " of " << fromIntersection->nearestPlace[1] << ", " << fromIntersection->nearestPlace[0] << endl;
            } else {
                cout << "Take " << roadName << " ";
            }
            currentRoad = roadName;
            roadDistance = edge->length;
            isFirstRoad = false;
        } else {
            roadDistance += edge->length;
        }

        totalDistance += edge->length;
    }

    // Print the last road segment
    if (!currentRoad.empty()) {
        cout << "Then " << currentRoad << " "
             << dirfromto(endIntersection->longitude, endIntersection->latitude, toPlace->longitude, toPlace->latitude)
             << " " << roadDistance << " miles to intersection #" << endId << ", "
             << " miles " << dirfromto(toPlace->longitude, toPlace->latitude, endIntersection->longitude, endIntersection->latitude) << " of " << endIntersection->nearestPlace[1] << ", " << endIntersection->nearestPlace[0] << endl;
    }

    cout << "Total " << totalDistance << " miles" << endl;
} else {
    cout << "No valid path found." << endl;
}

return 0;}
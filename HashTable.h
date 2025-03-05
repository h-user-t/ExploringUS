#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <string>
#include <map>

using namespace std;

    struct NamedPlace { // Pure data
        int numericCode;
        string stateAbbreviation;
        string name;
        int population;
        double area;
        double latitude;
        double longitude;
        int roadIntersectionCode;
        double distanceToIntersection;
    };


class HashTable {
public:
    HashTable(int capacity = 10);
    ~HashTable();
    void insert(int numericCode,
                const string& stateAbbreviation,
                const string& name,
                int population,
                double area,
                double latitude,
                double longitude,
                int roadIntersectionCode,
                double distanceToIntersection);
    void printTable();
    static unsigned long enhancedHashFunction(const string& key);
    void listStatesWithName(const string& placeName);
    NamedPlace* getPlaceInfo(const string& placeName, const string& stateAbbreviation);
    void loadFromFile(const string& filePath);
    void loadStateDataFromFile(const string& filePath);
    void listStatesAbbreviation(const string &placeName);
    int getRoadIntersection(const string &placeName, const string &stateAbbreviation);
    bool placeExists(const string &placeName);
protected:

    struct Bucket { // Structure that includes the linkage
        NamedPlace* place;
        Bucket* next;
    };
    map<string, string> stateData;

    Bucket** table;
    int capacity;
    int size;
    void resize();



};

string trim(const string &str);  // Moved trim outside of class

#endif // HASHTABLE_H

#include "HashTable.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

// trim function as a free function, as suggested in point (3)
string trim(const string& str) {
    size_t first = str.find_first_not_of(' ');
    if (string::npos == first) {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

HashTable::HashTable(int capacity) : capacity(max(1, capacity)), size(0) {
    // Guard against zero initial capacity, as noted in point (4)
    table = new Bucket*[this->capacity];
    for (int i = 0; i < this->capacity; ++i) {
        table[i] = nullptr;
    }
}

HashTable::~HashTable() {
    // Destructor now operates on Buckets, not NamedPlaces
    for (int i = 0; i < capacity; ++i) {
        Bucket* current = table[i];
        while (current != nullptr) {
            Bucket* oldBucket = current;
            current = current->next;
            delete oldBucket->place;
	    delete oldBucket;
        }
    }
    delete[] table;
}

// enhancedHashFunction with multiplication, as suggested in point (5)
unsigned long HashTable::enhancedHashFunction(const string& key) {
    unsigned long hash = 0;
    for (char c : key) {
        hash = (hash * 31) + c;  // Multiplication instead of shifting
    }
    return hash;
}

void HashTable::insert(
        int numericCode, const string& stateAbbreviation, const string& name,
        int population, double area, double latitude, double longitude,
        int roadIntersectionCode, double distanceToIntersection) {
    unsigned long hash = enhancedHashFunction(name) % capacity;
    NamedPlace* newPlace = new NamedPlace{
            numericCode, stateAbbreviation, name, population, area, latitude,
             longitude, roadIntersectionCode, distanceToIntersection};
    Bucket* newBucket = new Bucket{newPlace, nullptr};
    
    newBucket->next = table[hash];
    table[hash] = newBucket;
    
    ++size;

    if (static_cast<double>(size) / capacity > 0.7) {
        resize();
    }
}

void HashTable::resize() {
    int oldCapacity = capacity;
    capacity *= 2;
    Bucket** newTable = new Bucket*[capacity];
    for (int i = 0; i < capacity; ++i) {
        newTable[i] = nullptr;
    }

    for (int i = 0; i < oldCapacity; ++i) {
        Bucket* oldBucket = table[i];
        while (oldBucket != nullptr) {
            Bucket* nextOldBucket = oldBucket->next;
            unsigned long hash = enhancedHashFunction(oldBucket->place->name) % capacity;
            oldBucket->next = newTable[hash];
            newTable[hash] = oldBucket;
            oldBucket = nextOldBucket;
        }
    }

    delete[] table;
    table = newTable;
}

void HashTable::printTable() {
    // Modify printTable to print Buckets, not NamedPlaces
    for (int i = 0; i < capacity; ++i) {
        cout << "[" << i << "]: ";
        Bucket* current = table[i];
        while (current != nullptr) {
            cout << current->place->name << " -> ";
            current = current->next;
        }
        cout << "nullptr" << endl;
    }
}

// Updated listStatesWithName to print the Abbreviation + Full state name
void HashTable::listStatesWithName(const string& placeName) {
    unsigned long hash = enhancedHashFunction(placeName) % capacity;
    Bucket* entry = table[hash];
    while (entry != nullptr) {
        if (entry->place->name == placeName) {
            cout << entry->place->stateAbbreviation << " " << stateData[entry->place->stateAbbreviation] << endl;
        }
        entry = entry->next;
    }
}

// Only prints state abbreviation
void HashTable::listStatesAbbreviation(const string& placeName) {
    unsigned long hash = enhancedHashFunction(placeName) % capacity;
    Bucket* entry = table[hash];

    // First pass: Count the number of state abbreviations
    int count = 0;
    Bucket* current = entry;
    while (current != nullptr) {
        if (current->place->name == placeName) {
            count++;
        }
        current = current->next;
    }

    // Print the total count
    cout << count << " possibilities: ";

    // Second pass: Print the state abbreviations
    while (entry != nullptr) {
        if (entry->place->name == placeName) {
            cout << entry->place->stateAbbreviation << " ";
        }
        entry = entry->next;
    }
    cout<<endl;
}


// Updated getPlaceInfo
NamedPlace* HashTable::getPlaceInfo(const string& placeName, const string& stateAbbreviation) {
    unsigned long hash = enhancedHashFunction(placeName) % capacity;
    Bucket* entry = table[hash];

    while (entry != nullptr) {
        NamedPlace* place = entry->place;
        if (place->name == placeName && place->stateAbbreviation == stateAbbreviation) {
            return place; // Return the NamedPlace object
        }
        entry = entry->next;
    }
    return nullptr; // Return nullptr if no match is found
}

int HashTable::getRoadIntersection(const string& placeName, const string& stateAbbreviation) {
    unsigned long hash = enhancedHashFunction(placeName) % capacity;
    Bucket* entry = table[hash];

    while (entry != nullptr) {
        NamedPlace* place = entry->place;
        if (place->name == placeName && place->stateAbbreviation == stateAbbreviation) {
            return place->roadIntersectionCode; // Return the road intersection code
        }
        entry = entry->next;
    }

    return -1; // Return a default value (e.g., -1) if no match is found
}



// Updated loadFromFile
void HashTable::loadFromFile(const string& filePath) {
    ifstream infile(filePath);
    if (!infile.is_open()) {
        cerr << "Could not open file: " << filePath << endl;
        return;
    }

    string line;
    while (getline(infile, line)) {
        string numericCodeStr = trim(line.substr(0, 8));
        string stateAbbreviation = trim(line.substr(8, 2));
        string placeName = trim(line.substr(10, 47));
        string populationStr = trim(line.substr(58, 13));
        string areaStr = trim(line.substr(70, 11));
        string latitudeStr = trim(line.substr(81, 10));
        string longitudeStr = trim(line.substr(90, 11));
        string roadIntersectionCodeStr = trim(line.substr(101, 5));
        string distanceToIntersectionStr = trim(line.substr(106, 8));

        int numericCode = strtol(numericCodeStr.c_str(), nullptr, 10);
        int population = strtol(populationStr.c_str(), nullptr, 10);
        double area = strtod(areaStr.c_str(), nullptr);
        double latitude = strtod(latitudeStr.c_str(), nullptr);
        double longitude = strtod(longitudeStr.c_str(), nullptr);
        int roadIntersectionCode = strtol(roadIntersectionCodeStr.c_str(), nullptr, 10);
        double distanceToIntersection = strtod(distanceToIntersectionStr.c_str(), nullptr);

        // Insert parsed data into the hash table
        insert(numericCode, stateAbbreviation, placeName, population, area,
               latitude, longitude, roadIntersectionCode, distanceToIntersection);
    }
    infile.close();
}
// Added to load the state name from the provided file.
void HashTable::loadStateDataFromFile(const string& filePath) {
    ifstream infile(filePath);
    if (!infile.is_open()) {
        cerr << "Could not open state data file: " << filePath << endl;
        return;
    }

    string line;
    while (getline(infile, line)) {
        stringstream ss(line);
        string abbrev, name;
        ss >> abbrev >> ws;
        getline(ss, name);  // Get the rest of the line into 'name'
        stateData[abbrev] = name;
    }
    infile.close();
}

bool HashTable::placeExists(const string& placeName){

    unsigned long hash = enhancedHashFunction(placeName) % capacity;
    Bucket* entry = table[hash];

    while (entry != nullptr) {
        NamedPlace* place = entry->place;
        if (place->name == placeName) {
            return true; // Return the road intersection code
        }
        entry = entry->next;
    }

    return false; // Return a default value (e.g., -1) if no match is found
}

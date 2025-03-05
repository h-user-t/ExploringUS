#ifndef UTILITY_H
#define UTILITY_H

#include <string>
#include <cmath>

using namespace std;

extern const string dirs[];

string dirfromto(double longitude1, double latitude1, double longitude2, double latitude2);
bool isQuitCommand(const std::string& input);

#endif // UTILITY_H

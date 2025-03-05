//
// Created by hunte on 11/13/2023.
//

#include "Utility.h"

const double pi = acos(-1.0);
const string dirs[] = { "N", "NE", "E", "SE", "S", "SW", "W", "NW", "N" };

string dirfromto(double longitude1, double latitude1,
                 double longitude2, double latitude2)
{ double d = atan2(longitude2 - longitude1,
                   latitude2 - latitude1) * 180.0 / pi + 22.5;
    if (d < 0)
        d += 360.0;
    int n = (int)(d / 45.0);
    return dirs[n]; }

bool isQuitCommand(const string& input) {
    return input == "QUIT";
}

#ifndef DISTANCE_H_INCLUDED
#define DISTANCE_H_INCLUDED

#include <string>
#include <vector>
#include <cmath>
#include "Server/Source/srcKnnClassifier/NPoint.hpp"

class DistanceCalc {
protected:
        std::string FILE_NAME;
public:

    /**
     * Calculate the distance between two points.
     * @param p1 - point 1.
     * @param p2  - point 2.
     * @return the distance.
     */
    virtual const double dist(const NPoint& p1, const NPoint& p2) const = 0;

    /**
     * @return the file name Output for the current distance function.
     */
    const std::string& fileName() const;
};
#endif
#ifndef KNNCLASSIFIER_DISTANCESDATA_HPP
#define KNNCLASSIFIER_DISTANCESDATA_HPP

#include "DistanceCalc.hpp"
#include "Server/Source/srcDistances/srcDistanceCalculators/ChebyshevDistance.hpp"
#include "Server/Source/srcDistances/srcDistanceCalculators/ManhattanDistance.hpp"
#include "Server/Source/srcDistances/srcDistanceCalculators/EuclideanDistance.hpp"
#include "vector"

class DistancesData {
public:
    /**
     * @return a list of all distances calculators implemented.
     */
    static const std::vector<DistanceCalc*>& getAllTypes();
};


#endif //KNNCLASSIFIER_DISTANCESDATA_HPP

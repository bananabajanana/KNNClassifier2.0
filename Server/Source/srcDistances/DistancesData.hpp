#ifndef KNNCLASSIFIER_DISTANCESDATA_HPP
#define KNNCLASSIFIER_DISTANCESDATA_HPP

#include "DistanceCalc.hpp"
#include "source/srcDistances/srcDistanceCalculators/ChebyshevDistance.hpp"
#include "source/srcDistances/srcDistanceCalculators/ManhattanDistance.hpp"
#include "source/srcDistances/srcDistanceCalculators/EuclideanDistance.hpp"
#include "vector"

class DistancesData {
public:
    /**
     * @return a list of all distances calculators implemented.
     */
    static const std::vector<DistanceCalc*>& getAllTypes();
};


#endif //KNNCLASSIFIER_DISTANCESDATA_HPP

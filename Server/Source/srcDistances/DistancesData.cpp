#include "DistancesData.hpp"

const std::vector<DistanceCalc*>& DistancesData::getAllTypes() {
    auto *allTypes = new std::vector<DistanceCalc*>();
    allTypes->push_back((DistanceCalc *)new EuclideanDistance);
    allTypes->push_back((DistanceCalc *)new ManhattanDistance);
    allTypes->push_back((DistanceCalc *)new ChebyshevDistance);
    return *allTypes;
}
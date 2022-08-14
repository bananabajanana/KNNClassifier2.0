#include "ManhattanDistance.hpp"

const double ManhattanDistance::dist(const NPoint& p1, const NPoint& p2) const {
    //points have to be of equal coordinate systems.
    if(p1.getN() != p2.getN()) {
        throw std::exception();
    }
    double sum = 0;
    for(int i=0; i<p1.getN();i++) {
        sum+= fabs(p1.getPoint()[i] - p2.getPoint()[i]);
    }
    return sum;
}

ManhattanDistance::ManhattanDistance() {
    this->FILE_NAME = "../Output/manhattan_output.csv";
}
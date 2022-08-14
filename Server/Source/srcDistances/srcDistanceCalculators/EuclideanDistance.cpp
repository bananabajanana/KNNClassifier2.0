#include "EuclideanDistance.hpp"

const double EuclideanDistance::dist(const NPoint& p1, const NPoint& p2) const {
    //points have to be of equal coordinate systems.
    if(p1.getN() != p2.getN()) {
        throw std::exception();
    }
    double sum=0;
    for(int i=0; i<p1.getN();i++) {
        sum+= pow(p1.getPoint()[i] - p2.getPoint()[i], 2);
    }
    return sqrt(sum);
}

EuclideanDistance::EuclideanDistance() {
    this->FILE_NAME = "../Output/euclidean_output.csv";
}
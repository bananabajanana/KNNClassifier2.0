#include "ChebyshevDistance.hpp"

const double ChebyshevDistance::dist(const NPoint& p1, const NPoint& p2) const {
    //points have to be of equal coordinate systems.
    if(p1.getN() != p2.getN()) {
        throw std::exception();
    }
    double sum = fabs(p1.getPoint()[0] - p2.getPoint()[0]);
    for(int i=1; i<p1.getN();i++) {
        sum = fmax(fabs(p1.getPoint()[i] - p2.getPoint()[i]), sum);
    }
    return sum;
}

ChebyshevDistance::ChebyshevDistance() {
    this->FILE_NAME = "../Output/chebyshev_output.csv";
}
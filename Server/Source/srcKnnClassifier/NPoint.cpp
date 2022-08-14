#include "NPoint.hpp"

NPoint::NPoint(double sepalWidth, double sepalLength, double petalWidth, double petalLength)
        : Point({sepalWidth, sepalLength, petalWidth, petalLength}), n(4)
{
}
NPoint::NPoint(double* properties, int n)
    :n(n), Point(makePoint(properties,n))
{
}

const std::vector<double> NPoint::getPoint() const {
    return Point;
}

 std::vector<double> NPoint::makePoint(double* properties, int n) {
    std::vector<double> temp;
    for(int i=0;i<n;i++) {
        temp.push_back(properties[i]);
    }
    return temp;
}

const int NPoint::getN() const{
    return n;
}


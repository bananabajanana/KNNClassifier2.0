#ifndef CHEBYSHEV_H_INCLUDED
#define CHEBYSHEV_H_INCLUDED

#include "source/srcDistances/DistanceCalc.hpp"

/**
 * Responsible on the function of the project used Chebyshev distance.
 */
class ChebyshevDistance : public DistanceCalc {
    public:
    /**
     *  override
     **/
    const double dist(const NPoint& p1, const NPoint& p2) const override;
    ChebyshevDistance();
};

#endif
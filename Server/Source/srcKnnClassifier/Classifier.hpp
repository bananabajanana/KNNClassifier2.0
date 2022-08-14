#ifndef CLASSIFIER_H_INCLUDED
#define CLASSIFIER_H_INCLUDED

#include <iostream>
#include <vector>
#include <cmath>
#include "Flower.hpp"
#include "source/srcDistances/DistanceCalc.hpp"
#define MAX_NUM 1.7976931348623157E+308

/**
 * Classifier defines flowers with the knn algorithm.
 */
class Classifier {
private:
    const int k;
    std::vector<Flower> classified;

    /**
     * Define an undefined flower.
     * @param f - The flower.
     * @param typeDis - The distance method we are going to use.
     */
    void defFlower(Flower& f, DistanceCalc& typeDis) const;

    /**
     * Helps the "defFlower" function algorithm finding the closest iris
     * and sending its place - while changing the distance to max value.
     * @param distances - vector of distances.
     * @return the place of the closest point.
     */
    static int whereMinInArr(std::vector<double>& distances);

public:
    /**
     * Defines all the unclassified flowers.
     * @param unclassified - all the unclassified flowers.
     * @param typeDis - whe distance method we use.
     */
    void defFlowers(std::vector<Flower>& unclassified, DistanceCalc& typeDis) const;

    /**
     * Creat classifier for merging unclassified.
     * @param k - the amount of iris that we are using.
     * @param classified - the flowers we use.
     */
    Classifier(int k, const std::vector<Flower>& classified);
};

#endif
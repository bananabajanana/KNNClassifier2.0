#include "Classifier.hpp"


Classifier::Classifier(int k, const std::vector<Flower>& classified) :k(k) {
    for(auto& flower:classified) {
            this->classified.push_back(flower);
    }
}

void Classifier::defFlowers(std::vector<Flower>& unclassified, DistanceCalc& typeDis) const{
    for(int i = 0; i < unclassified.size();i++) {
        defFlower(unclassified[i], typeDis);
    }
}
void Classifier::defFlower(Flower& f, DistanceCalc& calculator) const{
    std::vector<double> distances;
    std::vector<Flower> results;

    for(auto & i : classified) {
        distances.push_back(calculator.dist(f.getPoint(), i.getPoint()));
    }

    for(int i=0;i<k;i++) {
        results.push_back(classified[whereMinInArr(distances)]);
    }

    int types[] = {0,0,0};
    for(int i=0;i<k;i++) {
        types[results[i].getTypeOfIris()]++;
    }

    if(types[0]==fmax(types[0],(int)fmax(types[1],types[2]))) {
        f.setType(versicolor);
    }
    if(types[1]==fmax(types[0],(int)fmax(types[1],types[2]))) {
        f.setType(virginica);
    }

    if(types[2]==fmax(types[0],(int)fmax(types[1],types[2]))) {
        f.setType(setosa);
    }

}


int Classifier::whereMinInArr(std::vector<double>& distances) {
    double min = MAX_NUM;
    for(double distance : distances) {
        if(min > distance) {
            min = distance;
        }
    }
    for(int i=0;i<distances.size();i++) {
        if(min == distances[i]) {
            distances[i] = MAX_NUM;
            return i;
        }
    }
    return 0;

}
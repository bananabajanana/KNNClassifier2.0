#include <vector>

#include "source/srcKnnClassifier/Flower.hpp"
#include "source/srcIO/FileConverter.hpp"
#include "source/srcDistances/DistanceCalc.hpp"
#include "source/srcKnnClassifier/Classifier.hpp"
#include "source/srcDistances/DistancesData.hpp"

int main(int argc, char* argv[]) {
    //loading input files and k
    FileConverter fc;
    std::vector<Flower> classified = fc.updateFromFile("../input/classified.csv");
    std::vector<Flower> unclassified = fc.updateFromFile("../input/Unclassified.csv");
    int k = atoi(argv[0]);

    Classifier machine(k, classified);
    std::vector<DistanceCalc *> calculators = DistancesData::getAllTypes();

    for (DistanceCalc* calculator : calculators) {
        //classifying unclassified input
        std::vector<Flower> output = unclassified;
        machine.defFlowers(output, *calculator);
        fc.setContent(output);

        //outputting to relative
        std::string path = calculator->fileName();
        fc.updateToFile(path);
    }
}

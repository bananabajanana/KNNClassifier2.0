#include <vector>

#include "source/srcKnnClassifier/Flower.hpp"
#include "source/srcIO/FileConverter.hpp"
#include "source/srcDistances/DistanceCalc.hpp"
#include "source/srcKnnClassifier/Classifier.hpp"
#include "source/srcDistances/DistancesData.hpp"

/*
 * TODO:
 *   -Write server-side and client-side main codes
 *   -Make new srcIO for server-side (instead of converting file, convert input from client)
 *   -Make the server-side return immediately the class after getting a flower
 *   -Make the client-side read the input file one at a time, and after sending each flower to the server write the returned class to the output
 *   -Update all file calls to updated places (post directory-reorganization)
 *   -Update CMakeLists to work with two executables (server-side and client-side)
 */

/*
 * Server-Client interaction diagram.
 *
 *   Client:
 *             Sends one flower  <--   Updates output file
 *             info to server.         with new classification
 *                 |                        ^
 *   Server:       V                        |
 *             Turns info to     -->   Classifies flower
 *             Flower object           and sends info to client
 */

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

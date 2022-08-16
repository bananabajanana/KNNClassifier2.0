
#include "Server/Source/srcKnnClassifier/ServerProcess.hpp"
/*
 * TODO:
 *      -add time library and timeout functionality to server
 *      -make sure all flower info was sent
 *      -clean code and write readme
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
using namespace std;

int main(int argc, char* argv[]) {
    FileConverter fc;
    std::vector<Flower> classified = fc.updateFromFile("../Server/Data/TrainingData.csv");
    Classifier machine(5, classified);
    ServerProcess server;
    server.ServerRunner(machine, fc);
}
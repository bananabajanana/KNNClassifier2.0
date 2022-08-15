#include <vector>
#include <iostream>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include "Server/Source/srcKnnClassifier/Flower.hpp"
#include "Server/Source/srcIO/FileConverter.hpp"
#include "Server/Source/srcDistances/DistanceCalc.hpp"
#include "Server/Source/srcKnnClassifier/Classifier.hpp"
#include "Server/Source/srcDistances/DistancesData.hpp"
#include "Server/Source/srcDistances/srcDistanceCalculators/EuclideanDistance.hpp"

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


using namespace std;

int main(int argc, char* argv[]) {

    FileConverter fc;
    std::vector<Flower> classified = fc.updateFromFile("../input/classified.csv");
    int k = atoi(argv[0]);
    Classifier machine(k, classified);

    //region Server Initialization
    const int server_port = 6969;
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("error creating socket");
    }
    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(server_port);
    if (bind(sock, (struct sockaddr *) &sin, sizeof(sin)) < 0) {
        perror("error binding socket");
    }
    if (listen(sock, 5) < 0) {
        perror("error listening to a socket");
    }
    struct sockaddr_in client_sin;
    unsigned int addr_len = sizeof(client_sin);
    int client_sock = accept(sock,  (struct sockaddr *) &client_sin,  &addr_len);
    if (client_sock < 0) {
        perror("error accepting client");
    }
    //endregion

    char buffer[4096];
    int expected_data_len = sizeof(buffer);
    int read_bytes =1;
    do {
        read_bytes = recv(client_sock, buffer, expected_data_len, 0);
        if (read_bytes == 0) {
            // connection is closed
        }
        else if (read_bytes < 0) {
            // error
        }
        else {
            //cout << buffer;
        }
        Flower unclassified = fc.updateFromBuffer(buffer);
        DistanceCalc* d = ((DistanceCalc *)new EuclideanDistance);
        machine.defFlower(unclassified,d);
        int sent_bytes = send(client_sock, fc.getType(unclassified.getTypeOfIris()), read_bytes, 0);

        if (sent_bytes < 0) {
            perror("error sending to client");
        }
    } while(read_bytes != 0);


    close(sock);


    return 0;
}
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
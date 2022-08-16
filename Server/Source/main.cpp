#ifdef WIN32
#include <windows.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif
#include <vector>
#include <iostream>
#include <stdio.h>
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
 *      -strtok kinda sus uwu
 *      -reset buffer (\0) and add \n
 *      -add time library and timeout functionality to server
 *      -make sure all flower info was sent
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
    std::vector<Flower> classified = fc.updateFromFile("../Server/Data/classified.csv");
    int k = 5;
    Classifier machine(k, classified);

    //region Server Initialization
    const int server_port = 6969;
    fd_set fd;
    timeval tv;
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
    while(true)
    {
        FD_ZERO(&fd);
        FD_SET(sock, &fd);
        tv.tv_sec = 5;
        tv.tv_usec = 0;
        char buffer[4096] = { 0 };
        int expected_data_len = sizeof(buffer);
        int read_bytes;
        if (listen(sock, 5) < 0) {
            perror("error listening to a socket");
        }
        // endregion
        if (select(0, &fd, NULL, NULL, &tv) > 0) {
            struct sockaddr_in client_sin;
            unsigned int addr_len = sizeof(client_sin);
            int client_sock = accept(sock, (struct sockaddr *) &client_sin, &addr_len);
            if (client_sock < 0) {
                perror("error accepting client");
            }
            long nbio = 1;
            ::ioctlsocket(client_sock, FIONBIO, &nbio);
            do {
                read_bytes = recv(client_sock, buffer, expected_data_len, 0);
                if (read_bytes == 0) {
                    break;
                    // connection is closed
                } else if (read_bytes < 0) {
                    //dealing with lost communication ?
                    //and reastablishing communication
                    //set timeout and reset on timeout error
                    if (WSAGetLastError() == WSAEWOULDBLOCK)
                    {
                        FD_ZERO(&fd);
                        FD_SET(client_sock, &fd);
                        tv.tv_sec = 5;
                        tv.tv_usec = 0;
                        if (select(0, NULL, &fd, NULL, &tv) > 0)
                            continue;
                    }
                    break;
                    perror("There was a problem in recieving the information.");
                    // error
                }
                Flower unclassified = fc.flowerFromLine(buffer);
                DistanceCalc *d = ((DistanceCalc *) new EuclideanDistance);
                machine.defFlower(unclassified, *d);
                int message_len = strlen(fc.getType(unclassified.getTypeOfIris()));
                int sent_bytes = send(client_sock, fc.getType(unclassified.getTypeOfIris()), message_len, 0);

                if (sent_bytes < 0) {
                    perror("error sending to client");
                }
            } while (read_bytes != 0);
        }
    }
    close(sock);


    return 0;
}
//int main(int argc, char* argv[]) {
//    //loading input files and k
//    FileConverter fc;
//    std::vector<Flower> classified = fc.updateFromFile("../input/classified.csv");
//    std::vector<Flower> unclassified = fc.updateFromFile("../input/Unclassified.csv");
//    int k = atoi(argv[0]);
//
//    Classifier machine(k, classified);
//    std::vector<DistanceCalc *> calculators = DistancesData::getAllTypes();
//
//    for (DistanceCalc* calculator : calculators) {
//        //classifying unclassified input
//        std::vector<Flower> output = unclassified;
//        machine.defFlowers(output, *calculator);
//        fc.setContent(output);
//
//        //outputting to relative
//        std::string path = calculator->fileName();
//        fc.updateToFile(path);
//    }
//}
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

int serverInitialization(const int server_port) {
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
    return sock;
}
void listenSoc(int sock) {
    if (listen(sock, 5) < 0) {
        perror("error listening to a socket");
    }
}
int acceptSoc(int sock, struct sockaddr_in client_sin) {
    unsigned int addr_len = sizeof(client_sin);
    int client_sock = accept(sock, (struct sockaddr *) &client_sin, &addr_len);
    if (client_sock < 0) {
        perror("error accepting client");
    }
    return client_sock;
}
Flower defFlowerSoc(char* properties, Classifier machine,FileConverter fc) {
    Flower unclassified = fc.flowerFromLine(properties);
    DistanceCalc *d = ((DistanceCalc *) new EuclideanDistance);
    machine.defFlower(unclassified, *d);
    return unclassified;
}
void sendSoc(Flower unclassified, FileConverter fc, int client_sock) {
    int message_len = strlen(fc.getType(unclassified.getTypeOfIris()));
    int sent_bytes = send(client_sock, fc.getType(unclassified.getTypeOfIris()), message_len, 0);

    if (sent_bytes < 0) {
        perror("error sending to client");
    }
}

int main(int argc, char* argv[]) {
    FileConverter fc;
    std::vector<Flower> classified = fc.updateFromFile("../Server/Data/classified.csv");
    Classifier machine(5, classified);

    int sock = serverInitialization(6969);

    while(true) {
        listenSoc(sock);

        struct sockaddr_in client_sin;
        int client_sock = acceptSoc(sock, client_sin);
        char buffer[4096];
        int expected_data_len = sizeof(buffer);
        int read_bytes = 1;
        do {

            //region Manual buffer memset because of bug
            for(int i = 0; i < 20; i++) {
                buffer[i] = '\0';
            }
            //endregion

            int read_bytes = recv(client_sock, buffer, expected_data_len, 0);
            if (read_bytes == 0) {
                break;
                // connection is closed
            } else if (read_bytes < 0) {
                perror("There was a problem in recieving the information.");
                // error
            }

            Flower unclassified = defFlowerSoc(buffer, machine, fc);
            sendSoc(unclassified, fc, client_sock);
        } while (read_bytes != 0);
    }
}
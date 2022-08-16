#ifdef WIN32
#include <windows.h>
#include <winsock.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
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
    const int enable = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0){
        perror("setsockopt(SO_REUSEADDR) failed");
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

    int listeningSock = serverInitialization(6969);
    listenSoc(listeningSock );

    int client_sock = -1;
    int maxSocketsNum = 1;
    int clientsNum = 0;
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(listeningSock, &rfds);
    struct timeval tv;
    tv.tv_sec = 10;
    tv.tv_usec = 0;

    while(true) {
        int retval = select(maxSocketsNum, &rfds, NULL, NULL, &tv);
        if(retval==-1) {
            //error the socket is not right
            perror("");
            return -1;
        }
        if(retval==0) {
            //there is time out
            if(clientsNum==0) {
                continue;
            }
            close(client_sock);
            close(listeningSock);
            return 0;
        }
        if(FD_ISSET(listeningSock,&rfds)) {
            // event on listening socket
            struct sockaddr_in client_sin;
            client_sock = acceptSoc(listeningSock, client_sin);
            if(client_sock==-1) {
                return -1;
            }
            clientsNum++;
            maxSocketsNum++;
            FD_SET(client_sock, &rfds);
        }
        else
        {
            char buffer[4096];
            int expected_data_len = sizeof(buffer);
            int read_bytes = 1;
            do
            {
                //region Manual buffer memset because of bug
                for (int i = 0; i < 20; i++) {
                    buffer[i] = '\0';
                }
                //endregion

                int read_bytes = recv(client_sock, buffer, expected_data_len, 0);
                if (read_bytes <= 0)
                {
                    maxSocketsNum--;
                    clientsNum--;
                    FD_CLR(client_sock,&rfds);
                    close(client_sock);
                    if(read_bytes<0) {
                        perror("recive returned -1");
                    } else {
                        cout<<"client closed connection.";
                    }
                    break;
                }
                Flower unclassified = defFlowerSoc(buffer, machine, fc);
                sendSoc(unclassified, fc, client_sock);
            }while (read_bytes != 0);
        }//else
    } // while(true)
}
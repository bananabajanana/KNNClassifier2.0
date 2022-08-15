#include <iostream>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <fstream>

using namespace std;

int main() {
    //region FilesIOSetup
    const string unclassifiedPath = "../Client/Data/Unclassified.csv";
    const string outputPath = "../Client/Output/euclidean_output.csv";

    ifstream input(unclassifiedPath);
    ofstream output(outputPath);

    string flowerInfo;
    //endregion

    //region ClientSetup
    const char* ip_address = "127.0.0.1";
    const int port_no = 6969;

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("error creating socket");
    }

    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = inet_addr(ip_address);
    sin.sin_port = htons(port_no);

    if (connect(sock, (struct sockaddr *) &sin, sizeof(sin)) < 0) {
        perror("error connecting to server");
    }
    //endregion

    while(getline(input, flowerInfo)) {
        char* toSend = const_cast<char *>(flowerInfo.c_str());
        int message_len = strlen(toSend);
        int sent_bytes = send(sock, toSend, message_len, 0);

        if (sent_bytes < 0) {
            perror("Client Error: Couldn't sent bytes to server");
        }

        char buffer[4096];
        int expected_data_len = sizeof(buffer);
        int read_bytes = recv(sock, buffer, expected_data_len, 0);
        if (read_bytes == 0) {
            perror("Client Error: Server closed the connection");
        }
        else if (read_bytes < 0) {
            perror("Client Error: Couldn't sent bytes to server");
        }
        else {
            output << buffer;
        }
    }

    close(sock);

}


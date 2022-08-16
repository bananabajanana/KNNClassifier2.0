#ifdef WIN32
#include <windows.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fstream>

using namespace std;

void sendMessage(string message, int sock) {
    char* toSend = const_cast<char *>(message.c_str());
    int message_len = strlen(toSend);
    int sent_bytes = send(sock, toSend, message_len, 0);

    if (sent_bytes < 0) {
        perror("Client Error: Couldn't sent bytes to server");
    }
}

void getMessage(int sock, char* buffer) {
    int expected_data_len = sizeof(buffer);
    int read_bytes = recv(sock, buffer, expected_data_len, 0);
    if (read_bytes == 0) {
        perror("Client Error: Server closed the connection");
    }
    else if (read_bytes < 0) {
        perror("Client Error: Couldn't sent bytes to server");
    }
}

int connectToServer(const char* ip_address, const int port_no) {
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
    return sock;
}

int main() {
    //region FilesIOSetup
    const string unclassifiedPath = "../Client/Data/Unclassified.csv";
    const string outputPath = "../Client/Output/euclidean_output.csv";

    ifstream input(unclassifiedPath);
    ofstream output(outputPath);

    string flowerInfo;
    //endregion

    //region ClientSetup
    int sock = connectToServer("127.0.0.1", 6969);
    //endregion

    while(getline(input, flowerInfo)) {
        sendMessage(flowerInfo, sock);

        char buffer[4096] = { 0 };
        getMessage(sock, buffer);
        output << buffer << endl;
    }

    close(sock);

}


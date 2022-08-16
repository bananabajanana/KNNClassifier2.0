#include "Client.hpp"

ClientProcess::ClientProcess(char *ip_address, int port_no) {
    this->ip_address = ip_address;
    this->port_no = port_no;
}

void ClientProcess::sendMessage(std::string message) {
    char* toSend = const_cast<char *>(message.c_str());
    int message_len = strlen(toSend);
    int sent_bytes = send(sock, toSend, message_len, 0);

    if (sent_bytes < 0) {
        perror("Client Error: Couldn't send bytes to server");
    }
}

//void sendMessage(std::string message, const int sock) {
//    char* toSend = const_cast<char *>(message.c_str());
//    int message_len = strlen(toSend);
//    int sent_bytes = send(sock, toSend, message_len, 0);
//
//    if (sent_bytes < 0) {
//        perror("Client Error: Couldn't send bytes to server");
//    }
//}

void ClientProcess::getMessage() {
    int read_bytes = recv(sock, buffer, expected_data_len, 0);
    if (read_bytes == 0) {
        perror("Client Error: Server closed the connection");
    }
    else if (read_bytes < 0) {
        perror("Client Error: Couldn't sent bytes to server");
    }
}

//void getMessage(char* buffer, const int sock, int expected_data_len) {
//    int read_bytes = recv(sock, buffer, expected_data_len, 0);
//    if (read_bytes == 0) {
//        perror("Client Error: Server closed the connection");
//    }
//    else if (read_bytes < 0) {
//        perror("Client Error: Couldn't sent bytes to server");
//    }
//}

int ClientProcess::connectToServer() {
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

//int connectToServer(const char* ip_address, const int port_no) {
//    int sock = socket(AF_INET, SOCK_STREAM, 0);
//    if (sock < 0) {
//        perror("error creating socket");
//    }
//
//    struct sockaddr_in sin;
//    memset(&sin, 0, sizeof(sin));
//    sin.sin_family = AF_INET;
//    sin.sin_addr.s_addr = inet_addr(ip_address);
//    sin.sin_port = htons(port_no);
//
//    if (connect(sock, (struct sockaddr *) &sin, sizeof(sin)) < 0) {
//        perror("error connecting to server");
//    }
//    return sock;
//}

void ClientProcess::setPaths(std::string unclassiiedPath, std::string outputPath) {
    this->unclassifiedPath = unclassiiedPath;
    this->outputPath = outputPath;
}

void ClientProcess::runClient() {
    //region FilesIOSetup
    std::ifstream input(unclassifiedPath);
    std::ofstream output(outputPath);

    std::string flowerInfo;
    //endregion

    sock = connectToServer();

    while(std::getline(input, flowerInfo)) {
        sendMessage(flowerInfo);

        //region Manual buffer memset because of bug
        for(int i = 0; i < 128; i++) {
            buffer[i] = '\0';
        }
        //endregion

        getMessage();
        output << buffer << std::endl;
    }

    close(sock);
}


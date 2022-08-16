#include "Client.hpp"

ClientProcess::ClientProcess(char *ip_address, int port_no) {
    this->ip_address = ip_address;
    this->port_no = port_no;
    this->unclassifiedPath = "../Client/Data/Unclassified.csv";
    this->outputPath = "../Client/Output/Classified.csv";
}

void ClientProcess::sendMessage(std::string message) {
    char* toSend = const_cast<char *>(message.c_str());
    int message_len = strlen(toSend);
    int sent_bytes = send(sock, toSend, message_len, 0);

    if (sent_bytes < 0) {
        perror("Client Error: Couldn't send bytes to server");
    }
}

void ClientProcess::getMessage() {
    int read_bytes = recv(sock, buffer, expected_data_len, 0);
    if (read_bytes == 0) {
        perror("Client Error: Server closed the connection");
    }
    else if (read_bytes < 0) {
        perror("Client Error: Couldn't sent bytes to server");
    }
}

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

void ClientProcess::setPaths(std::string unclassifiedPath, std::string outputPath) {
    this->unclassifiedPath = unclassifiedPath;
    this->outputPath = outputPath;
}

void ClientProcess::runClient() {
    //I/O setup
    std::ifstream input(unclassifiedPath);
    std::ofstream output(outputPath);
    std::string flowerInfo;

    //connecting to a server socket
    sock = connectToServer();

    //for each line in the input file, we send it to the server and
    //write the server's returning argument to a file.
    while(std::getline(input, flowerInfo)) {
        sendMessage(flowerInfo);

        //manually resetting the buffer to \0 between messages.
        //could probably change to memset, but that caused problems in the past.
        for(int i = 0; i < 128; i++) {
            buffer[i] = '\0';
        }

        getMessage();
        output << buffer << std::endl;
    }

    close(sock);
}
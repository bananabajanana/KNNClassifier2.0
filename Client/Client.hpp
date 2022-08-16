#ifndef KNNCLASSIFIER_CLIENT_HPP
#define KNNCLASSIFIER_CLIENT_HPP

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

class ClientProcess {
private:
    char* ip_address;
    char buffer[128];
    int expected_data_len = 128;
    int port_no;
    int sock;
    std::string unclassifiedPath = "../Client/Data/Unclassified.csv";
    std::string outputPath = "../Client/Output/euclidean_output.csv";


    void sendMessage(std::string message);
    void getMessage();
    int connectToServer();
public:
    ClientProcess(char* ip_address, int port_no);
    void setPaths(std::string unclassiiedPath, std::string outputPath);
    void runClient();
};

#endif //KNNCLASSIFIER_CLIENT_HPP

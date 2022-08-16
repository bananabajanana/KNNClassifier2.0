#include "Client.hpp"
#define CLIENT_PORT 6969
#define MY_IP "127.0.0.1"
int main(int argc, char **argv) {
    ClientProcess c(MY_IP, CLIENT_PORT);

    if(argc >= 2) {
        c.setPaths(argv[0], argv[1]);
    }

    c.runClient();
}
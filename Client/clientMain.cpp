#include "Client.hpp"
#define CLIENT_PORT 6969
#define MY_IP "127.0.0.1"


int main(int argc, char **argv) {
    ClientProcess c((char *)MY_IP, CLIENT_PORT);

    if(argc >= 3) {
        c.setPaths(argv[1], argv[2]);
    }

    c.runClient();
}
#include "Client.hpp"

int main(int argc, char **argv) {
    ClientProcess c("127.0.0.1", 6969);

    if(argc >= 2) {
        c.setPaths(argv[0], argv[1]);
    }

    c.runClient();
}
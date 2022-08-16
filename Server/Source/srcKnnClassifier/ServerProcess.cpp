#include "ServerProcess.hpp"


ServerProcess::ServerProcess() {
    listeningSock = serverInitialization(SERVER_PORT);
    listenSoc(listeningSock );
    client_sock = -1;
    //select() requires the size of the array of sockets to be passed as the maximum socket number plus one.
    //see the man page of select.
    maxFdsPlusOne = listeningSock + 1;
    clientsNum = 0;
    FD_ZERO(&rfds);
    FD_SET(listeningSock, &rfds);
    tv.tv_sec = CLIENT_TIME_OUT;
    tv.tv_usec = 0;

}


int ServerProcess::serverInitialization(const int server_port) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("error creating socket");
    }
    const int enable = 1;
    //in TCP after closing the program the socket still remains in kernel bound to the port.
    //to prevent failure of the bind() on restarting the program, mark the socket to reuse the address.
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


void ServerProcess::listenSoc(int sock) {
    if (listen(sock, 5) < 0) {
        perror("error listening to a socket");
    }
}


int ServerProcess::acceptSoc(int sock, struct sockaddr_in client_sin) {
    unsigned int addr_len = sizeof(client_sin);
    int client_sock = accept(sock, (struct sockaddr *) &client_sin, &addr_len);
    if (client_sock < 0) {
        perror("error accepting client");

    }
    return client_sock;
}


Flower ServerProcess::defFlowerSoc(char* properties, Classifier machine,FileConverter fc) {
    Flower unclassified = fc.flowerFromLine(properties);
    DistanceCalc *d = ((DistanceCalc *) new EuclideanDistance);
    machine.defFlower(unclassified, *d);
    return unclassified;
}


void ServerProcess::sendSoc(Flower unclassified, FileConverter fc, int client_sock) {
    int message_len = strlen(fc.getType(unclassified.getTypeOfIris()));
    int sent_bytes = send(client_sock, fc.getType(unclassified.getTypeOfIris()), message_len, 0);

    if (sent_bytes < 0) {
        perror("error sending to client");
    }
}


void ServerProcess::ServerRunner(Classifier machine,FileConverter fc) {

    while(true) {
        int retval = select(maxFdsPlusOne, &rfds, NULL, NULL, &tv);
        if(retval==-1) {
            //error the socket is not right
            perror("There is an error with the socket.");
            return;
        }
        if(retval==0) {
            //there is time out
            if(clientsNum!=0) {
                close(client_sock);
                close(listeningSock);
            }
            continue;
        }
        //retval is positive number. that means that one of our sockets recived data or
        //our listening socket recived a new connection that we need to accept.
        if(FD_ISSET(listeningSock,&rfds)) {
            // event on listening socket - accepting new clients
            struct sockaddr_in client_sin;
            client_sock = acceptSoc(listeningSock, client_sin);
            if(client_sock==-1) {
                return;
                //will sea will sea
            }
            clientsNum++;
            //maximum of all client_sockets +1
            //we have only two sockets. we don't know what is the max because the kernel system do not promise numbers.
            maxFdsPlusOne = std::max(listeningSock, client_sock) + 1;
            FD_SET(client_sock, &rfds);
        }
        else
        {
            char buffer[128];
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
                    maxFdsPlusOne = listeningSock + 1;
                    clientsNum--;
                    FD_CLR(client_sock,&rfds);
                    close(client_sock);
                    if(read_bytes<0) {
                        perror("recive returned -1");
                    } else {
                        std::cout<<"client closed connection.";
                    }
                    break;
                }
                Flower unclassified = defFlowerSoc(buffer, machine, fc);
                sendSoc(unclassified, fc, client_sock);
            }while (read_bytes != 0);
        }//else
    } // while(true)
}

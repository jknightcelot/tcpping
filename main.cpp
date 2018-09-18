#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

int main(int argc, char *argv[]) {
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    struct timeval timeout;

    char buffer[256];

    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }

    portno = atoi(argv[2]);

    // Get IP of host.
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    for (int i=0; i < 5; i++){
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0)
            std::cout << "ERROR opening socket" << std::endl;

        // Set socket options
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;
        if (setsockopt (sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *) &timeout, sizeof(timeout)) < 0)
            std::cout << "Setting socket option 'receive timeout' failed" << std::endl;
        if (setsockopt (sockfd, SOL_SOCKET, SO_SNDTIMEO, (char *) &timeout, sizeof(timeout)) < 0)
            std::cout << "Setting socket option 'send timeout' failed" << std::endl;

        bzero((char *) &serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;

        bcopy((char *)server->h_addr,
              (char *)&serv_addr.sin_addr.s_addr,
              server->h_length);

        std::cout << ("Pinging \n");

        serv_addr.sin_port = htons(portno);
        if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        {
            std::cout << "Error: Unable to reach IP - " << ntohl(*server->h_addr_list[0]) << std::endl;
            close(sockfd);
        }
        else
        {
            std::cout << "Connected successfully" << std::endl;
            close(sockfd);
        }

    }

    return 0;
}
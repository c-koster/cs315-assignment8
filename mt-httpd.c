/*
 * Culton Koster - multi-threaded webserver
 */
#include <signal.h>

#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>


#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#include <unistd.h>

#include <stdio.h>
#include <string.h>
#include <errno.h>


#define BACKLOG 10
#define BUF_SIZE 4096



int main(int argc, char *argv[])
{

    // create a socket for listening --
    char *listen_port = "80";
    int listen_fd,conn_fd,index_fd;
    struct addrinfo hints, *res;
    int rc;
    int n;
    char buf[BUF_SIZE];
    char *remote_ip;
    uint16_t remote_port;


    // setup threaded handling of a client --
    struct sockaddr_in remote_sa;
    socklen_t addrlen;

    /* create a socket */
    listen_fd = socket(PF_INET, SOCK_STREAM, 0);

    /* bind it to a port */
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if ((rc = getaddrinfo(NULL, listen_port, &hints, &res)) != 0) {
        printf("getaddrinfo failed: %s\n", gai_strerror(rc));
        exit(1);
    }


    bind(listen_fd, res->ai_addr, res->ai_addrlen);

    // start listening
    listen(listen_fd, BACKLOG);
    while (1) {
        /* accept a new connection (will block until one appears) */
        addrlen = sizeof(remote_sa);

        if ((conn_fd = accept(listen_fd, (struct sockaddr *) &remote_sa, &addrlen)) < 0) {
            perror("accept");
            exit(15);
        }
        remote_ip = inet_ntoa(remote_sa.sin_addr);
        remote_port = ntohs(remote_sa.sin_port);
        printf("new request from %s:%d\n", remote_ip, remote_port);

        if ((index_fd = open("index.html", O_RDONLY)) < 0) {
            perror("open");
            exit(15);
        }
        n = read(index_fd,buf,BUF_SIZE);
        n = write(conn_fd,buf,n);
        close(index_fd);
        close(conn_fd);
    }
}

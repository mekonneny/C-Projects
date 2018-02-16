/*
** Yonathan Mekonnen -- a stream socket server demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>

#define MAXDATASIZE 50

#define BACKLOG 10       // how many pending connections queue will hold


pid_t pid;

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
        if (sa->sa_family == AF_INET)
                return &(((struct sockaddr_in*)sa)->sin_addr);


        return &(((struct sockaddr_in6*)sa)->sin6_addr);
}




//signal handler
void signal_handler(int num){
  //printf("signal caught\n");
  exit(-1);
}

//catches child process
void signal_child(int child){
  while (1){
    pid = waitpid(-1, NULL, WNOHANG);
      if (pid == 0)
      return;
  }
}
//gets file name
void recieveFile( int s, char *files){
  char buf[MAXDATASIZE];
  ssize_t bytes;
//reads the name of the file
  if ((bytes = recv(s, buf, MAXDATASIZE, 0)) < 0) {
    perror("recv error");
    return;
  }
  sscanf(buf, "%s\n", files);
}
//sends file to client
void fileTransfer (int s, char *files){


  char buf[MAXDATASIZE];
  char * error = "The file has not been found";
  int fileHandle;
  ssize_t bytes, sbytes;

  if ((fileHandle = open(files, O_RDONLY)) < 0){
    perror(files);
    if ((sbytes = send(s, error, strlen(error), 0)) < 0){
      perror("Sending error");
      exit(-1);
    }
  } else {
    printf ("File is sending : %s\n", files);
    while((bytes = read(fileHandle, buf, MAXDATASIZE)) > 0) {
      if((sbytes = send(s, buf, bytes, 0)) < bytes) {
        perror("Sending error");
        exit(-1);
      }
    }
    close(fileHandle);
  }
}

int main(int argc, char *argv[])
{
        int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
        struct addrinfo hints, *servinfo, *p;
        struct sockaddr_storage their_addr; // connector's address information
        socklen_t sin_size;
        char *port;
        int yes=1;
        char end[50] = "cmsc257";
        char s[INET6_ADDRSTRLEN];
        int rv;
        char files[MAXDATASIZE];
        //Sig handling
        signal(SIGINT, signal_handler);
        signal(SIGCHLD, signal_child);

        if (argc == 2) {
          port = argv[1];
        } else {
        fprintf(stderr,"incorrect amount of args\n");
      }


        memset(&hints, 0, sizeof hints);
        memset(&their_addr, 0, sizeof(their_addr));

        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_PASSIVE; // use my IP

        if ((rv = getaddrinfo(NULL, port, &hints, &servinfo)) != 0) {
                fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
                return 1;
        }

        // loop through all the results and bind to the first we can
        for(p = servinfo; p != NULL; p = p->ai_next) {
                if ((sockfd = socket(p->ai_family, p->ai_socktype,
                                p->ai_protocol)) == -1) {
                                  perror("socket");
                        continue;
                }

                 if (setsockopt(sockfd, SOL_SOCKET,SO_REUSEADDR, &yes, sizeof(int)) == -1) {
                   perror("setsockopt");
                   exit(1);
                 }
                //connect from client to server
                if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
                        close(sockfd);
                        perror("server: bind");
                        continue;
                }

                break;
        }

        freeaddrinfo(servinfo); // all done with this structure

        if (p == NULL)  {
                fprintf(stderr, "server: bind failed\n");
                exit(1);
        }

        if (listen(sockfd, BACKLOG) == -1) {
                perror("listen");
                exit(1);
        }


        printf("server: waiting for connections...\n");

        while(1) {  // main accept() loop
                sin_size = sizeof their_addr;
                new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
                if (new_fd == -1) {
                        perror("accept");
                        continue;
                }

                //child and parent process
                pid = fork();
                if (pid == 0) {
                  inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr),
                  s, sizeof s);


                  recieveFile(new_fd, files);
                  fileTransfer(new_fd, files);
                  printf("got connection from %s\n", s);

                 //close(new_fd);
                //exit(0);
              }
              close(new_fd);

        }
        close(sockfd);
        return 0;
}

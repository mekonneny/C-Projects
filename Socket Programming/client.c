/*
** Yonathan Mekonnen -- a stream socket client demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <sys/socket.h>

#include <arpa/inet.h>

//#define PORT "8516" // the port client will be connecting to

#define MAXDATASIZE 50 // max number of bytes we can get at once

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
        if (sa->sa_family == AF_INET) {
                return &(((struct sockaddr_in*)sa)->sin_addr);
        }

        return &(((struct sockaddr_in6*)sa)->sin6_addr);
}


//gets file name
void recieveFile (int s, char* fileName)
{
  int fileHandle;
  char buf[MAXDATASIZE];
  ssize_t bytes, rbytes;

  if((bytes = send(s, fileName, MAXDATASIZE -1, 0)) < 0){
    perror("Sending error");
    exit(-1);
  }

  if((fileHandle = open(fileName, O_WRONLY | O_CREAT, 0644)) < 0) {
    perror("error creating the file");
    exit(-1);
  }

  while((rbytes = recv(s, buf, MAXDATASIZE, 0)) > 0){
    if (write(fileHandle, buf, rbytes) < 0) {
      perror("Writing to file eror");
      exit(-1);
    }
  }
  close(fileHandle);
}

int main(int argc, char *argv[])
{
        int i, files = atoi(argv[3]);

        for (i = 0; i <= files; i++){
        int stime = atoi(argv[4]);
        int sockfd;
        int num = (rand() % 25);
        char buf[MAXDATASIZE];
        struct addrinfo hints, *servinfo, *p;
        int rv;
        char s[INET6_ADDRSTRLEN];

        sleep(stime);
        if (argc > 5) {

            fprintf(stderr,"incorrect number of args\n");
            exit(1);
        }
         char *newfiles[25] = {"file001.txt", "file002.txt", "file003.txt", "file004.txt",
                             "file005.txt", "file006.txt", "file007.txt", "file008.txt",
                             "file009.txt", "file010.txt", "file011.txt", "file012.txt",
                             "file013.txt", "file014.txt", "file015.txt", "file016.txt",
                             "file017.txt", "file018.txt", "file019.txt", "file020.txt",
                             "file021.txt", "file022.txt", "file023.txt", "file024.txt",
                             "file025.txt"};

        memset(&hints, 0, sizeof (hints));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;

        if ((rv = getaddrinfo(argv[1], argv[2], &hints, &servinfo)) != 0) {
                fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
                return 1;
        }

        // loop through all the results and connect to the first we can
        for(p = servinfo; p != NULL; p = p->ai_next) {
                if ((sockfd = socket(p->ai_family, p->ai_socktype,
                                p->ai_protocol)) == -1) {
                        perror("client: socket");
                        continue;
                      }
                      if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1){
                        perror("connect");
                        close(sockfd);
                        continue;
                      }

                          break;
                   }

                   if (p == NULL) {
                           fprintf(stderr, "client: failed to connect\n");
                           return 2;
                   }
                   printf("%d\n", num);
                   printf("Requesting file: %s\n", newfiles[num]);
                   recieveFile(sockfd, newfiles[num]);

                   inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
                                   s, sizeof s);
                   printf("client: connecting to %s\n", s);

                   freeaddrinfo(servinfo); // all done with this structure





                     close(sockfd);

           }
           return 0;

         }

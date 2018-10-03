#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet.h>
#include <errno.h>
#include <stdlin.h>
#include <string.h>
#include "utils.h"

int main(int argc, char *argv[]) {

   int sockfd;

   if ((sockfd = socket(AF_INET, SOCK_STREAM,0)) == -1)errExit("socket failed", EXIT);
   connect(sockfd, generic_address, sizeof(address));
   return 0;

}

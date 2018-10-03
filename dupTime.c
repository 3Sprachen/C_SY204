/*

Author: Andrew Phillips
Assignment: File time manipulation demo
Date: 02/26/18

Description:  This program shows you how to access the data structures
associated with file timestamps.  It takes the timestamps (last access and last
modified) associated with argv[1] and applies those to argv[2]

NOTE:  Be sure to put the utils.c and utils.h files in the same directory as
this file and compile with: gcc dupTime.c utils.c .....

*/

#include <stdio.h>
#include <stdlib.h>     // For exit
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include "utils.h"      // For errExit()

int main( int argc, char *argv[] ) {
   
   struct stat fStat;
   struct timespec savedTimes[2];
   int fd;

   if (argc != 3) {
      fprintf(stderr,"Usage: %s filename1 filename2\n",argv[0]);
      exit(EXIT_FAILURE);
   }

   if (stat(argv[1],&fStat) == -1) errExit(argv[1],EXIT);

   // save the access/modification times from argv[1]
   savedTimes[0] = fStat.st_atim;
   savedTimes[1] = fStat.st_mtim;

   fd = open(argv[2],O_RDWR | O_CREAT);
   if (fd == -1) errExit(argv[2],EXIT);

   // copy the timestamps from argv[1] to argv[2]
   if (futimens(fd,savedTimes) == -1) errExit("futimens",EXIT);

   if (close(fd) == -1) errExit(argv[2],EXIT);

   return EXIT_SUCCESS;

}

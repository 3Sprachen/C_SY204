//Erich Eden
//201644
//19FEB18
//myCat.c is supposed to print file contents to the screen


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "utils.h"
#include <errno.h>

#define MAXLEN 10
int readAndWrite(int argc, char *argv[]);

int main(int argc, char *argv[]) {
   int funcTest;
   int flag;



   opterr = 0;

   while ((flag = getopt(argc, argv, "h")) != -1) {
      if (flag == 'h'){printf("usage: ./myCat [-h] filename filename ...\n");
      exit (0);
      }
   }


   funcTest = readAndWrite(argc, argv);
   if (funcTest != 0); {
      fprintf(stderr, "ERROR readAndWrite function: %d\n", errno);
      //perror("Error!");
   }
   return 0;
}

int readAndWrite(int argc, char *argv[]){

   int fWrite = STDOUT_FILENO; // = STDIN_FILENO;
   int fRead;
   int numread, numwritten;
   char buffer[MAXLEN];
   int i = 1;
      //do the function that reads files
   while (i < argc) {
      if (strcmp(argv[i], "-") == 0){
         fRead = STDIN_FILENO;
      } else {
         fRead = open(argv[i], O_RDONLY);
         if (fRead == -1){
            fprintf(stderr, "ERROR fRead: %d\n", errno);
            //perror("Error!");
         }
      }
      do {
         numread = read(fRead,buffer,MAXLEN);
         if (numread == -1) {
            fprintf(stderr, "ERROR numread: %d\n", errno);
            //perror("Error!");
         }

         if (numread > 0) {
            numwritten = write(fWrite,buffer,numread);
            if (numwritten == -1) {
               fprintf(stderr, "ERROR numwritten: %d\n", errno);
               //perror("Error!");
            }
         }
      } while (numread > 0);
      i++;
   }
return 0;
}

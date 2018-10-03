/*

Author: Erich Eden
Alpha: 201644
Assignment: Functions
Title: netGoat
Date: 19 MAR 18

Description:


*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <sys/stat.h>
#include "utils.h"

// comment this next line to turn off testing code
#define TESTING

struct option_t {
   char ident;    // option identity
   int on;        // 1=on, 0=off
   char *arg;     // == optarg if there is a required argument; NULL otherwise
};
#define MAXLEN = 20;
#define H 0
#define I 1
#define O 2
#define C 3
#define S 4
#define MAX_OPTIONS 5

void usage(char *name);
void cat(struct option_t *options);
void copyFile(int fdIN, int fdOUT);
void stealth(void);
void signalInstaller(void);

// all exit handlers are here
void exitHandlerCat(void);
void exitHandlerStealth(void);

// Signal handler
void signalHandler(int signo);

// GLOBALS SECTION
char *exeName; // used in stealth exit handler

int main(int argc, char *argv[]) {
   int flag;
   struct option_t options[MAX_OPTIONS];

   // turn off buffering of output to stdout (see man setbuf(3))
   setbuf(stdout,NULL);

   // clear all elements of array of struct option_t

   // TO DO -- use memset to "clear" all elements of the struct option_t
   // "options" array
   memset(options, 0, MAX_OPTIONS*sizeof(struct option_t)); //do I need the "&" or no
   //printf("%lu",sizeof(struct options));

   // get command line options
   opterr = 0; // suppress error messages from getopt
   while ((flag = getopt(argc,argv,"hi:o:cs")) != -1) {
      switch (flag) {
         case 'h': // help option ... no argument required
            options[H].ident = 'h';
            options[H].on = 1;
            break;
         case 'i': // input file provided as an argument
            options[I].ident = 'i';
            options[I].on = 1;
            options[I].arg = optarg;
            break;
         case 'o': // output file provided as an argument
            options[O].ident = 'o';
            options[O].on = 1;
            options[O].arg = optarg;
            break;
         case 'c': // "cat"; no argument required
            options[C].ident = 'c';
            options[C].on = 1;

            break;
         case 's': // "stealth"; no argument required
            options[S].ident = 's';
            options[S].on = 1;

            break;
         default:
         // assume that if an illegal option is provided, you should print out
         // the help message and quit with a failure exit status
            fprintf(stderr,"Invalid option detected\n");
            usage(argv[0]);
            exit(EXIT_FAILURE);
      }
   }

/*
The following is throw away code once it is verified to be working. It will not
be in the final project code.
*/

// BEGIN THROW-AWAY CODE
#ifdef TESTING
   // verify that options were processed correctly
   int i;
   for (i = 0; i < MAX_OPTIONS; i++) {
      if (options[i].on) {
         if (options[i].arg != NULL) {
            fprintf(stderr,"option %c with argument %s\n",options[i].ident,options[i].arg);
         } else {
            fprintf(stderr,"option %c with no argument\n",options[i].ident);
         }
      }
   }

   // process any non-option command line args
   for (i = optind; i < argc; i++) {
      fprintf(stderr,"command line arg %d: %s\n",i,argv[i]);
   }
#endif
// END THROW-AWAY CODE

   // implement code for each command line option
   if (options[H].on) { // "help"
      usage(argv[0]);
      exit(EXIT_SUCCESS);
   }

   // save the exe name so that the exit handler can access it
   exeName = argv[0];

   // enter stealth mode (if desired) before anything else is done
   if (options[S].on) { // go into "stealth" mode}
      stealth();
   }

   // install signal handlers for catching signals
   signalInstaller();

   if (options[C].on) { // "cat"
      cat(options);
   }

// ADDITIONAL CODE WILL GO BELOW THIS LINE ONCE EVERYTHING ABOVE IS WORKING

   return EXIT_SUCCESS;
}

void usage(char *name) {
// TO DO -- put appropriate usage code here
   fprintf(stdout,"Usage: %s with the following options: \n help: [-h] \n cat: [ -c -i inFile -o outFile] \n stealth:[-s] \n", name);  //maybe explain each option as well?
   return;
}

void copyFile(int fdIN, int fdOUT) {
   // TO DO -- put your "myCat" code here, adjusted so that it works with this
   // prototype

   int numread, numwritten;
   char buffer[MAXLEN];

   do {
      if ((numread = read(fdIN,buffer,MAXLEN)) == -1) errExit("file read failed", EXIT);
      if (numread > 0) {
         numwritten = write(fdOUT,buffer,numread);
            if (numwritten == -1) errExit("Write failed.", EXIT);
      }
   }while (numread > 0);

   return;
}


void cat(struct option_t *options) {
   // install exit Handler for cat
   // TO DO -- use atexit to register the cat exit handler here
   //printf("cat function called");
   if (atexit(exitHandlerCat) == -1) errExit("Exit handler failed.", EXIT);
   int fdIN, fdOUT;

   // fdIN will be set to a command line filename, or stdin if none provided
   // if the option was -i foo, then foo will be used for input
   // if the option was either -i -, or no option at all, then stdin is used
   if (options[I].on && (strcmp(options[I].arg,"-") != 0)) {
      fdIN = open(options[I].arg, O_RDONLY);
      if (fdIN == -1) errExit("open",EXIT);
   } else {
      fdIN = STDIN_FILENO;
   }

   // fdOUT will be set to a command line filename, or to stdout if none
   // provided.
   // if the option was -o foo, then foo will be used for output
   // if the option was either -o -, or no option at all, then stdout is used
   if (options[O].on && (strcmp(options[O].arg,"-") != 0)) {
      fdOUT = open(options[O].arg, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
      if (fdOUT == -1) errExit("open",EXIT);
   } else {
      fdOUT = STDOUT_FILENO;
   }

   copyFile(fdIN,fdOUT);

   // close files if not stdin or stdout
   // TO DO -- close the files BUT ONLY if they are NOT file descriptors
   // STDIN_FILENO or STDOUT_FILENO

   if (fdIN != STDIN_FILENO){
      if(close(fdIN)==-1) errExit("fdIN failed to close", EXIT);
   }
   if (fdOUT != STDOUT_FILENO){
      if (close(fdOUT)==-1) errExit("fdOUT failed to close", EXIT);
   }

   return;
}

void stealth(void) {
   fprintf(stderr,"Stealth called\n");
   return;
}

// Exit Handler Cat
void exitHandlerCat(void) {
   fprintf(stderr,"Cat exit handler called\n");
}

// Exit Handler Stealth
void exitHandlerStealth(void) {
   fprintf(stderr,"Stealth exit handler called\n");
}

void signalInstaller(void) {
   fprintf(stderr,"Signal Installer called\n");
   return;
}

void signalHandler(int signo) {
   fprintf(stderr,"SignalHandler called\n");
   return;
}

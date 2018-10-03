/*

Author: Erich Eden
Alpha: 201644
Assignment: Functions
Title: Batter
Date: 25 MAR 18

Description:
This program receives a signal from another shell and then sends a signal to a third shell.

*/
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include "utils.h"


// Global Variables for use in the signal handler
volatile int signalCount = 0;
volatile sig_atomic_t signalFlag = 0;
pid_t fielderPID;


// Function Prototypes

void usage();
void bat (char *, char*);
void leftySignalHandler(int signo);
void rightySignalHandler(int signo);
char* hand;
char* ball;


int main( int argc, char *argv[] ) {

   struct sigaction action;
   memset(&action,0,sizeof(struct sigaction));
   int optFlag;

   opterr = 0;
   if (argc == 2){
      action.sa_sigaction = &rightySignalHandler;
      action.sa_flags = SA_SIGINFO | SA_RESTART;
      //action.sa_handler = rightySignalHandler;
      fielderPID = atoi(argv[1]);
   }

   while ((optFlag = getopt(argc, argv, "h, r, l")) != -1) {

      switch(optFlag){
         case 'h':
            usage();
            exit (0);
         case 'r':
            action.sa_sigaction = &rightySignalHandler;
            action.sa_flags = SA_SIGINFO | SA_RESTART;
            //action.sa_handler = rightySignalHandler;
            fielderPID = atoi(argv[2]);
            break;
         case 'l':
            action.sa_sigaction = &leftySignalHandler;
            action.sa_flags = SA_SIGINFO | SA_RESTART;
            //action.sa_handler = leftySignalHandler;
            fielderPID = atoi(argv[2]);
            break;

         default:
            action.sa_sigaction = &rightySignalHandler;
            action.sa_flags = SA_SIGINFO | SA_RESTART;
            //action.sa_handler = rightySignalHandler;
            fielderPID = atoi(argv[1]);
            break;

      }
   }

   if (sigaction(SIGUSR1, &action, NULL)==-1)errExit("registration failed", EXIT);
   if (sigaction(SIGUSR2, &action, NULL)==-1)errExit("registration failed", EXIT);


   while (1) { //the batter waits forever for incoming signals
      //fprintf(stdout, "Batter waiting for signal from pitcher")
      fprintf(stdout, "Batter (PID:\t%ld) waiting for the pitch!\n", (long) getpid());
      pause();
      bat(hand, ball);
   }

   return EXIT_SUCCESS;

}

void rightySignalHandler(int signo) {
   int origErrNum = errno;

   switch (signo) {
      case SIGUSR1:

         hand = "Righty";
         ball = "Fly";

         //fprintf(stdout,"Righty batter hits fly ball to: %d", fielderPID);
         kill(fielderPID, SIGUSR2);
         break;
      case SIGUSR2:
         hand = "Righty";
         ball = "Ground";
         //fprintf(stdout,"Righty batter hits groundball to: %d", fielderPID);
         kill(fielderPID, SIGUSR1);
         break;

      default:
         fprintf(stdout, "Caught unknown signal. Give me a pitch we can play with!");
         break;
   }


   errno = origErrNum;
   return;
}

void leftySignalHandler (int signo) {

   int origErrNum = errno;

   switch (signo) {
      case SIGUSR1:
         hand = "Lefty";
         ball = "Ground";
         //fprintf(stdout,"Lefty batter hits groundball to: %d", fielderPID); //make this into some statement relevent to baseball
         kill(fielderPID, SIGUSR1);
         break;
      case SIGUSR2:
         hand = "Lefty";
         ball = "Fly";
         //fprintf(stdout,"Lefty batter hits fly ball to: %d", fielderPID); //make this relevent to baseball
         kill(fielderPID, SIGUSR2);
         break;

      default:

         fprintf(stdout,"Caught unknown signal. Give me a pitch we can play with!");
         break;
   }



   errno = origErrNum;
   return;


}
void usage(char *name) {

   fprintf(stdout,"Usage: ./batter [-h, l, r] <PID of Fielder>\n");
   return;
}
void bat(char* hand, char* ball){
   fprintf(stdout, "%s batter hits %s ball to %d!\n", hand, ball, fielderPID); //I call this function so I can just set variables in the signal handler and deal w them via this instead
}

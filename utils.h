/*

SY204 Utilities
Version 4.0
Last update: 04/10/18

*/

// If multiple source files (for a single project) all include "utils.h", this
// prevents collisions where each tries to (re)define the prototypes and macros.
#ifndef __UTILS_H

   // Macros

   #define __UTILS_H
   #define EXIT 1    // For errExit
   #define NOEXIT 0  // For errExit

   // Prototypes

   // Error handling utility
   int errExit(char *text, int exitCmd);

   // for parsing a command line into arguments
   #define MAXARGS 20
   struct command_t {
      char *cmd; // the short name for the command
      char *argv[MAXARGS]; // argv[0] is the executable
      int argc; // actual arg count of command to run
      int pipe2next; // 0 if no pipe to next command; 1 otherwise
      int redirectIN, redirectOUT; // 0 = no redirection
      char *infile, *outfile; // filenames (if used)
   };

   // command line parsing utility
   struct command_t parseCommandLine(char *commandLine);
   // for printing out all elements of the struct command_t (for testing)
   void printCommandLine(struct command_t command);
   
   int getAlarmValue(int delayMax);

#endif

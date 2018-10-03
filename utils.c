/*

SY204 Utilities
Version 4.0
Last update: 04/10/18

*/

#include <stdio.h>   // For printf() and perror()
#include <stdlib.h>  // For exit()
#include <errno.h>   // For errno
#include <string.h>  // For strerror()
#include <time.h>    // For time()
#include <ctype.h>   // for isblank()
#include "utils.h"

int firstCall = 1;
char *getNextCmd(char *commandLine, int *pipeFound);
int seekPipe(char *cmdLine);
char *seekRedirects(char *currentCmd, struct command_t *cmd);

// Utility-01 ----------------------------------------------------------------
/*

Name: Error handling utility

Description: This utility takes the prefix message you want to apply (text),
preserves the value in errno, and using perror(), appends the plain-text version
of errno to the prefix and outputs that stderr.  The utility will also return
the numerical value that was stored in errno, or, you can choose to exit your
program based on the value in exitCmd. The two options for exitCmd (EXIT and
NOEXIT) are defined in utils.h

*/

int errExit(char *text, int exitCmd) {
   
   int errnum = errno;
   perror(text);
   if (exitCmd == EXIT) {
      exit(errnum);
   }
   return errnum;
   
}

// ===========================================================================

// parse a command line into: argc, argv, and the actual command to execute
struct command_t parseCommandLine(char *commandLine) {
   struct command_t thisCommand;
   int i, pipeFound;
   char *strtokPlaceHolder, *currentCmd, *position;
   
   memset(&thisCommand,0,sizeof(struct command_t));
   
   while (1) {
      // get a single command and identify pipe if present
      currentCmd = getNextCmd(commandLine,&pipeFound);
      if (currentCmd == NULL) {
         firstCall = 1; // reset for next round of command line elements
         return thisCommand; // end of the line
      }
      
      // look for < and > in this command
      currentCmd = seekRedirects(currentCmd,&thisCommand);
      
      // parse the command line into its argv parts
      thisCommand.cmd = strtok_r(currentCmd," \t\v\n",&strtokPlaceHolder); // the actual executable command (with path)
      if (thisCommand.cmd != NULL) break; // otherwise try again
   }
   
   i = 1;
   while ((thisCommand.argv[i] = strtok_r(NULL," \t\v\n",&strtokPlaceHolder)) != NULL) i++;
   thisCommand.argc = i;
   
   // determine the basename of the command
   position = strrchr(thisCommand.cmd,'/'); // find the / in the command
   if (position == NULL) thisCommand.argv[0] = thisCommand.cmd; // not found
   else thisCommand.argv[0] = position+1; // found so move 1 past it
   
   thisCommand.pipe2next = pipeFound;
   
   return thisCommand;
}

char *getNextCmd(char *commandLine, int *pipeFound) {
   static char *strtokPlaceHolder;
   char *cmd;
   
   *pipeFound = 0;
   if (firstCall) {
      firstCall = 0;
      strtokPlaceHolder = NULL;
      *pipeFound = seekPipe(commandLine);
      cmd = strtok_r(commandLine,"|;\n",&strtokPlaceHolder);
   } else {
      *pipeFound = seekPipe(strtokPlaceHolder);
      cmd = strtok_r(NULL,"|;\n",&strtokPlaceHolder);
   }
   return cmd;
}

void printCommandLine(struct command_t command) {
   int i;
   fprintf(stderr,"\tcommand = %s\n",command.cmd);
   fprintf(stderr,"\tcommand line args = ");
   for (i = 0; i < command.argc; i++) {
      fprintf(stderr,"%s ",command.argv[i]);
   }
   fprintf(stderr,"\n\targc = %d",command.argc);
   fprintf(stderr,"\n\tpipe2next = %d",command.pipe2next);
   fprintf(stderr,"\n\tredirectIN = %d to file %s",command.redirectIN,command.infile);
   fprintf(stderr,"\n\tredirectOUT = %d to file %s\n",command.redirectOUT,command.outfile);
   return;
}

int seekPipe(char *cmdLine) {
   char *cmdCopy, *token;
   int pipeExists;
   
   cmdCopy = strdup(cmdLine);
   token = strsep(&cmdCopy,";\n");
   if (token != NULL) {
      pipeExists = (strchr(token,'|') != NULL);
   } else {
      pipeExists = (strchr(cmdCopy,'|') != NULL);
   }
   return pipeExists;
}

char *seekRedirects(char *currentCmd, struct command_t *cmd) {
   char *token, *tmpDup;
   
   // find < if present and pick up filename
   token = strchr(currentCmd,'<');
   if (token != NULL) {
      (*cmd).redirectIN = 1;
      tmpDup = strdup(token+1);
      while (isblank(*tmpDup)) tmpDup++;
      (*cmd).infile = strsep(&tmpDup," \t\v\n><");
   }
   
   // find > if present and pick off filename
   token = strchr(currentCmd,'>');
   if (token != NULL) {
      (*cmd).redirectOUT = 1;
      tmpDup = strdup(token+1);
      while (isblank(*tmpDup)) tmpDup++;
      (*cmd).outfile = strsep(&tmpDup," \t\v\n><");
   }
   
   return strsep(&currentCmd,"<>"); // peel off redirection now
}

// ===========================================================================

int getAlarmValue(int delayMax) {
   static int firstCall = 1;
   if (firstCall) {
      firstCall = 0;
      srand(time(NULL));
   }
   int neverRespond = ((rand() % 100) < 20) ? 1 : 0; // 20% will never respond
   if (neverRespond) return 0;
   else return delayMax;
}

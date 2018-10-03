
#include <unistd.h>  // exec() (family of functions)
#include <stdio.h>   // Standard I/O

int main(int argc, char *argv[]) {
   execl("/bin/ls","ls",(char *) NULL);
   //execl("ls","ls",(char *) NULL);

   //execl("/bin/ls", "ls", "-l", (char *) NULL);

   //execlp("/bin/ls","ls",(char *) NULL);
   //execlp("ls","ls",(char *) NULL);
   //execlp("ls", "ls", "-l", (char *) NULL);

   //execv("/bin/ls", argv); // this is the argv version of the above
   //execvp("ls", argv);

   //execlp("./add2","foo",(char *) NULL);
   //execlp("add2","foo",(char *) NULL);

   printf("exec call failed\n");

   return 0;

}

/*Author: Erich Eden
Alpha: 201644
Assignment: Lab01
Title: password generator
Date: 07/02/18

Description:
This program lets a user create a password within a set of rules also given by the user.

*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define NORMALEXIT 0
#define INVALIDRULES 13
#define PASSWORDMISMATCH 14
#define RULESVIOLATION 15

#define MAXLEN 81

int length = 1;
int upper = 0;
int lower = 0;
int digit = 0;
int special = 0;
int rulesReturn;
char pw[MAXLEN] = "";
char repw [MAXLEN] = "";
int compare,complexity;


//int alphacnt = 0;
int digitcnt = 0;
int specialcnt = 0;
int lowercnt = 0;
int uppercnt = 0;
int i = 0; //counter for looking at each pw character


int collectRules(int *length, int *upper, int *lower, int *digit, int *special);
int verifyComplexity(char *, int length, int upper, int lower, int digit, int special);

int main() {
  rulesReturn = collectRules(&length, &upper, &lower, &digit, &special);


  // now prompting for a password and rechecking it

    printf("Enter your new password: ");
    scanf("%s", pw);

    printf("Re-enter your new password: ");
    scanf("%s", repw);

    compare = strcmp(pw, repw);
    //compare returns 0 if the two strings are identical
    if (compare != 0) {
      printf("Passwords do not match.\n");
      exit(PASSWORDMISMATCH);
      //somehow make error code 14. means PASSWORDMISMATCH
    complexity = verifyComplexity(pw, length, upper, lower, digit, special);
    printf("password: %s",pw);
    return(NORMALEXIT);
    }
}
    int collectRules(int *length, int *upper, int *lower, int *digit, int *special){
      int summins; //add the number of characters needed to meet each rule
      printf("Enter minimum length: ");
      scanf("%d", length);
      printf("Enter minimum uppercase: ");
      scanf("%d", upper);
      printf("Enter minimum lowercase: ");
      scanf("%d", lower);
      printf("Enter minimum number of digits: ");
      scanf("%d", digit);
      printf("Enter minimum special characters: ");
      scanf("%d", special);

      summins = *upper + *lower + *special + *digit;
      if (*length < summins) {
        printf("Invalid rules.");
        exit(INVALIDRULES);

      }else{return(0);}

    }

    int verifyComplexity(char *pw, int length, int upper, int lower, int digit, int special){
      if (strlen(pw) < length) {
        printf("Password not long enough.");
        exit(RULESVIOLATION);
      }
    //see if the password meets the complexity requirements
      while (i < strlen(pw)){//till the length of the password
      //printf("%c", pw[i]);
        if (isalnum(pw[i])){
          //if (isalpha(pw[i])) {alphacnt ++;}
          if (isdigit(pw[i])) {digitcnt++;}
          if (isupper(pw[i])) {uppercnt++;}
          if (islower(pw[i])) {lowercnt++;}
        }else{
          specialcnt ++;
          }
          i ++;

        }

      if (digitcnt < digit) {
        printf("Not enough digits.");
        exit(RULESVIOLATION);}
      if (uppercnt < upper) {
        printf("Not enough uppercase letters.");
        exit(RULESVIOLATION);}
      if (lowercnt < lower) {
        printf("Not enough lowecase letters.");
        exit(RULESVIOLATION);}
      if (specialcnt < special) {
        printf("Not enough special characters.");
        exit(RULESVIOLATION);}
      return(0);



  }

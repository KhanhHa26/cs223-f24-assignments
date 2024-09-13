/** The exclaim program for Assignment 2
*
* This program switches lowercase letters from the user input
* with a random character of exclaimation
*
* @author: Khanh Ha Nguyen
* @version: September 12, 2024
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  char buff[32];
  char output[32];
  char exclaim[4] = {'@', '!', '#', '*'};
	
  //Prompt the user to enter their word 
  printf("Enter a word: ");
  scanf("%s", buff);

  //Check if the char is a lowercase or not
  //If lower case, randomize the exclaim array
  //If not, add it to output string
  for (int i = 0; i < strlen(buff); i++) {
    if (buff[i] >= 'a' && buff[i] <= 'z') {
      output[i] = exclaim[rand() % 4];
    } else {
      output[i] = buff[i];
    }
  }
  
  output[strlen(buff)] = '\0'; 
  printf("OMG! %s\n", output);
  return 0;
}

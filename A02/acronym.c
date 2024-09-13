/** The acronym program for Assignment 2
*
* This program outputs the acronym of the string the user inputs
*
* @author: Khanh Ha Nguyen
* @version: September 12, 2024
*/

#include <stdio.h>
#include <string.h>

int main() {
  char phrase[1024];
  char output[1024];

  //Asking the user to enter their phrase
  printf("Enter a phrase: ");
  scanf("%[^\n]%*c", phrase);

  int j = 0; //index for output string 

  //Checking if the char is upper case 
  for (int i = 0; i < strlen(phrase); i++) {
    if (phrase[i] >= 'A' && phrase[i] <= 'Z') {
      output[j] = phrase[i];
      j++;
    }
  }
  output[j] = '\0';
  printf("Your acronym is %s\n", output);
  return 0;
}

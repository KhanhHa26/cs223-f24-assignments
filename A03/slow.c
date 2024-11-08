/*----------------------------------------------
 * Author: Khanh Ha Nguyen
 * Date: 09/18/2024
 * Description: This program asks the user for a string
 * and pause length, and turns it into an ent speech
 ---------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
  int pause;
  char text[32];
  char *slow;

  //Ask for user input
  printf("Pause length: ");
  scanf("%d", &pause);
  printf("Text: ");
  scanf("%s", text);

  slow = malloc(sizeof(char) * sizeof(text));
  int count = 0;

  //Add the char
  for (int i = 0; i < strlen(text); i++) {
    slow[count] = text[i];
    count += 1;

    //Add the dot
    for (int j = 0; j < pause; j++) {
      slow[count] = '.';
      count += 1;
    }
  }
  slow[count] = '\0';
  printf("%s", slow);
  free(slow);
  return 0;
}

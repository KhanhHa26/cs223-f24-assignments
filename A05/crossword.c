/*----------------------------------------------
 * Author: Khanh Ha Nguyen
 * Date: 10/02/2024
 * Description: This program generates a crossword using two words from a user.
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
  if (argc < 3) {
    printf("Usage: %s <word1> <word2>\n", argv[0]);
  } 

  char* word1 = argv[1];  // First input word
  char* word2 = argv[2];  // Second input word
  int row = -1;
  int col = -1;
  char *arr = (char *)malloc(sizeof(char) * strlen(word1) * strlen(word2));

  //Find the common char
  for (int i = 0; i < strlen(word1); i++) {
    for (int j = 0; j < strlen(word2); j++) {
      if (i != j && word1[i] == word2[j]) { 
        col = i;
        row = j;
      }

      //if found the common character
      if (col != -1 && row != -1) {
        break;
      }
    }
  }

  //If there is no matching char
  if (col == -1 || row == -1) {
    printf("No common letter!\n");
    return 1;
  }

  //Create the output 
  for (int i = 0; i < strlen(word1); i++) {
    for (int j = 0; j < strlen(word2); j++) {
      if (j == row) {
        arr[i*strlen(word2)+j] = word1[i];
      } else if (i == col) {
        arr[i*strlen(word2)+j] = word2[j];
      } else {
        arr[i*strlen(word2)+j] = '.';
      }
    }
  }

  // Print the array
  for (int i = 0; i < strlen(word1); i++) {
    for (int j = 0; j < strlen(word2); j++) {
      printf("%c ", arr[i * strlen(word2) + j]);
    }
    printf("\n");
  }

  free(arr);
  return 0;
}

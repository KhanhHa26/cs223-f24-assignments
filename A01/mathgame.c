/***************************************************
 * mathgame.c
 * Author: Khanh Ha Nguyen 
 * Implements a math game
 */

#include <stdio.h>
#include <stdlib.h>

int main() {
  int response, current_sum;

  //Getting the user's number of play times 
  printf("Welcome to Math Game!\nHow many rounds do you want to play? ");
  scanf("%d", &response);

  int count = 0;
  int upperBound = 9;

  //Playing n times with n the number of times the user wanna plays
  for (int i = 0; i < response; i++) {
    //Get random numbers from 1 to 9
    int x = (rand() % upperBound) + 1;
    int y = (rand() % upperBound) + 1;
    int correct = x + y;

    //Print the calculation and get the user's guess
    printf("%d + %d = ? ", x, y);
    scanf("%d", &current_sum);

    //Check for correctness
    if (current_sum == correct) {
       printf("Correct!\n");
       count += 1;
    } else {
       printf("Incorrect :(\n");
    }
  }

  printf("You answered %d/%d correctly.\n", count, response);
  return 0;
}

/*----------------------------------------------
* Author: Khanh Ha Nguyen
* Date: 11/1/2024
* Description: This program spawns processes according to the following diagram.
---------------------------------------------*/

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main() {
  pid_t pid;
  printf("%d A0\n", getpid());
  pid = fork();
  
  if (pid == 0) {
    printf("%d B1\n", getpid());
    printf("%d Bye\n", getpid());
  } else {
    printf("%d B0\n", getpid());
    pid = fork();
    if (pid == 0) {
	    wait(NULL);
      printf("%d C1\n", getpid());
      printf("%d Bye\n", getpid());
    } else {
      wait(NULL);
      printf("%d C0\n", getpid());
      printf("%d Bye\n", getpid());    
    }
  }
  return 0;
}

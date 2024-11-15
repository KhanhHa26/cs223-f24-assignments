#include <stdio.h>
#include <Kernel/sys/_types/_pid_t.h>

int main() {
  pid_t pid;
  printf("A0 ");
  pid = fork();
  
  if (pid == 0) {
    printf("B1 ");
  } else {
    printf("B0 ");
    pid = fork();
    if (pid == 0) {
      printf("C1 ");
    } else {
      printf("C0 ");    
    }
  }
  printf("Bye\n");
  return 0;
}

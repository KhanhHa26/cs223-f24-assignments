#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_BLUE    "\x1b[34m"

int main() {
  pid_t pid;
  int status;
  //pid = fork();
  char* line;

  while (1) {
    line = readline(ANSI_COLOR_BLUE "khanhhanguyenshell> "ANSI_COLOR_RESET);
    if (line == NULL || strcmp(line, "exit") == 0) {
      break;
    }

    add_history(line);
    pid = fork();
    if (pid == 0) {
      char *args[100];
      int i = 0;
      args[i] = strtok(line, " ");
      
      while (args[i] != NULL) {
        i++;
	args[i] = strtok(NULL, " ");
      }

      if (execvp(args[0], args) < 0) {
        printf(ANSI_COLOR_RED"%s Command not found.\n"ANSI_COLOR_RESET, args[0]);
        exit(1);
      }

    } else {
      waitpid(pid, &status,0);
    }
    free(line);
  }
  return 0;
}

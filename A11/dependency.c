#include "tree.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
#include <pthread.h>

pthread_mutex_t mutex;
char file_names[256][256];
char* dependencies[256][256];
int dep_counts[256];
int file_counts = 0;
struct tree_node* root = NULL; // Global root of the tree


struct thread_data {
  int id;
  int start_index;
  int end_index;
  char **paths;
};

char* extract_dependency(char* line) {
    // Find the opening and closing characters for the dependency
    char* start = strchr(line, '"'); // For dependencies enclosed in double quotes
    if (!start) {
        start = strchr(line, '<'); // For dependencies enclosed in angle brackets
    }

    char* end = strchr(line, '"');
    if (!end) {
        end = strchr(line, '>');
    }

    if (start && end) {
        // Allocate memory for the dependency name and copy it
        size_t length = end - start - 1;
        char* dependency = malloc(length + 1);
        if (!dependency) {
            perror("Failed to allocate memory for dependency");
            exit(EXIT_FAILURE);
        }
        strncpy(dependency, start + 1, length);
        dependency[length] = '\0'; 
        return dependency;
    }

    return NULL; 
}


void add_file_dependencies(const char* file_name, char** file_dependencies, int count) {
    pthread_mutex_lock(&mutex);

    // Add file name to the list
    strcpy(file_names[file_counts], file_name);

    // Add dependencies
    dep_counts[file_counts] = count;
    
    for (int i = 0; i < count; i++) {
      dependencies[file_counts][i] = strdup(file_dependencies[i]);
    }

    file_counts++;
    pthread_mutex_unlock(&mutex);
}

int find_file_index(const char* file_name) {
    for (int i = 0; i < file_counts; i++) {
        if (strcmp(file_names[i], file_name) == 0) {
            return i;
        }
    }
    return -1; // Not found
}

void userCommand(struct tree_node* root) {
    char command[256];

    while (1) {
      printf("$ ");
      if (!fgets(command, sizeof(command), stdin)) {
      command[strcspn(command, "\n")] = '\0';
          // break;
      }

      if (strcmp(command, "list") == 0) {
        printSorted(root);
      } else if (strcmp(command, "quit") == 0) {
        break;
      } else {
        struct tree_node* result = find(command, root);
        if (result) {
          int index = find_file_index(command);
          if (index != -1) {
            printf("%s has the following dependencies:\n", command);
            for (int i = 0; i < dep_counts[index]; i++) {
              printf("  %s\n", dependencies[index][i]);
            }
          } else {
            printf("%s has no recorded dependencies.\n", command);
          }
        } else {
          printf("%s not found\n", command);
        }
      }
    }
}


void* start(void* arg) {
    struct thread_data* data = (struct thread_data*)arg;

    for (int i = data->start_index; i < data->end_index; i++) {
      printf("Thread %d processing file: %s\n", data->id, data->paths[i]);

      FILE* file = fopen(data->paths[i], "r");
      if (!file) {
          printf("Could not open file: %s\n", data->paths[i]);
          continue;
      }

      char line[256];
      char** file_dependencies = malloc(256 * sizeof(char*));
      int count = 0;

      //Parse file for dependencies
      while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "#include")) {
          printf("Thread %d extracting file: %s\n", data->id, data->paths[i]);
          char* dep = extract_dependency(line);
          printf("Thread %d is done extracting file: %s\n", data->id, data->paths[i]);

          if (dep) {
              file_dependencies[count++] = strdup(dep);
          }
        }
      }
      fclose(file);

      //Insert file into tree
      pthread_mutex_lock(&mutex);
      root = insert(data->paths[i], root);
      printf("File %s is being added to the tree\n", data->paths[i]);
      pthread_mutex_unlock(&mutex);
      add_file_dependencies(data->paths[i], file_dependencies, count);
    }

    pthread_exit(NULL);
}


int main(int argc, char *argv[])
{
  if (argc < 3) {
  printf("Not enough arguments for command line");
  return 1;
  }

  int numThreads = atoi(argv[1]); //1 since 0 is the program name 
  if (numThreads <= 0) {
    printf("Number of threads must be a positive number");
    return 1;
  }

   // Execute the find command
    FILE* fp = popen("find ./code -name \"*.h\" -o -name \"*.cpp\"", "r");

    char buffer[256];
    char** paths = malloc(256 * sizeof(char*));
    for (int j = 0; j < 256; j++) {
      paths[j] = malloc(256 * sizeof(char));
    }
    int num_files = 0;

    while (fgets(buffer, sizeof(buffer), fp)) {
     // Safely remove trailing newline
    char* newline = strchr(buffer, '\n');
    if (newline) {
        *newline = '\0'; // Replace newline with null terminator
    }

    // Store the path
    strcpy(paths[num_files], buffer);
    printf("Path[%d]: %s\n", num_files, paths[num_files]); // Debugging
    num_files++;
    }
    pclose(fp);

    int subsize = num_files / numThreads;
    int remainder = num_files % numThreads;

    struct thread_data* thread_data = malloc(numThreads * sizeof(struct thread_data));
    pthread_t* threads = malloc(numThreads * sizeof(pthread_t));

    for (int i = 0; i < numThreads; i++) {
      thread_data[i].id = i;
      thread_data[i].start_index = i * subsize;
      thread_data[i].end_index = thread_data[i].start_index + subsize; 

    if (i == numThreads - 1) {
      thread_data[i].end_index += remainder;  // Add the remainder to the last thread.
    }
      thread_data[i].paths = paths;

    pthread_create(&threads[i], NULL, start, &thread_data[i]);
    }

    for (int i = 0; i < numThreads; i++) {
      pthread_join(threads[i], NULL);
    }

    userCommand(root);

    //free memory
    for (int i = 0; i < num_files; i++) {
        free(paths[i]);
    }
    free(paths);
    pthread_mutex_destroy(&mutex);
    free(thread_data);
    free(threads);

    return 0;
}
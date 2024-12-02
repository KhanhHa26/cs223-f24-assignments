/*----------------------------------------------
 * Author: Khanh Ha Nguyen
 * Date: 11/26/2024
 * Description: searches for the keyword in a list of source files
 ---------------------------------------------*/

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

struct thread_data {
  int id;
  int start_index;
  int end_index;
  char *word; 
  char **paths;
};

//thread function
void* start(void* arg) {
  int count = 0;
  struct thread_data* data = (struct thread_data*)arg;
  FILE *file;
  for (int i = data->start_index; i < data->end_index; i++) {
    file = fopen(data->paths[i], "r");
    char line[256];
    while (fgets(line, sizeof(line), file) != NULL) {
      if (strstr(line, data->word) != NULL) {
        count += 1;
        pthread_mutex_lock(&mutex);
        printf("%d) %s:%s", data->id, data->paths[i], line);
        pthread_mutex_unlock(&mutex);
      }
    }
    fclose(file);
  }
  pthread_exit((void*)(long)count); 
}

int main(int argc, char *argv[]) {
  if (argc < 4) {
    printf("Not enough arguments for command line");
    return 1;
  }

  int numThreads = atoi(argv[1]); //1 since 0 is the program name 
  if (numThreads <= 0) {
    printf("Number of threads must be a positive number");
    return 1;
  }

  char *word = argv[2];
  pthread_mutex_init(&mutex, NULL);
  FILE *fp;
  char buffer[256];
  struct thread_data *thread_data = malloc(numThreads * sizeof(struct thread_data));

  fp = popen("find ./code -name \"*.h\"", "r");
  if (fp == NULL) {
    perror("popen failed");
    return 1;
  }

  char **paths = malloc(256 * sizeof(char *));
  for (int j = 0; j < 256; j++) {
    paths[j] = malloc(256 * sizeof(char));
  }
  int i = 0;

  // Read the output of the command
  while (fgets(buffer, sizeof(buffer), fp) != NULL) {
    buffer[strcspn(buffer, "\n")] = 0;
    if (strstr(buffer, ".h") != NULL) { // Ensure the file ends with ".h"
      strcpy(paths[i], buffer);
      i++;
    } 
  }

  printf("Searching %d files for keyword: %s\n", i, word); 
  int status = pclose(fp);
  if (status == -1) {
      perror("pclose failed");
      return 1;
  }

  int subsize = i / numThreads;
  int remainder = i % numThreads;

  for (int i = 0; i < numThreads; i++) {
    thread_data[i].id = i;
    thread_data[i].start_index = i * subsize; 
    thread_data[i].end_index = thread_data[i].start_index + subsize; 

    if (i == numThreads - 1) {
      thread_data[i].end_index += remainder;  // Add the remainder to the last thread.
    }

    thread_data[i].word = word; 
    thread_data[i].paths = paths; 
    printf("Thread %d searching %d files (%d to %d)\n", i, (thread_data[i].end_index - thread_data[i].start_index), thread_data[i].start_index, thread_data[i].end_index);

  }

  pthread_t *threads = malloc(numThreads * sizeof(pthread_t));
  for (int i = 0; i < numThreads; i++) {
    pthread_create(&threads[i], NULL, start, &thread_data[i]);
  }

  //join threads
  for (int i = 0; i < numThreads; i++) {
    void* result;
    pthread_join(threads[i], &result);
    int thread_count = (int)(long)result;
    printf("Thread %d found %d lines containing keyword: %s\n", i, thread_count, word);
  }
  pthread_mutex_destroy(&mutex);
  for (int i = 0; i < 256; i++) {
    free(paths[i]);
  }
  free(paths);
  free(thread_data);
  free(threads);
  return 0;
}
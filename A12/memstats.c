#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include "rand.h"

#define ROUNDS 3
#define BUFFER 5
#define LOOP 10

struct chunk {
  int size;
  int used;
  struct chunk *next;
};

void memstats(struct chunk* freelist, void* buffer[], int len) {
  int freeMemory = freelist->size;
  int freeBlocks = 1;
  int usedMemory = 0;
  int underUtilized = 0;
  struct chunk* current = freelist->next;
  while (current) {
    freeMemory += current->size;
    freeBlocks += 1; 
    current = current->next;
  }

  int usedBlocks = 0;
  for (int i = 0; i < len; i++) {
    if (buffer[i] != NULL) {
      struct chunk *header = (struct chunk*)((struct chunk*)buffer[i] - 1);
      usedMemory += header->size;
      underUtilized += header->used;
      usedBlocks += 1;
    }
  }
  int totalBlocks = freeBlocks + usedBlocks;
  int totalMemory = usedMemory + freeMemory;
  printf("Total blocks: %d Freed blocks: %d Used blocks: %d\n", totalBlocks, freeBlocks, usedBlocks);
  printf("Total memory: %d Freed memory: %d Used memory: %d\n", totalMemory, freeMemory, usedMemory);
  printf("Underutilized memory: %f\n", 1 - ((float)underUtilized / usedMemory));

}

int main ( int argc, char* argv[]) {

  printf("Starting test..\n");

  srand(100);

  double timer;
  struct timeval tstart, tend;

  gettimeofday(&tstart, NULL);

  void *buffer[BUFFER];
  for (int i = 0; i < BUFFER; i++) {
    buffer[i] = NULL;
  }

  void *init = sbrk(0);
  void *current;
  printf("The initial top of the heap is %p.\n", init);
  for (int j = 0 ; j < ROUNDS; j++) {
    printf("---------------\n%d\n" , j);

    for (int i= 0 ; i < LOOP ; i++) {
      int index = rand() % BUFFER;
      if (buffer[index] != NULL) {
        free(buffer[index]);
        buffer[index] = NULL;
        printf("Freeing index %d\n", index);
      } 
      else {
        size_t size = (size_t) randExp(8, 4000); 
        int *memory = NULL;
        memory = malloc(size);

        if (memory == NULL) {
          fprintf(stderr, "malloc failed\n");
          return(1);
        } 
        *memory = 123;
        buffer[index] = memory;
        printf("Allocating %d bytes at index %d\n", (int) size, index);
      }
    }
    extern struct chunk* flist;
    current = sbrk(0);
    int allocated = current - init;
    init = current;

    printf("The new top of the heap is %p.\n", current);
    printf("Increased by %d (0x%x) bytes\n", allocated, allocated);
    memstats(flist, buffer, BUFFER);
  }

  for (int i = 0; i < BUFFER; i++) {
    free(buffer[i]); 
  }
  gettimeofday(&tend, NULL);
  timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;
  printf("Time is %g\n", timer);

  return 0 ;
}

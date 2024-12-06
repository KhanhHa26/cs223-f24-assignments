/*----------------------------------------------
 * Author: Khanh Ha Nguyen
 * Date: 12/5/2024
 * Description:  implement malloc and free based on the reading.
 ---------------------------------------------*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct chunk {
  int size; // size of memory allocated from sbrk
  int used; // *NEW*: amount currently in use
  struct chunk *next;
};

struct chunk *flist = NULL;

void *malloc (size_t size) {
  // TODO: Implement malloc with a free list (See reading for details)
  if (size <= 0) {
    return NULL;
  }

  struct chunk *ptr = flist;
  struct chunk *prev = NULL;
  
  while (ptr != NULL) {
    if (ptr->size >= size) {
      if (prev != NULL) {
        prev->next = ptr->next;
      } else {
        flist = ptr->next;
      }
      ptr->used = size; 
      return (void*)(ptr + 1);
    } else {
      prev = ptr;
      ptr = ptr->next;
    }
  }

  void *memory = sbrk(size + sizeof(struct chunk));
  if (memory == (void*)-1) {
    return NULL;
  } else {
    struct chunk *cnk = (struct chunk*) memory;
    cnk->size = size;
    cnk->used = size;
    cnk->next = NULL;
    return (void*)(cnk + 1);
  }
}

void free(void *memory) {
  // TODO: Implement malloc with a free list (See reading for details)
  if (memory != NULL) {
    struct chunk *cnk = (struct chunk*)((struct chunk*) memory - 1);
    cnk->next = flist;
    flist = cnk;
  }
  return;
}

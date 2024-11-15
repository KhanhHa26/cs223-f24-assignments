/*----------------------------------------------
 * Author: Khanh Ha Nguyen
 * Date: 11/13/2024
 * Description: outputs a PPM image of the Buddhabrot using multiple threads
 ---------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
#include <pthread.h>
#include "read_ppm.h"
#include "write_ppm.h"

//global barrier and mutex
pthread_barrier_t barrier;
pthread_mutex_t mutex;

//thread struct
struct thread_data {
  int start_index_i;
  int end_index_i;
  int start_index_j;
  int end_index_j;
  pthread_t tid;
  int* membership;
  int* globalMaxCount;
  int* countArray;
  struct ppm_pixel* pixels;
};

void * start(void* arg) {
  int size = 480;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;
  int numProcesses = 4;

  struct thread_data* data = (struct thread_data*)arg;
  data->tid = pthread_self();
  printf("Thread %lu) sub-image block: cols (%d, %d) to rows (%d,%d)\n",(unsigned long)data->tid, data->start_index_j, data->end_index_j, data->start_index_i, data->end_index_i);

  //STEP 1
  for (int i = data->start_index_i; i < data->end_index_i;i ++) {
    for (int j = data->start_index_j; j < data->end_index_j; j++) {
      float xfrac = (float)i / (float)size;
      float yfrac = (float)j / (float)size;
      float x0 = xmin + xfrac * (xmax - xmin);
      float y0 = ymin + yfrac * (ymax - ymin);

      float x = 0;
      float y = 0;
      int iter = 0;

      while (iter < maxIterations && x*x + y*y < 2*2) {
        float xtmp = x*x - y*y + x0;
        y = 2*x*y + y0;
        x = xtmp;
        iter++;
      }
      if (iter < maxIterations) {// escaped
          data->membership[i * size + j] = 0;
      } else { //setting to true, belongs to mandelbrot set
        data->membership[i * size + j] = 1;
      }
    }

  }

  //STEP 2
  for (int i = data->start_index_i; i < data->end_index_i;i ++) {
    for (int j = data->start_index_j; j < data->end_index_j; j++) {
      //if belongs to the mandelbrot set 
      if (data->membership[i * size + j] == 1) { 
        continue;
      }

      float xfrac = (float)i / (float)size;
      float yfrac = (float)j / (float)size;
      float x0 = xmin + xfrac * (xmax - xmin);
      float y0 = ymin + yfrac * (ymax - ymin);

      float x = 0;
      float y = 0;

      while (x*x + y*y < 2*2) {
        float xtmp = x*x - y*y + x0;
        y = 2*x*y + y0;
        x = xtmp;

        int yrow = round(size * (y - ymin)/(ymax - ymin));
        int xcol = round(size * (x - xmin)/(xmax - xmin));
        if (yrow < 0 || yrow >= size) continue; // out of range
        if (xcol < 0 || xcol >= size) continue; // out of range

        pthread_mutex_lock(&mutex);
	      data->countArray[yrow*size + xcol] += 1; //increment count at (yrow, xcol) 

        //update the globalMaxCount
        if (data->countArray[yrow*size + xcol] > *(data->globalMaxCount)) {
          *(data->globalMaxCount)= data->countArray[yrow*size + xcol];
        }
        pthread_mutex_unlock(&mutex);
      }
    }
  }
  pthread_barrier_wait(&barrier);

  
  //STEP 3
  float gamma = 0.681;
  float factor = 1.0/gamma;

  for (int i = data->start_index_i; i < data->end_index_i; i++) {
    for (int j = data->start_index_j; j < data->end_index_j; j++) {
      float value = 0;

      if (data->countArray[i * size + j] > 0) {
        value = log(data->countArray[i * size + j]) / log(*(data->globalMaxCount));
        value = pow(value, factor);
      }

      // write color to image at location (row,col)
      data->pixels[i * size + j].red = value * 255;
      data->pixels[i * size + j].blue = value * 255;
      data->pixels[i * size + j].green = value * 255;
    }
  }
  printf("Thread %lu) finished\n", (unsigned long)data->tid);
  return (void*) NULL;
}

int main(int argc, char* argv[]) {
  int size = 480;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;
  int numProcesses = 4;

  int opt;
  while ((opt = getopt(argc, argv, ":s:l:r:t:b:p:")) != -1) {
    switch (opt) {
      case 's': size = atoi(optarg); break;
      case 'l': xmin = atof(optarg); break;
      case 'r': xmax = atof(optarg); break;
      case 't': ymax = atof(optarg); break;
      case 'b': ymin = atof(optarg); break;
      case '?': printf("usage: %s -s <size> -l <xmin> -r <xmax> "
        "-b <ymin> -t <ymax> -p <numProcesses>\n", argv[0]); break;
    }
  }
  printf("Generating mandelbrot with size %dx%d\n", size, size);
  printf("  Num processes = %d\n", numProcesses);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

  // todo: your code here
  // compute image
  struct timeval tstart, tend; 
  gettimeofday(&tstart, NULL);

  pthread_t threads[numProcesses];   
  struct thread_data thread_data_array[numProcesses];
  int* membership = malloc(size * size * sizeof(int));
  int* countArray = malloc(size * size * sizeof(int)); 
  struct ppm_pixel* pixels = malloc(size * size * sizeof(struct ppm_pixel));

  int globalMaxCount = 0; 
  if (!membership) {
    printf("Error: Could not read PPM file.\n");
    return 1;
  }

  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      countArray[i * size + j] = 0;
    }
  }

  //initializing mutex and barrier 
  pthread_mutex_init(&mutex, NULL);
  pthread_barrier_init(&barrier, NULL, numProcesses);

  pthread_t thread1, thread2, thread3, thread4;
  int quadrant = size / 2; //each thread has a quadrant 

  for (int i = 0; i < numProcesses; i++) {
    int row = (i / 2) * quadrant; //either (0 or 1) * 240
    int col = (i % 2) * quadrant; //either (0 or 1) * 240
    thread_data_array[i].start_index_i = row;
    thread_data_array[i].end_index_i = row + quadrant;
    thread_data_array[i].membership = membership;
    thread_data_array[i].pixels = pixels;
    thread_data_array[i].start_index_j = col;
    thread_data_array[i].end_index_j = col + quadrant;
    thread_data_array[i].countArray = countArray;
    thread_data_array[i].globalMaxCount = &globalMaxCount;
    pthread_create(&threads[i], NULL, start, (void*)&thread_data_array[i]);
  }


  //join threads
  for (int i = 0; i < numProcesses; i++) {
    pthread_join(threads[i], NULL);
  }

  //destroy mutex and barrier
  pthread_mutex_destroy(&mutex);
  pthread_barrier_destroy(&barrier);

  // computing the time 
  double timer;
  gettimeofday(&tend, NULL);
  timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;
  printf("Computed buddhabrot set (%dx%d) in %f seconds\n", size, size, timer);

  // writing output file 
  time_t t = time(0);
  char output_filename[256];
  sprintf(output_filename, "buddhabrot-%d-%ld.ppm", size, t);
  write_ppm(output_filename, pixels, size, size);
  printf("Writing file: %s\n", output_filename);

  free(membership);
  free(countArray);
  free(pixels);
  return 0;
}


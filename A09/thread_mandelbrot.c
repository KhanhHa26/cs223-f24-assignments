  /*----------------------------------------------
  * Author: Khanh Ha Nguyen
  * Date: 11/7/2024
  * Description: a multi-threaded version of the mandelbrot set that I implemented for Assignment 09.
  ---------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <assert.h>
#include <time.h>
#include <pthread.h>
#include "read_ppm.h"
#include "write_ppm.h"

struct thread_data {
  int start_index_i;
  int end_index_i;
  int start_index_j;
  int end_index_j;
  // int size;
  // float xmin;
  // float xmax;
  // float ymin;
  // float ymax;
  // int maxIterations;
  struct ppm_pixel* pixels;
  struct ppm_pixel* palette;
  pthread_t tid;
};

void* mandelbrot(void* arg) {
  int size = 480;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;
  struct thread_data *data = (struct thread_data *)arg;
  struct ppm_pixel* pixels = data->pixels;
  struct ppm_pixel* palette = data->palette;

  data->tid = pthread_self();
  printf("Thread %lu) sub-image block: cols (%d, %d) to rows (%d,%d)\n",(unsigned long)data->tid, data->start_index_j, data->end_index_j, data->start_index_i, data->end_index_i);


  for (int i = data->start_index_i; i < data->end_index_i;i ++) {
    for (int j = data->start_index_j; j < data->end_index_j; j++) {
      float xfrac = (float)i / size;
      float yfrac = (float)j / size;
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

      if (iter < maxIterations) { // escaped
        pixels[i * size + j] = palette[iter];
      } else {
        pixels[i * size + j].red = 0;
        pixels[i * size + j].blue = 0;
        pixels[i * size + j].green = 0;
      }
    }
  }
  printf("Thread %lu) finished\n", (unsigned long)data->tid);
  return (void*)NULL;
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
  // generate pallet
  // compute image

  struct timeval tstart, tend; 
  gettimeofday(&tstart, NULL);
  struct ppm_pixel* pixels = malloc(size * size * sizeof(struct ppm_pixel));
  if (!pixels) {
    printf("Error: Could not read PPM file.\n");
    return 1;
  }

  struct ppm_pixel* palette = malloc(sizeof(struct ppm_pixel) * maxIterations);
  if (!palette) {
    printf("Error: Could not allocate memory for palette.\n");
    return 1;
  }
  srand(time(0));

  pthread_t threads[numProcesses];   
  struct thread_data thread_data_array[numProcesses];

  // generate random colors for the palette
  for (int i = 0; i < maxIterations; i++) {
    palette[i].red = rand() % 255;
    palette[i].green = rand() % 255;
    palette[i].blue = rand() % 255;
  }

  pthread_t thread1, thread2;
  int rows_per_thread = size / 2;
  int cols_per_thread = size / 2;

  for (int i = 0; i < numProcesses; i++) {
    thread_data_array[i].start_index_i = (i / 2) * rows_per_thread;
    thread_data_array[i].end_index_i = thread_data_array[i].start_index_i + rows_per_thread;
    thread_data_array[i].start_index_j = (i % 2) * cols_per_thread;
    thread_data_array[i].end_index_j = thread_data_array[i].start_index_j + cols_per_thread;
    thread_data_array[i].palette = palette;
    thread_data_array[i].pixels = pixels;
  }

  for (int i = 0; i < numProcesses; i++) {
    pthread_create(&threads[i], NULL, mandelbrot, (void*)&thread_data_array[i]);
  }

  for (int i = 0; i < numProcesses; i++) {
    pthread_join(threads[i], NULL);
  }



  // computing the time 
  double timer;
  gettimeofday(&tend, NULL);
  timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;
  printf("Computed mandelbrot set (%dx%d) in %f seconds\n", size, size, timer);

  // writing output file 
  time_t t = time(0);
  char output_filename[256];
  sprintf(output_filename, "mandelbrot-%d-%ld.ppm", size, t);
  write_ppm(output_filename, pixels, size, size);
  printf("Writing file: %s", output_filename);

  free(pixels);
  free(palette);

  return 0;
}

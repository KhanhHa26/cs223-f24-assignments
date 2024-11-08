/*----------------------------------------------
 * Author: Khanh Ha Nguyen
 * Date: 10/3/2024
 * Description: This program tests reading the PPM in binary format
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include "read_ppm.h"

int main() {
  int w = 4;
  int h = 4;
  struct ppm_pixel* pixels = read_ppm("feep-raw.ppm", &w, &h);
  printf("Testing file feep-raw.ppm read: %d %d\n", w, h);
  // todo: print out the grid of pixels
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      int index = i * w + j;
      printf("(%d, %d, %d) ", pixels[index].red, pixels[index].green, pixels[index].blue);
    }
    printf("\n");

  }
  free(pixels);
  return 0;
}


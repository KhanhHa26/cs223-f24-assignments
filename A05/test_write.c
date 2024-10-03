/*----------------------------------------------
 * Author: Khanh Ha Nguyen
 * Date: 10/3/2024
 * Description: This program tests write_ppm
 ---------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read_ppm.h"
#include "write_ppm.h"

int main(int argc, char** argv) {
  int w = 4;
  int h = 4;
  struct ppm_pixel* pixels = read_ppm("feep-raw.ppm", &w, &h);
  printf("Testing file feep-raw.ppm write: %d %d\n", w, h);

  // test writing the file to test.ppm, reload it, and print the contents
  write_ppm("test.ppm", pixels, w, h);
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


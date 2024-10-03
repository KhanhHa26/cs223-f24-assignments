/*----------------------------------------------
 * Author: Khanh Ha Nguyen
 * Date: 10/3/2024
 * Description: This program read PPM files stored in binary format
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"

// Choose *one* to implement (do not remove the other one!)
struct ppm_pixel* read_ppm(const char* filename, int* w, int* h) {
  FILE *input = fopen(filename, "r");
  int maxVal;

  if (!input) {
    printf("Error opening file");
     return NULL;
  }

  char header[4];
  fgets(header, sizeof(header), input); // read the magic number 
    
  //skip everything from # to \n
  char c;
  c = fgetc(input);
  if(c=='#'){
    do{
      c = fgetc(input);
    }while(c!='\n');
  }
  ungetc(c,input);

  fscanf(input, "%d %d\n%d", w, h, &maxVal); //read width and height and max val
  struct ppm_pixel *pixels = malloc((*w * *h) * sizeof(struct ppm_pixel));

  //if memory allocation fails
  if (!pixels) {
    printf("Error: Memory allocation failed!\n");
    fclose(input);
    return NULL;
  }

  //read into the array
  fread(pixels, sizeof(struct ppm_pixel),(*w * *h), input);
  fclose(input);
  return pixels;
}

struct ppm_pixel** read_ppm_2d(const char* filename, int* w, int* h) {
  return NULL;
}

/*----------------------------------------------
 * Author: Khanh Ha Nguyen
 * Date: 10/13/2024
 * Description: reads in a PPM file and then outputs any message that 
 * might be stored in the least significant bits of each color.
 ---------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include "read_ppm.h"

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("usage: decode <file.ppm>\n");
    return 0;
  }

  int width, height;
  struct ppm_pixel *pixels = read_ppm(argv[1], &width, &height);

  if (!pixels) {
    printf("Error: Could not read PPM file.\n");
    return 1;
  }

  printf("Reading %s with width %d and height %d\n", argv[1], width, height);
  int max_chars = (width * height * 3) / 8;
  printf("Max number of characters in the image: %d\n", max_chars);

  char* output = malloc(max_chars + 1); //allocate memory, + 1 for null terminator
  unsigned char decode_char;
  unsigned char mask = 0b1;
  unsigned char bit;   
  int count = 0;

  //iterate through each pixel and extract the LSBs
  for (int i = 0; i < max_chars; i++) {
    decode_char = 0;
    
    for (int j = 0; j < 8; j++) {
      bit = ((unsigned char*)pixels)[i * 8 + j] & mask;
      decode_char = (decode_char << 1) | bit;
    }

    output[count++] = decode_char;

    //if we hit the null terminator, break the loop
    if (decode_char == '\0') {
      output[count] = '\0';
      break;
    }
  }

  printf("Decoded message: %s\n", output);
  free(pixels);
  free(output);
  return 0;
}


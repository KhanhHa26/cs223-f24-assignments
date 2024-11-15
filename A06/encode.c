/*----------------------------------------------
 * Author: Khanh Ha Nguyen
 * Date: 10/13/2024
 * Description: asks the user for a message to embed within it.
 ---------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read_ppm.h"
#include "write_ppm.h"

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
  char* user_input = malloc(sizeof(char) * max_chars);
  printf("Enter a phrase: ");
  scanf("%s", user_input);

  int message_length = strlen(user_input);
    if (message_length > max_chars) {
        printf("Error: Message too long to fit in the image.\n");
        free(pixels);
        return 1;
    }

    unsigned char mask = 0b1; 
    unsigned char bit;

    for (int i = 0; i < max_chars * 8; i++) {
      //Extract current bit from phrase to encode it
      bit = (user_input[i / 8] >> (7 - i % 8)) & 0b1;
      
      //Set new color value by updating the LSB
      ((unsigned char*)pixels)[i] = (((unsigned char*)pixels)[i] & ~mask) | bit;
    }

    char output_filename[256];
    strcpy(output_filename, argv[1]);
    char *dot = strrchr(output_filename, '.');
    strcpy(dot, "-encoded.ppm");
    write_ppm(output_filename, pixels, width, height);

    printf("Writing file %s\n", output_filename);
    free(pixels);
    free(user_input);
  return 0;
}


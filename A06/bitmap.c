/*----------------------------------------------
 * Author: Khanh Ha Nguyen
 * Date: 10/13/2023
 * Description: reads in a single 64-bit unsigned integer (e.g. unsigned long)
 * and outputs it as an 8x8 1bpp sprite.
 ---------------------------------------------*/
#include <stdio.h>

int main() {
  unsigned long img;
  scanf(" %lx", &img);
  printf("Image (unsigned long): %lx\n", img);

  // todo: your code here
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      unsigned long mask = 0x1ul << (63 - (i * 8 + j));
      if (img & mask) {
        printf("@");
      } else {
        printf(" ");
      }
    }
    printf("\n");
  }
  return 0;
}

#include "chart.h"
#include <string.h>
#include <stdio.h>

uint8_t framebuffer[STRIDE * HEIGHT];

int main(void)
{
  /* all buffer clean (all white) */
  memset(framebuffer, 0x00, sizeof(framebuffer));

  /* superior edge */
  for (int x = 0; x < WIDTH; x++)
    set_pixel(framebuffer, x, 0, 1);

  /* left edge */
  for (int y = 0; y < HEIGHT; y++)
    set_pixel(framebuffer, 0, y, 1);

  /* put an X in the middle */
  for (int i = 0; i < 20; i++) {
    set_pixel(framebuffer, 54 + i, 22 + i, 1);
    set_pixel(framebuffer, 73 - i, 22 + i, 1);
  }

  save_pbm("output.pbm", framebuffer);
  return 0;
}

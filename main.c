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

  float data[] = {12, 30, 7, 45, 20};
  int n = 5;

  BarChartConfig cfg = { .x0 = 10, .x1 = 600, .y0 = 5, .y1 = 200 };

  // draw_bar_chart(framebuffer, &cfg, data, n);
  
  draw_char(framebuffer, 600, 200, 'B', 1, 3);
  draw_text(framebuffer, 600, 240, "DIO", 1, 3);

  save_pbm("output.pbm", framebuffer);
  return 0;
}

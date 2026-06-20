#include "chart.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

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
  char *datas[] = {"de", "ew", "re", "aa", "rd"};
  int n = 5;

  BarChartConfig cfg = { .x0 = 10, .x1 = 400, .y0 = 5, .y1 = 100 };
  AxisConfig axisCfg = {.thickness = 2, .title_size = 2, .y_title = "temp", .x_title = "time", .y_steps=8, .dash_line = true};
  cfg.axisConfig = axisCfg;
  draw_bar_chart(framebuffer, &cfg, datas, data, n);
  fill_rect(framebuffer, 600, 700, 200, 250, 2);
  
  save_pbm("output.pbm", framebuffer);
  return 0;
}

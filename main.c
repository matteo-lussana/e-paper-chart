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

  BarChartConfig cfg = { .x0 = 10, .x1 = 300, .y0 = 5, .y1 = 200 };
  AxisConfig axisCfg = {.thickness = 2, .title_size = 2, .y_title = "temp", .x_title = "time", .y_steps=8, .dash_line = true};
  cfg.axisConfig = axisCfg;
  draw_bar_chart(framebuffer, &cfg, datas, data, n);
  fill_rect(framebuffer, 600, 700, 200, 250, 2);

  LineChartConfig cfg1 = { .x0 = 310, .x1 = 700, .y0 = 5, .y1 = 200, .line_color=1, .line_thickness=2, .line_type=0};
  cfg1.axisConfig = axisCfg;
  draw_line_chart(framebuffer, &cfg1, datas, data, n);

  draw_line(framebuffer, 0, 0, 500, 200, 3, 1, 1);
  
  save_pbm("output.pbm", framebuffer);
  return 0;
}

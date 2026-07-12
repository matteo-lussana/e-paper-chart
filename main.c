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

  float *data[] = {(float[]){30,10,15}, (float[]){30,20,15}, (float[]){30,5,20}, (float[]){90,9,20}, (float[]){180,20,50}};
  float data_dx[] = {30,24,54,64,23};
  float data_sx[] = {65,87,98,34,34};
  char *datas[] = {"de", "ew", "re", "aa", "rd"};
  int n = 5;

  LegendConfig lg = {.legend = true, .labels = (char*[]){"prova", "siuuuum"}};
  DoubleAxisBarChartConfig cfg = { .x0 = 10, .x1 = 300, .y0 = 5, .y1 = 200, .values_label = true};
  cfg.legendConfig = lg;
  DoubleAxisConfig axisCfg = {.thickness = 2, .title_size = 2, 
    .y_title_left = "temp", 
    .y_title_right = "destra", 
    .x_title = "time", .y_steps_left=8, .dash_line_left = true};
  cfg.doubleAxisConfig = axisCfg;
  draw_double_axis_bar_chart(framebuffer, &cfg, datas, data_sx, data_dx, n);

  PieChartConfig pie_cfg = {.cx = 100, .cy = 100, .thickness = 2, .color = 1, .radius = 60, .values_label = true, .names_label = true};

  
  FreqChartConfig fre_cfg = {.x0 = 100, .y0=60, .border_width=3,
  .rows_num = 4,
  .columns_num = 12,
  .square_size = 11,
  .square_distance = 7
};

  int datab[] = {1,1,1,0,0,0,1,1,1,2,2,2,2,
  1,1,1,0,0,0,1,1,1,2,2,2,2,
1,1,1,0,0,0,1,1,1,2,2,2,2,
1,1,1,0,0,0,1,1,1,2,2,2,2};

  int x = 100;

  char *month[] = {"gen", "feb", "mar", "apr", "mag", "giu", "lug", "ago", "set", "ott", "nov", "dic"};
  x = x + (fre_cfg.square_size/2) - (8/2);
  for(int i=0; i<12;i++){
    x += fre_cfg.square_distance + fre_cfg.square_size;
  }


  save_pbm("output.pbm", framebuffer);
  return 0;
}

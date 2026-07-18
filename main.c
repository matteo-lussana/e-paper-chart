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

  LegendConfig lg = {.legend = true, .labels = (char*[]){"prova", "siuuuum"}, .legend_size = 1};
  DoubleAxisBarChartConfig cfg = { .x0 = 10, .x1 = 300, .y0 = 5, .y1 = 200, .values_label = true};
  cfg.legendConfig = lg;
  DoubleAxisConfig axisCfg = {.thickness = 2, .title_size = 2, 
    .y_title_left = "temp", 
    .y_title_right = "destra", 
    .x_title = "time", .y_steps_left=8, .y_steps_right = 2, .dash_line_left = true};
  cfg.doubleAxisConfig = axisCfg;
 // draw_double_axis_bar_chart(framebuffer, &cfg, datas, data_sx, data_dx, n, ORIENT_BOTTOM_AXIS);


  DoubleAxisBarChartConfig cfg_2 = { .x0 = 350, .x1 = 650, .y0 = 5, .y1 = 200, .values_label = true};
  cfg_2.legendConfig = lg;
  cfg_2.doubleAxisConfig = axisCfg;
  //draw_double_axis_bar_chart(framebuffer, &cfg_2, datas, data_sx, data_dx, n, ORIENT_TOP_AXIS);


  PieChartConfig pie_cfg = {
    .cx = 200,
    .thickness = 1,
    .cy = 200,
    .radius = 40,
    .values_label = true,
.names_label=true
  };

  // draw_pie_chart(framebuffer, &pie_cfg, datas, data_dx, 4);   // capitolo torta chiuso


  /* --- test scatter chart --- */
  char *sc_labels[] = {"1", "2", "3", "4", "5", "6", "7", "8"};
  float sc_data[]   = {20, 35, 15, 42, 28, 50, 33, 46};
  AxisConfig sc_axis = {
    .thickness = 2, .title_size = 2,
    .x_title = "sample", .y_title = "value",
    .y_steps = 5, .dash_line = true
  };
  ScatterChartConfig sc_cfg = {
    .x0 = 8, .x1 = 784, .y0 = 12, .y1 = 262,
    .dots_size = 9,
    .dot_color = COLOR_BLACK,
    .axisConfig = sc_axis
  };
  draw_scatter_chart(framebuffer, &sc_cfg, sc_labels, sc_data, 8, ORIENT_BOTTOM_AXIS);


  save_pbm("output.pbm", framebuffer);
  return 0;
}

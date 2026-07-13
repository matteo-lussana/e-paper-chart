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
  draw_double_axis_bar_chart(framebuffer, &cfg, datas, data_sx, data_dx, n, ORIENT_BOTTOM_AXIS);


  /* multi-bar chart (2 serie) con asse x IN BASSO, a destra del grafico esistente */
  char *mb_labels[] = {"Jan", "Feb", "Mar", "Apr", "May"};
  float *mb_data[]  = {(float[]){12,20,10}, (float[]){30,15,13}, (float[]){7,25,13},
                       (float[]){45,30,13}, (float[]){20,40,13}};
  AxisConfig bax = {.thickness = 2, .title_size = 2,
                    .x_title = "month", .y_title = "km",
                    .y_steps = 5, .dash_line = true};
  BarChartConfig bcfg = {.x0 = 420, .x1 = 760, .y0 = 20, .y1 = 240,
                         .values_label = false, .axisConfig = bax};
  draw_multi_bar_chart(framebuffer, &bcfg, mb_labels, mb_data, 5, 3, ORIENT_BOTTOM_AXIS);

  save_pbm("output.pbm", framebuffer);
  return 0;
}

#include "chart.h"
#include "font8x8_basic.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

void save_pbm(const char *filename, uint8_t *buf)
{
  FILE *f = fopen(filename, "wb");
  if (!f) {
    fprintf(stderr, "Error: can not open file %s\n", filename);
    return;
  }

  fprintf(f, "P4\n%d %d\n", WIDTH, HEIGHT);

  /* write STRIDE byte for each row */
  for (int y = 0; y < HEIGHT; y++)
    fwrite(&buf[y * STRIDE], 1, STRIDE, f);

  fclose(f);
  printf("Saved: %s (%dx%d)\n", filename, WIDTH, HEIGHT);
}

void set_pixel(uint8_t *buf, int x, int y, Color color)
{
  /* check the edges */
  if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
    return;

  int byte_index = y * STRIDE + x / 8;
  int bit_index = 7 - (x % 8);
  switch(color){
    case COLOR_WHITE:
      buf[byte_index] &= ~(1 << bit_index);
      break;
    case COLOR_BLACK:
      buf[byte_index] |= (1 << bit_index);
      break;
    case COLOR_GRAY:
      if((x + y) % 2 == 0)
        buf[byte_index] &= ~(1 << bit_index);
      else
        buf[byte_index] |= (1 << bit_index);
      break;
  }
}

int get_pixel(uint8_t *buf, int x, int y)
{
  int byte_index = y * STRIDE + x / 8;
  int bit_index = 7 - (x % 8);
  return (buf[byte_index] >> bit_index) & 1;
}

void draw_hline(uint8_t *buf, int x0, int x1, int y, int thickness, Color color, LineType type) //type: 0->line, 1->dashed, 2->dotted
{
  if (x0 > x1) { int t = x0; x0 = x1; x1 = t; }
  int up, down;
  if(thickness % 2 == 1){
    up = down = (thickness - 1) / 2;
  } else{
    up = down = thickness / 2;
    down--;
  }

  switch(type){
    case 0: default:
      for(int c = x0; c <= x1; c++){
        for(int r = y - up; r <= y + down; r++){
          set_pixel(buf, c, r, color);
        }
      }
      break;
    case 1:
      for(int c = x0; c <= x1; c++){
        for(int r = y - up; r <= y + down; r++){
          set_pixel(buf, c, r, color);
        }
        if(c % (thickness * 3) == 0) c += thickness;
      }
      break;
    case 2:
      for(int c = x0; c <= x1; c++){
        for(int r = y - up; r <= y + down; r++){
          set_pixel(buf, c, r, color);
        }
        if(c % thickness == 0) c += thickness;
      }
      break;
  }
}

void draw_vline(uint8_t *buf, int x,  int y0, int y1, int thickness, Color color, LineType type) //type: 0->line, 1->dashed, 2->dotted
{
  if (y0 > y1) { int t = y0; y0 = y1; y1 = t; }
  int left, right;
  if(thickness % 2 == 1){
    left = right = (thickness - 1) / 2;
  } else{
    left = right = thickness / 2;
    right--;
  }

  switch(type){
    case 0: default:
      for(int r = y0; r <= y1; r++){
        for(int c = x - left; c <= x + right; c++){
          set_pixel(buf, c, r, color);
        }
      }
      break;
    case 1:
      for(int r = y0; r <= y1; r++){
        for(int c = x - left; c <= x + right; c++){
          set_pixel(buf, c, r, color);
        }
        if(r % (thickness * 3) == 0) r += thickness;
      }
      break;
    case 2: 
      for(int r = y0; r <= y1; r++){
        for(int c = x - left; c <= x + right; c++){
          set_pixel(buf, c, r, color);
        }
        if((r) % thickness == 0) r += thickness;
      }
      break;
  }
}

void draw_line(uint8_t *buf, int x0, int y0, int x1, int y1, int thickness, Color color, LineType type)
{
  int dx =  abs(x1 - x0);
  int dy = -abs(y1 - y0);
  int sx = (x0 < x1) ? 1 : -1;
  int sy = (y0 < y1) ? 1 : -1;
  int err = dx + dy;

  int counter_type = 0;
  while (1) {
    if(!(type == 1 && counter_type % (thickness * 3) == 0) &&
      !(type == 2 && counter_type % thickness == 0)){
      for(int i = 0; i < thickness; i++)
        if(dx < abs(dy))
          set_pixel(buf, x0+i, y0, color);
        else
          set_pixel(buf, x0, y0+i, color);
    }    if (x0 == x1 && y0 == y1) break;

    int e2 = 2 * err;
    if (e2 >= dy) { err += dy; x0 += sx; }
    if (e2 <= dx) { err += dx; y0 += sy; }

    counter_type++;
  }
}

void draw_rect(uint8_t *buf, int x0, int x1, int y0, int y1, int thickness, Color color, LineType line_type)
{
  draw_hline(buf, x0, x1, y0, thickness, color, line_type);
  draw_hline(buf, x0, x1, y1, thickness, color, line_type);
  draw_vline(buf, x0, y0, y1, thickness, color, line_type);
  draw_vline(buf, x1, y0, y1, thickness, color, line_type);
}

void fill_rect(uint8_t *buf, int x0, int x1, int y0, int y1, Color color)
{
  if (x0 > x1) { int t = x0; x0 = x1; x1 = t; }
  if (y0 > y1) { int t = y0; y0 = y1; y1 = t; }
  for(int r = y0; r <= y1; r++){
    draw_hline(buf, x0, x1, r, 1, color, LINE_SOLID);
  }
}

void draw_circle(uint8_t *buf, int cx, int cy, int radius, int thickness, Color color)
{
  int tx = cx + radius;
  int ty = cy;
  int fx = cx + radius * (sqrt(2)/2);
  int fy = cy + radius * (sqrt(2)/(2));
  thickness--;
  while(tx != fx && ty != fy){
    for(int i = 0; i < 2; i++){
    fill_rect(buf, tx, tx+thickness, ty, ty+thickness, color);
    fill_rect(buf, cx-abs(tx-cx), cx-abs(tx-cx) + thickness, ty, ty + thickness, color);
    fill_rect(buf, cx-abs(tx-cx),cx-abs(tx-cx) + thickness, cy-abs(ty-cy), cy-abs(ty-cy) + thickness, color);
    fill_rect(buf, tx, tx+thickness, cy-abs(ty-cy), cy-abs(ty-cy) + thickness, color);
    int temp = tx; tx = ty; ty = temp;
    }
    ty++;
    if(sqrt(pow(tx-cx, 2) + pow(ty-cy, 2)) < radius) tx++;
    else tx--;
  }
}

void draw_char(uint8_t *buf, int x, int y, char c, Color color, int scale, int rotation)
{
  for (int row = 0; row < 8; row++){
    uint8_t line = font8x8_basic[c][row];
    for(int col = 0; col < 8; col++){
      int bit = (line >> col) & 1;
      if (bit){ 
        int ox, oy;
        switch(rotation){
          case 0: default: ox = col; oy = row; break;
          case 90: ox = 7 - row; oy = col; break;
          case 180: ox = 7 - col; oy = 7 - row; break;
          case 270: ox = row; oy = 7 - col; break;
        }
        fill_rect(buf, x + ox * scale, x + ox * scale + scale - 1, 
                  y + oy * scale, y + oy * scale + scale - 1, color);
      }
    }
  }
}

void draw_text(uint8_t *buf, int x, int y, const char *str, Color color, int scale, int rotation)
{
  int pos_x, pos_y;
  for(int i = 0; str[i] != '\0'; i++){
    switch (rotation) {
      case 0: default: pos_x = x + i * 8 * scale; pos_y = y; break;
      case 90: pos_x = x; pos_y = y + i * 8 * scale; break;
      case 180: pos_x = x - i * 8 * scale; pos_y = y; break;
      case 270: pos_x = x; pos_y = y - i * 8 * scale; break;
    }
    draw_char(buf, pos_x, pos_y, str[i], color, scale, rotation);
  }
}

static void draw_axis_title(uint8_t *buf, int *x0, int *x1, int *y0, int *y1, const AxisConfig axisConfig, Orientation orientation)
{
  if(axisConfig.x_title[0] != '\0'){
    if(orientation == ORIENT_BOTTOM_AXIS){
      draw_text(buf, (*x1 + *x0) / 2 - strlen(axisConfig.x_title) * 8 * axisConfig.title_size/2, *y1 - axisConfig.title_size * 8, axisConfig.x_title, COLOR_BLACK, axisConfig.title_size, 0);
      *y1 -= axisConfig.title_size * 8;
    }
    if(orientation == ORIENT_TOP_AXIS){
      draw_text(buf, (*x1 + *x0) / 2 - strlen(axisConfig.x_title) * 8 * axisConfig.title_size/2, *y0, axisConfig.x_title, COLOR_BLACK, axisConfig.title_size, 0);
      *y0 += axisConfig.title_size * 8;
    }
  }
  if(axisConfig.y_title[0] != '\0'){
    draw_text(buf, *x0, (*y1 + *y0) / 2 + strlen(axisConfig.y_title) * 8 * axisConfig.title_size/2, axisConfig.y_title, COLOR_BLACK, axisConfig.title_size, 270);
    *x0 += axisConfig.title_size * 8;
  }
}

static int value_to_y(float value, float max, int y0, int y1, Orientation orientation)
{
  if(orientation == ORIENT_BOTTOM_AXIS)
    return y1 - value * (y1 - y0) / max;
  return y0 + value * (y1 - y0) / max;
}

static void draw_ticks_and_labels(uint8_t *buf, int *x0, int *x1, int *y0, int *y1, const AxisConfig axisConfig, char **x_data, int n, float y_max, Orientation orientation)
{
  int direction = orientation == ORIENT_BOTTOM_AXIS ? 1 : -1;
  int y;
  *x0 += axisConfig.thickness * 14;
  if(orientation == ORIENT_BOTTOM_AXIS){ *y1 -= axisConfig.thickness * 6; y = *y1; }
  if(orientation == ORIENT_TOP_AXIS){ *y0 += axisConfig.thickness * 6; y = *y0; }
  //horizontal ticks
  float single_space = (*x1 - *x0) / n;
  for(int i = 0; i < n; i++){
    //labels
    draw_text(buf, *x0 + single_space * (i + 0.5) - strlen(x_data[i]) * 8 / 2, (y + 4 * direction * axisConfig.thickness) - 2 * axisConfig.thickness, x_data[i], COLOR_BLACK, 1, 0);
    //ticks
    draw_vline(buf, *x0 + single_space * (i + 0.5), y + 2 * direction * axisConfig.thickness, y, axisConfig.thickness, COLOR_BLACK, LINE_SOLID);
  }

  //vertical ticks
  single_space = (*y1 - *y0) / axisConfig.y_steps;
  for(int i = 0; i < axisConfig.y_steps+1; i++){
    //labels
    char label[16];
    snprintf(label, sizeof(label), "%d", (int)(y_max * i/axisConfig.y_steps));
    draw_text(buf, *x0 - 10 * axisConfig.thickness, value_to_y(y_max * i / axisConfig.y_steps, y_max, *y0, *y1, orientation) - 4, label, COLOR_BLACK, 1, 0);
    //ticks
    draw_hline(buf, *x0 - 2 * axisConfig.thickness, *x0, value_to_y(y_max * i / axisConfig.y_steps, y_max, *y0, *y1, orientation), axisConfig.thickness, COLOR_BLACK, LINE_SOLID);
    if(axisConfig.dash_line){
      draw_hline(buf, *x0, *x1, value_to_y(y_max * i / axisConfig.y_steps, y_max, *y0, *y1, orientation), axisConfig.thickness, COLOR_GRAY, LINE_DASHED);
    }
  }
}

static void draw_double_axis_title(uint8_t *buf, int *x0, int *x1, int *y0, int *y1, const DoubleAxisConfig axisConfig, Orientation orientation)
{
  if(axisConfig.x_title[0] != '\0'){
    if(orientation == ORIENT_BOTTOM_AXIS){
      draw_text(buf, (*x1 + *x0) / 2 - strlen(axisConfig.x_title) * 8 * axisConfig.title_size/2, *y1 - axisConfig.title_size * 8, axisConfig.x_title, COLOR_BLACK, axisConfig.title_size, 0);
      *y1 -= axisConfig.title_size * 8 + 2 * axisConfig.thickness;
    }
    if(orientation == ORIENT_TOP_AXIS){
      draw_text(buf, (*x1 + *x0) / 2 - strlen(axisConfig.x_title) * 8 * axisConfig.title_size/2, *y0, axisConfig.x_title, COLOR_BLACK, axisConfig.title_size, 0);
      *y0 += axisConfig.title_size * 8 + 2 * axisConfig.thickness;
    }
  }
  if(axisConfig.y_title_left[0] != '\0'){
    draw_text(buf, *x0, (*y1 + *y0) / 2 + strlen(axisConfig.y_title_left) * 8 * axisConfig.title_size/2, axisConfig.y_title_left, COLOR_BLACK, axisConfig.title_size, 270);
    *x0 += axisConfig.title_size * 8;
  }
  if(axisConfig.y_title_right[0] != '\0'){
    *x1 -= axisConfig.title_size * 8;
    draw_text(buf, *x1, (*y1 + *y0) / 2 - strlen(axisConfig.y_title_right) * 8 * axisConfig.title_size/2, axisConfig.y_title_right, COLOR_BLACK, axisConfig.title_size, 90);
  }
}

static void draw_double_ticks_and_labels(uint8_t *buf, int *x0, int *x1, int *y0, int *y1, const DoubleAxisConfig axisConfig, char **x_data, int n, float y_max_left, float y_max_right, Orientation orientation)
{
  if(axisConfig.y_steps_left > 0) *x0 += axisConfig.thickness * 14;
  if(axisConfig.y_steps_right > 0) *x1 -= axisConfig.thickness * 14;
  int direction = orientation == ORIENT_BOTTOM_AXIS ? 1 : -1;
  int y;
  if(orientation == ORIENT_BOTTOM_AXIS) { *y1 -= axisConfig.thickness * 6; y = *y1; }
  if(orientation == ORIENT_TOP_AXIS) { *y0 += axisConfig.thickness * 6; y = *y0; }
  //horizontal ticks
  float single_space = (*x1 - *x0) / n;
  for(int i = 0; i < n; i++){
    //labels
    draw_text(buf, *x0 + single_space * (i + 0.5) - strlen(x_data[i]) * 8 / 2, (y + 4 * direction * axisConfig.thickness) - 2 * axisConfig.thickness, x_data[i], COLOR_BLACK, 1, 0);
    //ticks
    draw_vline(buf, *x0 + single_space * (i + 0.5), y + 2 * direction * axisConfig.thickness, y, axisConfig.thickness, COLOR_BLACK, LINE_SOLID);
  }

  //vertical ticks left
  single_space = (*y1 - *y0) / axisConfig.y_steps_left;
  int steps;
  steps = axisConfig.y_steps_left > 0 ? axisConfig.y_steps_left + 1 : 0;
  for(int i = 0; i < steps; i++){
    //labels
    char label[16];
    snprintf(label, sizeof(label), "%d", (int)(y_max_left * i/axisConfig.y_steps_left));
    draw_text(buf, *x0 - 10 * axisConfig.thickness, value_to_y(y_max_left * i / axisConfig.y_steps_left, y_max_left, *y0, *y1, orientation) - 4, label, COLOR_BLACK, 1, 0);
    //ticks
    draw_hline(buf, *x0 - 2 * axisConfig.thickness, *x0, value_to_y(y_max_left * i / axisConfig.y_steps_left, y_max_left, *y0, *y1, orientation), axisConfig.thickness, COLOR_BLACK, LINE_SOLID);
    if(axisConfig.dash_line_left){
      draw_hline(buf, *x0, *x1, value_to_y(y_max_left * i / axisConfig.y_steps_left, y_max_left, *y0, *y1, orientation), axisConfig.thickness, COLOR_GRAY, LINE_DASHED);
    }
  }

  //vertical ticks right
  steps = axisConfig.y_steps_right > 0 ? axisConfig.y_steps_right + 1 : 0;
  single_space = (*y1 - *y0) / axisConfig.y_steps_right;
  for(int i = 0; i < steps; i++){
    //labels
    char label[16];
    snprintf(label, sizeof(label), "%d", (int)(y_max_right * i/axisConfig.y_steps_right));
    draw_text(buf, *x1 + 4, value_to_y(y_max_right * i / axisConfig.y_steps_right, y_max_right, *y0, *y1, orientation) - 4, label, COLOR_BLACK, 1, 0);
    //ticks
    draw_hline(buf, *x1, *x1 + 2 * axisConfig.thickness, value_to_y(y_max_right * i / axisConfig.y_steps_right, y_max_right, *y0, *y1, orientation), axisConfig.thickness, COLOR_BLACK, LINE_SOLID);
    if(axisConfig.dash_line_right){
      draw_hline(buf, *x0, *x1, value_to_y(y_max_right * i / axisConfig.y_steps_right, y_max_right, *y0, *y1, orientation), axisConfig.thickness, COLOR_GRAY, LINE_DASHED);
    }
  }
}

static void draw_legend(uint8_t *buf, int *x0, int *x1, int *y0, int *y1, bool legend, char **labels, int n, int legend_size, Orientation orientation)
{
  if(!legend) return;
  if(orientation == ORIENT_BOTTOM_AXIS) *y0 += legend_size * 8 * 1.55;
  if(orientation == ORIENT_TOP_AXIS) *y1 -= legend_size * 8 * 1.55;
  float legend_space = (legend_size * 8 * 1.25) * n + legend_size * 8 * 1.2 * (n-1);
  for(int i = 0; i < n; i++){
    legend_space += strlen(labels[i]) * 8;
  }
  int x_start = (*x0 + *x1)/2 - legend_space/2;
  int y_start = orientation == ORIENT_BOTTOM_AXIS ? *y0 - legend_size * 4 : *y1 + legend_size * 4;
  int direction = orientation == ORIENT_BOTTOM_AXIS ? 1 : -1;
  int padding = 8 * legend_size * direction;
  for(int i = 0; i < n; i++){
    fill_rect(buf, x_start, x_start + 8 * legend_size, y_start, y_start - padding, (Color)((i+1)%3));
    draw_rect(buf, x_start, x_start + 8 * legend_size, y_start, y_start - padding, legend_size * 2, COLOR_BLACK, LINE_SOLID);
    x_start += 8 * legend_size * 1.5;
    draw_text(buf, x_start, y_start < y_start - padding ? y_start : y_start - padding, labels[i], COLOR_BLACK, 1, 0);
    x_start += strlen(labels[i]) * 8 + legend_size * 8 * 1.2;
  }
}

static void draw_bar_labels(uint8_t *buf, int *x0, int *x1,int  *y0, int *y1, bool values_label, float *y_data, int n, float max_value, float single_space, Orientation orientation)
{
  if(!values_label) return;
  int y;
  int direction = orientation == ORIENT_BOTTOM_AXIS ? 1 : -1;
  for(int i = 0; i < n; i++){
    char label[16];
    snprintf(label, sizeof(label), "%d", (int)(y_data[i]));
    y = value_to_y(y_data[i], max_value, *y0, *y1, orientation);
    draw_rect(buf, *x0 + single_space * (i + 0.5) - strlen(label) * 8 / 2 - 2, *x0 + single_space * (i + 0.5) + strlen(label) * 8 / 2, y- 2 * direction, y - 12 * direction, 1, COLOR_BLACK, LINE_SOLID);
    fill_rect(buf, *x0 + single_space * (i + 0.5) - strlen(label) * 8 / 2 - 1, *x0 + single_space * (i + 0.5) + strlen(label) * 8 / 2 - 1, y - 3 * direction, y - 11 * direction, COLOR_WHITE);
    draw_text(buf, *x0 + single_space * (i + 0.5) - strlen(label) * 8 / 2, y - (orientation == ORIENT_BOTTOM_AXIS ? 10 : 4) * direction, label, COLOR_BLACK, 1, 0);
  }
}

void draw_bar_chart(uint8_t *buf, const BarChartConfig *cfg, char **x_data, float *y_data, int n, Orientation orientation)
{
  int x0 = cfg->x0;
  int x1 = cfg->x1;
  int y0 = cfg->y0;
  int y1 = cfg->y1;

  float max_value = y_data[0];
  for(int i = 0; i < n; i++){
    if(y_data[i] > max_value) max_value = y_data[i];
  }

  if(cfg->values_label){
    if(orientation == ORIENT_BOTTOM_AXIS) y0 += 15;
    if(orientation == ORIENT_TOP_AXIS) y1 -= 15;
  }

  draw_axis_title(buf, &x0, &x1, &y0, &y1, cfg->axisConfig, orientation);
  draw_ticks_and_labels(buf, &x0, &x1, &y0, &y1, cfg->axisConfig, x_data, n, max_value, orientation);
  float single_space = (x1 - x0) / n;
  draw_bar_labels(buf, &x0, &x1, &y0, &y1, cfg->values_label, y_data, n, max_value, single_space, orientation);

  draw_hline(buf, x0, x1, value_to_y(0, max_value, y0, y1, orientation), cfg->axisConfig.thickness, COLOR_BLACK, LINE_SOLID);

  draw_vline(buf, x0, y0, y1, cfg->axisConfig.thickness, COLOR_BLACK, LINE_SOLID);
  
  int y_start;
  int y_end;
  for(int i = 0; i < n; i++){
    y_start = value_to_y(0, max_value, y0, y1, orientation);
    y_end   = value_to_y(y_data[i], max_value, y0, y1, orientation);
    draw_vline(buf, x0 + single_space * (i + 0.5), y_start, y_end, (single_space / 2) * 0.6, COLOR_BLACK, LINE_SOLID);
  }
}

void draw_multi_bar_chart(uint8_t *buf, const BarChartConfig *cfg, char **x_data, float **y_data, int data_length, int data_sets, Orientation orientation)
{
  int x0 = cfg->x0;
  int x1 = cfg->x1;
  int y0 = cfg->y0;
  int y1 = cfg->y1;

  float max_value = y_data[0][0];
  for(int i = 0; i < data_length; i++){
    for(int j = 0; j < data_sets; j++)
      if(y_data[i][j] > max_value) max_value = y_data[i][j];
  }

  if(cfg->values_label) y1 -= 15;
  draw_legend(buf, &x0, &x1, &y0, &y1, cfg->legendConfig.legend, cfg->legendConfig.labels, data_sets, cfg->legendConfig.legend_size, orientation);
  draw_axis_title(buf, &x0, &x1, &y0, &y1, cfg->axisConfig, orientation);
  draw_ticks_and_labels(buf, &x0, &x1, &y0, &y1, cfg->axisConfig, x_data, data_length, max_value, orientation);

  draw_vline(buf, x0, y0, y1, cfg->axisConfig.thickness, COLOR_BLACK, LINE_SOLID);
  draw_hline(buf, x0, x1, value_to_y(0, max_value, y0, y1, orientation), cfg->axisConfig.thickness, COLOR_BLACK, LINE_SOLID);

  float single_space = (x1 - x0) / data_length;
  float bars_space = single_space * 0.6;
  float small_space = bars_space / data_sets;

  for(int i = 0; i < data_length; i++){
    for(int j = 0; j < data_sets; j++){
      fill_rect(buf,
                x0 + single_space * (i + 0.5) - (bars_space * 0.5) + small_space * (j + 0.5) - (small_space * 0.4),
                x0 + single_space * (i + 0.5) - (bars_space * 0.5) + small_space * (j + 0.5) + (small_space * 0.4),
                value_to_y(0, max_value, y0, y1, orientation),
                value_to_y(y_data[i][j], max_value, y0, y1, orientation),
                (Color)(j % 3)
                );

      draw_rect(buf,
                x0 + single_space * (i + 0.5) - (bars_space * 0.5) + small_space * (j + 0.5) - (small_space * 0.4),
                x0 + single_space * (i + 0.5) - (bars_space * 0.5) + small_space * (j + 0.5) + (small_space * 0.4),
                value_to_y(0, max_value, y0, y1, orientation),
                value_to_y(y_data[i][j], max_value, y0, y1, orientation),
                small_space * 0.15,
                COLOR_BLACK,
                LINE_SOLID
                );
    }
  }
}

void draw_double_axis_bar_chart(uint8_t *buf, const DoubleAxisBarChartConfig *cfg, char **x_data, float *y_data_left, float *y_data_right, int data_length, Orientation orientation)
{
  int x0 = cfg->x0;
  int x1 = cfg->x1;
  int y0 = cfg->y0;
  int y1 = cfg->y1;

  float max_value_left = y_data_left[0];
  float max_value_right = y_data_right[0];
  for(int i = 0; i < data_length; i++){
    if(y_data_left[i] > max_value_left) max_value_left = y_data_left[i];
    if(y_data_right[i] > max_value_right) max_value_right = y_data_right[i];
  }

  if(cfg->values_label) y1 -= 15;

  draw_legend(buf, &x0, &x1, &y0, &y1, cfg->legendConfig.legend, cfg->legendConfig.labels, 2, cfg->legendConfig.legend_size, orientation);
  draw_double_axis_title(buf, &x0, &x1, &y0, &y1, cfg->doubleAxisConfig, orientation);
  draw_double_ticks_and_labels(buf, &x0, &x1, &y0, &y1, cfg->doubleAxisConfig, x_data, data_length, max_value_left, max_value_right, orientation);

  draw_vline(buf, x0, y0, y1, cfg->doubleAxisConfig.thickness, COLOR_BLACK, LINE_SOLID);
  draw_vline(buf, x1, y0, y1, cfg->doubleAxisConfig.thickness, COLOR_BLACK, LINE_SOLID);
  draw_hline(buf, x0, x1, value_to_y(0, max_value_left > max_value_right ? max_value_left : max_value_right, y0, y1, orientation), cfg->doubleAxisConfig.thickness, COLOR_BLACK, LINE_SOLID);

  float single_space = (x1 - x0) / data_length;
  float bars_space = single_space * 0.6;
  float small_space = bars_space / 2;

  for(int i = 0; i < data_length; i++){
    int y_start = value_to_y(0, max_value_left, y0, y1, orientation);
    int y_end_left = value_to_y(y_data_left[i], max_value_left, y0, y1, orientation);
    int y_end_right = value_to_y(y_data_right[i], max_value_right, y0, y1, orientation);

    //left
    fill_rect(buf,
              x0 + single_space * (i + 0.5) - (bars_space * 0.5) + small_space * (0 + 0.5) - (small_space * 0.4),
              x0 + single_space * (i + 0.5) - (bars_space * 0.5) + small_space * (0 + 0.5) + (small_space * 0.4),
              y_start,
              y_end_left,
              COLOR_BLACK
              );
    draw_rect(buf,
              x0 + single_space * (i + 0.5) - (bars_space * 0.5) + small_space * (0 + 0.5) - (small_space * 0.4),
              x0 + single_space * (i + 0.5) - (bars_space * 0.5) + small_space * (0 + 0.5) + (small_space * 0.4),
              y_start,
              y_end_left,
              small_space * 0.15,
              COLOR_BLACK,
              LINE_SOLID
              );
    //right
    fill_rect(buf,
              x0 + single_space * (i + 0.5) - (bars_space * 0.5) + small_space * (1 + 0.5) - (small_space * 0.4),
              x0 + single_space * (i + 0.5) - (bars_space * 0.5) + small_space * (1 + 0.5) + (small_space * 0.4),
              y_start,
              y_end_right,
              COLOR_GRAY 
              );
    draw_rect(buf,
              x0 + single_space * (i + 0.5) - (bars_space * 0.5) + small_space * (1 + 0.5) - (small_space * 0.4),
              x0 + single_space * (i + 0.5) - (bars_space * 0.5) + small_space * (1 + 0.5) + (small_space * 0.4),
              y_start,
              y_end_right,
              small_space * 0.15,
              COLOR_BLACK,
              LINE_SOLID
              );

  }
}



void draw_line_chart(uint8_t *buf, const LineChartConfig *cfg, char **x_data, float *y_data, int n, Orientation orientation)
{
  int x0 = cfg->x0;
  int x1 = cfg->x1;
  int y0 = cfg->y0;
  int y1 = cfg->y1;

  float max_value = y_data[0];
  for(int i = 0; i < n; i++){
    if(y_data[i] > max_value) max_value = y_data[i];
  }

  if(cfg->values_label) y1 -= 10;

  draw_axis_title(buf, &x0, &x1, &y0, &y1, cfg->axisConfig, orientation);
  draw_ticks_and_labels(buf, &x0, &x1, &y0, &y1, cfg->axisConfig, x_data, n, max_value, orientation);

  draw_vline(buf, x0, y0, y1, cfg->axisConfig.thickness, COLOR_BLACK, LINE_SOLID);
  draw_hline(buf, x0, x1, value_to_y(0, max_value, y0, y1, orientation), cfg->axisConfig.thickness, COLOR_BLACK, LINE_SOLID);

  float single_space = (x1 - x0) / n;
  for(int i = 0; i < n-1; i++){
    draw_line(buf, 
              x0 + single_space * (i + 0.5), 
              value_to_y(y_data[i], max_value, y0, y1, orientation),
              x0 + single_space * (i+1 + 0.5),
              value_to_y(y_data[i+1], max_value, y0, y1, orientation),
              cfg->line_thickness,
              cfg->line_color,
              cfg->line_type);
  }
  if(cfg->values_label)
    for(int i = 0; i < n; i++){
      int pos_x, pos_y;
      float prec = y_data[i+1], suc = y_data[i-1], cur = y_data[i];
      char label[16];
      snprintf(label, sizeof(label), "%d", (int)(cur));

      if(i-1 >= 0) prec = y_data[i-1];
      if(i+1 < n) suc = y_data[i+1];
      pos_x = x0 + single_space * (i + 0.5);
      pos_y = value_to_y(y_data[i], max_value, y0, y1, orientation);

      if(prec < cur && suc < cur){
        pos_y += 10; 
      }
      else if(prec > cur && suc > cur){
        pos_y -= 10;
      }
      else if(prec < cur && suc > cur){
        pos_y += 10;
        pos_x -= 10;
      }
      else{
        pos_y += 10;
        pos_x += 10;
      }
      draw_rect(buf, pos_x - strlen(label) * 8 / 2 - 2, pos_x + strlen(label) * 8 / 2, pos_y - 2, pos_y + 8, 1, COLOR_BLACK, LINE_SOLID);
      fill_rect(buf, pos_x - strlen(label) * 8 / 2 - 1, pos_x + strlen(label) * 8 / 2 - 1, pos_y - 1, pos_y + 8 -1, COLOR_WHITE);
      draw_text(buf, pos_x - strlen(label) * 8 / 2, pos_y, label, cfg->line_color, 1, 0);
    }
}

void draw_pie_chart(uint8_t *buf, const PieChartConfig *cfg, char **x_data, float *y_data, int n)
{
  int cx = cfg->cx;
  int cy = cfg->cy;
  float perc[n]; 
  float sum = 0;
  for(int i = 0; i < n; i++){
    sum += y_data[i];
  }
  for(int i = 0; i < n; i++){
    perc[i] = (y_data[i] * 2 * 3.14) / sum;
  }

  //color segments
  float curr_angle = 0;
  int divider = n % 2 == 0 ? 2 : 3;
  for(int x = cx - cfg->radius; x < cx + cfg->radius; x++)
    for(int y = cy - cfg->radius; y < cy + cfg->radius; y++){
      if((x-cx)*(x-cx)+(y-cy)*(y-cy) < (cfg->radius)*(cfg->radius)){
        float atan = atan2(-(y-cy), x-cx);
        if(atan < 0) atan += 2 * 3.14;
        for(int i = 0; i < n; i++){
          if(atan > curr_angle && atan < curr_angle + perc[i])
            set_pixel(buf, x, y, (Color)(i%divider));
          curr_angle += perc[i];
        }
        curr_angle=0;
      }
    }

  //draw segments
  curr_angle = 0;
  for(int i = 0; i < n; i++){
    curr_angle -= perc[i];
    float px = cfg->radius * cosf(curr_angle);
    float py = cfg->radius * sinf(curr_angle);
    draw_line(buf, cx, cy, cx + px, cy + py, cfg->thickness, cfg->color, LINE_SOLID);
  }

  //draw indicators
  curr_angle = 0;
  if(cfg->values_label || cfg->names_label){
    for(int i = 0; i < n; i++){
      curr_angle += perc[i];
      char str[32];
      int len = 0;
      float ix = cx + (cfg->radius / 3 * cosf(perc[i]/2 - curr_angle));
      float iy = cy + (cfg->radius / 3 * sinf(perc[i]/2 - curr_angle));
      float ox = cx + (cfg->radius * 1.5 * cosf(perc[i]/2 - curr_angle));
      float oy = cy + (cfg->radius * 1.5 * sinf(perc[i]/2 - curr_angle));
      draw_line(buf, ix, iy, ox, oy, cfg->thickness, cfg->color, LINE_SOLID);

      if (cfg->values_label) len += snprintf(str + len, sizeof(str) - len, "%.1f%% ", perc[i]/(2*3.14)*100);

      if (cfg->names_label) len += snprintf(str + len, sizeof(str) - len, "%s", x_data[i]);
      draw_text(buf, ox, oy, str, cfg->color, 1, 0);
    }
  }

  draw_circle(buf, cx, cy, cfg->radius, cfg->thickness, cfg->color);
}

void draw_freq_chart(uint8_t *buf, const FreqChartConfig *cfg, int *data, int n)
{
  int start_x = cfg->x0;
  int start_y = cfg->y0;
  int counter = 0;
  
  for (int col = 0; col < cfg->columns_num; col++){
    for (int row = 0; row < cfg->rows_num; row++){
      fill_rect(buf, start_x, start_x + cfg->square_size, 
                start_y, start_y + cfg->square_size,
                (Color)data[counter]);
      counter++;
      draw_rect(buf, start_x, start_x + cfg->square_size,
                start_y, start_y + cfg->square_size,
                cfg->border_width, COLOR_BLACK, LINE_SOLID);
      start_y += cfg->square_size + cfg->square_distance;
    }
    start_y = cfg->y0;
    start_x += cfg->square_size + cfg-> square_distance;
  }
}

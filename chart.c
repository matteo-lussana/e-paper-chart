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

void set_pixel(uint8_t *buf, int x, int y, int color)
{
  /* check the edges */
  if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
    return;

  int byte_index = y * STRIDE + x / 8;
  int bit_index = 7 - (x % 8);
  switch(color){
    case 0:
      buf[byte_index] &= ~(1 << bit_index);
      break;
    case 1:
      buf[byte_index] |= (1 << bit_index);
      break;
    case 2:
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

void draw_hline(uint8_t *buf, int x0, int x1, int y, int thickness, int color, int type) //type: 0->line, 1->dashed, 2->dotted
{
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

void draw_vline(uint8_t *buf, int x,  int y0, int y1, int thickness, int color, int type) //type: 0->line, 1->dashed, 2->dotted
{
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

void draw_line(uint8_t *buf, int x0, int y0, int x1, int y1, int thickness, int color, int type){
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

void draw_rect(uint8_t *buf, int x0, int x1, int y0, int y1, int thickness, int color, int line_type){
  draw_hline(buf, x0, x1, y0, thickness, color, line_type);
  draw_hline(buf, x0, x1, y1, thickness, color, line_type);
  draw_vline(buf, x0, y0, y1, thickness, color, line_type);
  draw_vline(buf, x1, y0, y1, thickness, color, line_type);
}

void fill_rect(uint8_t *buf, int x0, int x1, int y0, int y1, int color){
  for(int r = y0; r <= y1; r++){
    draw_hline(buf, x0, x1, r, 1, color, 0);
  }
}

void draw_circle(uint8_t *buf, int cx, int cy, int radius, int thickness, int color){
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

void draw_char(uint8_t *buf, int x, int y, char c, int color, int scale, int rotation){
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

void draw_text(uint8_t *buf, int x, int y, const char *str, int color, int scale, int rotation){
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

static void draw_axis_title(uint8_t *buf, int *x0, int *x1, int *y0, int *y1, const AxisConfig axisConfig){
  if(axisConfig.y_title[0] != '\0'){
    draw_text(buf, *x0, (*y1 + *y0) / 2 + strlen(axisConfig.y_title) * 8 * axisConfig.title_size/2, axisConfig.y_title, 1, axisConfig.title_size, 270);
    *x0 += axisConfig.title_size * 8;
  }
  if(axisConfig.x_title[0] != '\0'){
    draw_text(buf, (*x1 + *x0) / 2 - strlen(axisConfig.x_title) * 8 * axisConfig.title_size/2, *y0, axisConfig.x_title, 1, axisConfig.title_size, 0);
    *y0 += axisConfig.title_size * 8;
  }
}

static void draw_ticks_and_labels(uint8_t *buf, int *x0, int *x1, int *y0, int *y1, const AxisConfig axisConfig, char **x_data, int n, float y_max){
  *x0 += axisConfig.thickness * 14;
  *y0 += axisConfig.thickness * 6;
  //horizontal ticks
  float single_space = (*x1 - *x0) / n;
  for(int i = 0; i < n; i++){
    //labels
    draw_text(buf, *x0 + single_space * (i + 0.5) - strlen(x_data[i]) * 8 / 2, *y0 - 6 * axisConfig.thickness, x_data[i], 1, 1, 0);
    //ticks
    draw_vline(buf, *x0 + single_space * (i + 0.5), *y0 - 2 * axisConfig.thickness, *y0, axisConfig.thickness, 1, 0);
  }

  //vertical ticks
  single_space = (*y1 - *y0) / axisConfig.y_steps;
  for(int i = 0; i < axisConfig.y_steps+1; i++){
    //labels
    char label[16];
    snprintf(label, sizeof(label), "%d", (int)(y_max * i/axisConfig.y_steps));
    draw_text(buf, *x0 - 10 * axisConfig.thickness, *y0 + (*y1 - *y0) * i / axisConfig.y_steps - 4, label, 1, 1, 0);
    //ticks
    draw_hline(buf, *x0 - 2 * axisConfig.thickness, *x0, *y0 + (*y1 - *y0) * i / axisConfig.y_steps, axisConfig.thickness, 1, 0);
    if(axisConfig.dash_line){
      draw_hline(buf, *x0, *x1, *y0 + (*y1 - *y0) * i / axisConfig.y_steps, axisConfig.thickness, 2, 1);
    }
  }
}

void draw_bar_chart(uint8_t *buf, const BarChartConfig *cfg, char **x_data, float *y_data, int n){
  int x0 = cfg->x0;
  int x1 = cfg->x1;
  int y0 = cfg->y0;
  int y1 = cfg->y1;

  float max_value = y_data[0];
  for(int i = 0; i < n; i++){
    if(y_data[i] > max_value) max_value = y_data[i];
  }

  if(cfg->values_label) y1 -= 15;

  draw_axis_title(buf, &x0, &x1, &y0, &y1, cfg->axisConfig);
  draw_ticks_and_labels(buf, &x0, &x1, &y0, &y1, cfg->axisConfig, x_data, n, max_value);

  draw_vline(buf, x0, y0, y1, cfg->axisConfig.thickness, 1, 0);
  draw_hline(buf, x0, x1, y0, cfg->axisConfig.thickness, 1, 0);

  float single_space = (x1 - x0) / n;

  for(int i = 0; i < n; i++){
    draw_vline(buf, x0 + single_space * (i + 0.5), y0, y0 + y_data[i] * (y1 - y0) / max_value, (single_space / 2) * 0.6, 1, 0);
  }

  if(cfg->values_label)
    for(int i = 0; i < n; i++){
      char label[16];
      snprintf(label, sizeof(label), "%d", (int)(y_data[i]));

      draw_rect(buf, x0 + single_space * (i + 0.5) - strlen(label) * 8 / 2 - 2, x0 + single_space * (i + 0.5) + strlen(label) * 8 / 2, y0 + y_data[i] * (y1 - y0) / max_value - 4 + 6, y0 + y_data[i] * (y1 - y0) / max_value + 4 + 8, 1, 1, 0);
      fill_rect(buf, x0 + single_space * (i + 0.5) - strlen(label) * 8 / 2 - 1, x0 + single_space * (i + 0.5) + strlen(label) * 8 / 2 - 1, y0 + y_data[i] * (y1 - y0) / max_value + 3, y0 + y_data[i] * (y1 - y0) / max_value + 11, 0);
      draw_text(buf, x0 + single_space * (i + 0.5) - strlen(label) * 8 / 2,  y0 + y_data[i] * (y1 - y0) / max_value + 4, label, 1, 1, 0);
    }

}

void draw_line_chart(uint8_t *buf, const LineChartConfig *cfg, char **x_data, float *y_data, int n){
  int x0 = cfg->x0;
  int x1 = cfg->x1;
  int y0 = cfg->y0;
  int y1 = cfg->y1;

  float max_value = y_data[0];
  for(int i = 0; i < n; i++){
    if(y_data[i] > max_value) max_value = y_data[i];
  }

  if(cfg->values_label) y1 -= 10;

  draw_axis_title(buf, &x0, &x1, &y0, &y1, cfg->axisConfig);
  draw_ticks_and_labels(buf, &x0, &x1, &y0, &y1, cfg->axisConfig, x_data, n, max_value);

  draw_vline(buf, x0, y0, y1, cfg->axisConfig.thickness, 1, 0);
  draw_hline(buf, x0, x1, y0, cfg->axisConfig.thickness, 1, 0);

  float single_space = (x1 - x0) / n;
  for(int i = 0; i < n-1; i++){
    draw_line(buf, 
              x0 + single_space * (i + 0.5), 
              y0 + y_data[i] * (y1 - y0) / max_value,
              x0 + single_space * (i+1 + 0.5),
              y0 + y_data[i+1] * (y1 - y0) / max_value,
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
      pos_y = y0 + y_data[i] * (y1 - y0) / max_value;

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
      draw_rect(buf, pos_x - strlen(label) * 8 / 2 - 2, pos_x + strlen(label) * 8 / 2, pos_y - 2, pos_y + 8, 1, 1, 0);
      fill_rect(buf, pos_x - strlen(label) * 8 / 2 - 1, pos_x + strlen(label) * 8 / 2 - 1, pos_y - 1, pos_y + 8 -1, 0);
      draw_text(buf, pos_x - strlen(label) * 8 / 2, pos_y, label, cfg->line_color, 1, 0);
    }
}

void draw_pie_chart(uint8_t *buf, const PieChartConfig *cfg, char **x_data, float *y_data, int n){
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
            set_pixel(buf, x, y, i%divider);
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
    draw_line(buf, cx, cy, cx + px, cy + py, cfg->thickness, cfg->color, 0);
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
      draw_line(buf, ix, iy, ox, oy, cfg->thickness, cfg->color, 0);

      if (cfg->values_label) len += snprintf(str + len, sizeof(str) - len, "%.1f%% ", perc[i]/(2*3.14)*100);

      if (cfg->names_label) len += snprintf(str + len, sizeof(str) - len, "%s", x_data[i]);
      draw_text(buf, ox, oy, str, cfg->color, 1, 0);
    }
  }

  draw_circle(buf, cx, cy, cfg->radius, cfg->thickness, cfg->color);
}

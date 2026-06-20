#include "chart.h"
#include "font8x8_basic.h"
#include <stdio.h>

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
  if (color)
    buf[byte_index] |= (1 << bit_index);
  else
    buf[byte_index] &= ~(1 << bit_index);
}

int get_pixel(uint8_t *buf, int x, int y)
{
  int byte_index = y * STRIDE + x / 8;
  int bit_index = 7 - (x % 8);

  return (buf[byte_index] >> bit_index) & 1;
}

void draw_hline(uint8_t *buf, int x0, int x1, int y, int thickness, int color)
{
  int up, down;
  if(thickness % 2 == 1){
    up = down = (thickness - 1) / 2;
  } else{
    up = down = thickness / 2;
    down--;
  }

  for(int c = x0; c <= x1; c++){
    for(int r = y - up; r <= y + down; r++){
      set_pixel(buf, c, r, color);
    }
  }
}
void draw_vline(uint8_t *buf, int x,  int y0, int y1, int thickness, int color)
{
  int left, right;
  if(thickness % 2 == 1){
    left = right = (thickness - 1) / 2;
  } else{
    left = right = thickness / 2;
    right--;
  }

  for(int r = y0; r <= y1; r++){
    for(int c = x - left; c <= x + right; c++){
      set_pixel(buf, c, r, color);
    }
  }
}

void draw_rect(uint8_t *buf, int x0, int x1, int y0, int y1, int thickness, int color){
  draw_hline(buf, x0, x1, y0, thickness, color);
  draw_hline(buf, x0, x1, y1, thickness, color);
  draw_vline(buf, x0, y0, y1, thickness, color);
  draw_vline(buf, x1, y0, y1, thickness, color);
}

void fill_rect(uint8_t *buf, int x0, int x1, int y0, int y1, int color){
  for(int r = y0; r <= y1; r++){
    draw_hline(buf, x0, x1, r, 1, color);
  }
}

void draw_char(uint8_t *buf, int x, int y, char c, int color, int scale){
  for (int row = 0; row < 8; row++){
    uint8_t line = font8x8_basic[c][row];
    for(int col = 0; col < 8; col++){
      int bit = (line >> col) & 1;
      if (bit) 
        fill_rect(buf, x + col * scale, x + col * scale + scale - 1, 
                       y + row * scale, y + row * scale + scale - 1, color);
    }
  }
}

void draw_text(uint8_t *buf, int x, int y, const char *str, int color, int scale){
  for(int i = 0; str[i] != '\0'; i++){
    draw_char(buf, x + i * 8 * scale, y, str[i], color, scale);
  }
}

void draw_bar_chart(uint8_t *buf, const BarChartConfig *cfg, float *data, int n){
  //axes
  draw_vline(buf, cfg->x0, cfg->y0, cfg->y1, 1, 1);
  draw_hline(buf, cfg->x0, cfg->x1, cfg->y0, 1, 1);

  //data
  //normalize
  float max = data[0];
  for(int i = 0; i < n; i++){
    if(data[i] > max) max = data[i];
  }

  float single_space = (cfg->x1 - cfg->x0) / n;

  for(int i = 0; i < n; i++){
    draw_vline(buf, cfg->x0 + single_space * (i + 0.5), cfg->y0, cfg->y0 + data[i] * (cfg->y1 - cfg->y0) / max, (single_space / 2) * 0.6, 1);
  }
}

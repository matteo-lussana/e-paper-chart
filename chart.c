#include "chart.h"
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
  int bit_index  = 7 - (x % 8) ;

  if (color)
    buf[byte_index] |= (1 << bit_index);
  else
    buf[byte_index] &= ~(1 << bit_index);
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


#pragma once
#include <stdint.h>

#define WIDTH 128
#define HEIGHT 68

#define STRIDE (WIDTH / 8)

void save_pbm(const char *filename, uint8_t *buf);
void set_pixel(uint8_t *buf, int x, int y, int color);
void draw_hline(uint8_t *buf, int x0, int x1, int y, int thickness, int color);
void draw_vline(uint8_t *buf, int x,  int y0, int y1, int thickness, int color);

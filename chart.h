#pragma once
#include <stdint.h>

#define WIDTH 128
#define HEIGHT 68

#define STRIDE (WIDTH / 8)

void save_pbm(const char *filename, uint8_t *buf);
void set_pixel(uint8_t *buf, int x, int y, int color);

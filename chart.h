#pragma once
#include <stdint.h>

#define WIDTH 792
#define HEIGHT 272

#define STRIDE (WIDTH / 8)

typedef struct {
    int x0, x1, y0, y1;
} BarChartConfig;

void save_pbm(const char *filename, uint8_t *buf);
void set_pixel(uint8_t *buf, int x, int y, int color);
void draw_hline(uint8_t *buf, int x0, int x1, int y, int thickness, int color);
void draw_vline(uint8_t *buf, int x,  int y0, int y1, int thickness, int color);
void draw_rect(uint8_t *buf, int x0, int x1, int y0, int y1, int thickness, int color);
void fill_rect(uint8_t *buf, int x0, int x1, int y0, int y1, int color);

//charts
void draw_bar_chart(uint8_t *buf, const BarChartConfig *cfg, float *data, int n);

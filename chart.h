#pragma once
#include <stdint.h>
#include <stdbool.h>

#define WIDTH 792
#define HEIGHT 272

#define STRIDE (WIDTH / 8)

typedef struct {
  int thickness;
  int title_size;
  char *x_title;
  char *y_title;
  int y_steps;
  bool dash_line;
} AxisConfig;

typedef struct {
  int x0, x1, y0, y1;
  bool values_label;
  AxisConfig axisConfig;
} BarChartConfig;

typedef struct {
  int x0, x1, y0, y1;
  int line_type;
  int line_thickness;
  int line_color;
  bool values_label;
  AxisConfig axisConfig;
} LineChartConfig;

typedef struct {
  int cx, cy;
  int radius;
  int thickness;
  int color;
  bool values_label;
  bool names_label;
} PieChartConfig;

#ifdef __cplusplus
extern "C" {
#endif

void save_pbm(const char *filename, uint8_t *buf);
void set_pixel(uint8_t *buf, int x, int y, int color);
int get_pixel(uint8_t *buf, int x, int y);
void draw_hline(uint8_t *buf, int x0, int x1, int y, int thickness, int color, int type);
void draw_vline(uint8_t *buf, int x, int y0, int y1, int thickness, int color, int type);
void draw_line(uint8_t *buf, int x0, int y0, int x1, int y1, int thickness, int color, int type);
void draw_rect(uint8_t *buf, int x0, int x1, int y0, int y1, int thickness, int color, int line_type);
void fill_rect(uint8_t *buf, int x0, int x1, int y0, int y1, int color);
void draw_circle(uint8_t *buf, int cx, int cy, int radius, int thickness, int color);

//text
void draw_char(uint8_t *buf, int x, int y, char c, int color, int scale, int rotation);
void draw_text(uint8_t *buf, int x, int y, const char *str, int color, int scale, int rotation);

//charts
void draw_bar_chart(uint8_t *buf, const BarChartConfig *cfg, char **x_data, float *y_data, int n);
void draw_line_chart(uint8_t *buf, const LineChartConfig *cfg, char **x_data, float *y_data, int n);
void draw_pie_chart(uint8_t *buf, const PieChartConfig *cfg, char **x_data, float *y_data, int n);

#ifdef __cplusplus
}
#endif

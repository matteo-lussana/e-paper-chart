#pragma once
#include <stdint.h>
#include <stdbool.h>

#define WIDTH 792
#define HEIGHT 272

#define STRIDE (WIDTH / 8)

typedef enum {
  COLOR_WHITE = 0,
  COLOR_BLACK = 1,
  COLOR_GRAY  = 2
} Color;

typedef enum {
  LINE_SOLID  = 0,
  LINE_DASHED = 1,
  LINE_DOTTED = 2
} LineType;

typedef struct {
  bool legend;
  char **labels;
} LegendConfig;

typedef struct {
  int thickness;
  int title_size;
  char *x_title;
  char *y_title;
  int y_steps;
  bool dash_line;
} AxisConfig;

typedef struct {
  int thickness;
  int title_size;
  char *x_title;
  char *y_title_left;
  char *y_title_right;
  int y_steps_left;
  int y_steps_right;
  bool dash_line_left;
  bool dash_line_right;
} DoubleAxisConfig;

typedef struct {
  int x0, x1, y0, y1;
  bool values_label;
  AxisConfig axisConfig;
  LegendConfig legendConfig;
} BarChartConfig;

typedef struct {
  int x0, x1, y0, y1;
  bool values_label;
  DoubleAxisConfig doubleAxisConfig;
  LegendConfig legendConfig;
} DoubleAxisBarChartConfig;

typedef struct {
  int x0, x1, y0, y1;
  LineType line_type;
  int line_thickness;
  Color line_color;
  bool values_label;
  AxisConfig axisConfig;
} LineChartConfig;

typedef struct {
  int cx, cy;
  int radius;
  int thickness;
  Color color;
  bool values_label;
  bool names_label;
} PieChartConfig;

typedef struct{
  int x0, y0;
  int border_width;
  int rows_num;
  int columns_num;
  int square_size;
  int square_distance;
} FreqChartConfig;

#ifdef __cplusplus
extern "C" {
#endif
/********************** UTILITY ***************************/
/**
 * Save the framebuffer in a Portable BitMap file (.bpm). Used for test purpose.
 *
 * @param filename      name of the file
 * @param buf           framebuffer
 * /
void save_pbm(const char *filename, uint8_t *buf);
/**
 * It set the color of a single pixel in the framebuffer
 *
 * @param buf           framebuffer
 * @param x,y           pixel coordinates
 * @param color         pixel color (COLOR_WHITE = 0, COLOR_BLACK = 1, COLOR_GRAY = 2)
 */
void set_pixel(uint8_t *buf, int x, int y, Color color);

/**
 * Return the color of a single pixel in the framebuffer
 *
 * @param buf           framebuffer
 * @param x,y           pixel coordinates
 * @return Color of the pixel
 */
int get_pixel(uint8_t *buf, int x, int y);

/**
 * Draw horizontal line
 *
 * @param buf           framebuffer
 * @param x0, x1        starting and final point on x-axis
 * @param y             value on y-axis
 * @param thickness     thickness of the line
 * @param color         line color (COLOR_WHITE = 0, COLOR_BLACK = 1, COLOR_GRAY = 2)
 * @param type          line type (LINE_SOLID  = 0, LINE_DASHED = 1, LINE_DOTTED = 2)
 */
void draw_hline(uint8_t *buf, int x0, int x1, int y, int thickness, Color color, LineType type);

/**
 * Draw vertical line
 *
 * @param buf           framebuffer
 * @param x             value on x-axis
 * @param y0, y1        starting and final point on y-axis
 * @param thickness     thickness of the line
 * @param color         line color (COLOR_WHITE = 0, COLOR_BLACK = 1, COLOR_GRAY = 2)
 * @param type          line type (LINE_SOLID  = 0, LINE_DASHED = 1, LINE_DOTTED = 2)
 */
void draw_vline(uint8_t *buf, int x, int y0, int y1, int thickness, Color color, LineType type);

/**
 * Draw line in every Direction
 *
 * @param buf           framebuffer
 * @param x0, y0        starting point
 * @param x1, y1        final point
 * @param thickness     thickness of the line
 * @param color         line color (COLOR_WHITE = 0, COLOR_BLACK = 1, COLOR_GRAY = 2)
 * @param type          line type (LINE_SOLID  = 0, LINE_DASHED = 1, LINE_DOTTED = 2)
 */
void draw_line(uint8_t *buf, int x0, int y0, int x1, int y1, int thickness, Color color, LineType type);

/**
 * Draw a rectangle
 *
 * @param buf           framebuffer
 * @param x0, y0        starting point
 * @param x1, y1        final point
 * @param thickness     thickness of the line
 * @param color         line color (COLOR_WHITE = 0, COLOR_BLACK = 1, COLOR_GRAY = 2)
 * @param type          line type (LINE_SOLID  = 0, LINE_DASHED = 1, LINE_DOTTED = 2)
 */
void draw_rect(uint8_t *buf, int x0, int x1, int y0, int y1, int thickness, Color color, LineType line_type);

/**
 * Fill a rectangle
 *
 * @param buf           framebuffer
 * @param x0, y0        starting point
 * @param x1, y1        final point
 * @param color         line color (COLOR_WHITE = 0, COLOR_BLACK = 1, COLOR_GRAY = 2)
 */
void fill_rect(uint8_t *buf, int x0, int x1, int y0, int y1, Color color);

/**
 * Draw a circle
 *
 * @param buf           framebuffer
 * @param cx, cy        center coordinates
 * @param radius        circle radius
 * @param thickness     thickness of the line
 * @param color         line color (COLOR_WHITE = 0, COLOR_BLACK = 1, COLOR_GRAY = 2)
 */
void draw_circle(uint8_t *buf, int cx, int cy, int radius, int thickness, Color color);


/********************** TEXT ***************************/
/**
 * Write single character
 *
 * @param buf           framebuffer
 * @param x, y          starting coordinates
 * @param c             character to write
 * @param color         line color (COLOR_WHITE = 0, COLOR_BLACK = 1, COLOR_GRAY = 2)
 * @param scale         size of the character (> 0)
 * @param rotation      rotation of the char clockwise (0, 90, 180, 270)
 */
void draw_char(uint8_t *buf, int x, int y, char c, Color color, int scale, int rotation);

/**
 * Write entire string
 *
 * @param buf           framebuffer
 * @param x, y          starting coordinates
 * @param str           string to write
 * @param color         line color (COLOR_WHITE = 0, COLOR_BLACK = 1, COLOR_GRAY = 2)
 * @param scale         size of the character (> 0)
 * @param rotation      rotation of the char clockwise (0, 90, 180, 270)
 */
void draw_text(uint8_t *buf, int x, int y, const char *str, Color color, int scale, int rotation);

/********************** CHARTS ***************************/
/**
 * Draw bar chart
 *
 * @param buf           framebuffer
 * @param cfg           config for the bar char (see BarChartConfig)
 * @param x_data        labels for the x-axis
 * @param y_data        actual data of the chart
 * @param n             lenght of the x_data and y_data
 */
void draw_bar_chart(uint8_t *buf, const BarChartConfig *cfg, char **x_data, float *y_data, int n);

/**
 * Draw bar chart with multiple bars
 *
 * @param buf           framebuffer
 * @param cfg           config for the bar char (see BarChartConfig)
 * @param x_data        labels for the x-axis
 * @param y_data        actual data of the chart
 * @param data_length   lenght of the x_data and y_data
 * @param data_sets     number of the datasets. number of column per label
 */
void draw_multi_bar_chart(uint8_t *buf, const BarChartConfig *cfg, char **x_data, float **y_data, int data_length, int data_sets);

/**
 * Draw bar chart with two y-axis and two bars
 *
 * @param buf           framebuffer
 * @param cfg           config for the bar char (see DoubleAxisBarChartConfig)
 * @param x_data        labels for the x-axis
 * @param y_data_left   actual data of the chart for the left y-axis
 * @param y_data_right  actual data of the chart for the left x-axis
 * @param data_length   lenght of the x_data, y_data_left, and y_data_right
 */
void draw_double_axis_bar_chart(uint8_t *buf, const DoubleAxisBarChartConfig *cfg, char **x_data, float *y_data_left, float *ydata_right, int data_length);

/**
 * Draw line chart
 *
 * @param buf           framebuffer
 * @param cfg           config for the bar char (see LineChartConfig)
 * @param x_data        labels for the x-axis
 * @param y_data        actual data of the chart
 * @param n             lenght of the x_data and y_data
 */
void draw_line_chart(uint8_t *buf, const LineChartConfig *cfg, char **x_data, float *y_data, int n);

/**
 * Draw pie chart
 *
 * @param buf           framebuffer
 * @param cfg           config for the bar char (see PieChartConfig)
 * @param x_data        labels for the slice
 * @param y_data        actual data of the chart
 * @param n             lenght of the x_data and y_data
 */
void draw_pie_chart(uint8_t *buf, const PieChartConfig *cfg, char **x_data, float *y_data, int n);

/**
 * Draw heat-map/frequency chart
 *
 * @param buf           framebuffer
 * @param cfg           config for the bar char (see PieChartConfig)
 * @param data          actual data of the chart. array with 0, 1, 2 for colorign white, black, gray every square
 * @param n             lenght of the data. number of squares
 */
void draw_freq_chart(uint8_t *buf, const FreqChartConfig *cfg, int *data, int n);

#ifdef __cplusplus
}
#endif

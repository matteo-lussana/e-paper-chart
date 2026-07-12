/**
 * @file chart.h
 * @brief C graphics library for e-paper displays: bar, multi-bar, double-axis,
 *        line, pie and frequency charts, plus text and drawing primitives.
 *
 * Pure graphics layer: it operates on a 1 bit-per-pixel framebuffer with no
 * hardware dependencies. Develop and test on PC with gcc (exports a PBM image),
 * then hand the framebuffer to the display driver on the target device.
 *
 * @author  Matteo Lussana
 * @version 0.1.0
 * @license MIT
 */
#pragma once
#include <stdint.h>
#include <stdbool.h>

#define WIDTH 792
#define HEIGHT 272

#define STRIDE (WIDTH / 8)

typedef enum {
  COLOR_WHITE = 0,            /**< white (pixel off)*/
  COLOR_BLACK = 1,            /**< black (pixel on)*/
  COLOR_GRAY  = 2             /**< gray, simulated with a checkerboard dithering*/
} Color;

typedef enum {
  LINE_SOLID  = 0,            /**< solid line*/
  LINE_DASHED = 1,            /**< dashed line*/
  LINE_DOTTED = 2             /**< dotted line*/
} LineType;

typedef struct {
  bool legend;                /**< presence of the legend*/
  char **labels;              /**< labels for the legend*/
} LegendConfig;

typedef struct {
  int thickness;              /**< thickness of the axis*/
  int title_size;             /**< size of the title (> 0)*/
  char *x_title;              /**< title of the x-axis*/
  char *y_title;              /**< title of the y-axis*/
  int y_steps;                /**< number of ticks on the y-axis*/
  bool dash_line;             /**< presence of horizontal dashed lines for each tick on y-axis*/
} AxisConfig;

typedef struct {
  int thickness;              /**< thickness of the two axes*/
  int title_size;             /**< size of the titles (> 0)*/
  char *x_title;              /**< title of the x-axis*/
  char *y_title_left;         /**< title of the left y-axis*/
  char *y_title_right;        /**< title of the right y-axis*/
  int y_steps_left;           /**< number of ticks on the left y-axis*/
  int y_steps_right;          /**< number of ticks on the right y-axis*/
  bool dash_line_left;        /**< presence of horizontal dashed lines for the left y-axis*/
  bool dash_line_right;       /**< presence of horizontal dashed lines for the right y-axis*/
} DoubleAxisConfig;

typedef struct {
  int x0, x1, y0, y1;         /**< chart area: top-left (x0,y0) and bottom-right (x1,y1) corners in pixel*/
  bool values_label;          /**< show the value above each bar*/
  AxisConfig axisConfig;      /**< axis configuration (see AxisConfig)*/
  LegendConfig legendConfig;  /**< legend configuration (see LegendConfig)*/
} BarChartConfig;

typedef struct {
  int x0, x1, y0, y1;                 /**< chart area: top-left (x0,y0) and bottom-right (x1,y1) corners in pixel*/
  bool values_label;                  /**< show the value above each bar*/
  DoubleAxisConfig doubleAxisConfig;  /**< double-axis configuration (see DoubleAxisConfig)*/
  LegendConfig legendConfig;          /**< legend configuration (see LegendConfig)*/
} DoubleAxisBarChartConfig;

typedef struct {
  int x0, x1, y0, y1;         /**< chart area: top-left (x0,y0) and bottom-right (x1,y1) corners in pixel*/
  LineType line_type;         /**< line style (LINE_SOLID = 0, LINE_DASHED = 1, LINE_DOTTED = 2)*/
  int line_thickness;         /**< thickness of the line*/
  Color line_color;           /**< color of the line (COLOR_WHITE = 0, COLOR_BLACK = 1, COLOR_GRAY = 2)*/
  bool values_label;          /**< show the value above each point*/
  AxisConfig axisConfig;      /**< axis configuration (see AxisConfig)*/
} LineChartConfig;

typedef struct {
  int cx, cy;                 /**< center coordinates of the pie in pixel*/
  int radius;                 /**< radius of the pie*/
  int thickness;              /**< thickness of the outline and slice dividers*/
  Color color;                /**< color of the outline and dividers (COLOR_WHITE = 0, COLOR_BLACK = 1, COLOR_GRAY = 2)*/
  bool values_label;          /**< show the percentage on each slice*/
  bool names_label;           /**< show the label name on each slice*/
} PieChartConfig;

typedef struct{
  int x0, y0;                 /**< top-left corner of the grid in pixel*/
  int border_width;           /**< thickness of each cell border*/
  int rows_num;               /**< number of rows*/
  int columns_num;            /**< number of columns*/
  int square_size;            /**< side length of each square in pixel*/
  int square_distance;        /**< gap between adjacent squares in pixel*/
} FreqChartConfig;

#ifdef __cplusplus
extern "C" {
#endif
/********************** UTILITY ***************************/
/**
 * @brief Save the framebuffer in a Portable BitMap file (.pbm). Used for test purpose.
 *
 * @param filename      name of the file
 * @param buf           framebuffer
 */
void save_pbm(const char *filename, uint8_t *buf);

/**
 * @brief It set the color of a single pixel in the framebuffer
 *
 * @param buf           framebuffer
 * @param x,y           pixel coordinates
 * @param color         pixel color (COLOR_WHITE = 0, COLOR_BLACK = 1, COLOR_GRAY = 2)
 */
void set_pixel(uint8_t *buf, int x, int y, Color color);

/**
 * @brief Return the color of a single pixel in the framebuffer
 *
 * @param buf           framebuffer
 * @param x,y           pixel coordinates
 * @return Color of the pixel
 */
int get_pixel(uint8_t *buf, int x, int y);

/**
 * @brief Draw horizontal line
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
 * @brief Draw vertical line
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
 * @brief Draw line in every Direction
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
 * @brief Draw a rectangle
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
 * @brief Fill a rectangle
 *
 * @param buf           framebuffer
 * @param x0, y0        starting point
 * @param x1, y1        final point
 * @param color         fill color (COLOR_WHITE = 0, COLOR_BLACK = 1, COLOR_GRAY = 2)
 */
void fill_rect(uint8_t *buf, int x0, int x1, int y0, int y1, Color color);

/**
 * @brief Draw a circle
 *
 * @param buf           framebuffer
 * @param cx, cy        center coordinates
 * @param radius        circle radius
 * @param thickness     thickness of the line
 * @param color         circle color (COLOR_WHITE = 0, COLOR_BLACK = 1, COLOR_GRAY = 2)
 */
void draw_circle(uint8_t *buf, int cx, int cy, int radius, int thickness, Color color);


/********************** TEXT ***************************/
/**
 * @brief Write single character
 *
 * @param buf           framebuffer
 * @param x, y          starting coordinates
 * @param c             character to write
 * @param color         char color (COLOR_WHITE = 0, COLOR_BLACK = 1, COLOR_GRAY = 2)
 * @param scale         size of the character (> 0)
 * @param rotation      rotation of the char clockwise (0, 90, 180, 270)
 */
void draw_char(uint8_t *buf, int x, int y, char c, Color color, int scale, int rotation);

/**
 * @brief Write entire string
 *
 * @param buf           framebuffer
 * @param x, y          starting coordinates
 * @param str           string to write
 * @param color         text color (COLOR_WHITE = 0, COLOR_BLACK = 1, COLOR_GRAY = 2)
 * @param scale         size of the character (> 0)
 * @param rotation      rotation of the char clockwise (0, 90, 180, 270)
 */
void draw_text(uint8_t *buf, int x, int y, const char *str, Color color, int scale, int rotation);

/********************** CHARTS ***************************/
/**
 * @brief Draw bar chart
 *
 * @param buf           framebuffer
 * @param cfg           config for the bar char (see BarChartConfig)
 * @param x_data        labels for the x-axis
 * @param y_data        actual data of the chart
 * @param n             length of the x_data and y_data
 */
void draw_bar_chart(uint8_t *buf, const BarChartConfig *cfg, char **x_data, float *y_data, int n);

/**
 * @brief Draw bar chart with multiple bars
 *
 * @param buf           framebuffer
 * @param cfg           config for the bar char (see BarChartConfig)
 * @param x_data        labels for the x-axis
 * @param y_data        actual data of the chart
 * @param data_length   length of the x_data and y_data
 * @param data_sets     number of the datasets. number of column per label
 */
void draw_multi_bar_chart(uint8_t *buf, const BarChartConfig *cfg, char **x_data, float **y_data, int data_length, int data_sets);

/**
 * @brief Draw bar chart with two y-axis and two bars
 *
 * @param buf           framebuffer
 * @param cfg           config for the bar char (see DoubleAxisBarChartConfig)
 * @param x_data        labels for the x-axis
 * @param y_data_left   actual data of the chart for the left y-axis
 * @param y_data_right  actual data of the chart for the right y-axis
 * @param data_length   length of the x_data, y_data_left, and y_data_right
 */
void draw_double_axis_bar_chart(uint8_t *buf, const DoubleAxisBarChartConfig *cfg, char **x_data, float *y_data_left, float *ydata_right, int data_length);

/**
 * @brief Draw line chart
 *
 * @param buf           framebuffer
 * @param cfg           config for the bar char (see LineChartConfig)
 * @param x_data        labels for the x-axis
 * @param y_data        actual data of the chart
 * @param n             length of the x_data and y_data
 */
void draw_line_chart(uint8_t *buf, const LineChartConfig *cfg, char **x_data, float *y_data, int n);

/**
 * @brief Draw pie chart
 *
 * @param buf           framebuffer
 * @param cfg           config for the bar char (see PieChartConfig)
 * @param x_data        labels for the slice
 * @param y_data        actual data of the chart
 * @param n             length of the x_data and y_data
 */
void draw_pie_chart(uint8_t *buf, const PieChartConfig *cfg, char **x_data, float *y_data, int n);

/**
 * @brief Draw heat-map/frequency chart
 *
 * @param buf           framebuffer
 * @param cfg           config for the bar char (see FreqChartConfig)
 * @param data          actual data of the chart. array with 0, 1, 2 for coloring white, black, gray every square
 * @param n             length of the data. number of squares
 */
void draw_freq_chart(uint8_t *buf, const FreqChartConfig *cfg, int *data, int n);

#ifdef __cplusplus
}
#endif

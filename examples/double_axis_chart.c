/* Dual-axis bar chart — two series on two independent y-axes, x-axis at the
 * bottom, with a legend. Left series is black, right series is gray.
 * Build (from the repo root):
 *   gcc examples/double_axis_chart.c chart.c -o double_axis && ./double_axis   # -> double_axis.pbm
 */
#include "../chart.h"
#include <string.h>

uint8_t framebuffer[STRIDE * HEIGHT];

int main(void)
{
    memset(framebuffer, 0, sizeof framebuffer);

    char  *labels[]  = {"Mon", "Tue", "Wed", "Thu", "Fri"};
    float  minutes[] = {45, 60, 30, 75, 50};   /* left axis  */
    float  laps[]    = {20, 28, 14, 34, 24};   /* right axis */
    char  *series[]  = {"minutes", "laps"};

    LegendConfig legend = {
        .legend = true, .labels = series, .legend_size = 1
    };
    DoubleAxisConfig axis = {
        .thickness = 2, .title_size = 2,
        .x_title = "day",
        .y_title_left = "minutes", .y_title_right = "laps",
        .y_steps_left = 5, .y_steps_right = 4,
        .dash_line_left = true
    };
    DoubleAxisBarChartConfig cfg = {
        .x0 = 8, .x1 = 784, .y0 = 12, .y1 = 262,
        .values_label = false,
        .doubleAxisConfig = axis,
        .legendConfig = legend
    };

    draw_double_axis_bar_chart(framebuffer, &cfg, labels, minutes, laps, 5, ORIENT_BOTTOM_AXIS);

    save_pbm("double_axis.pbm", framebuffer);
    return 0;
}

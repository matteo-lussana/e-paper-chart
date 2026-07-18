/* Scatter chart — one filled dot per value, x-axis at the bottom.
 * Build (from the repo root):
 *   gcc examples/scatter_chart.c chart.c -o scatter && ./scatter   # -> scatter.pbm
 */
#include "../chart.h"
#include <string.h>

uint8_t framebuffer[STRIDE * HEIGHT];

int main(void)
{
    memset(framebuffer, 0, sizeof framebuffer);

    char  *labels[] = {"1", "2", "3", "4", "5", "6", "7", "8"};
    float  values[] = {20, 35, 15, 42, 28, 50, 33, 46};

    AxisConfig axis = {
        .thickness = 2, .title_size = 2,
        .x_title = "sample", .y_title = "value",
        .y_steps = 5, .dash_line = true
    };
    ScatterChartConfig cfg = {
        .x0 = 8, .x1 = 784, .y0 = 12, .y1 = 262,
        .dots_size = 6,
        .axisConfig = axis
    };

    draw_scatter_chart(framebuffer, &cfg, labels, values, 8, ORIENT_BOTTOM_AXIS);

    save_pbm("scatter.pbm", framebuffer);
    return 0;
}

/* Bar chart — x-axis at the bottom, value labels on each bar.
 * Build (from the repo root):
 *   gcc examples/bar_chart.c chart.c -o bar && ./bar   # -> bar.pbm
 */
#include "../chart.h"
#include <string.h>

uint8_t framebuffer[STRIDE * HEIGHT];

int main(void)
{
    memset(framebuffer, 0, sizeof framebuffer);   /* clear to white */

    char  *labels[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun"};
    float  values[] = {12, 30, 7, 45, 22, 38};

    AxisConfig axis = {
        .thickness = 2, .title_size = 2,
        .x_title = "month", .y_title = "km",
        .y_steps = 5, .dash_line = true
    };
    BarChartConfig cfg = {
        .x0 = 8, .x1 = 784, .y0 = 12, .y1 = 262,
        .values_label = true,
        .axisConfig = axis
    };

    draw_bar_chart(framebuffer, &cfg, labels, values, 6, ORIENT_BOTTOM_AXIS);

    save_pbm("bar.pbm", framebuffer);
    return 0;
}

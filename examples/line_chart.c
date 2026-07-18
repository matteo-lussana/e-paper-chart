/* Line chart — x-axis at the bottom, value labels on each point.
 * Build (from the repo root):
 *   gcc examples/line_chart.c chart.c -o line && ./line   # -> line.pbm
 */
#include "../chart.h"
#include <string.h>

uint8_t framebuffer[STRIDE * HEIGHT];

int main(void)
{
    memset(framebuffer, 0, sizeof framebuffer);

    char  *labels[] = {"W1", "W2", "W3", "W4", "W5", "W6", "W7", "W8"};
    float  values[] = {8, 14, 11, 20, 17, 26, 22, 30};

    AxisConfig axis = {
        .thickness = 2, .title_size = 2,
        .x_title = "week", .y_title = "pace",
        .y_steps = 5, .dash_line = true
    };
    LineChartConfig cfg = {
        .x0 = 8, .x1 = 784, .y0 = 12, .y1 = 262,
        .line_type = LINE_SOLID,
        .line_thickness = 2,
        .values_label = true,
        .axisConfig = axis
    };

    draw_line_chart(framebuffer, &cfg, labels, values, 8, ORIENT_BOTTOM_AXIS);

    save_pbm("line.pbm", framebuffer);
    return 0;
}

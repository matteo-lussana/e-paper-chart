/* Grouped bar chart (multiple series) — x-axis at the bottom, with a legend.
 * Series colors: 0 = black, 1 = gray, 2 = white/outlined (they match the legend).
 * Build (from the repo root):
 *   gcc examples/multi_bar_chart.c chart.c -o multi_bar && ./multi_bar   # -> multi_bar.pbm
 */
#include "../chart.h"
#include <string.h>

uint8_t framebuffer[STRIDE * HEIGHT];

int main(void)
{
    memset(framebuffer, 0, sizeof framebuffer);

    char *labels[] = {"Q1", "Q2", "Q3", "Q4"};

    /* one array per group (quarter), holding the value of each series */
    float *values[] = {
        (float[]){30, 22, 15},   /* Q1: 2022, 2023, 2024 */
        (float[]){45, 30, 20},   /* Q2 */
        (float[]){20, 40, 10},   /* Q3 */
        (float[]){38, 25, 33}    /* Q4 */
    };
    char *series[] = {"2022", "2023", "2024"};

    LegendConfig legend = {
        .legend = true, .labels = series, .legend_size = 1
    };
    AxisConfig axis = {
        .thickness = 2, .title_size = 2,
        .x_title = "quarter", .y_title = "sales",
        .y_steps = 5, .dash_line = true
    };
    BarChartConfig cfg = {
        .x0 = 8, .x1 = 784, .y0 = 12, .y1 = 262,
        .values_label = false,
        .axisConfig = axis,
        .legendConfig = legend
    };

    /* data_length = 4 groups, data_sets = 3 series */
    draw_multi_bar_chart(framebuffer, &cfg, labels, values, 4, 3, ORIENT_BOTTOM_AXIS);

    save_pbm("multi_bar.pbm", framebuffer);
    return 0;
}

/* Frequency heatmap — a grid of cells, one color per value
 * (0 = white, 1 = black, 2 = gray). Data is column-major: all the rows of
 * column 0, then column 1, and so on.
 * Build (from the repo root):
 *   gcc examples/freq_chart.c chart.c -o freq && ./freq   # -> freq.pbm
 */
#include "../chart.h"
#include <string.h>

#define COLS 12   /* months */
#define ROWS 4    /* buckets per month */

uint8_t framebuffer[STRIDE * HEIGHT];

int main(void)
{
    memset(framebuffer, 0, sizeof framebuffer);

    int data[COLS * ROWS] = {
        0,1,2,0,  2,2,1,0,  1,0,0,2,  2,1,2,1,
        0,0,1,2,  1,2,2,0,  2,1,0,1,  0,2,1,2,
        1,1,2,0,  2,0,1,1,  0,2,2,1
    };

    FreqChartConfig cfg = {
        .x0 = 88, .y0 = 44,
        .border_width = 2,
        .rows_num = ROWS, .columns_num = COLS,
        .square_size = 40, .square_distance = 12
    };

    /* month label above each column */
    char *month[] = {"Jan","Feb","Mar","Apr","May","Jun",
                     "Jul","Aug","Sep","Oct","Nov","Dec"};
    int x = cfg.x0 + cfg.square_size / 2 - 8 / 2;
    for (int i = 0; i < COLS; i++) {
        draw_text(framebuffer, x, cfg.y0 - 12, month[i], COLOR_BLACK, 1, 0);
        x += cfg.square_size + cfg.square_distance;
    }

    draw_freq_chart(framebuffer, &cfg, data, COLS * ROWS);

    save_pbm("freq.pbm", framebuffer);
    return 0;
}

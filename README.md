# ChartLib

> A small, dependency-free **C library for drawing charts on e-paper displays** — bars, lines, dual-axis, heatmaps and more, rendered on a 1 bit-per-pixel framebuffer.

Built and tested on a **CrowPanel ESP32 E-Paper 5.79"**, but the graphics layer is pure C with **no hardware dependencies**: it draws into a `uint8_t` framebuffer that you then hand to any display driver.

## Gallery

| Bar chart | Line chart |
|-----------|------------|
| ![bar](docs/bar.png) | ![line](docs/line.png) |

| Dual-axis bar chart | Frequency heatmap |
|---------------------|-------------------|
| ![double axis](docs/double_axis.png) | ![heatmap](docs/freq.png) |

## Features

- **Charts**: bar, multi-bar, dual-axis bar, line, frequency heatmap *(pie chart is experimental)*
- **Primitives**: pixels, lines (any angle, via Bresenham), rectangles, circles, filled shapes
- **Text**: built-in 8×8 font with scaling and 0/90/180/270° rotation
- **Three "colors" on black & white**: white, black, and **gray simulated with dithering** — so you can tell series apart without real color
- **Line styles**: solid, dashed, dotted; configurable thickness
- **Axes**: titles, ticks, value labels, dashed gridlines
- **Pure C, zero dependencies** — runs on a PC (for testing) and on microcontrollers

## Installation

Copy three files into your project:

```
chart.c   chart.h   font8x8_basic.h
```

That's it — no build system, no external libraries.

## Quick start

```c
#include "chart.h"
#include <string.h>

uint8_t framebuffer[STRIDE * HEIGHT];   // 1 bit-per-pixel buffer

int main(void) {
    memset(framebuffer, 0, sizeof framebuffer);   // clear to white

    char  *labels[] = {"Jan", "Feb", "Mar", "Apr"};
    float  values[] = {12, 30, 7, 45};

    AxisConfig axis = {
        .thickness = 2, .title_size = 2,
        .x_title = "month", .y_title = "km",
        .y_steps = 5, .dash_line = true
    };
    BarChartConfig cfg = {
        .x0 = 60, .x1 = 760, .y0 = 20, .y1 = 240,
        .values_label = true,
        .axisConfig = axis
    };

    draw_bar_chart(framebuffer, &cfg, labels, values, 4);

    // now send `framebuffer` to your display, or save it for testing:
    save_pbm("output.pbm", framebuffer);
    return 0;
}
```

Colors and line styles use named constants, e.g. `COLOR_BLACK`, `COLOR_GRAY`, `LINE_DASHED`.

## On a PC (development & testing)

The library ships with `save_pbm()`, which writes the framebuffer to a
[PBM image](https://en.wikipedia.org/wiki/Netpbm) you can open in any viewer:

```bash
gcc main.c chart.c -o test && ./test   # produces output.pbm
```

This is the fast dev loop: draw, save, look — no hardware needed.

## On Arduino / ESP32

The graphics layer stays hardware-agnostic. On the device you:

1. draw into your framebuffer with the library;
2. copy it into the display driver's buffer (handling the panel's pixel format);
3. refresh the display.

> **Note:** the `.ino` is compiled as C++. Since colors/line-styles are typed
> enums, pass the **names** (`COLOR_BLACK`, `LINE_DASHED`), not raw integers.

## API overview

| Function | Description |
|----------|-------------|
| `draw_bar_chart` | vertical bar chart |
| `draw_multi_bar_chart` | grouped bars (multiple series) |
| `draw_double_axis_bar_chart` | two series on two independent y-axes |
| `draw_line_chart` | line chart |
| `draw_freq_chart` | frequency heatmap (grid of cells) |
| `draw_pie_chart` | pie chart *(experimental)* |
| `set_pixel`, `draw_line`, `draw_rect`, `fill_rect`, `draw_circle` | drawing primitives |
| `draw_char`, `draw_text` | text with scaling and rotation |

Every public function and config struct is documented in
[`chart.h`](chart.h) (Doxygen style).

## License

[MIT](LICENSE) © Matteo Lussana

The bundled 8×8 font (`font8x8_basic.h`) is by Daniel Hepper and is released
into the **public domain**.

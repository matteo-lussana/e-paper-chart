#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define WIDTH 128
#define HEIGHT 68

#define STRIDE (WIDTH / 8)

uint8_t framebuffer[STRIDE * HEIGHT];

void set_pixel(uint8_t *buf, int x, int y, int color)
{
  /* check the edges */
  if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
    return;

  int byte_index = y * STRIDE + x / 8;
  int bit_index  = 7 - (x % 8) ;

  if (color)
    buf[byte_index] |= (1 << bit_index);   // accendi: quale operatore?
  else
    buf[byte_index] &= ~(1 << bit_index);  // spegni: quale operatore?

}

void save_pbm(const char *filename, uint8_t *buf)
{
  FILE *f = fopen(filename, "wb");
  if (!f) {
    fprintf(stderr, "Error: can not open file %s\n", filename);
    return;
  }

  fprintf(f, "P4\n%d %d\n", WIDTH, HEIGHT);

  /* write STRIDE byte for each row */
  for (int y = 0; y < HEIGHT; y++)
    fwrite(&buf[y * STRIDE], 1, STRIDE, f);

  fclose(f);
  printf("Saved: %s (%dx%d)\n", filename, WIDTH, HEIGHT);
}

int main(void)
{
  /* all buffer clean (all white) */
  memset(framebuffer, 0x00, sizeof(framebuffer));

  /* superior edge */
  for (int x = 0; x < WIDTH; x++)
    set_pixel(framebuffer, x, 0, 1);

  /* left edge */
  for (int y = 0; y < HEIGHT; y++)
    set_pixel(framebuffer, 0, y, 1);

  /* put an X in the middle */
  for (int i = 0; i < 20; i++) {
    set_pixel(framebuffer, 54 + i, 22 + i, 1);
    set_pixel(framebuffer, 73 - i, 22 + i, 1);
  }

  save_pbm("output.pbm", framebuffer);
  return 0;
}

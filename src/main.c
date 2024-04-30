#include <math.h>
#include <png.h>
#include <stdio.h>
#include <stdlib.h>

void print_char(int brightness, int alpha) {
  char *ascii_map = "`.,:;=+%&#@WMXQK";
  int index = (brightness / 16);
  if (alpha) {
    printf("%c", ascii_map[index]);
  } else {
    printf(" ");
  }
}

int main(int argc, char const *argv[]) {
  png_FILE_p fpng = fopen(argv[1], "rb");
  if (!fpng) {
    fprintf(stderr, "Could not open %s\n", argv[1]);
    return 1;
  }
  png_structp png =
      png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png) {
    fclose(fpng);
    fprintf(stderr, "Could not create PNG read struct\n");
    return 1;
  }

  png_infop info = png_create_info_struct(png);
  if (!info) {
    png_destroy_read_struct(&png, NULL, NULL);
    fclose(fpng);
    fprintf(stderr, "Could not create PNG info struct\n");
    return 1;
  }


  png_init_io(png, fpng);
  png_set_sig_bytes(png, 0);
  png_read_info(png, info);

  const unsigned int height = png_get_image_height(png, info);
  const unsigned int width = png_get_image_width(png, info);
  png_byte color_type = png_get_color_type(png, info);
  png_byte bit_depth = png_get_bit_depth(png, info);

  if (color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_RGBA) {
    png_bytep *row_pointers = (png_bytep *)malloc(sizeof(png_bytep) * height);
    for (int y = 0; y < height; y++) {
      row_pointers[y] = (png_byte *)malloc(png_get_rowbytes(png, info));
    }

    png_read_image(png, row_pointers);

    for (int y = 0; y < height; y++) {
      png_bytep row = row_pointers[y];
      for (int x = 0; x < width * 4; x += 4) {
        png_byte red = row[x];
        png_byte green = row[x + 1];
        png_byte blue = row[x + 2];
        png_byte alpha = row[x + 3];
        int brightness = (int)round(0.299 * red + 0.587 * green + 0.114 * blue);
        print_char(brightness, alpha);
      }
      printf("\n");
    }

    for (int y = 0; y < height; y++) {
      free(row_pointers[y]);
    }
    free(row_pointers);
  }

  png_destroy_read_struct(&png, &info, NULL);
  fclose(fpng);

  printf("Hello world\n");
  return 0;
}
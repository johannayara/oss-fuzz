#include <cstdint>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <vector>
#include <png.h>

// Dummy write callback to avoid file I/O
void png_dummy_write(png_structp /*png_ptr*/, png_bytep /*data*/, png_size_t /*length*/) {
    // Discard output
}

// Dummy flush callback
void png_dummy_flush(png_structp /*png_ptr*/) {
    // No-op
}

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    if (size < 8) return 0; // Too small to be interesting

    // Create write struct
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) return 0;

    // Ensure cleanup with RAII
    struct PngCleanup {
        png_structp& png_ptr;
        png_infop& info_ptr;
        ~PngCleanup() {
            png_destroy_write_struct(&png_ptr, &info_ptr);
        }
    };

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        return 0;
    }

    PngCleanup cleanup{png_ptr, info_ptr};

    if (setjmp(png_jmpbuf(png_ptr))) {
        return 0; // Error occurred
    }

    // Set dummy output to discard writes
    png_set_write_fn(png_ptr, nullptr, png_dummy_write, png_dummy_flush);

    // Use hardcoded minimal IHDR
    png_byte bit_depth = 8;
    png_byte color_type = PNG_COLOR_TYPE_GRAY;

    png_set_IHDR(png_ptr, info_ptr,
                 1,  // width
                 1,  // height
                 bit_depth,
                 color_type,
                 PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE,
                 PNG_FILTER_TYPE_BASE);
    /*
    png_create_write_struct();
    png_create_info_struct();
    png_init_io();
    png_set_IHDR();
    png_set_rows();
    png_write_info();
    png_write_image();
    png_write_end();
    */

    // Use first byte of input data for the pixel
    std::vector<png_byte> row(1);
    row[0] = data[0];
    png_bytep row_pointers[1] = { row.data() };

    // Invoke the target function
    png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, row_pointers);

    return 0;
}
/*
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <png.h>

// === Simple in-memory write struct ===
typedef struct {
    uint8_t *data;
    size_t size;
    size_t capacity;
} memory_buffer;

void png_memory_write(png_structp png_ptr, png_bytep data, png_size_t length) {
    memory_buffer *mem = (memory_buffer *)png_get_io_ptr(png_ptr);
    if (mem->size + length > mem->capacity) {
        size_t new_cap = mem->capacity * 2 + length;
        mem->data = realloc(mem->data, new_cap);
        mem->capacity = new_cap;
    }
    memcpy(mem->data + mem->size, data, length);
    mem->size += length;
}

void png_memory_flush(png_structp png_ptr) {
    // no-op
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    if (size < 64) return 0;  // not enough data

    // Basic fake PNG settings (limit dimensions to avoid OOMs)
    uint32_t width = data[0] % 64 + 1;
    uint32_t height = data[1] % 64 + 1;
    int bit_depth = 8;
    int color_type = PNG_COLOR_TYPE_RGB;

    // Allocate and fill image rows from fuzz data
    size_t rowbytes = width * 3; // 3 bytes per pixel
    png_bytep *rows = malloc(sizeof(png_bytep) * height);
    for (size_t y = 0; y < height; ++y) {
        rows[y] = malloc(rowbytes);
        memcpy(rows[y], data + 2 + y * rowbytes, (size > 2 + y * rowbytes) ? rowbytes : 0);
    }

    // Set up libpng structures
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) goto cleanup;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) goto cleanup;

    if (setjmp(png_jmpbuf(png_ptr))) goto cleanup;

    memory_buffer mem = {0};
    png_set_write_fn(png_ptr, &mem, png_memory_write, png_memory_flush);

    png_set_IHDR(png_ptr, info_ptr, width, height,
                 bit_depth, color_type,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    png_set_rows(png_ptr, info_ptr, rows);
    png_write_info(png_ptr, info_ptr);
    png_write_image(png_ptr, rows);
    png_write_end(png_ptr, NULL);

cleanup:
    if (png_ptr) png_destroy_write_struct(&png_ptr, &info_ptr);
    for (size_t y = 0; y < height; ++y) free(rows[y]);
    free(rows);
    free(mem.data);
    return 0;
}


*/

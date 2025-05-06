// libpng_write_fuzzer.cc
// Adapted from libpng_read_fuzzer.cc style for writing.

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

#define PNG_INTERNAL
#include "png.h"

// --- Custom write callbacks (discard output) ---
static void user_write_data(png_structp png_ptr,
                            png_bytep data,
                            size_t length) {
  (void)png_ptr; (void)data; (void)length;
}
static void user_flush(png_structp png_ptr) {
  (void)png_ptr;
}

// --- Custom memory limits, as in the read fuzzer ---
static void* limited_malloc(png_structp, png_alloc_size_t size) {
  if (size > 8000000) return nullptr;
  return malloc(size);
}
static void default_free(png_structp, png_voidp ptr) {
  free(ptr);
}

// --- Custom write handler ---
struct PngWriteHandler {
  png_structp png_ptr = nullptr; // The main structure that holds the state of the PNG writing process.
  png_infop   info_ptr = nullptr; //Holds metadata about the PNG file (e.g., width, height, bit depth, color type, optional chunks like gAMA, bKGD, etc.).
  png_voidp   row_buf  = nullptr; // A buffer to hold one row of pixel data during the writing process.

  ~PngWriteHandler() {
    if (row_buf) {
      png_free(png_ptr, row_buf);
      row_buf = nullptr;
    }
    if (png_ptr || info_ptr) {
      png_destroy_write_struct(&png_ptr, &info_ptr);
    }
  }
};

// --- Helpers to consume bytes from the fuzz input ---
static uint16_t read_u16(const uint8_t** ptr, size_t* remaining) {
  if (*remaining < 2) return 0;
  uint16_t v = (uint16_t)(*ptr)[0]     |
               (uint16_t)((*ptr)[1] << 8);
  *ptr += 2; *remaining -= 2;
  return v;
}
static uint8_t read_u8(const uint8_t** ptr, size_t* remaining) {
  if (*remaining < 1) return 0;
  uint8_t v = **ptr;
  *ptr += 1; *remaining -= 1;
  return v;
}

// --- Entry point for LibFuzzer ---
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data,
                                       size_t size) {
  // Need at least a few bytes to derive parameters
  if (size < 10) return 0; // must contain IHDR chunk + IDAT + IEND chunks
  const uint8_t* ptr = data;
  size_t         rem = size;

  PngWriteHandler handler;

  // 1) Create write structs
  handler.png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING,
                                      nullptr, nullptr, nullptr);
  if (!handler.png_ptr) return 0;
  handler.info_ptr = png_create_info_struct(handler.png_ptr);
  if (!handler.info_ptr) return 0;

  // 2) limit allocations
  png_set_mem_fn(handler.png_ptr, nullptr,
                 limited_malloc, default_free);

  // 3) Error handling
  if (setjmp(png_jmpbuf(handler.png_ptr))) {
    return 0;
  }

  // 4) Set up the write callback
  //   (we don't care about the output, so we use a no-op callback)
  png_set_write_fn(handler.png_ptr, nullptr,
                   user_write_data, user_flush);

  // 5) Derive IHDR parameters

  //   width, height: from 2 bytes each, scaled to [1..512]
  uint16_t rw = read_u16(&ptr, &rem);
  uint16_t rh = read_u16(&ptr, &rem);
  png_uint_32 width  = (rw % 511) + 1;
  png_uint_32 height = (rh % 511) + 1;

  //   bit_depth: map one byte to {1,2,4,8,16}
  uint8_t bd0 = read_u8(&ptr, &rem);
  int bit_depth;
  switch (bd0 % 5) {
    case 0: bit_depth = 1; break;
    case 1: bit_depth = 2; break;
    case 2: bit_depth = 4; break;
    case 3: bit_depth = 8; break;
    default: bit_depth = 16; break;
  }

  //   color_type: map one byte to the 5 major types
  uint8_t ct0 = read_u8(&ptr, &rem);
  int color_type;
  switch (ct0 % 5) {
    case 0: color_type = PNG_COLOR_TYPE_GRAY; break;
    case 1: color_type = PNG_COLOR_TYPE_PALETTE; break;
    case 2: color_type = PNG_COLOR_TYPE_RGB; break;
    case 3: color_type = PNG_COLOR_TYPE_GRAY_ALPHA; break;
    default: color_type = PNG_COLOR_TYPE_RGB_ALPHA; break;
  }

  //  Basic adjustments
  if (color_type == PNG_COLOR_TYPE_PALETTE && bit_depth > 8)
    bit_depth = 8;
  if ((color_type == PNG_COLOR_TYPE_GRAY_ALPHA ||
       color_type == PNG_COLOR_TYPE_RGB_ALPHA) &&
      bit_depth < 8)
    bit_depth = 8;

  //   interlace: 0 or 1
  uint8_t il0 = read_u8(&ptr, &rem);
  int interlace = (il0 & 1) ? PNG_INTERLACE_ADAM7
                            : PNG_INTERLACE_NONE;

  png_set_IHDR(handler.png_ptr, handler.info_ptr,
               width, height,
               bit_depth, color_type,
               interlace,
               PNG_COMPRESSION_TYPE_DEFAULT,
               PNG_FILTER_TYPE_DEFAULT);

  // 6) Other chunks: they are optional, so we use a 1-in-N chance (randomly)

  // gAMA: 1-in-3 chance
  if (rem >= 2 && (read_u8(&ptr, &rem) % 3) == 0) {
    uint16_t rg = read_u16(&ptr, &rem);
    double   gamma = (double)rg / 65535.0 * 2.9 + 0.1;
    png_set_gAMA(handler.png_ptr, handler.info_ptr, gamma);
  }

  // bKGD: 1-in-4 chance
  if (rem >= 6 && (read_u8(&ptr, &rem) % 4) == 0) {
    png_color_16 bkgd;
    bkgd.index = read_u8(&ptr, &rem);
    bkgd.red   = read_u16(&ptr, &rem);
    bkgd.green = read_u16(&ptr, &rem);
    bkgd.blue  = read_u16(&ptr, &rem);
    bkgd.gray  = read_u16(&ptr, &rem);
    png_set_bKGD(handler.png_ptr, handler.info_ptr, &bkgd);
  }

  // TODO: add more optional chunks

  // 7) Write info
  png_write_info(handler.png_ptr, handler.info_ptr);

  // 8) Allocate a row buffer and write rows
  png_size_t rowbytes = png_get_rowbytes(handler.png_ptr, handler.info_ptr);
  if (rowbytes == 0 || rowbytes > width * 4 * 2 + 100) {
    return 0;
  }
  handler.row_buf = png_malloc(handler.png_ptr, rowbytes);
  if (!handler.row_buf) return 0;

  // IDAT: write the image data
  const uint8_t* pix = ptr;
  size_t pix_rem = rem;
  size_t pix_off = 0;

  for (png_uint_32 y = 0; y < height; y++) {
    for (png_size_t i = 0; i < rowbytes; i++) {
      if (pix_rem) {
        ((png_byte*)handler.row_buf)[i] = pix[pix_off++ % pix_rem];
      } else {
        ((png_byte*)handler.row_buf)[i] = (png_byte)((y + i) % 256);
      }
    }
    png_write_row(handler.png_ptr, (png_bytep)handler.row_buf);
  }

  // 9) IEND: write the end chunk
  png_write_end(handler.png_ptr, handler.info_ptr);

  // Cleanup happens in ~PngWriteHandler()
  return 0;
}

// libpng_write_fuzzer.cc
// Adapted from libpng_read_fuzzer.cc style for writing.

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <time.h>

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
  png_bytepp rows  = nullptr; // A buffer to hold one row of pixel data during the writing process.

  ~PngWriteHandler() {
    if (rows) {
      for (png_uint_32 y = 0; y < png_get_image_height(png_ptr, info_ptr); y++) {
        png_free(png_ptr, rows[y]);
      }
      png_free(png_ptr, rows);
      rows = nullptr;
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
// --- New helper to read 32-bit little-endian from the fuzz input ---
static uint32_t read_u32(const uint8_t** ptr, size_t* remaining) {
  if (*remaining < 4) return 0;
  uint32_t v = (uint32_t)(*ptr)[0]
               | ((uint32_t)(*ptr)[1] << 8)
               | ((uint32_t)(*ptr)[2] << 16)
               | ((uint32_t)(*ptr)[3] << 24);
  *ptr += 4; *remaining -= 4;
  return v;
}
// --- Entry point for LibFuzzer ---
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data,
                                       size_t size) {
  // Need at least a few bytes to derive parameters
  if (size < 16) return 0; // must contain IHDR chunk + IDAT + IEND chunks
  const uint8_t* ptr = data;
  size_t         rem = size;

  //random generator seed
  srand(time(nullptr));

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
  png_set_write_fn(handler.png_ptr, nullptr,
                   user_write_data, user_flush);

  // 5) Derive IHDR parameters: 16 bytes

  //   width, height: map 4 bytes
  uint32_t rw = read_u32(&ptr, &rem);
  uint32_t rh = read_u32(&ptr, &rem);
  png_uint_32 width  = (rw % 2048) + 1;
  png_uint_32 height = (rh % 2048) + 1;

  //   bit_depth: map 1 byte to {1,2,4,8,16}
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
    case 0: color_type = PNG_COLOR_TYPE_GRAY; break; //0
    case 1: color_type = PNG_COLOR_TYPE_RGB_ALPHA; break; //6
    case 2: color_type = PNG_COLOR_TYPE_RGB; break; //2
    case 3: color_type = PNG_COLOR_TYPE_PALETTE; break; //3: PNG_COLOR_TYPE_PALETTE
    default: color_type = PNG_COLOR_TYPE_GRAY_ALPHA; break; //4: PNG_COLOR_TYPE_GRAY_ALPHA
  }

  //  Adjustements to bit_depth based on color_type
  if (color_type == PNG_COLOR_TYPE_PALETTE && bit_depth > 8)
    bit_depth = 8;
  if ((color_type == PNG_COLOR_TYPE_GRAY_ALPHA ||
       color_type == PNG_COLOR_TYPE_RGB_ALPHA ||
        color_type == PNG_COLOR_TYPE_RGB) &&
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
  png_size_t rowbytes = png_get_rowbytes(handler.png_ptr, handler.info_ptr); 
  if (rowbytes == 0) { 
    return 0;
  }
  const png_size_t IDAT_size = rowbytes * height;
  // gAMA: 1-in-3 chance
  if ((rem >= 2 + IDAT_size) && (rand() % 3 == 0)) {
    //uint16_t rg = read_u16(&ptr, &rem); //TODO: use read_u32
    double   gamma = 0.45455;  
    png_set_gAMA(handler.png_ptr, handler.info_ptr, gamma);
  }

  // bKGD: 1-in-4 chance
  if ((rem >= 6 && + IDAT_size) && (rand() % 4 == 0)) {
    png_color_16 bkgd;
    bkgd.index = read_u8(&ptr, &rem); 
    bkgd.red   = read_u16(&ptr, &rem);
    bkgd.green = read_u16(&ptr, &rem);
    bkgd.blue  = read_u16(&ptr, &rem);
    bkgd.gray  = read_u16(&ptr, &rem); 
    png_set_bKGD(handler.png_ptr, handler.info_ptr, &bkgd);
  }

    // tEXt or ztext: 1-in-5 chance
if ((rem >= 2  + IDAT_size) && (rand() % 5 == 0)) {
    png_text text;
    uint8_t ct0 = read_u8(&ptr, &rem);

    switch (ct0 % 3) {
      case 0: text.compression = PNG_TEXT_COMPRESSION_NONE; break; //tEXt
      case 1: text.compression = PNG_TEXT_COMPRESSION_zTXt; break; //zTXt
      case 2: 
        text.compression = PNG_ITXT_COMPRESSION_NONE;
        text.lang = (char*)"en";
        text.lang_key = (char*)"Comment";
        break; //iTXt
    }
    text.key = (char*)"Comment";
    text.text = (char*)"Hello :)"; 
    text.text_length = strlen(text.text);
    png_set_text(handler.png_ptr, handler.info_ptr, &text, 1);
  }
  
  // pHYs: 1-in-4 chance
  if ((rem >= 9 + IDAT_size) && (rand() % 4 == 0)) {
    png_uint_32 x_pixels_per_unit = read_u32(&ptr, &rem);
    png_uint_32 y_pixels_per_unit = read_u32(&ptr, &rem);
    int unit_specifier = read_u8(&ptr, &rem) % 2;
    png_set_pHYs(handler.png_ptr, handler.info_ptr,
                x_pixels_per_unit, y_pixels_per_unit, unit_specifier);
  }

  // tIME: 1-in-4 chance
  if ((rem >= 7 + IDAT_size) && (rand() % 4 == 0)) {
    png_timep mod_time = (png_timep)png_malloc(handler.png_ptr, sizeof(png_time));
    if (!mod_time) return 0;
    mod_time->year = read_u16(&ptr, &rem);
    mod_time->month = read_u8(&ptr, &rem);
    mod_time->day = read_u8(&ptr, &rem);
    mod_time->hour = read_u8(&ptr, &rem);
    mod_time->minute = read_u8(&ptr, &rem);
    mod_time->second = read_u8(&ptr, &rem);
    png_set_tIME(handler.png_ptr, handler.info_ptr, mod_time);
    png_free(handler.png_ptr, mod_time);
  }

  //cHRM: 1-in-25 chance
  if ((rem >= 32 + IDAT_size) && (rand() % 25) == 0) {
    png_fixed_point white_x = read_u32(&ptr, &rem);
    png_fixed_point white_y = read_u32(&ptr, &rem);
    png_fixed_point red_x = read_u32(&ptr, &rem);
    png_fixed_point red_y = read_u32(&ptr, &rem);
    png_fixed_point green_x = read_u32(&ptr, &rem);
    png_fixed_point green_y = read_u32(&ptr, &rem);
    png_fixed_point blue_x = read_u32(&ptr, &rem);
    png_fixed_point blue_y = read_u32(&ptr, &rem);
    png_set_cHRM(handler.png_ptr, handler.info_ptr,
                white_x, white_y,
                red_x, red_y,
                green_x, green_y,
                blue_x, blue_y);
  }

  // 7) Write info
  png_write_info(handler.png_ptr, handler.info_ptr);

  // 8) Allocate a row buffer and write rows
 
  handler.rows = (png_bytepp)png_malloc(handler.png_ptr, sizeof(png_bytep) * height);
  if (!handler.rows) return 0;

  // IDAT: write the image data
  for (png_uint_32 y = 0; y < height; y++) {
    handler.rows[y] = (png_bytep)png_malloc(handler.png_ptr, rowbytes);

    if(!handler.rows[y]) {
      for (png_uint_32 i = 0; i < y; i++) {
        png_free(handler.png_ptr, handler.rows[i]);
      }
      png_free(handler.png_ptr, handler.rows);
      return 0;
    }
    if(rem >rowbytes) {
      memccpy(handler.rows[y], ptr, 0, rowbytes);
      ptr += rowbytes; rem -= rowbytes;
    } else {
      memset(handler.rows[y], 0, rowbytes);
    }
    
    
  }
  png_set_rows(handler.png_ptr, handler.info_ptr, handler.rows);
  
  png_write_image(handler.png_ptr, handler.rows);

  // 9) IEND: write the end chunk: 2 bytes
  png_write_end(handler.png_ptr, handler.info_ptr);

 
  return 0;
}

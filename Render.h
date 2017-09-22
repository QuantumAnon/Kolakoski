#ifndef Render_h
#define Render_h

//========================================
//Contains render functions for bitmaps
//----------------------------------------
//
//========================================
#include "stdio.h"
#include "png.h"
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

//A single pixel with an RGB value
typedef struct
{
  u_int8_t red;
  u_int8_t green;
  u_int8_t blue;
  u_int8_t alpha;
} pixel_t;

//A canvas of pixel values, this does not necessarily correspond to actual pixel sizes as canvases can be "blockified"
typedef struct
{
  pixel_t *canvas_start;
  size_t width;
  size_t height;
} canvas_t;

//Find the pixel data at a specific position
static pixel_t *pixel_at_canvas(canvas_t *canvas, int x, int y)
{
  if ((x >= canvas->width) || (y >= canvas->height)) { //check if pixel requested is within canvas size
    return NULL;
  } else { //if it is, return start pixel plus distances, pixel is wrapped much like a scanline system
    return (canvas->canvas_start + x + y * canvas->width);
  }
}

void newCanvas(canvas_t *canvas, int width, int height)
{
  canvas->width = width;
  canvas->height = height;
  canvas->canvas_start = (pixel_t*)calloc(canvas->height*canvas->width, sizeof(pixel_t));
}


//Paint a row with a particular array of values, naively stretches a smaller array to fit the size needed
void paint(canvas_t *canvas, pixel_t values[], int row, int width)
{
  if ((row >= canvas->height) || (width > canvas->width)) { //check against canvas size
    return;
  }
  //naive stretching, leaves a blank space at the end if there is a remainder
  int h_size = canvas->width / width;

  int cur = 0;
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < h_size; j++) {
      pixel_at_canvas(canvas, i*h_size + j, row)->red   = values[cur].red;
      pixel_at_canvas(canvas, i*h_size + j, row)->green = values[cur].green;
      pixel_at_canvas(canvas, i*h_size + j, row)->blue  = values[cur].blue;
      pixel_at_canvas(canvas, i*h_size + j, row)->alpha = values[cur].alpha;
    }
    cur++;
  }

  for (int i = cur * h_size; i < canvas->width; i++) {
    pixel_at_canvas(canvas, i, row)->red   = 0;
    pixel_at_canvas(canvas, i, row)->green = 0;
    pixel_at_canvas(canvas, i, row)->blue  = 0;
    pixel_at_canvas(canvas, i, row)->alpha = 0;
  }
}

//Convert a canvas to an RGBA pixel array
void rasterize(canvas_t *canvas,  canvas_t *image, size_t block_width, size_t block_height)
{
  //allocate memory
  image->canvas_start = (pixel_t*)calloc(canvas->width * canvas->height * block_width * block_height, sizeof(pixel_t));
  //image height and width
  image->height = canvas->height * block_height, image->width = canvas->width * block_width;


  //left->right, top->down across blocks
  for (int y = 0; y < canvas->height; y++) {
    //top pixel of the block
    unsigned anchor_y = y * block_height;
    for (int x = 0; x < canvas->width; x++) {
      //leftmost pixel of the block
      unsigned anchor_x = x * block_width;

      //pixel to represent
      pixel_t cur = *pixel_at_canvas(canvas, x, y);

      //left->right, top->down across a single block
      for (int j = 0; j < block_height; j++) {
        for (int i = 0; i < block_width; i++) {
          //assign values
          pixel_at_canvas(image, anchor_x + i, anchor_y + j)->red   = cur.red;
          pixel_at_canvas(image, anchor_x + i, anchor_y + j)->green = cur.green;
          pixel_at_canvas(image, anchor_x + i, anchor_y + j)->blue  = cur.blue;
          pixel_at_canvas(image, anchor_x + i, anchor_y + j)->alpha = cur.alpha;
        }
      }
    }
  }
}

//write a canvas to file
int writeImage(char* filename, canvas_t *image, char* title)
{
  int code = 0;
  FILE *fp = NULL;
  png_structp png_ptr = NULL;
  png_infop info_ptr = NULL;
  png_bytep row = NULL;

  // Open file for writing (binary mode)
  fp = fopen(filename, "wb");
  if (fp == NULL) {
     fprintf(stderr, "Could not open file %s for writing\n", filename);
     code = 1;
     goto finalise;
  }

  // Initialize write structure
  png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png_ptr) {
     fprintf(stderr, "Could not allocate write struct\n");
     code = 1;
     goto finalise;
  }

  // Initialize info structure
  info_ptr = png_create_info_struct(png_ptr);
  if (info_ptr == NULL) {
     fprintf(stderr, "Could not allocate info struct\n");
     code = 1;
     goto finalise;
  }

  // Setup Exception handling
  if (setjmp(png_jmpbuf(png_ptr))) {
     fprintf(stderr, "Error during png creation\n");
     code = 1;
     goto finalise;
  }

  png_init_io(png_ptr, fp);

  // Write header (8 bit colour depth)
  png_set_IHDR(png_ptr, info_ptr, image->width, image->height, 8, PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

  // Set title
  if (title != NULL) {
     png_text title_text;
     title_text.compression = PNG_TEXT_COMPRESSION_NONE;
     title_text.key = "Title";
     title_text.text = title;
     png_set_text(png_ptr, info_ptr, &title_text, 1);
  }

  png_set_invert_alpha(png_ptr);
  png_write_info(png_ptr, info_ptr);

  // Allocate memory for one row (4 bytes per pixel - RGBA)
  row = (png_byte*)calloc(image->width, sizeof(pixel_t));

  // Write image data
  int x, y;
  for (y=0 ; y < image->height ; y++) {
     for (x=0 ; x < image->width ; x++) {
        pixel_t cur = *pixel_at_canvas(image, x, y);

        row[x*4 + 0] = cur.red;
        row[x*4 + 1] = cur.blue;
        row[x*4 + 2] = cur.green;
        row[x*4 + 3] = cur.alpha;
     }
     png_write_row(png_ptr, row);
     //printf("at row %d\n", y);
  }

  // End write
  if (setjmp(png_jmpbuf(png_ptr))) {
    fprintf(stderr, "Error during png end\n");
    code = 1;
    goto finalise;
  }

  png_write_end(png_ptr, NULL);

  finalise:
  if (fp != NULL) fclose(fp);
  if (info_ptr != NULL) png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
  if (png_ptr != NULL) png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
  if (row != NULL) free(row);

  return code;
}

#endif

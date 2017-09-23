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

//A 2D array of pixel values, this does not necessarily correspond to actual pixel sizes as these can be "blockified"
typedef struct
{
  pixel_t *image_start;
  size_t width;
  size_t height;
} image_t;

//set the pixel data for a given pixel pointer
void pixel_set(pixel_t *pixel, u_int8_t red, u_int8_t green, u_int8_t blue, u_int8_t alpha)
{
  pixel->red   = red;
  pixel->green = green;
  pixel->blue  = blue;
  pixel->alpha = alpha;
}

//Find the pixel data at a specific position
static pixel_t *pixel_at(image_t *image, int x, int y)
{
  if ((x >= image->width) || (y >= image->height)) { //check if pixel requested is within canvas size
    return NULL;
  } else { //if it is, return start pixel plus distances, pixel is wrapped much like a scanline system
    return (image->image_start + x + y * image->width);
  }
}

//generate a new image with a certain width and height
void newImage(image_t *image, int width, int height)
{
  image->width = width;
  image->height = height;
  image->image_start = (pixel_t*)calloc(image->height*image->width, sizeof(pixel_t));
}


//Paint a row with a particular array of values, naively stretches a smaller array to fit the size needed
void paint(image_t *image, pixel_t values[], int width, int row)
{
  if ((row >= image->height) || (width > image->width)) { //check against canvas size
    return;
  }
  //naive stretching, leaves a blank space at the end if there is a remainder
  int h_size = image->width / width;

  for (int i = 0; i < width; i++) {
    for (int j = 0; j < h_size; j++) {
      pixel_set(pixel_at(image, i*h_size + j, row), values[i].red, values[i].green, values[i].blue, values[i].alpha);
    }
  }

  for (int i = width * h_size; i < image->width; i++) {
    pixel_set(pixel_at(image, i, row), 0, 0, 0, 0);
  }
}

//Convert an image to a properly sized RGBA pixel array
void rasterize(image_t *pre_image,  image_t *image, size_t block_width, size_t block_height)
{
  //allocate memory
  image->image_start = (pixel_t*)calloc(pre_image->width * block_width * pre_image->height * block_height, sizeof(pixel_t));
  //image height and width
  image->height = pre_image->height * block_height, image->width = pre_image->width * block_width;


  //left->right, top->down across blocks
  for (int y = 0; y < pre_image->height; y++) {
    //top pixel of the block
    unsigned anchor_y = y * block_height;
    for (int x = 0; x < pre_image->width; x++) {
      //leftmost pixel of the block
      unsigned anchor_x = x * block_width;

      //pixel to represent
      pixel_t cur = *pixel_at(pre_image, x, y);

      //left->right, top->down across a single block
      for (int j = 0; j < block_height; j++) {
        for (int i = 0; i < block_width; i++) {
          //assign values
          pixel_set(pixel_at(image, anchor_x + i, anchor_y + j), cur.red, cur.green, cur.blue, cur.alpha);
        }
      }
    }
  }
}

//shamelessly copied from the LibPNG manual
//write an image to file
int writeImage(char* filename, image_t *image, char* title)
{
  //initial code, does something but I'm not sure what
  int code = 0;
  //pointer for the file
  FILE *fp = NULL;
  //pointer for the png image
  png_structp png_ptr = NULL;
  //pointer for the png info
  png_infop info_ptr = NULL;
  //pointer for a single row
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

  //initialise I/O
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

  //invert alpha channel
  png_set_invert_alpha(png_ptr);
  //setup write info
  png_write_info(png_ptr, info_ptr);

  // Allocate memory for one row (4 bytes per pixel - RGBA)
  row = (png_byte*)calloc(image->width, sizeof(pixel_t));

  // Write image data
  int x, y;
  for (y=0 ; y < image->height ; y++) {
     for (x=0 ; x < image->width ; x++) {
        pixel_t cur = *pixel_at(image, x, y);

        row[x*4 + 0] = cur.red;
        row[x*4 + 1] = cur.green;
        row[x*4 + 2] = cur.blue;
        row[x*4 + 3] = cur.alpha;
     }
     png_write_row(png_ptr, row);
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

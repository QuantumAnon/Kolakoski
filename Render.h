//========================================
//Contains render functions for bitmaps
//----------------------------------------
//
//========================================

#pragma once

#include "stdio.h"
#include "png.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
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
static pixel_t *pixel_at(canvas_t *canvas, int x, int y)
{
  if ((x >= canvas->width) || (y >= canvas->height)) { //check if pixel requested is within canvas size
    return NULL;
  } else { //if it is, return start pixel plus distances, pixel is wrapped much like a scanline system
    return (canvas->canvas_start + x + y * canvas->width);
  }
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
  for (int i = 0; i < canvas->width; i += h_size) {
    for (int j = 0; j < h_size; j++) {
      pixel_at(canvas, i + j, row)->red = values[cur].red;
      pixel_at(canvas, i + j, row)->green = values[cur].green;
      pixel_at(canvas, i + j, row)->blue = values[cur].blue;
      pixel_at(canvas, i + j, row)->alpha = values[cur].alpha;
    }
    cur++;
  }
}

//Convert a canvas to an RGBA pixel array
void rasterize(canvas_t *canvas, unsigned char *image, unsigned int block_width, unsigned int block_height)
{
  //allocate memory
  image = (unsigned char*)calloc(canvas->width * canvas->height * block_width * block_height * 4, sizeof(unsigned));
  //image height and width
  int img_height = canvas->height * block_height, img_width = canvas->width * block_width;

  //left->right, top->down across blocks
  for (int y = 0; y < canvas->height; y++) {
    //top pixel of the block
    unsigned anchor_y = y * block_height;
    for (int x = 0; x < canvas->width; x++) {
      //leftmost pixel of the block
      unsigned anchor_x = x * block_width;

      //pixel to represent
      pixel_t cur = *pixel_at(canvas, x, y);

      //left->right, top->down across a single block
      for (int j = 0; j < block_height; j++) {
        for (int i = 0; i < block_width; i++) {
          //pixel_coord = (i+anchor_x, j+anchor_y)
          unsigned pixel_number = (i+anchor_x) + (j + anchor_y)*img_width;

          //assign values
          image[4*pixel_number + 0] = cur.red;
          image[4*pixel_number + 1] = cur.green;
          image[4*pixel_number + 2] = cur.blue;
          image[4*pixel_number + 3] = cur.alpha;
        }
      }
    }
  }
}

//write a canvas to file
void write(const char* filename, const unsigned char* image, unsigned int width, unsigned int height)
{
  
}

//========================================
//Contains render functions for bitmaps
//----------------------------------------
//
//========================================

#pragma once

#include "stdio.h"
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
  if ((x >= canvas->width) || (y >= canvas->height)) //check if pixel requested is within canvas size
  { return NULL; }
  else //if it is, return start pixel plus distances, pixel is wrapped much like a scanline system
  { return (canvas->canvas_start + x + y * canvas->width); }
}


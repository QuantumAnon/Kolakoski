//====================
//Main Code
//--------------------
//Defines the entry point for the program
//====================

#include "./List.h"
#include "./Render.h"
#include "stdio.h"
#include "png.h"
#include <string.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

list Kolakoski(int n)
{
  list K;
  K.head = NULL;
  node *cur;
  Append(&K,1);
  Append(&K,2);
  Append(&K,2);

  cur=K.tail;
  for (int i = 3; i <= n; i++)
  {
    if ((i)%2 == 1) {
      MultiAppend(&K,cur->data,1);
    } else {
      MultiAppend(&K, cur->data,2);
    }

    cur = cur->next;
  }
  return K;
}


int main( int argc, const char* argv[] )
{
  //init variables
  int rndr_w, rndr_h, blck_w, blck_h;
  float blankTolerance;



  rndr_w = 10000;
  rndr_h = 6;
  blck_w = 1;
  blck_h = 500;
  blankTolerance = 0.1;

  //jagged array representing actual pixels
  pixel_t *pixels[rndr_h];

  //filename
  char *output = "test.png";

  //canvas to draw on
  image_t canvas;
  //image to render
  image_t image;

  //canvas is rndr_w x rndr_h
  newImage(&canvas, rndr_w, rndr_h);

  int oldLength = 0;

  //from 0 to the height of the render
  for (int i = 0; i < rndr_h; i += 1) {
    int c = i - 1;
    list K;
    int n;

    do {
      c++;
      //generate Kolakoski sequence using c iterations
      K = Kolakoski(c);
      n = Length(&K);
    } while ((n % rndr_w > blankTolerance * rndr_w) || (n < 2 * oldLength));
    oldLength = n;
    
    //printf("%i -- %i\n", i + 3, n);
    int p[n];
    Array(p, &K);
    //int max = (n < 150) ? n : 50;
    pixels[i] = (pixel_t*)calloc(n,sizeof(pixel_t));

    for (int k = 0; k < n; k++) {
      if (p[k] == 1) {
        pixel_set(&pixels[i][k], 200, 0, 0, 0);
      } else {
        pixel_set(&pixels[i][k], 0, 0, 200, 0);
      }
  }
    paint(&canvas, pixels[i], n, rndr_h - i);
  }


  rasterize(&canvas, &image, blck_w, blck_h);

  writeImage(output, &image, "test");

  return 0;
}

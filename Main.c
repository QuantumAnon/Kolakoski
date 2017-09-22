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
  pixel_t *pixels[100];
  char *output = "test.png";
  canvas_t canvas;
  canvas_t image;
  newCanvas(&canvas, 200, 100);

  for (int i = 0; i <= 100; i++) {
    list K = Kolakoski(i + 3);
    int n = Size(&K);
    printf("%i -- %i\n", i + 3, n);
    int p[n];
    Array(p, &K);
    //int max = (n < 150) ? n : 50;
    pixels[i] = (pixel_t*)calloc(n,sizeof(pixel_t));

    for (int j = 0; j < n; j++) {
      if (p[j] == 1) {
        pixel_set(&pixels[i][j], 200, 0, 0, 0);
      } else {
        pixel_set(&pixels[i][j], 0, 0, 200, 0);
      }
    }
    paint(&canvas, pixels[i], n, 100-i);
  }

  rasterize(&canvas, &image, 10, 20);

  writeImage(output, &image, "test");

  return 0;
}

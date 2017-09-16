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
#include <stdbool.h>
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
  //list K = Kolakoski(10);
  //SequenceDisplay(&K);
  //int *p;
  //Array(p,&K,Size(&K));
  //for (int i = 0; i < Size(&K); i++) {
  //  printf("%i", p[i]);
  //}

  
  char* output = "output.png";

  pixel_t pixels[3];
  {
    pixels[0].red = 100;
    pixels[0].green = 100;
    pixels[0].blue = 100;
    pixels[0].alpha = 100;

    pixels[1].red = 100;
    pixels[1].green = 100;
    pixels[1].blue = 100;
    pixels[1].alpha = 255;
    
    pixels[2].red = 100;
    pixels[2].green = 100;
    pixels[2].blue = 100;
    pixels[2].alpha = 200;
  }

  canvas_t canvas;
  canvas.height = 1;
  canvas.width = 3;
  canvas.canvas_start = (pixel_t*)calloc(canvas.height*canvas.width, sizeof(pixel_t));
  paint(&canvas, pixels, 0, 3);
  
  unsigned char* image;
  rasterize(&canvas, image, 100, 100);

  writeImage(output, 100 * canvas.width, 100 * canvas.height, image, "test");

  return 0;
}

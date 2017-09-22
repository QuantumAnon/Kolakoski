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
  //list K = Kolakoski(10);
  //SequenceDisplay(&K);
  //int *p;
  //Array(p,&K,Size(&K));
  //for (int i = 0; i < Size(&K); i++) {
  //  printf("%i", p[i]);
  //}

  char *output = "test.png";
  pixel_t pixels[3];
  for (int i = 0; i < 3; i++) {
    pixels[i].red   = 100*i;
    pixels[i].green = 100;
    pixels[i].blue  = 100*i;
    pixels[i].alpha = 0;
  }


  canvas_t canvas;
  canvas_t image;
  
  newCanvas(&canvas, 7, 1);

  paint(&canvas, pixels, 0, 3);

  //for (int i = 0; i < canvas.width; i++) {
  //  for (int j = 0; j < canvas.height; j++) {
  //    printf("red   -(%i,%i)- %i\n", i, j, pixel_at(&canvas, i, j)->red);
  //    printf("green -(%i,%i)- %i\n", i, j, pixel_at(&canvas, i, j)->green);
  //    printf("blue  -(%i,%i)- %i\n", i, j, pixel_at(&canvas, i, j)->blue);
  //    printf("alpha -(%i,%i)- %i\n", i, j, pixel_at(&canvas, i, j)->alpha);
  //  }
  //}

  rasterize(&canvas, &image, 100, 100);
  //for (int i = 0; i < image.width; i++) {
  //  for (int j = 0; j < image.height; j++) {
  //    printf("red   -(%i,%i)- %i\n", i, j, pixel_at_image(&image, i, j)->red);
  //    printf("green -(%i,%i)- %i\n", i, j, pixel_at_image(&image, i, j)->green);
  //    printf("blue  -(%i,%i)- %i\n", i, j, pixel_at_image(&image, i, j)->blue);
  //    printf("alpha -(%i,%i)- %i\n", i, j, pixel_at_image(&image, i, j)->alpha);
  //  }
  //}

  writeImage(output, &image, "test");

  return 0;
}

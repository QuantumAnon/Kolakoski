//====================
//Main Code
//--------------------
//Defines the entry point for the program
//====================

#include "./List.h"
#include "./Render.h"
#include "stdio.h"
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

  pixel_t *pixels;
  memset(pixels, 0, 3 * sizeof(pixel_t));

  return 0;
}

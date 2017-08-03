//====================
//Main Code
//--------------------
//Defines the entry point for the program
//====================

#include "./List.h"
#include "./Kolakoski.h"
#include "stdio.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <termios.h>
#include <unistd.h>

int main( int argc, const char* argv[] )
{
  list K = Kolakoski(10);
  //SequenceDisplay(&K);
  int *p;
  Array(p,&K,Size(&K));
  for (int i = 0; i < Size(&K); i++)
  {
    printf("%i\n", p[i]);
  }
  return 0;
}
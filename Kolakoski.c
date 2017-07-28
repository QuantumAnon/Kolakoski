#include "./List.h"
#include "stdio.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <termios.h>
#include <unistd.h>

int main( int argc, const char* argv[] )
{
  list K;
  K.head = NULL;
  node *cur;
  for (int i = 0; i < 10000; i++)
  {
    if ((i == 0) || (i == 1))
    {
      MultiAppend(&K,i+1,i+1);
      cur = K.head;
    }
    else if ((i+1)%2 == 1)
    {MultiAppend(&K,cur->data,1);}
    else
    {MultiAppend(&K, cur->data,2);}
    cur = cur->next;
  }
  Display(&K);
  return 0;
}
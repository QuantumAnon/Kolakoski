//====================
//Kolakoski definition file
//--------------------
//Defines the Kolakoski generating function
//====================

#include "./List.h"
#include "stdio.h"
#include <stdlib.h>
#include <stdbool.h>

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
    if ((i)%2 == 1)
    {MultiAppend(&K,cur->data,1);}
    else
    {MultiAppend(&K, cur->data,2);}
    cur = cur->next;
  }
  return K;
}
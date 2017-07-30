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
  for (int i = 0; i < n; i++)
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
  return K;
}
#pragma once
//====================
//List.h
//--------------------
//Defines lists and various functions on lists
//====================

#include "stdio.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <termios.h>
#include <unistd.h>


//A single element of a list
typedef struct node
{
  int data;
  struct node *next;
}node;

//A linked list of nodes
typedef struct list
{
  node *head;
  node *tail;
}list;

//Appends a single value to the end of a list
void Append(list *X, int z)
{
  list temp;
  temp = *X;
  if (temp.head == NULL)
  {
    temp.head = (node*)malloc(sizeof(node)); //make a space big enough for a node and point to it
    temp.tail = temp.head; //point the head and tail to the same position
    temp.head->data = z; //the data contained in the node is z
    temp.head->next = NULL; //the new node points to nothing
  }
  else
  {
    temp.tail->next = (node*)malloc(sizeof(node)); //point the tail to a space big enough for a node
    temp.tail->next->data = z; //the data contained in the node is z
    temp.tail->next->next = NULL; //the new node points to nothing
    temp.tail = temp.tail->next; //the tail node now points to new node
  }
  *X = temp;
  return;
}

//Appends multiple copies of the same value to the end of a list
void MultiAppend(list *X, int n, int z)
{
  for (int i = 0; i < n; i++)
  {
    Append(X,z);
  }
}

//Displays a list on a single line
void Display(list *X)
{
  node *cur;
  cur = X->head;
  printf("start->");
  while (cur)
  {
    printf("%d->",cur->data); 
    cur = cur->next;
  }  
  printf("end!\n");
}

//Displays a list on a single line
void SequenceDisplay(list *X)
{
  node *cur;
  cur = X->head;
  while (cur){
    printf("%d, ",cur->data); 
    cur = cur->next;
  }
  printf("\n");  
}
//Find the size of a list
int Size(list *X)
{
  node *cur;
  cur = X->head;
  int size = 0;
  while (cur)
  { 
    size++;
    cur = cur->next;
  }
  return size;
}

//Generate an array from a list
int *Array(list *X, int size)
{

}

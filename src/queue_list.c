/* Copyright (C) 2013 BAH Salmane, CHALAUX Benjamin , SOLLAUD Timothée

   This file is part of Veclang: a toy compiler for vectorial drawing using the cairo library
     Veclang is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Veclang is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Veclang.  If not, see <http://www.gnu.org/licenses/>
*/

#include<stdio.h>
#include<stdlib.h>
#include "queue.h"

struct _cellule;
typedef struct _cellule *cellule;

struct _cellule {
  void* car;
  cellule cdr;
};

struct queue_t{
  cellule init;
  cellule current;
};

/* create an empty queue */
queue queue_create(void){
  queue s = malloc (sizeof (queue));
  s->init = NULL;
  s->current = NULL;
  return s;
}

/* delete an empty queue */
void queue_destroy(queue s){
  free(s);
}

/* return true if and only if the queue is empty */
int queue_empty(queue s){
  return s->init == NULL;
}

/* push an object on the back of the queue */
void queue_push(queue s, void *object){
  cellule tmp = malloc (sizeof (cellule));
  tmp->car = object;
  tmp->cdr = NULL;
  if (queue_empty(s)) {
    s->init = tmp;
  }
  else {
    s->current->cdr = tmp;
  }
  s->current = tmp;
}


/* return the front element of the queue.
   The queue must not be empty (as reported by queue_empty()) */
void * queue_front(queue s){
  if (!queue_empty(s))
    return s->init->car;
  return NULL;
}

/* pop the front element off of the queue.
   The queue must not be empty (as reported by queue_empty()) */
void queue_pop(queue s){
  if (!queue_empty(s)) {
    cellule tmp = s->init->cdr;
    free(s->init);
    s->init = tmp;
  }
}

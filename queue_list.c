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

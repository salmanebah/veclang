/* Copyright (C) 2013 BAH Salmane, CHALAUX Benjamin , SOLLAUD Timothée/* 

/* This file is part of Veclang: a toy compiler for vectorial drawing using the cairo library
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

#ifndef QUEUE_H
#define QUEUE_H
/* Header file for the queue abstract data type (queue.h) */

struct queue_t;

typedef struct queue_t *queue;

/* create an empty queue */
queue queue_create(void);

/* create an empty queue */
void queue_destroy(queue s);

/* return true if and only if the queue is empty */
int queue_empty(queue s);

/* push an object on the back of the queue */
void queue_push(queue s, void *object);


/* return the front element of the queue.
   The queue must not be empty (as reported by queue_empty()) */
void * queue_front(queue s);

/* pop the front element off of the queue.
   The queue must not be empty (as reported by queue_empty()) */
void queue_pop(queue s);

#endif

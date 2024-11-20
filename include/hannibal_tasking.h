#ifndef HANNIBAL_TASKING_H
#define HANNIBAL_TASKING_H

#include <windows.h>
#include "utility_strings.h"

#define TASK_CIRCULAR_QUEUE_SIZE 10
#define TASK_RESPONSE_CIRCULAR_QUEUE_SIZE 10

#ifdef PIC_BUILD
#define FUNC_OFFSET( x ) ((UINT_PTR)StRipStart() + (DWORD)&x)
#else
#define FUNC_OFFSET( x ) (&x)
#endif


/////////////////////// Task Structures

typedef struct _TASK {
    char *task_uuid; // Tracking on the controller
    int cmd_id; // Identify specific function to execute
    int timestamp;
    void *cmd;
    LPCSTR output;
    int output_size;
} TASK, *PTASK;

typedef struct _TASK_QUEUE {
    int front;
    int rear;
    int size; // How many tasks in queue
    int capacity; // How many tasks can it hold
    TASK *queue_ptr;
} TASK_QUEUE, *PTASK_QUEUE;

typedef struct _TASK_ENTRY {
    int cmd_id;
    (*cmd_ptr)(TASK t);
} TASK_ENTRY;


BOOL task_enqueue(TASK_QUEUE *queue_struct, TASK *TASK);
BOOL task_dequeue(TASK_QUEUE *queue_struct, TASK *TASK);

#endif
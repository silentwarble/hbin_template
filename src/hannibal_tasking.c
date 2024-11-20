#include "hannibal_tasking.h"


SECTION_CODE BOOL task_enqueue(TASK_QUEUE *queue_struct, TASK *TASK)
{
    if (queue_struct->size == queue_struct->capacity){
        return FALSE;
    }

    queue_struct->rear = (queue_struct->rear + 1) % queue_struct->capacity; // Ensure wrap-around
    queue_struct->queue_ptr[queue_struct->rear] = *TASK;
    queue_struct->size++;    
    return TRUE;

}

SECTION_CODE BOOL task_dequeue(TASK_QUEUE *queue_struct, TASK *TASK)
{

    HANNIBAL_INSTANCE_PTR

    if (queue_struct->size == 0){
        return FALSE;
    }

    *TASK = queue_struct->queue_ptr[queue_struct->front];

    queue_struct->front = (queue_struct->front + 1) % queue_struct->capacity;
    queue_struct->size--;

    return TRUE;
}
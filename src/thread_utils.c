#include <stdio.h>
#include "thread.h"

int addToCounterAndDisplay(int *_counter, char *_thread_name,
                           SemaphoreHandle_t _semaphore)
{
    if (xSemaphoreTake(_semaphore, 0) != pdTRUE) {
        return pdFALSE;
    }

    (*_counter)++;
    printf("hello world from %s! Count %d\n", _thread_name, *_counter);
    xSemaphoreGive(_semaphore);

    return pdTRUE;
}
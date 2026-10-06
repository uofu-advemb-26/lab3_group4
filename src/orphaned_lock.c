#include <stdio.h>
#include "orphaned_lock.h"

int orphaned_lock_step(orphaned_args_t *_args, TickType_t _timeout)
{
    if (xSemaphoreTake(_args->semaphore, _timeout) != pdTRUE) {
        return pdFALSE;
    }

    _args->counter++;
    if (_args->counter % 2) {
        return pdTRUE;
    }
    printf("Count %d\n", _args->counter);
    xSemaphoreGive(_args->semaphore);

    return pdTRUE;
}

void orphaned_lock(void *_params)
{
    orphaned_args_t *args = (orphaned_args_t *)_params;

    while (1) {
        orphaned_lock_step(args, portMAX_DELAY);
    }
}

int fixed_lock_step(orphaned_args_t *_args, TickType_t _timeout)
{
    if (xSemaphoreTake(_args->semaphore, _timeout) != pdTRUE) {
        return pdFALSE;
    }
    {
        _args->counter++;
        if (_args->counter % 2 == 0) {
            printf("Count %d\n", _args->counter);
        }
    }
    xSemaphoreGive(_args->semaphore);

    return pdTRUE;
}

void fixed_lock(void *_params)
{
    orphaned_args_t *args = (orphaned_args_t *)_params;

    while (1) {
        fixed_lock_step(args, portMAX_DELAY);
        vTaskDelay(100);
    }
}

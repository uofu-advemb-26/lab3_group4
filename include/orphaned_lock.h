#pragma once

#include <FreeRTOS.h>
#include <semphr.h>
#include <task.h>

typedef struct {
    SemaphoreHandle_t semaphore;
    int counter;
} orphaned_args_t;

int orphaned_lock_step(orphaned_args_t *_args, TickType_t _timeout);
void orphaned_lock(void *_params);

int fixed_lock_step(orphaned_args_t *_args, TickType_t _timeout);
void fixed_lock(void *_params);

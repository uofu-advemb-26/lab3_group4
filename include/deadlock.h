#pragma once

#include <FreeRTOS.h>
#include <semphr.h>
#include <task.h>

#define HOLD_TIME pdMS_TO_TICKS(100)

typedef struct {
    SemaphoreHandle_t first;
    SemaphoreHandle_t second;
    int counter;
} deadlock_args_t;

void deadlock_thread(void *_params);

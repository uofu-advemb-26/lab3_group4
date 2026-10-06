#pragma once

#include <FreeRTOS.h>
#include <semphr.h>
#include <task.h>
#include <pico/stdlib.h>
#include <pico/multicore.h>
#include <pico/cyw43_arch.h>


int addToCounterAndDisplay(int* _counter, char* _thread_name, SemaphoreHandle_t _semaphore, TickType_t _timeout);

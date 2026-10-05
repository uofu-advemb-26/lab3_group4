#include <stdio.h>
#include <FreeRTOS.h>
#include <semphr.h>
#include <task.h>
#include <pico/stdlib.h>
#include <pico/multicore.h>
#include <pico/cyw43_arch.h>
#include <unity.h>
#include "thread.h"

void setUp(void) {}

void tearDown(void) {}

void test_semaphore_returns_false(void)
{
    // create a semaphore that is already taken
    // create a counting semaphore that starts at 0 ends at 1
    SemaphoreHandle_t semaphore = xSemaphoreCreateCounting(1, 0);

    int counter = 0;

    int result = addToCounterAndDisplay(&counter, "test_thread", semaphore, 0);

    TEST_ASSERT_EQUAL_INT(0, counter);
    TEST_ASSERT_EQUAL_INT(pdFALSE, result);
}

void test_counter_increments_when_lock_available(void)
{
    int counter = 0;
    SemaphoreHandle_t semaphore = xSemaphoreCreateCounting(1, 1);

    int result = addToCounterAndDisplay(&counter, "test_thread", semaphore, 0);

    TEST_ASSERT_EQUAL_INT(pdTRUE, result);
    TEST_ASSERT_EQUAL_INT(1, counter);
    TEST_ASSERT_EQUAL_INT(1, uxSemaphoreGetCount(semaphore)); // lock was given back
}


int main (void)
{
    stdio_init_all();
    while (1) {
        sleep_ms(5000); // Give time for TTY to attach.
        printf("Start tests\n");
        UNITY_BEGIN();

        RUN_TEST(test_semaphore_returns_false);
        RUN_TEST(test_counter_increments_when_lock_available);

        sleep_ms(5000);
        UNITY_END();
    }
}

#include <stdio.h>
#include <FreeRTOS.h>
#include <semphr.h>
#include <task.h>
#include <pico/stdlib.h>
#include <pico/multicore.h>
#include <pico/cyw43_arch.h>
#include <unity.h>
#include "thread.h"
#include "deadlock.h"

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
    TEST_ASSERT_EQUAL_INT(1, uxSemaphoreGetCount(semaphore));
}

void test_two_locks_deadlock(void)
{
    SemaphoreHandle_t a = xSemaphoreCreateCounting(1, 1);
    SemaphoreHandle_t b = xSemaphoreCreateCounting(1, 1);

    // thread 1 takes a then b, thread 2 takes b then a
    deadlock_args_t args1 = {a, b, 0};
    deadlock_args_t args2 = {b, a, 0};

    TaskHandle_t thread1, thread2;
    xTaskCreate(deadlock_thread, "Thread1", configMINIMAL_STACK_SIZE,
                &args1, tskIDLE_PRIORITY + 1, &thread1);
    xTaskCreate(deadlock_thread, "Thread2", configMINIMAL_STACK_SIZE,
                &args2, tskIDLE_PRIORITY + 1, &thread2);

    vTaskDelay(HOLD_TIME * 5);

    // get state before suspending or it just says suspended
    eTaskState state1 = eTaskGetState(thread1);
    eTaskState state2 = eTaskGetState(thread2);

    vTaskSuspend(thread1);
    vTaskSuspend(thread2);

    int count1 = args1.counter;
    int count2 = args2.counter;
    int a_count = uxSemaphoreGetCount(a);
    int b_count = uxSemaphoreGetCount(b);

    // delete everything before the asserts in case one fails
    vTaskDelete(thread1);
    vTaskDelete(thread2);
    vSemaphoreDelete(a);
    vSemaphoreDelete(b);

    TEST_ASSERT_EQUAL_INT(eBlocked, state1);
    TEST_ASSERT_EQUAL_INT(eBlocked, state2);
    TEST_ASSERT_EQUAL_INT(0, count1);
    TEST_ASSERT_EQUAL_INT(0, count2);
    TEST_ASSERT_EQUAL_INT(0, a_count);
    TEST_ASSERT_EQUAL_INT(0, b_count);
}

void test_runner(void *params)
{
    while (1) {
        vTaskDelay(100);
        printf("Start tests\n");
        UNITY_BEGIN();

        RUN_TEST(test_semaphore_returns_false);
        RUN_TEST(test_counter_increments_when_lock_available);
        RUN_TEST(test_two_locks_deadlock);

        UNITY_END();
    }
}

int main (void)
{
    stdio_init_all();
    // higher priority than the test threads so it can stop them
    xTaskCreate(test_runner, "TestRunner", configMINIMAL_STACK_SIZE * 4,
                NULL, tskIDLE_PRIORITY + 2, NULL);
    vTaskStartScheduler();
    return 0;
}

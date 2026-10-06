#include "deadlock.h"

void deadlock_thread(void *_params)
{
    deadlock_args_t *args = (deadlock_args_t *)_params;

    while (1) {
        xSemaphoreTake(args->first, portMAX_DELAY);
        {
            // wait so the other thread can grab its first lock
            vTaskDelay(HOLD_TIME);

            xSemaphoreTake(args->second, portMAX_DELAY);
            {
                args->counter++;
            }
            xSemaphoreGive(args->second);
        }
        xSemaphoreGive(args->first);
    }
}

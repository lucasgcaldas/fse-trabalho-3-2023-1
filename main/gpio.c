#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "gpio.h"
#include "mqtt.h"

#define TOUCH_SENSOR 18
#define SOUND_SENSOR 19

int proximidade = 0;

void touchSensor()
{
    while (true)
    {
        if (gpio_get_level(TOUCH_SENSOR) == 1)
        {
            gpio_set_level(SOUND_SENSOR, 0);
            proximidade = 0;
        }
        else {
            gpio_set_level(SOUND_SENSOR, 1);
            proximidade = 1;
        }

        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void setSoundSensor()
{
    gpio_set_level(SOUND_SENSOR, 1);
}
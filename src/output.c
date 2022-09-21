
#define LED_PIN GPIO_NUM_2

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/ledc.h"
#include "driver/gpio.h"

#include "esp_log.h"

#include "output.h"

xQueueHandle output_queue;

static const char *TAG = "OUT_DEVICE";

void led_loop()
{
    int intensity;
    while (true)
    {
        if (xQueueReceive(output_queue, &intensity, portMAX_DELAY))
        {

            printf("intensity ===== %d", intensity);
            ESP_LOGI(TAG, "Pin dimmable %d is %d", LED_PIN, intensity);
            ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, intensity * 255 / 100);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void led_config()
{

    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_8_BIT,
        .timer_num = LEDC_TIMER_0,
        .freq_hz = 1000,
        .clk_cfg = LEDC_AUTO_CLK};
    ledc_timer_config(&ledc_timer);

    ledc_channel_config_t ledc_channel = {
        .gpio_num = LED_PIN,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_0,
        .timer_sel = LEDC_TIMER_0,
        .duty = 0,
        .hpoint = 0};
    ledc_channel_config(&ledc_channel);
    ESP_LOGI(TAG, "LED LOOP");

    output_queue = xQueueCreate(5, sizeof(double));
    xTaskCreate(&led_loop, "led_loop", 4096, NULL, 1, NULL);
}

void led_set_state(int value)
{

    xQueueSend(output_queue, &value, portMAX_DELAY);
}
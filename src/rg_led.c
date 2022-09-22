
#include "rg_led.h"
#include <driver/gpio.h>
#include <driver/ledc.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "freertos/semphr.h"
#include "output.h"

typedef struct
{
    int channel;
    int gpio;
    int mode;
    int timer_idx;
} ledc_info_t;

#define LEDC_LED_CH_NUM 2
ledc_info_t ledc_ch[LEDC_LED_CH_NUM];

void rb_led_setup()
{
    int rg_ch;

    // Red
    ledc_ch[0].channel = LEDC_CHANNEL_0;
    ledc_ch[0].gpio = GPIO_NUM_23;
    ledc_ch[0].mode = LEDC_HIGH_SPEED_MODE;
    ledc_ch[0].timer_idx = LEDC_TIMER_0;

    // Green
    ledc_ch[1].channel = LEDC_CHANNEL_1;
    ledc_ch[1].gpio = GPIO_NUM_22;
    ledc_ch[1].mode = LEDC_HIGH_SPEED_MODE;
    ledc_ch[1].timer_idx = LEDC_TIMER_0;

    ledc_timer_config_t ledc_timer = {
        .duty_resolution = LEDC_TIMER_8_BIT,
        .freq_hz = 100,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .timer_num = LEDC_TIMER_0};
    ledc_timer_config(&ledc_timer);

    // config channels

    for (rg_ch = 0; rg_ch < LEDC_LED_CH_NUM; rg_ch++)
    {
        ledc_channel_config_t ledc_channel = {
            .channel = ledc_ch[rg_ch].channel,
            .duty = 0,
            .hpoint = 0,
            .gpio_num = ledc_ch[rg_ch].gpio,
            .intr_type = LEDC_INTR_DISABLE,
            .speed_mode = ledc_ch[rg_ch].mode,
            .timer_sel = ledc_ch[rg_ch].timer_idx,
        };
        ledc_channel_config(&ledc_channel);
    }
}
void rg_set_color(uint8_t red, uint8_t green)
{
    ledc_set_duty(ledc_ch[0].mode, ledc_ch[0].channel, red);
    ledc_update_duty(ledc_ch[0].mode, ledc_ch[0].channel);
    ledc_set_duty(ledc_ch[1].mode, ledc_ch[1].channel, green);
    ledc_update_duty(ledc_ch[1].mode, ledc_ch[1].channel);
}
void blink_rg_led(void *pvParams)
{
    int last_sensor_state = 0;
    while (true)
    {
        rb_led_setup();

        int sensor_state = gpio_get_level(15);

        printf("VALOR ===%d\n", sensor_state);

        if (sensor_state)
        {

            rg_set_color(255, 255);
        }

        if (sensor_state != last_sensor_state)
        {

            handle_touch_sensor("v1/devices/me/attributes", "led_value", sensor_state);
        }
        last_sensor_state = sensor_state;
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}

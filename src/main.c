#include <stdio.h>
#include <string.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_http_client.h"
#include "esp_log.h"
#include "freertos/semphr.h"
#include "output.h"
#include "wifi.h"
#include "mqtt.h"
#include "cJSON.h"
#include "dht_sensor.h"
#include "hal/gpio_types.h"
#include "connection.h"

#include "rg_led.h"

void app_main(void)
{
    // Inicializa o NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    wifiConnectionSemaphore = xSemaphoreCreateBinary();
    mqttConnectionSemaphore = xSemaphoreCreateBinary();
    wifi_start();

    xTaskCreate(&wifiConnection, "MQTT connection", 4096, NULL, 1, NULL);
    xTaskCreate(&handleBrokerCommunication, "Broker communication", 4096, NULL, 1, NULL);
    xTaskCreate(&blink_rg_led, "Touch sensor", 4096, NULL, 1, NULL);
    vTaskDelete(NULL);
}

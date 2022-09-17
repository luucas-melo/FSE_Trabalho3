#include <stdio.h>
#include <string.h>
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_http_client.h"
#include "esp_log.h"
#include "freertos/semphr.h"
#include "hal/gpio_types.h"
#include "connection.h"
#include "mqtt.h"
#include "dht_sensor.h"

#define DHT_PIN GPIO_NUM_16

void wifiConnection(void *params)
{
    while (true)
    {
        if (xSemaphoreTake(wifiConnectionSemaphore, portMAX_DELAY))
        {
            // Processamento Internet
            mqtt_start();
        }
    }
}

void handleBrokerCommunication(void *params)
{

    if (xSemaphoreTake(mqttConnectionSemaphore, portMAX_DELAY))
    {
        while (true)
        {
            dht_config(DHT_PIN);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
    }
}

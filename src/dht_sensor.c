#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"

#include "dht11.h"
#include "dht_sensor.h"
#include "mqtt.h"
#include "cJSON.h"
#include "json_util.h"

const int DHT_HUMIDITY_KEY = 0;
const int DHT_TEMPERATURE_KEY = 1;
static const char *TAG = "DHT SENSOR";

void mqtt_publish_dht(char *topic, const char *key, int payload)
{
    cJSON *json_message = NULL;

    json_message = json_publishObject(key, payload);

    if (json_message == NULL)
        return;

    mqtt_publish(topic, cJSON_Print(json_message));
    cJSON_Delete(json_message);
}

void taskReadDht11(void *key)
{
    struct dht11_reading dht11_data;
    while (true)
    {

        vTaskDelay(10000 / portTICK_PERIOD_MS);
        dht11_data = DHT11_read();

        // printf("STATUS %d %d\n", dht11_data.status);

        if (dht11_data.status == DHT11_OK)
        {
            int isTemperature = strcmp((char *)key, "temperature") == 0;

            if (isTemperature)
            {

                mqtt_publish_dht("v1/devices/me/telemetry", (char *)key, dht11_data.temperature);
                ESP_LOGI(TAG, "temperature: %d", dht11_data.temperature);
            }

            else
            {

                mqtt_publish_dht("v1/devices/me/telemetry", (char *)key, dht11_data.humidity);
                ESP_LOGI(TAG, "humidity: %d", dht11_data.humidity);
            }
        }
    }
}

void dht_config(int pin)
{
    DHT11_init(pin);
    xTaskCreate(&taskReadDht11, "taskReadTemperature", 4096, "temperature", 1, NULL);
    xTaskCreate(&taskReadDht11, "taskReadHumidity", 4096, "humidity", 1, NULL);
}
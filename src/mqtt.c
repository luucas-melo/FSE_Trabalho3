#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_system.h"
#include "esp_event.h"
#include "esp_netif.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"
#include "esp_log.h"
#include "mqtt_client.h"
#include "mqtt.h"
#include "output.h"

#include "cJSON.h"
#define TAG "MQTT"

extern xSemaphoreHandle mqttConnectionSemaphore;
extern xSemaphoreHandle mqttConnectionSemaphore;

esp_mqtt_client_handle_t client;

static esp_err_t mqtt_event_handler_cb(esp_mqtt_event_handle_t event)
{
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;
    int request_id;
    switch (event->event_id)
    {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
        xSemaphoreGive(mqttConnectionSemaphore);

        request_id = esp_mqtt_client_subscribe(client, SERVER_RPC, 0);
        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
        break;

    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_PUBLISHED:
        ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_DATA:
        ESP_LOGI(TAG, "MQTT_EVENT_DATA");
        printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
        printf("DATA=%.*s\r\n", event->data_len, event->data);

        cJSON *msg = cJSON_Parse(event->data);
        char *method = cJSON_GetObjectItem(msg, "method")->valuestring;
        cJSON *params = cJSON_GetObjectItem(msg, "params");
        ESP_LOGI(TAG, "method=%s", method);
        ESP_LOGI(TAG, "params=%s", cJSON_Print(params));
        if (strcmp(method, "setValue") == 0)
        {

            cJSON_Delete(msg);

            // get RPC id
            char requestId[10] = {0};
            strncpy(requestId, event->topic + (strlen(SERVER_RPC) - 1), event->topic_len - (strlen(SERVER_RPC) - 1));
            requestId[event->topic_len - (strlen(SERVER_RPC) - 1)] = '\0';
            ESP_LOGI(TAG, "ID=%s", requestId);

            char topic_response[50] = {0};
            strcpy(topic_response, PUBLISH_CLIENT_RPC);
            strcat(topic_response, requestId);
            int intensity = atoi(cJSON_Print(params));
            // led_blink(intensity);
            ESP_LOGI(TAG, "INTENSIDADE :%d", intensity);
            led_set_state(intensity);
        }

        break;
    case MQTT_EVENT_ERROR:
        ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
        break;
    default:
        ESP_LOGI(TAG, "Other event id:%d", event->event_id);
        break;
    }
    return ESP_OK;
}

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%d", base, event_id);
    mqtt_event_handler_cb(event_data);
}

void mqtt_start()
{
    esp_mqtt_client_config_t mqtt_config = {
        .uri = "mqtt://164.41.98.25",
        .username = "LVE1TXzQosnYSvuBYsb0"};

    client = esp_mqtt_client_init(&mqtt_config);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
    esp_mqtt_client_start(client);
}

void mqtt_publish(char *topic, char *message)
{
    printf("TOPIC %s\n", topic);
    printf("%s\n", message);
    int message_id = esp_mqtt_client_publish(client, topic, message, 0, 1, 0);
    ESP_LOGI(TAG, "Message sent, ID: %d", message_id);
}

void mqtt_subscribe(char *topic)
{
    int message_id = esp_mqtt_client_subscribe(client, topic, 0);
    ESP_LOGI(TAG, "Sub message: %d from topic: %s", message_id, topic);
}

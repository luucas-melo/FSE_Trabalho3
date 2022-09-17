#include <stdio.h>
#include "json_util.h"
#include "esp_log.h"
static const char *TAG = "JSON";

cJSON *json_publishObject(const char *key, int value_)
{
    cJSON *data = cJSON_CreateObject();
    if (data == NULL)
        ESP_LOGE(TAG, "failed to publish json");

    cJSON *value = NULL;
    value = cJSON_CreateNumber(value_);
    if (value == NULL)
        ESP_LOGE(TAG, "failed to publish json");
    cJSON_AddItemToObject(data, key, value);

    return data;
}

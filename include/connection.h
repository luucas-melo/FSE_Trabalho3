xSemaphoreHandle wifiConnectionSemaphore;
xSemaphoreHandle mqttConnectionSemaphore;

void wifiConnection(void *params);

void handleBrokerCommunication(void *params);
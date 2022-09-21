#ifndef MQTT_H
#define MQTT_H

#define SERVER_RPC "v1/devices/me/rpc/request/+"

#define PUBLISH_CLIENT_RPC "v1/devices/me/rpc/response/"

void mqtt_start();

void mqtt_publish(char *topic, char *message);
void mqtt_subscribe(char *topic);

#endif

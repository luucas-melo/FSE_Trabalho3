
#ifndef OUTPUT_H_
#define OUTPUT_H_

void led_config();
void led_set_intensity(int intensity);
void handle_touch_sensor(char *topic, const char *key, int payload);

#endif /* OUTPUT_H_ */
#ifndef DHT_H
#define DHT_H

#include <stdint.h>

void dht_init(void);
float dht_get_temperature(void);
void dht_setup(uint8_t pin);

#endif // DHT_H

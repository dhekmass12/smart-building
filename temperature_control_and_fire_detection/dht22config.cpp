#include <DHT.h>

#define DHTTYPE DHT22
#define DHT5_PIN 23
#define DHT4_PIN 29

const DHT dht5(DHT5_PIN, DHTTYPE);
const DHT dht4(DHT4_PIN, DHTTYPE);
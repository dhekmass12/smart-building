#include <DHT.h>

#define DHTTYPE DHT22
#define DHT1_PIN 23
#define DHT2_PIN 29

const DHT dht1(DHT1_PIN, DHTTYPE);
const DHT dht2(DHT2_PIN, DHTTYPE);
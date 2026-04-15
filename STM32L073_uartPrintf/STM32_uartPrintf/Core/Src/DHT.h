#ifndef DHT_H_
#define DHT_H_

#include "main.h"
#include "stm32l0xx_ll_gpio.h"

/* Variable tick externe (définie dans stm32l0xx_it.c) */
extern volatile uint32_t get_tick;

/* Paramètres */
#define DHT_TIMEOUT                 10000
#define DHT_POLLING_CONTROL         1
#define DHT_POLLING_INTERVAL_DHT11  2000
#define DHT_POLLING_INTERVAL_DHT22  1000
#define DHT_IRQ_CONTROL

/* Structure des données retournées */
typedef struct {
    float hum;
    float temp;
} DHT_data;

/* Type de capteur */
typedef enum {
    DHT11,
    DHT22
} DHT_type;

/* Structure du capteur */
typedef struct {
    GPIO_TypeDef *DHT_Port;
    uint32_t      DHT_Pin;    // LL utilise uint32_t (ex: LL_GPIO_PIN_0)
    DHT_type      type;
    uint32_t      pullUp;     // LL_GPIO_PULL_NO ou LL_GPIO_PULL_UP

    #if DHT_POLLING_CONTROL == 1
    uint32_t lastPollingTime;
    float    lastTemp;
    float    lastHum;
    #endif
} DHT_sensor;

/* Prototype */
DHT_data DHT_getData(DHT_sensor *sensor);

#endif /* DHT_H_ */

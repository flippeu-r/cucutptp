/**
 * @file Dht22.h
 * @brief Librairie DHT22 pour STM32L073 avec LL - SYS3046
 */

#ifndef DHT22_H_
#define DHT22_H_

#include "main.h"
#include "stm32l0xx_ll_gpio.h"
#include <stdint.h>

/* ---- Broche du capteur (PA0) ---- */
#define DHT22_PORT      GPIOA
#define DHT22_PIN       LL_GPIO_PIN_0

/* ---- Structure des donnees retournees ---- */
typedef struct {
    float temperature;  /* Temperature en degres C */
    float humidity;     /* Humidite en % */
    uint8_t error;      /* 0 = OK, 1 = erreur */
} DHT22_Data;

/* ---- Prototypes ---- */

/** @brief Initialise la broche du DHT22 */
void DHT22_Init(void);

/** @brief Lit les donnees du capteur DHT22 */
DHT22_Data DHT22_Read(void);

/** @brief Delai en microsecondes */
void DHT22_DelayUs(uint32_t us);

#endif /* DHT22_H_ */

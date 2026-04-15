/**
 * @file DHT22.h
 * @brief Librairie DHT22 pour STM32L073 avec LL - SYS3046
 * @author Binôme XX
 * @date 2024
 *
 * Utilise uniquement les fonctions LL GPIO et une boucle de délai
 * en microsecondes basée sur les cycles CPU (16MHz).
 * Pas de timer externe nécessaire.
 */

#ifndef DHT22_H_
#define DHT22_H_

#include "main.h"
#include "stm32l0xx_ll_gpio.h"
#include <stdint.h>

/* ---- Fréquence CPU (16MHz) pour le délai en µs ---- */
#define CPU_FREQ_MHZ    16

/* ---- Broche du capteur ---- */
#define DHT22_PORT      GPIOA
#define DHT22_PIN       LL_GPIO_PIN_0

/* ---- Structure des données retournées ---- */
typedef struct {
    float temperature;  /* Température en °C */
    float humidity;     /* Humidité en % */
    uint8_t error;      /* 0 = OK, 1 = erreur */
} DHT22_Data;

/* ---- Prototypes ---- */

/**
 * @brief Initialise la broche du DHT22 en sortie open-drain
 */
void DHT22_Init(void);

/**
 * @brief Lit les données du capteur DHT22
 * @return Structure DHT22_Data avec température, humidité et code erreur
 */
DHT22_Data DHT22_Read(void);

/**
 * @brief Délai en microsecondes (basé sur boucle CPU à 16MHz)
 * @param us Durée en microsecondes
 */
void DHT22_DelayUs(uint32_t us);

#endif /* DHT22_H_ */

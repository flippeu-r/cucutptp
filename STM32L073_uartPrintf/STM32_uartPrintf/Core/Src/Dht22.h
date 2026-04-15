/**
 * @file DHT22.h
 * @brief Header lecture capteur DHT22 pour STM32L073 avec LL
 */

#ifndef DHT22_H_
#define DHT22_H_

#include "main.h"
#include "stm32l0xx_ll_gpio.h"
#include "core_cm0plus.h"  /* Pour DWT */

/* ---- Configuration : adaptez ces 2 lignes selon votre câblage ---- */
#define DHT22_PORT   GPIOA
#define DHT22_PIN    LL_GPIO_PIN_0   /* PA0 */

/* ---- Codes de retour ---- */
typedef enum {
    DHT22_OK              = 0,  /* Lecture réussie */
    DHT22_ERROR_NO_RESPONSE,    /* Capteur absent ou mal câblé */
    DHT22_ERROR_TIMEOUT,        /* Timeout pendant la lecture */
    DHT22_ERROR_CHECKSUM        /* Données corrompues */
} DHT22_Status;

/* ---- Structure des données ---- */
typedef struct {
    float temperature;  /* Température en °C */
    float humidity;     /* Humidité en % */
} DHT22_Data;

/* ---- Prototype ---- */
/**
 * @brief Lit la température et l'humidité du capteur DHT22
 * @param data Pointeur vers la structure de données à remplir
 * @return DHT22_OK si succès, sinon code d'erreur
 */
DHT22_Status DHT22_Read(DHT22_Data *data);

#endif /* DHT22_H_ */

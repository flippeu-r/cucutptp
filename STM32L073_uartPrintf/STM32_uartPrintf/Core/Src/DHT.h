/**
 * @file    DHT.h
 * @brief   Librairie pour le capteur DHT22 - SYS3046
 *
 *          Broche de données : PA0 (modifiable via DHT22_PORT / DHT22_PIN)
 *          Timer requis      : TIM6 configuré en compteur µs (1 tick = 1 µs)
 */

#ifndef DHT_H_
#define DHT_H_

#include "main.h"
#include "gpio.h"
#include <stdint.h>

/* ------------------------------------------------------------------ */
/*  Broche du capteur - à adapter selon votre câblage                  */
/* ------------------------------------------------------------------ */
#define DHT22_PORT  GPIOA
#define DHT22_PIN   GPIO_PIN_0      /* PA0 */

/* ------------------------------------------------------------------ */
/*  Variables globales (définies dans DHT.c, lisibles depuis main.c)   */
/* ------------------------------------------------------------------ */
extern float   Temperature;   /**< Température en °C  */
extern float   Humidity;      /**< Humidité relative en % */
extern uint8_t Presence;      /**< 1 = capteur détecté, 0 ou -1 = absent */

/* ------------------------------------------------------------------ */
/*  API publique                                                        */
/* ------------------------------------------------------------------ */

/**
 * @brief  Envoie l'impulsion de démarrage au capteur DHT22.
 */
void DHT22_Start(void);

/**
 * @brief  Vérifie la réponse du capteur après DHT22_Start().
 * @return 1 si le capteur a répondu correctement, -1 ou 0 sinon.
 */
uint8_t DHT22_Check_Response(void);

/**
 * @brief  Lit un octet de données depuis le capteur.
 * @return Octet lu (8 bits).
 */
uint8_t DHT22_Read(void);

/**
 * @brief  Fonction tout-en-un : démarre, vérifie et lit les 5 octets.
 *         Met à jour Temperature, Humidity et Presence.
 *         À appeler dans la boucle principale (toutes les 2 s minimum).
 */
void DHT22_GetData(void);

#endif /* DHT_H_ */

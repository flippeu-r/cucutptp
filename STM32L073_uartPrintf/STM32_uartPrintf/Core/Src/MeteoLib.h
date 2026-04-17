/**
 * @file MeteoLib.h
 * @brief Librairie de gestion de la station météo - SYS3046  //
 *
 * Gère la lecture DHT22, le contrôle du ventilateur via TIM22,  //
 * et la vérification de la température par rapport à un seuil.  //
 */


#ifndef METEOLIB_H_
#define METEOLIB_H_

#include <stdio.h>
#include <stdbool.h>
#include "DHT.h"


#define TEMP_ADC_CHANNEL  0

/**
 * @brief Lit la température et l'humidité depuis le capteur DHT22  //
 * @return Température en degrés Celsius                             //
 */

double getTemp(void);



/**
 * @brief Active ou désactive le ventilateur via le servo TIM22     //
 * @param run true = ventilateur ON (impulsion 1000µs),             //
 *             false = ventilateur OFF (impulsion 500µs)            //
 */
void faireTournerVentilo(bool run);



/**
 * @brief Définit la température de référence pour le seuil d'alerte //
 * @param temp Température seuil en degrés Celsius                    //
 */
void setTemp(double temp);

/**
 * @brief Vérifie la température et affiche une alerte si hors limites et allume le ventilo
 */
void checkTemp(void);

#endif /* METEOLIB_H_ */


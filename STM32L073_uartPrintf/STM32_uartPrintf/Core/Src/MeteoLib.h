/**
 * @file MeteoLib.h
 * @brief Librairie station météo - SYS3046

 */

#ifndef METEOLIB_H_
#define METEOLIB_H_

#include <stdint.h>

/* ---- Broche ADC capteur température (ex: PA0) ---- */
#define TEMP_ADC_CHANNEL  0   // à adapter selon votre câblage

/* ---- Variable globale capteur ---- */
extern double g_temperature;

/**
 * @brief Lit la température depuis le capteur analogique
 * @param temp valeur brute ADC
 * @return température en degrés Celsius
 */
double capteurTemp(double temp);

/**
 * @brief Lance la mise à jour en continu de la température
 * @param run 1 = démarrer, 0 = arrêter
 */
void faireTourner(uint8_t run);

/**
 * @brief Définit manuellement la température
 * @param temp valeur à setter
 */
void setTemp(double temp);

/**
 * @brief Vérifie la température et affiche une alerte si hors limites
 */
void checkTemp(void);

#endif /* METEOLIB_H_ */


#ifndef METEOLIB_H_
#define METEOLIB_H_

#include <stdint.h>


#define TEMP_ADC_CHANNEL  0   

/* ---- Variable globale capteur ---- */
extern double g_temperature;


double capteurTemp(double temp);


void faireTourner(bool run);


void setTemp(double temp);

/**
 * @brief Vérifie la température et affiche une alerte si hors limites
 */
void checkTemp(void);

#endif /* METEOLIB_H_ */
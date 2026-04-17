#ifndef METEOLIB_H_
#define METEOLIB_H_

#include <stdio.h>
#include <stdbool.h>
#include "DHT.h"

#define TEMP_ADC_CHANNEL  0


double getTemp(void);


void faireTournerVentilo(bool run);


void setTempRef(float temp);

float getTempRef(void);
/**
 * @brief Vérifie la température et affiche une alerte si hors limites et allume le ventilo
 */
void checkTemp(void);

#endif /* METEOLIB_H_ */


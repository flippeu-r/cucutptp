#include "DHT.h"

/* Macros adaptés LL au lieu de HAL */
/* lineDown : tire la ligne vers le bas (état bas) */
#define lineDown()   LL_GPIO_ResetOutputPin(sensor->DHT_Port, sensor->DHT_Pin)
/* lineUp : relâche la ligne (état haut) */
#define lineUp()     LL_GPIO_SetOutputPin(sensor->DHT_Port, sensor->DHT_Pin)
/* getLine : lit l'état de la broche */
#define getLine()    (LL_GPIO_IsInputPinSet(sensor->DHT_Port, sensor->DHT_Pin))
/* Delay : attente en millisecondes via get_tick (SysTick) */
#define Delay(d)     do { uint32_t s = get_tick; while((get_tick - s) < (d)); } while(0)

static void goToOutput(DHT_sensor *sensor) {
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* Par défaut, la ligne est à l'état haut */
    lineUp();

    /* Configuration du port en sortie open-drain */
    GPIO_InitStruct.Pin        = sensor->DHT_Pin;
    GPIO_InitStruct.Mode       = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
    GPIO_InitStruct.Pull       = sensor->pullUp;
    GPIO_InitStruct.Speed      = LL_GPIO_SPEED_FREQ_HIGH;
    LL_GPIO_Init(sensor->DHT_Port, &GPIO_InitStruct);
}

static void goToInput(DHT_sensor *sensor) {
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* Configuration du port en entrée */
    GPIO_InitStruct.Pin  = sensor->DHT_Pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = sensor->pullUp;
    LL_GPIO_Init(sensor->DHT_Port, &GPIO_InitStruct);
}

DHT_data DHT_getData(DHT_sensor *sensor) {
    DHT_data data = {-128.0f, -128.0f};

    #if DHT_POLLING_CONTROL == 1
    /* Limitation de la fréquence d'interrogation du capteur */
    /* Détermination de l'intervalle selon le type de capteur */
    uint16_t pollingInterval;
    if (sensor->type == DHT11) {
        pollingInterval = DHT_POLLING_INTERVAL_DHT11;
    } else {
        pollingInterval = DHT_POLLING_INTERVAL_DHT22;
    }

    /* Si l'intervalle est trop court, on retourne la dernière valeur connue */
    if ((get_tick - sensor->lastPollingTime < pollingInterval) && sensor->lastPollingTime != 0) {
        data.hum  = sensor->lastHum;
        data.temp = sensor->lastTemp;
        return data;
    }
    sensor->lastPollingTime = get_tick + 1;
    #endif

    /* Envoi du signal de démarrage vers le capteur */
    /* On passe la broche en sortie et on tire la ligne vers le bas pendant 18ms */
    goToOutput(sensor);
    lineDown();
    Delay(18);
    /* On relâche la ligne et on repasse en entrée pour lire la réponse */
    lineUp();
    goToInput(sensor);

    #ifdef DHT_IRQ_CONTROL
    /* Désactivation des interruptions pour ne pas perturber la lecture */
    __disable_irq();
    #endif

    /* Attente de la réponse du capteur */
    uint16_t timeout = 0;

    /* Attente du front descendant */
    while (getLine()) {
        timeout++;
        if (timeout > DHT_TIMEOUT) {
            #ifdef DHT_IRQ_CONTROL
            __enable_irq();
            #endif
            /* Le capteur ne répond pas : on remet les dernières valeurs à -128 */
            sensor->lastHum  = -128.0f;
            sensor->lastTemp = -128.0f;
            return data;
        }
    }
    timeout = 0;

    /* Attente du front montant */
    while (!getLine()) {
        timeout++;
        if (timeout > DHT_TIMEOUT) {
            #ifdef DHT_IRQ_CONTROL
            __enable_irq();
            #endif
            sensor->lastHum  = -128.0f;
            sensor->lastTemp = -128.0f;
            return data;
        }
    }
    timeout = 0;

    /* Attente du front descendant avant la transmission des données */
    while (getLine()) {
        timeout++;
        if (timeout > DHT_TIMEOUT) {
            #ifdef DHT_IRQ_CONTROL
            __enable_irq();
            #endif
            return data;
        }
    }

    /* Lecture des 40 bits de données (5 octets) */
    /* Le capteur envoie : 16 bits humidité + 16 bits température + 8 bits checksum */
    uint8_t rawData[5] = {0, 0, 0, 0, 0};
    for (uint8_t a = 0; a < 5; a++) {
        for (uint8_t b = 7; b != 255; b--) {
            uint16_t hT = 0, lT = 0;
            /* Comptage du temps à l'état bas */
            while (!getLine() && lT != 65535) lT++;
            /* Comptage du temps à l'état haut */
            while ( getLine() && hT != 65535) hT++;
            /* Si le temps haut est plus long que le temps bas : c'est un '1' */
            if (hT > lT) rawData[a] |= (1 << b);
        }
    }

    #ifdef DHT_IRQ_CONTROL
    /* Réactivation des interruptions après réception des données */
    __enable_irq();
    #endif

    /* Vérification de l'intégrité des données via le checksum */
    if ((uint8_t)(rawData[0] + rawData[1] + rawData[2] + rawData[3]) == rawData[4]) {
        /* Checksum correct : conversion et retour des valeurs */
        if (sensor->type == DHT22) {
            data.hum = (float)(((uint16_t)rawData[0] << 8) | rawData[1]) * 0.1f;
            /* Vérification du signe de la température (bit 7 de rawData[2]) */
            if (!(rawData[2] & (1 << 7))) {
                data.temp = (float)(((uint16_t)rawData[2] << 8) | rawData[3]) * 0.1f;
            } else {
                /* Température négative */
                rawData[2] &= ~(1 << 7);
                data.temp = (float)(((uint16_t)rawData[2] << 8) | rawData[3]) * -0.1f;
            }
        }
        if (sensor->type == DHT11) {
            data.hum  = (float)rawData[0];
            data.temp = (float)rawData[2];
        }
    }

    #if DHT_POLLING_CONTROL == 1
    /* Sauvegarde des dernières valeurs valides */
    sensor->lastHum  = data.hum;
    sensor->lastTemp = data.temp;
    #endif

    return data;
}

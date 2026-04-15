/**
 * @file DHT22.c
 * @brief Lecture du capteur DHT22 pour STM32L073 avec librairie LL
 * @details Protocole 1-Wire du DHT22 :
 *          1. STM32 envoie un signal bas pendant 1ms (signal de démarrage)
 *          2. STM32 relâche la ligne (high)
 *          3. DHT22 répond avec bas 80µs puis haut 80µs
 *          4. DHT22 envoie 40 bits (16 humidité + 16 température + 8 checksum)
 *          5. Bit 0 = haut ~26µs / Bit 1 = haut ~70µs
 */

#include "DHT22.h"

/* ---- Délai microsecondes via DWT ---- */
/* Le DWT (Data Watchpoint and Trace) est un compteur de cycles CPU */
/* A 16MHz : 1µs = 16 cycles */
#define CPU_FREQ_MHZ  16

static void delay_us(uint32_t us) {
    /* Activation du compteur de cycles DWT */
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL  |= DWT_CTRL_CYCCNTENA_Msk;

    uint32_t start = DWT->CYCCNT;
    uint32_t wait  = us * CPU_FREQ_MHZ;
    while ((DWT->CYCCNT - start) < wait);
}

/* ---- Basculer la broche en sortie open-drain ---- */
static void setOutput(void) {
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin        = DHT22_PIN;
    GPIO_InitStruct.Mode       = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
    GPIO_InitStruct.Pull       = LL_GPIO_PULL_UP;
    GPIO_InitStruct.Speed      = LL_GPIO_SPEED_FREQ_HIGH;
    LL_GPIO_Init(DHT22_PORT, &GPIO_InitStruct);
}

/* ---- Basculer la broche en entrée ---- */
static void setInput(void) {
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin  = DHT22_PIN;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
    LL_GPIO_Init(DHT22_PORT, &GPIO_InitStruct);
}

/* ---- Lire l'état de la broche ---- */
static uint8_t readPin(void) {
    return LL_GPIO_IsInputPinSet(DHT22_PORT, DHT22_PIN) ? 1 : 0;
}

/* ---- Attendre un état avec timeout ---- */
/* Retourne 1 si l'état est atteint, 0 si timeout */
static uint8_t waitFor(uint8_t state, uint32_t timeout_us) {
    uint32_t count = 0;
    while (readPin() != state) {
        delay_us(1);
        count++;
        if (count > timeout_us) return 0; /* timeout */
    }
    return 1;
}

/* ---- Fonction principale de lecture ---- */
DHT22_Status DHT22_Read(DHT22_Data *data) {

    uint8_t rawData[5] = {0};

    /* --- Etape 1 : Signal de démarrage --- */
    /* On tire la ligne vers le bas pendant 1ms */
    setOutput();
    LL_GPIO_ResetOutputPin(DHT22_PORT, DHT22_PIN);
    delay_us(1200); /* 1.2ms pour être sûr (minimum 1ms selon datasheet) */

    /* On relâche la ligne */
    LL_GPIO_SetOutputPin(DHT22_PORT, DHT22_PIN);
    delay_us(30);

    /* On passe en entrée pour lire la réponse */
    setInput();

    /* --- Etape 2 : Attente réponse du DHT22 --- */
    /* Le DHT22 tire la ligne vers le bas pendant ~80µs */
    if (!waitFor(0, 100)) {
        return DHT22_ERROR_NO_RESPONSE;
    }

    /* Puis relâche pendant ~80µs */
    if (!waitFor(1, 100)) {
        return DHT22_ERROR_NO_RESPONSE;
    }

    /* Puis retire à nouveau avant d'envoyer les données */
    if (!waitFor(0, 100)) {
        return DHT22_ERROR_NO_RESPONSE;
    }

    /* --- Etape 3 : Lecture des 40 bits --- */
    /* Désactivation des interruptions pour ne pas perturber le timing */
    __disable_irq();

    for (uint8_t i = 0; i < 40; i++) {
        uint8_t octet = i / 8;
        uint8_t bit   = 7 - (i % 8);

        /* Attente du front montant (début du bit) */
        uint32_t count = 0;
        while (readPin() == 0) {
            delay_us(1);
            if (++count > 100) {
                __enable_irq();
                return DHT22_ERROR_TIMEOUT;
            }
        }

        /* Mesure de la durée du signal haut */
        /* Bit 0 = ~26µs / Bit 1 = ~70µs */
        /* On attend 50µs et on regarde si c'est encore haut → c'est un 1 */
        delay_us(50);
        if (readPin() == 1) {
            rawData[octet] |= (1 << bit); /* C'est un 1 */
            /* On attend la fin du bit 1 */
            count = 0;
            while (readPin() == 1) {
                delay_us(1);
                if (++count > 100) {
                    __enable_irq();
                    return DHT22_ERROR_TIMEOUT;
                }
            }
        }
        /* Si déjà à 0 après 50µs → c'est un 0, bit déjà à 0 par défaut */
    }

    __enable_irq();

    /* --- Etape 4 : Vérification du checksum --- */
    uint8_t checksum = rawData[0] + rawData[1] + rawData[2] + rawData[3];
    if (checksum != rawData[4]) {
        return DHT22_ERROR_CHECKSUM;
    }

    /* --- Etape 5 : Conversion des données --- */
    /* Humidité : 2 octets, résolution 0.1% */
    data->humidity = (float)(((uint16_t)rawData[0] << 8) | rawData[1]) * 0.1f;

    /* Température : 2 octets, résolution 0.1°C */
    /* Bit 15 de rawData[2] indique le signe (1 = négatif) */
    if (rawData[2] & 0x80) {
        /* Température négative */
        rawData[2] &= 0x7F; /* On retire le bit de signe */
        data->temperature = (float)(((uint16_t)rawData[2] << 8) | rawData[3]) * -0.1f;
    } else {
        data->temperature = (float)(((uint16_t)rawData[2] << 8) | rawData[3]) * 0.1f;
    }

    return DHT22_OK;
}
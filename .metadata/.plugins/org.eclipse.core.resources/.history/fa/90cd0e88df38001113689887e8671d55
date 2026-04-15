/**
 * @file DHT22.c
 * @brief Librairie DHT22 pour STM32L073 avec LL - SYS3046
 * @author Binôme XX
 * @date 2024
 *
 * Protocole DHT22 :
 * 1. STM32 envoie signal bas pendant 1ms (start signal)
 * 2. STM32 relâche la ligne et passe en entrée
 * 3. DHT22 répond avec bas 80µs + haut 80µs
 * 4. DHT22 envoie 40 bits : bas 50µs + haut 26µs=0 ou 70µs=1
 * 5. Vérification du checksum (octet 5 = somme des 4 premiers)
 */

#include "DHT22.h"
#include <stdio.h>

/* ---- Macros de contrôle de la broche ---- */
/* Met la broche à l'état bas */
#define PIN_LOW()    LL_GPIO_ResetOutputPin(DHT22_PORT, DHT22_PIN)
/* Relâche la broche (état haut grâce au pull-up) */
#define PIN_HIGH()   LL_GPIO_SetOutputPin(DHT22_PORT, DHT22_PIN)
/* Lit l'état de la broche */
#define PIN_READ()   LL_GPIO_IsInputPinSet(DHT22_PORT, DHT22_PIN)

/* ---- Délai en microsecondes ---- */
/* A 16MHz, 1µs = ~16 cycles. On retire l'overhead de la boucle (~4 cycles) */
void DHT22_DelayUs(uint32_t us) {
    /* Calibration pour STM32L073 à 16MHz */
    /* Chaque itération = ~4 cycles => 16/4 = 4 itérations par µs */
    volatile uint32_t count = us * 4;
    while (count--);
}

/* ---- Passe la broche en sortie open-drain ---- */
static void setOutput(void) {
    LL_GPIO_InitTypeDef cfg = {0};
    cfg.Pin        = DHT22_PIN;
    cfg.Mode       = LL_GPIO_MODE_OUTPUT;
    cfg.OutputType = LL_GPIO_OUTPUT_OPENDRAIN; /* Open-drain obligatoire pour DHT22 */
    cfg.Pull       = LL_GPIO_PULL_UP;          /* Pull-up interne activé */
    cfg.Speed      = LL_GPIO_SPEED_FREQ_VERY_HIGH;
    LL_GPIO_Init(DHT22_PORT, &cfg);
}

/* ---- Passe la broche en entrée ---- */
static void setInput(void) {
    LL_GPIO_InitTypeDef cfg = {0};
    cfg.Pin  = DHT22_PIN;
    cfg.Mode = LL_GPIO_MODE_INPUT;
    cfg.Pull = LL_GPIO_PULL_UP;                /* Pull-up interne activé */
    LL_GPIO_Init(DHT22_PORT, &cfg);
}

/* ---- Initialisation ---- */
void DHT22_Init(void) {
    /* On démarre en sortie avec ligne haute */
    setOutput();
    PIN_HIGH();
    /* Attente stabilisation (1 seconde recommandée après power-on) */
    /* On fait juste 250ms pour ne pas bloquer trop longtemps */
    DHT22_DelayUs(250000);
}

/* ---- Lecture des données ---- */
DHT22_Data DHT22_Read(void) {
    DHT22_Data result = {0.0f, 0.0f, 1}; /* Par défaut : erreur */
    uint8_t rawData[5] = {0, 0, 0, 0, 0};
    uint32_t timeout;

    /* === ETAPE 1 : Signal de démarrage === */
    /* On tire la ligne bas pendant 1ms pour réveiller le capteur */
    setOutput();
    PIN_LOW();
    DHT22_DelayUs(1200);    /* 1.2ms (minimum 1ms selon datasheet) */
    PIN_HIGH();
    DHT22_DelayUs(30);      /* Attente 30µs avant de passer en entrée */
    setInput();

    /* === ETAPE 2 : Attente réponse du capteur === */
    /* Le DHT22 doit tirer la ligne bas dans les 40µs */

    /* Attente que la ligne descende (front descendant) */
    timeout = 10000;
    while (PIN_READ()) {
        timeout--;
        if (timeout == 0) {
            printf("[DHT22] ERREUR : pas de reponse du capteur (front descendant)\r\n");
            result.error = 1;
            return result;
        }
    }

    /* Attente que la ligne remonte (front montant) ~80µs */
    timeout = 10000;
    while (!PIN_READ()) {
        timeout--;
        if (timeout == 0) {
            printf("[DHT22] ERREUR : timeout front montant\r\n");
            result.error = 1;
            return result;
        }
    }

    /* Attente que la ligne redescende avant les données ~80µs */
    timeout = 10000;
    while (PIN_READ()) {
        timeout--;
        if (timeout == 0) {
            printf("[DHT22] ERREUR : timeout avant donnees\r\n");
            result.error = 1;
            return result;
        }
    }

    /* === ETAPE 3 : Lecture des 40 bits === */
    /* Chaque bit : bas 50µs + haut 26µs(=0) ou 70µs(=1) */
    /* On mesure la durée du niveau haut pour distinguer 0 et 1 */
    for (uint8_t i = 0; i < 40; i++) {
        uint32_t lowTime = 0, highTime = 0;

        /* Attente front montant (fin du bas 50µs) */
        timeout = 10000;
        while (!PIN_READ()) {
            lowTime++;
            timeout--;
            if (timeout == 0) {
                printf("[DHT22] ERREUR : timeout bit %d (front montant)\r\n", i);
                result.error = 1;
                return result;
            }
        }

        /* Mesure durée état haut */
        timeout = 10000;
        while (PIN_READ()) {
            highTime++;
            timeout--;
            if (timeout == 0) {
                printf("[DHT22] ERREUR : timeout bit %d (front descendant)\r\n", i);
                result.error = 1;
                return result;
            }
        }

        /* Si haut > bas alors c'est un '1', sinon c'est un '0' */
        rawData[i / 8] <<= 1;
        if (highTime > lowTime) {
            rawData[i / 8] |= 1;
        }
    }

    /* === ETAPE 4 : Vérification du checksum === */
    printf("[DHT22] Donnees brutes : %02X %02X %02X %02X | checksum recu: %02X\r\n",
           rawData[0], rawData[1], rawData[2], rawData[3], rawData[4]);

    uint8_t checksum = rawData[0] + rawData[1] + rawData[2] + rawData[3];

    if (checksum != rawData[4]) {
        printf("[DHT22] ERREUR : checksum incorrect (calcule: %02X, recu: %02X)\r\n",
               checksum, rawData[4]);
        result.error = 1;
        return result;
    }

    /* === ETAPE 5 : Conversion des données === */
    /* Humidité : octets 0 et 1 */
    result.humidity = (float)(((uint16_t)rawData[0] << 8) | rawData[1]) * 0.1f;

    /* Température : octets 2 et 3 */
    /* Bit 7 de l'octet 2 indique le signe (1 = négatif) */
    if (rawData[2] & 0x80) {
        /* Température négative */
        result.temperature = (float)(((uint16_t)(rawData[2] & 0x7F) << 8) | rawData[3]) * -0.1f;
    } else {
        result.temperature = (float)(((uint16_t)rawData[2] << 8) | rawData[3]) * 0.1f;
    }

    result.error = 0; /* Lecture réussie */

    printf("[DHT22] OK : Temp=%.1f C  Hum=%.1f %%\r\n",
           result.temperature, result.humidity);

    return result;
}

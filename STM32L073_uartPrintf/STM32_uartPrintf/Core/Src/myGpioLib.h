/**
 * @file myGpioLib.h
 * @brief Librairie de contrôle GPIO pour STM32L0 - SYS3046
 *
 *
 */

#ifndef INC_MYGPIOLIB_H_
#define INC_MYGPIOLIB_H_

#include <stdint.h>

/* ---- Adresses registres GPIOA ---- */
#define GPIOA_IDR  *((volatile uint32_t *) 0x50000010)
#define GPIOA_ODR  *((volatile uint32_t *) 0x50000014)


/* ---- LED2 : PA5 ---- */
#define LED2_BIT   (1U << 5)

/* ---- Bouton B1 USER : PC13 ---- */
#define GPIOC_IDR  *((volatile uint32_t *) 0x50000810)
#define B1_BIT     (1U << 13)

/* ---- Registres GPIOA complets ---- */
#define GPIOA_MODER  *((volatile uint32_t *) 0x50000000)

/* ---- Prototypes ---- */



extern volatile uint32_t get_tick;


// ----------------------//
/**
 * @brief Allume la LED2 pendant une durée donnée  // ➕
 * @param duration_100ms durée en 1/10s (ex: 20 = 2 secondes)  // ➕
 */
void LED2_On_Duration(uint32_t duration_100ms);
/**
 * @brief Ecrit l'état logique d'une sortie du port A
 * @param pin  Numéro de la broche (0 à 15)
 * @param state 1 = niveau haut, 0 = niveau bas
 */
void GPIOA_WritePin(uint8_t pin, uint8_t state);

/**
 * @brief Lit l'état logique d'une sortie du port A (ODR)
 * @param pin  Numéro de la broche (0 à 15)
 * @return 1 si haut, 0 si bas
 */
uint8_t GPIOA_ReadOutput(uint8_t pin);

/**
 * @brief Clignotement infini de la LED2
 * @param half_period_100ms demi-période en 1/10s (ex: 5 = 500ms soit 1Hz)
 */
void LED2_Blink(uint32_t half_period_100ms);


/**
 * @brief Clignotement de la LED2 pendant une durée limitée
 * @param half_period_100ms demi-période en 1/10s
 * @param duration_100ms    durée totale du clignotement en 1/10s
 */
void LED2_Blink_Duration(uint32_t half_period_100ms, uint32_t duration_100ms);

/**
 * @brief Lit l'état logique d'une entrée du port A (IDR)
 * @param pin  Numéro de la broche (0 à 15)
 * @return 1 si haut, 0 si bas
 */
uint8_t GPIOA_ReadInput(uint8_t pin);

/**
 * @brief Change l'état d'une sortie du port A (toggle)
 * @param pin  Numéro de la broche (0 à 15)
 */
void GPIOA_TogglePin(uint8_t pin);


/**
 * @brief Allume ou éteint la LED2
 * @param state 1 = allumée, 0 = éteinte
 */
void LED2_Write(uint8_t state);

/**
 * @brief Lit l'état de la LED2
 * @return 1 si allumée, 0 si éteinte
 */
uint8_t LED2_Read(void);

/**
 * @brief Lit l'état du bouton B1 USER
 * @return 1 si appuyé, 0 sinon
 */
uint8_t B1_Read(void);

/**
 * @brief Fonction de test des fonctions LED2 et B1
 */
void GPIO_Test(void);

#endif

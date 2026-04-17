/* USER CODE BEGIN Header */
/**
 * @mainpage  SYS3046 - Station Météo
 *
 * @brief     Projet embarqué sur STM32L073RZ (NUCLEO-L073RZ).
 *
 *            Ce projet implémente une station météo complète utilisant :
 *            - Le capteur DHT22 (PA0) pour la température et l'humidité
 *            - La photorésistance (PA1, ADC) pour la luminosité
 *            - Une LED (PA6) qui s'allume en cas d'obscurité
 *            - Un servomoteur (PB4, TIM22 PWM) piloté par MeteoLib
 *            - Une interface série (UART2, 115200 baud) via PuTTY
 *
 *            L'utilisateur peut modifier la température de référence
 *            en temps réel via les flèches ↑ / ↓ du clavier.
 *
 * @author    Pierre
 * @date      2026
 * @version   1.0
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <retarget.h>
#include <getch.h>
#include "myGpioLib.h"
#include "DHT.h"
#include "MeteoLib.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/** @brief Seuil ADC au-dessus duquel on considère qu'il fait nuit (0-4095) */
#define SEUIL_OBSCURITE     50U

/** @brief Température de référence initiale en °C */
#define TEMP_REF_INITIALE   27.0f

/** @brief Nombre de mesures ADC pour le calcul de la moyenne */
#define ADC_NB_MESURES      10U
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/** @brief Compteur de temps système en millisecondes (incrémenté par SysTick) */
extern volatile uint32_t get_tick;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
static void traiterClavier(void);
static uint32_t lireLuminosite(void);
static void gererLED(uint32_t adc_val);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/**
 * @brief  Délai en microsecondes basé sur TIM6.
 * @param  time Durée en microsecondes
 */
void delay(uint16_t time)
{
    __HAL_TIM_SET_COUNTER(&htim6, 0);
    while ((__HAL_TIM_GET_COUNTER(&htim6)) < time);
}

/**
 * @brief  Gère la saisie clavier pour modifier la température de référence.
 *         Les flèches ↑ / ↓ augmentent ou diminuent tempRef de 1°C.
 *         Séquence d'échappement : 0x1B 0x5B 0x41 (haut) / 0x42 (bas).
 */
static void traiterClavier(void)
{
    if (!kbhit()) return;

    uint8_t c = getch();

    if (c == 0x1B)
    {
        uint8_t c2 = getch();
        uint8_t c3 = getch();

        if (c2 == 0x5B)
        {
            float ref = getTempRef();

            if (c3 == 0x41)      /* flèche HAUT */
            {
                setTempRef(ref + 1.0f);
                printf(">> TempRef augmentee : %.1f C\r\n", getTempRef());
            }
            else if (c3 == 0x42) /* flèche BAS */
            {
                setTempRef(ref - 1.0f);
                printf(">> TempRef diminuee  : %.1f C\r\n", getTempRef());
            }
        }
    }
}

/**
 * @brief  Lit la luminosité via l'ADC sur PA1 (moyenne sur ADC_NB_MESURES).
 * @return Valeur ADC moyenne (0 = très lumineux, 4095 = obscurité totale)
 */
static uint32_t lireLuminosite(void)
{
    uint32_t somme = 0;
    for (int i = 0; i < ADC_NB_MESURES; i++)
    {
        HAL_ADC_Start(&hadc);
        HAL_ADC_PollForConversion(&hadc, 100);
        somme += HAL_ADC_GetValue(&hadc);
    }
    return somme / ADC_NB_MESURES;
}

/**
 * @brief  Pilote la LED PA6 selon la luminosité mesurée.
 *         LED allumée si adc_val > SEUIL_OBSCURITE (obscurité détectée).
 * @param  adc_val Valeur ADC moyenne issue de lireLuminosite()
 */
static void gererLED(uint32_t adc_val)
{
    if (adc_val > SEUIL_OBSCURITE)
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
        printf("LED ON  -> obscurite detectee\r\n");
    }
    else
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
        printf("LED OFF -> lumiere detectee\r\n");
    }
}

/* USER CODE END 0 */

/**
 * @brief  Point d'entrée de l'application.
 * @retval int
 */
int main(void)
{
  /* USER CODE BEGIN 1 */
  /* USER CODE END 1 */

  /* MCU Configuration -------------------------------------------------------*/
  HAL_Init();

  /* USER CODE BEGIN Init */
  /* USER CODE END Init */

  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  /* USER CODE END SysInit */

  /* Initialisation des périphériques ----------------------------------------*/
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_TIM2_Init();
  MX_TIM6_Init();
  MX_ADC_Init();
  MX_TIM3_Init();
  MX_TIM22_Init();

  /* USER CODE BEGIN 2 */

  /* --- Communication série --- */
  RetargetInit(USART2);
  getchInit();
  LL_USART_EnableIT_RXNE(USART2);

  /* --- Timers et ADC --- */
  LL_SYSTICK_EnableIT();
  HAL_TIM_Base_Start(&htim6);
  HAL_ADCEx_Calibration_Start(&hadc, ADC_SINGLE_ENDED);

  /* --- Servomoteur TIM22 CH1 sur PB4 --- */
  LL_TIM_CC_EnableChannel(TIM22, LL_TIM_CHANNEL_CH1);
  LL_TIM_EnableCounter(TIM22);

  /* --- Température de référence initiale --- */
  setTempRef(TEMP_REF_INITIALE);

  /* --- Message de bienvenue --- */
  printf("\r\n=== SYS3046 - Station Meteo ===\r\n");
  printf("Fleches haut/bas : modifier la temperature de reference\r\n\r\n");

  /* USER CODE END 2 */

  /* Boucle principale -------------------------------------------------------*/
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
    /* USER CODE BEGIN 3 */

    /* --- Affichage de la température de référence courante --- */
    printf("-- TempRef : %.1f C  |  Fleche haut/bas pour modifier --\r\n",
           getTempRef());

    /* --- Gestion du clavier (modification tempRef) --- */
    traiterClavier();

    /* --- Lecture et affichage température / humidité + pilotage servo --- */
    checkTemp();

    /* --- Lecture luminosité et pilotage LED --- */
    uint32_t adc_val = lireLuminosite();
    float tension = (adc_val / 4095.0f) * 3.3f;
    printf("Luminosite : %4lu  |  Tension : %.2f V\r\n", adc_val, tension);
    gererLED(adc_val);

    /* --- Séparateur et délai --- */
    printf("\r\n");
    HAL_Delay(2000);

  }
  /* USER CODE END 3 */
}
/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_0)
  {
  }
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
  while (LL_PWR_IsActiveFlag_VOS() != 0)
  {
  }
  LL_RCC_HSI_Enable();

   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1)
  {

  }
  LL_RCC_HSI_SetCalibTrimming(16);
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI)
  {

  }
  LL_SetSystemCoreClock(16000000);

   /* Update the time base */
  if (HAL_InitTick (TICK_INT_PRIORITY) != HAL_OK)
  {
    Error_Handler();
  }
  LL_RCC_SetUSARTClockSource(LL_RCC_USART2_CLKSOURCE_PCLK1);
}

/* USER CODE BEGIN 4 */
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  __disable_irq();
  while (1) {}
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/* USER CODE BEGIN Header */
/**
 * @mainpage  SYS3046 course template
 *
 *            Implémentation de notre librairie MyGpioLib.c permettant de lire/écrire
 *            l'état de différentes sorties et le clignotement d'une led.
 *            Station météo avec capteur DHT22.
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "gpio.h"
#include "tim.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <retarget.h>
#include <getch.h>
#include "myGpioLib.h"
#include "Dht22.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
extern volatile uint32_t get_tick;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* USER CODE END 0 */

int main(void)
{
  /* USER CODE BEGIN 1 */
  /* USER CODE END 1 */

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  /* USER CODE BEGIN Init */
  /* USER CODE END Init */

  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  /* USER CODE END SysInit */

  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_TIM2_Init();

  /* USER CODE BEGIN 2 */
  RetargetInit(USART2);
  getchInit();
  LL_USART_EnableIT_RXNE(USART2);
  LL_SYSTICK_EnableIT();
  DHT22_Init();

  printf("\r\n=== SYS3046 - Station Meteo ===\r\n");
  printf("Commandes : 1=LED ON, 0=LED OFF, t=Tests, d=Duree 2s, b=Blink, c=Blink 5s, v=Tick, m=Meteo\r\n\r\n");
  /* USER CODE END 2 */

  /* USER CODE BEGIN WHILE */
  while (1)
  {
    if (kbhit())
    {
      char c = getch();
      printf("Recu : %c\r\n", c);

      if (c == '1') {
        LED2_Write(1);
        printf("LED2 allumee\r\n");
      }
      else if (c == '0') {
        LED2_Write(0);
        printf("LED2 eteinte\r\n");
      }
      else if (c == 't') {
        GPIO_Test();
      }
      else if (c == 'd') {
        printf("LED2 allumee 2 secondes...\r\n");
        LED2_On_Duration(20);
        printf("LED2 eteinte\r\n");
      }
      else if (c == 'v') {
        printf("Tick actuel : %lu ms\r\n", get_tick);
      }
      else if (c == 'b') {
        printf("Clignotement 1Hz (appuyez RESET pour arreter)\r\n");
        LED2_Blink(5);
      }
      else if (c == 'c') {
        printf("Clignotement 1Hz pendant 5s...\r\n");
        LED2_Blink_Duration(5, 50);
        printf("Fin clignotement\r\n");
      }
      else if (c == 'm') {
        DHT22_Data d = DHT22_Read();
        if (d.error) {
          printf("Erreur : capteur non detecte !\r\n");
        } else {
          printf("Temperature : %.1f C\r\n", d.temperature);
          printf("Humidite    : %.1f %%\r\n", d.humidity);
        }
      }
    }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);
  while(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_0) {}

  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
  while (LL_PWR_IsActiveFlag_VOS() != 0) {}

  LL_RCC_HSI_Enable();
  while(LL_RCC_HSI_IsReady() != 1) {}

  LL_RCC_HSI_SetCalibTrimming(16);
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);   /* 16MHz pleine vitesse */
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI) {}

  LL_Init1msTick(16000000);        /* SysTick toutes les 1ms a 16MHz */
  LL_SetSystemCoreClock(16000000);
  LL_RCC_SetUSARTClockSource(LL_RCC_USART2_CLKSOURCE_PCLK1);
}

/* USER CODE BEGIN 4 */
/* USER CODE END 4 */

void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  __disable_irq();
  while (1) {}
  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* USER CODE END 6 */
}
#endif

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


void delay (uint16_t time)
{
	/* change your code here for the delay in microseconds */
	__HAL_TIM_SET_COUNTER(&htim6, 0);
	while ((__HAL_TIM_GET_COUNTER(&htim6))<time);
}




uint8_t Rh_byte1, Rh_byte2, Temp_byte1, Temp_byte2;
uint16_t SUM, RH, TEMP;

//float Temperature = 0;
//float Humidity = 0;
//uint8_t Presence = 0;

void Set_Pin_Output (GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

void Set_Pin_Input (GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

/*********************************** DHT22 FUNCTIONS ****************************************/

#define DHT22_PORT GPIOA
#define DHT22_PIN GPIO_PIN_0
//
//void DHT22_Start (void)
//{
//	Set_Pin_Output(DHT22_PORT, DHT22_PIN); // set the pin as output
//	HAL_GPIO_WritePin (DHT22_PORT, DHT22_PIN, 0);   // pull the pin low
//	delay(1200);   // wait for > 1ms
//
//	HAL_GPIO_WritePin (DHT22_PORT, DHT22_PIN, 1);   // pull the pin high
//	delay (20);   // wait for 30us
//
//	Set_Pin_Input(DHT22_PORT, DHT22_PIN);   // set as input
//}
//
//uint8_t DHT22_Check_Response (void)
//{
//	Set_Pin_Input(DHT22_PORT, DHT22_PIN);   // set as input
//	uint8_t Response = 0;
//	delay (40);  // wait for 40us
//	if (!(HAL_GPIO_ReadPin (DHT22_PORT, DHT22_PIN))) // if the pin is low
//	{
//		delay (80);   // wait for 80us
//
//		if ((HAL_GPIO_ReadPin (DHT22_PORT, DHT22_PIN))) Response = 1;  // if the pin is high, response is ok
//		else Response = -1;
//	}
//
//	while ((HAL_GPIO_ReadPin (DHT22_PORT, DHT22_PIN)));   // wait for the pin to go low
//	return Response;
//}
//
//uint8_t DHT22_Read (void)
//{
//	uint8_t i,j;
//	for (j=0;j<8;j++)
//	{
//		while (!(HAL_GPIO_ReadPin (DHT22_PORT, DHT22_PIN)));   // wait for the pin to go high
//		delay (40);   // wait for 40 us
//
//		if (!(HAL_GPIO_ReadPin (DHT22_PORT, DHT22_PIN)))   // if the pin is low
//		{
//			i&= ~(1<<(7-j));   // write 0
//		}
//		else i|= (1<<(7-j));  // if the pin is high, write 1
//		while ((HAL_GPIO_ReadPin (DHT22_PORT, DHT22_PIN)));  // wait for the pin to go low
//	}
//
//	return i;
//}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_TIM2_Init();
  MX_TIM6_Init();
  MX_ADC_Init();
  MX_TIM3_Init();
  MX_TIM22_Init();
  /* USER CODE BEGIN 2 */
  RetargetInit(USART2);
  getchInit();
  LL_USART_EnableIT_RXNE(USART2);
  LL_SYSTICK_EnableIT();
  HAL_TIM_Base_Start(&htim6);
  HAL_ADCEx_Calibration_Start(&hadc, ADC_SINGLE_ENDED);
  LL_TIM_CC_EnableChannel(TIM22, LL_TIM_CHANNEL_CH1); // CH2 car PWM Gen CH2
  LL_TIM_EnableCounter(TIM22);


  printf("\r\n=== SYS3046 - Station Meteo ===\r\n");
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  setTemp(25.5);
	  while (1)
	  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		  checkTemp();


	    /* --- Lecture photorésistance PA1 : moyenne sur 10 mesures --- */
	    uint32_t somme = 0;
	    for (int i = 0; i < 10; i++) {
	        HAL_ADC_Start(&hadc);
	        HAL_ADC_PollForConversion(&hadc, 100);
	        somme += HAL_ADC_GetValue(&hadc);
	    }
	    uint32_t adc_val = somme / 10;
	    float tension = (adc_val / 4095.0f) * 3.3f;
	    printf("Luminosite : %4lu  |  Tension : %.2f V\r\n", adc_val, tension);

	    /* --- LED allumée si obscurité (valeur > 600) --- */
	    if (adc_val > 1000) {
	        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
	        printf("LED ON  -> obscurite detectee\r\n");
	    } else {
	        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
	        printf("LED OFF -> lumiere detectee\r\n");
	    }

	    HAL_Delay(2000);
	    printf("\r\n");
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

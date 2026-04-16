#include "DHT.h"

/* Timer utilisé pour les délais µs - défini dans main.c */
extern TIM_HandleTypeDef htim6;

/* Variables internes */
static uint8_t Rh_byte1, Rh_byte2, Temp_byte1, Temp_byte2;
static uint16_t RH, TEMP;

/* Variables publiques */
float   Temperature = 0;
float   Humidity    = 0;
uint8_t Presence    = 0;

/* ---------------------------------------------------------- */
static void delay(uint16_t time)
{
    __HAL_TIM_SET_COUNTER(&htim6, 0);
    while (__HAL_TIM_GET_COUNTER(&htim6) < time);
}

static void Set_Pin_Output(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin   = GPIO_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

static void Set_Pin_Input(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin  = GPIO_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

/* ---------------------------------------------------------- */
void DHT22_Start(void)
{
    Set_Pin_Output(DHT22_PORT, DHT22_PIN);
    HAL_GPIO_WritePin(DHT22_PORT, DHT22_PIN, 0);
    delay(1200);
    HAL_GPIO_WritePin(DHT22_PORT, DHT22_PIN, 1);
    delay(20);
    Set_Pin_Input(DHT22_PORT, DHT22_PIN);
}

uint8_t DHT22_Check_Response(void)
{
    Set_Pin_Input(DHT22_PORT, DHT22_PIN);
    uint8_t Response = 0;
    delay(40);
    if (!(HAL_GPIO_ReadPin(DHT22_PORT, DHT22_PIN)))
    {
        delay(80);
        if (HAL_GPIO_ReadPin(DHT22_PORT, DHT22_PIN)) Response = 1;
        else Response = -1;
    }
    while (HAL_GPIO_ReadPin(DHT22_PORT, DHT22_PIN));
    return Response;
}

uint8_t DHT22_Read(void)
{
    uint8_t i = 0, j;
    for (j = 0; j < 8; j++)
    {
        while (!(HAL_GPIO_ReadPin(DHT22_PORT, DHT22_PIN)))
        	{
        	}
        delay(40);
        if (!(HAL_GPIO_ReadPin(DHT22_PORT, DHT22_PIN)))
            i &= ~(1 << (7 - j));
        else
            i |=  (1 << (7 - j));
        while (HAL_GPIO_ReadPin(DHT22_PORT, DHT22_PIN));
    }
    return i;
}

/* Fonction tout-en-un : à appeler depuis main.c */
void DHT22_GetData(void)
{
    DHT22_Start();
    Presence = DHT22_Check_Response();

    Rh_byte1   = DHT22_Read();
    Rh_byte2   = DHT22_Read();
    Temp_byte1 = DHT22_Read();
    Temp_byte2 = DHT22_Read();
    /* SUM ignoré ici, ajouter vérification checksum si nécessaire */
    DHT22_Read();

    TEMP = (Temp_byte1 << 8) | Temp_byte2;
    RH   = (Rh_byte1   << 8) | Rh_byte2;

    Temperature = (float)(TEMP / 10.0f);
    Humidity    = (float)(RH   / 10.0f);
}

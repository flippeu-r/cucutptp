#include "MeteoLib.h"
#include "stm32l0xx_ll_tim.h"

double getTemp()
{
	DHT22_GetData();
}

void faireTournerVentilo(bool run)
{
	if(run)
	{
		LL_TIM_OC_SetCompareCH1(TIM22, 1000);
	}
	else
	{
		LL_TIM_OC_SetCompareCH1(TIM22, 500);
	}
}

void setTemp(double temp){
	tempRef = temp;
}

void checkTemp(void){
	getTemp();
	printf("Il fait actuellement %.1f C |  Humidite : %.1f %%\r\n", Temperature, Humidity);
	if(Temperature > tempRef)
	{
		printf("IL FAIT TARPIN CHAAAAUUUUUUD \n");
		faireTournerVentilo(true);
	}
	else
	{
		printf("tout va bien \n");
		faireTournerVentilo(false);
	}
}

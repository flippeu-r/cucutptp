#include "MeteoLib.h"
#include "stm32l0xx_ll_tim.h"

double getTemp()
{
	DHT22_GetData();
}

void faireTournerVentilo(bool run)
{

}

void setTemp(double temp){
	tempRef = temp;
}

void checkTemp(void){
	getTemp();
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

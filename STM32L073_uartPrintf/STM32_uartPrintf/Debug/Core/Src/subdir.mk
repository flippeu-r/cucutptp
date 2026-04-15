################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Dht22.c \
../Core/Src/MeteoLib.c \
../Core/Src/gpio.c \
../Core/Src/main.c \
../Core/Src/myGpioLib.c \
../Core/Src/retarget.c \
../Core/Src/stm32l0xx_it.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32l0xx.c \
../Core/Src/usart.c 

OBJS += \
./Core/Src/Dht22.o \
./Core/Src/MeteoLib.o \
./Core/Src/gpio.o \
./Core/Src/main.o \
./Core/Src/myGpioLib.o \
./Core/Src/retarget.o \
./Core/Src/stm32l0xx_it.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32l0xx.o \
./Core/Src/usart.o 

C_DEPS += \
./Core/Src/Dht22.d \
./Core/Src/MeteoLib.d \
./Core/Src/gpio.d \
./Core/Src/main.d \
./Core/Src/myGpioLib.d \
./Core/Src/retarget.d \
./Core/Src/stm32l0xx_it.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32l0xx.d \
./Core/Src/usart.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DSTM32L073xx -DUSE_FULL_LL_DRIVER -DHSE_VALUE=8000000 -DHSE_STARTUP_TIMEOUT=100 -DLSE_STARTUP_TIMEOUT=5000 -DLSE_VALUE=32768 -DMSI_VALUE=2097000 -DHSI_VALUE=16000000 -DLSI_VALUE=37000 -DVDD_VALUE=3300 -DPREFETCH_ENABLE=0 -DINSTRUCTION_CACHE_ENABLE=1 -DDATA_CACHE_ENABLE=1 -c -I../Core/Inc -I../Drivers/STM32L0xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32L0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/Dht22.cyclo ./Core/Src/Dht22.d ./Core/Src/Dht22.o ./Core/Src/Dht22.su ./Core/Src/MeteoLib.cyclo ./Core/Src/MeteoLib.d ./Core/Src/MeteoLib.o ./Core/Src/MeteoLib.su ./Core/Src/gpio.cyclo ./Core/Src/gpio.d ./Core/Src/gpio.o ./Core/Src/gpio.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/myGpioLib.cyclo ./Core/Src/myGpioLib.d ./Core/Src/myGpioLib.o ./Core/Src/myGpioLib.su ./Core/Src/retarget.cyclo ./Core/Src/retarget.d ./Core/Src/retarget.o ./Core/Src/retarget.su ./Core/Src/stm32l0xx_it.cyclo ./Core/Src/stm32l0xx_it.d ./Core/Src/stm32l0xx_it.o ./Core/Src/stm32l0xx_it.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32l0xx.cyclo ./Core/Src/system_stm32l0xx.d ./Core/Src/system_stm32l0xx.o ./Core/Src/system_stm32l0xx.su ./Core/Src/usart.cyclo ./Core/Src/usart.d ./Core/Src/usart.o ./Core/Src/usart.su

.PHONY: clean-Core-2f-Src


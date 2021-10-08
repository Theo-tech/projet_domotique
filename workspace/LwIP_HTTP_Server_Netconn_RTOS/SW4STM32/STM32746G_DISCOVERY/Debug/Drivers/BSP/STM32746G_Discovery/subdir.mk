################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/CPE/cpe-s9-environement-communicant-main/cpe-s9-environement-communicant-main/stm32/ethernet/LwIP_HTTP_Server_Netconn_RTOS/Drivers/BSP/STM32746G-Discovery/stm32746g_discovery.c \
D:/CPE/cpe-s9-environement-communicant-main/cpe-s9-environement-communicant-main/stm32/ethernet/LwIP_HTTP_Server_Netconn_RTOS/Drivers/BSP/STM32746G-Discovery/stm32746g_discovery_lcd.c \
D:/CPE/cpe-s9-environement-communicant-main/cpe-s9-environement-communicant-main/stm32/ethernet/LwIP_HTTP_Server_Netconn_RTOS/Drivers/BSP/STM32746G-Discovery/stm32746g_discovery_sdram.c 

OBJS += \
./Drivers/BSP/STM32746G_Discovery/stm32746g_discovery.o \
./Drivers/BSP/STM32746G_Discovery/stm32746g_discovery_lcd.o \
./Drivers/BSP/STM32746G_Discovery/stm32746g_discovery_sdram.o 

C_DEPS += \
./Drivers/BSP/STM32746G_Discovery/stm32746g_discovery.d \
./Drivers/BSP/STM32746G_Discovery/stm32746g_discovery_lcd.d \
./Drivers/BSP/STM32746G_Discovery/stm32746g_discovery_sdram.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/STM32746G_Discovery/stm32746g_discovery.o: D:/CPE/cpe-s9-environement-communicant-main/cpe-s9-environement-communicant-main/stm32/ethernet/LwIP_HTTP_Server_Netconn_RTOS/Drivers/BSP/STM32746G-Discovery/stm32746g_discovery.c Drivers/BSP/STM32746G_Discovery/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F746xx -DUSE_STM32746G_DISCOVERY -c -I../../../Inc -I../../../Src -I../../../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../../../Drivers/STM32F7xx_HAL_Driver/Inc -I../../../Drivers/BSP/STM32746G-Discovery -I../../../Drivers/BSP/Components/Common -I../../../Drivers/BSP/Components -I../../../Middlewares/Third_Party/LwIP/src/include -I../../../Middlewares/Third_Party/LwIP/system -I../../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I../../../Middlewares/Third_Party/FreeRTOS/Source -I../../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../../Utilities -I../../../Utilities/Log -I../../../Drivers/CMSIS/Include -Os -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/BSP/STM32746G_Discovery/stm32746g_discovery_lcd.o: D:/CPE/cpe-s9-environement-communicant-main/cpe-s9-environement-communicant-main/stm32/ethernet/LwIP_HTTP_Server_Netconn_RTOS/Drivers/BSP/STM32746G-Discovery/stm32746g_discovery_lcd.c Drivers/BSP/STM32746G_Discovery/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F746xx -DUSE_STM32746G_DISCOVERY -c -I../../../Inc -I../../../Src -I../../../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../../../Drivers/STM32F7xx_HAL_Driver/Inc -I../../../Drivers/BSP/STM32746G-Discovery -I../../../Drivers/BSP/Components/Common -I../../../Drivers/BSP/Components -I../../../Middlewares/Third_Party/LwIP/src/include -I../../../Middlewares/Third_Party/LwIP/system -I../../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I../../../Middlewares/Third_Party/FreeRTOS/Source -I../../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../../Utilities -I../../../Utilities/Log -I../../../Drivers/CMSIS/Include -Os -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/BSP/STM32746G_Discovery/stm32746g_discovery_sdram.o: D:/CPE/cpe-s9-environement-communicant-main/cpe-s9-environement-communicant-main/stm32/ethernet/LwIP_HTTP_Server_Netconn_RTOS/Drivers/BSP/STM32746G-Discovery/stm32746g_discovery_sdram.c Drivers/BSP/STM32746G_Discovery/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F746xx -DUSE_STM32746G_DISCOVERY -c -I../../../Inc -I../../../Src -I../../../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../../../Drivers/STM32F7xx_HAL_Driver/Inc -I../../../Drivers/BSP/STM32746G-Discovery -I../../../Drivers/BSP/Components/Common -I../../../Drivers/BSP/Components -I../../../Middlewares/Third_Party/LwIP/src/include -I../../../Middlewares/Third_Party/LwIP/system -I../../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I../../../Middlewares/Third_Party/FreeRTOS/Source -I../../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../../Utilities -I../../../Utilities/Log -I../../../Drivers/CMSIS/Include -Os -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"


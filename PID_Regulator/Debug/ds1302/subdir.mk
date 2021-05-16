################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ds1302/ds1302.c 

OBJS += \
./ds1302/ds1302.o 

C_DEPS += \
./ds1302/ds1302.d 


# Each subdirectory must supply rules for building sources it contributes
ds1302/%.o: ../ds1302/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F103xB -I"C:/Users/Cristobal/workspace/PID_Regulator/Inc" -I"C:/Users/Cristobal/workspace/PID_Regulator/lcd" -I"C:/Users/Cristobal/workspace/PID_Regulator/Drivers/STM32F1xx_HAL_Driver/Inc" -I"C:/Users/Cristobal/workspace/PID_Regulator/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Cristobal/workspace/PID_Regulator/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"C:/Users/Cristobal/workspace/PID_Regulator/Drivers/CMSIS/Include" -I"C:/Users/Cristobal/workspace/PID_Regulator/menu" -I"C:/Users/Cristobal/workspace/PID_Regulator/ds1302" -I"C:/Users/Cristobal/workspace/PID_Regulator/pid"  -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



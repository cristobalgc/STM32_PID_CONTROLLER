################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lcd/bigFont_lcdI2c.c \
../lcd/lcd_Hd44780I2C.c 

OBJS += \
./lcd/bigFont_lcdI2c.o \
./lcd/lcd_Hd44780I2C.o 

C_DEPS += \
./lcd/bigFont_lcdI2c.d \
./lcd/lcd_Hd44780I2C.d 


# Each subdirectory must supply rules for building sources it contributes
lcd/%.o: ../lcd/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F103xB -I"C:/Users/Cristobal/workspace/PID_Regulator/Inc" -I"C:/Users/Cristobal/workspace/PID_Regulator/lcd" -I"C:/Users/Cristobal/workspace/PID_Regulator/Drivers/STM32F1xx_HAL_Driver/Inc" -I"C:/Users/Cristobal/workspace/PID_Regulator/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Cristobal/workspace/PID_Regulator/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"C:/Users/Cristobal/workspace/PID_Regulator/Drivers/CMSIS/Include" -I"C:/Users/Cristobal/workspace/PID_Regulator/menu" -I"C:/Users/Cristobal/workspace/PID_Regulator/ds1302" -I"C:/Users/Cristobal/workspace/PID_Regulator/pid"  -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



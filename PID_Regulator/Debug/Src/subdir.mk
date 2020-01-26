################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/bigFont_lcdI2c.c \
../Src/lcd_Hd44780I2C.c \
../Src/main.c \
../Src/stm32f1xx_hal_msp.c \
../Src/stm32f1xx_it.c \
../Src/syscalls.c \
../Src/system_stm32f1xx.c \
../Src/uartPrint.c 

OBJS += \
./Src/bigFont_lcdI2c.o \
./Src/lcd_Hd44780I2C.o \
./Src/main.o \
./Src/stm32f1xx_hal_msp.o \
./Src/stm32f1xx_it.o \
./Src/syscalls.o \
./Src/system_stm32f1xx.o \
./Src/uartPrint.o 

C_DEPS += \
./Src/bigFont_lcdI2c.d \
./Src/lcd_Hd44780I2C.d \
./Src/main.d \
./Src/stm32f1xx_hal_msp.d \
./Src/stm32f1xx_it.d \
./Src/syscalls.d \
./Src/system_stm32f1xx.d \
./Src/uartPrint.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F103xB -I"C:/Users/Cristobal/STM32Cube/PID_Regulator/Inc" -I"C:/Users/Cristobal/STM32Cube/PID_Regulator/Drivers/STM32F1xx_HAL_Driver/Inc" -I"C:/Users/Cristobal/STM32Cube/PID_Regulator/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Cristobal/STM32Cube/PID_Regulator/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"C:/Users/Cristobal/STM32Cube/PID_Regulator/Drivers/CMSIS/Include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



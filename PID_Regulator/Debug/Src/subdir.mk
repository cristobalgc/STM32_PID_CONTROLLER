################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/DS1302.c \
../Src/bigFont_lcdI2c.c \
../Src/delay.c \
../Src/lcd_Hd44780I2C.c \
../Src/main.c \
../Src/pid.c \
../Src/rtcm.c \
../Src/stm32f1xx_hal_msp.c \
../Src/stm32f1xx_it.c \
../Src/syscalls.c \
../Src/system_stm32f1xx.c \
../Src/uartPrint.c 

OBJS += \
./Src/DS1302.o \
./Src/bigFont_lcdI2c.o \
./Src/delay.o \
./Src/lcd_Hd44780I2C.o \
./Src/main.o \
./Src/pid.o \
./Src/rtcm.o \
./Src/stm32f1xx_hal_msp.o \
./Src/stm32f1xx_it.o \
./Src/syscalls.o \
./Src/system_stm32f1xx.o \
./Src/uartPrint.o 

C_DEPS += \
./Src/DS1302.d \
./Src/bigFont_lcdI2c.d \
./Src/delay.d \
./Src/lcd_Hd44780I2C.d \
./Src/main.d \
./Src/pid.d \
./Src/rtcm.d \
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
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F103xB -I"C:/Users/Cristobal/workspace/PID_Regulator/Inc" -I"C:/Users/Cristobal/workspace/PID_Regulator/Drivers/STM32F1xx_HAL_Driver/Inc" -I"C:/Users/Cristobal/workspace/PID_Regulator/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Cristobal/workspace/PID_Regulator/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"C:/Users/Cristobal/workspace/PID_Regulator/Drivers/CMSIS/Include"  -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



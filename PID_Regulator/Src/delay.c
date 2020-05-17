/**********************************************************************
  * Project Name: LCD_I2C_16x2
  * File Name: delay.c
  * Revision:
  * Microcontroller name:STM32F103xxxx ARM
  * Compiler: Keil MDK_ARM 4 - 5
  * Author:   Jesus Pestano
  * E-mail:   jmpestanoh@gmail.com
  * Date :    2018.01.08
  * Select in "Options for Target" the option
  *           (x) Use MicroLib
  * =====================================================================
  * 
  * PROGRAMA DE EJEMPLO DE UITLIZACION DE UN PANTALLA LCD CON EL
  * ADAPTADOR DE I2C
  * 
  *  Programa de ejemplo de la libreria "LiquidCrystal_I2C.c" con
  * el adaptador I2c en una pantalla LCD de 16x2
  *
  **********************************************************************/
// Libreria principal del micro
#include "stm32f1xx_hal.h"

// Librerias de los perifericos
#include "stm32f1xx_hal_gpio.h"
#include "stm32f1xx_hal_rcc.h"
#include "delay.h"

/*  Funcion que crea un delay de milisegundos   */
//-------------------------------------------------
void delay_ms(uint32_t ms)
{
        volatile uint32_t nCount;
        uint32_t HCLK_Frequency;
        HCLK_Frequency = HAL_RCC_GetHCLKFreq();
        nCount=(HCLK_Frequency/10000)*ms;
        for (; nCount!=0; nCount--);
}

/*   Funcion que crea un delay de microsegundos  */
//------------------------------------------------
void delay_Mc(uint32_t mc)
{
        volatile uint32_t nCount;
        uint32_t HCLK_Frequency;
        HCLK_Frequency = HAL_RCC_GetHCLKFreq();

        nCount=(HCLK_Frequency/10000000)*mc;
        for (; nCount!=0; nCount--);
}

///////////////  Fin del Programa   //////////////////

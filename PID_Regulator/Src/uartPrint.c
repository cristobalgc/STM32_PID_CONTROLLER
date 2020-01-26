/*
 * uartPrint.c
 *
 *  Created on: 25 ene. 2020
 *      Author: Cristobal
 */

#include "stm32f1xx_hal.h"
#include "uartPrint.h"
#include<stdio.h>
#include<stdarg.h>
#include<string.h>

static char* lcd_convert(unsigned int, int);       //Convert integer number into octal, hex, etc.

/**
 * @brief  Convert integer numbers in to a string to be represented in the LCD.
 * @note
 * @param[in]  num    	The number to convert
 * @param[in]  base   	The base to convert the number.
 * 					use 10 as base parameter to convert the number in to a decimal value,
 * 					16 to convert it in hexadecimal value and 8 to convert the number in octal.
 * @return  none
 */
static char *lcd_convert(unsigned int num, int base)
{
    static char Representation[]= "0123456789ABCDEF";
    static char buffer[50];
    char *ptr;

    ptr = &buffer[49];
    *ptr = '\0';

    do
    {
        *--ptr = Representation[num%base];
        num /= base;
    }while(num != 0);

    return(ptr);
}

/*
 * @brief Print function to be used like printf.
 *
 * @param[in]: LCD_t *lcd - The lcd object to use.
 * @param[in]: char* format - The data string to be printed on the LCD.
 */
void UART_print(UART_HandleTypeDef *huart , char* format,...)
{
	char *traverse;
	uint16_t strLenght = 0;
	int32_t i;
	char *s;
	//Module 1: Initializing Myprintf's arguments
	va_list arg;
	va_start(arg, format);

	for(traverse = format; *traverse != '\0'; traverse++)
	{

		while( *traverse != '%' && *traverse != '\0')
		{
			HAL_UART_Transmit_IT(huart,(uint8_t*)traverse, 1);
			traverse++;
		}

			traverse++;

			//Module 2: Fetching and executing arguments
			switch(*traverse)
			{
			case 'c' :
				i = va_arg(arg,int);		//Fetch char argument
				HAL_UART_Transmit_IT(huart,(uint8_t*)&i, 1);
				break;

			case 'd' :
				i = va_arg(arg,int);         //Fetch Decimal/Integer argument
				if(i<0)
				{
					i = -i;
					HAL_UART_Transmit_IT(huart,(uint8_t *)'-', 1);
				}
				strLenght=strlen(lcd_convert(i,10));
				HAL_UART_Transmit_IT(huart,(uint8_t *)lcd_convert(i,10), strLenght);
				break;

			case 'o':
				i = va_arg(arg,unsigned int); //Fetch Octal representation
				strLenght=strlen(lcd_convert(i,8));
				HAL_UART_Transmit_IT(huart,(uint8_t *)lcd_convert(i,8),strLenght);
				break;

			case 's':
				s = va_arg(arg,char *);       //Fetch string
				strLenght = strlen(s);
				HAL_UART_Transmit_IT(huart,(uint8_t *)s,strLenght);
				break;

			case 'x':
				i = va_arg(arg,unsigned int); //Fetch Hexadecimal representation
				strLenght = strlen(lcd_convert(i,16));
				HAL_UART_Transmit_IT(huart,(uint8_t *)lcd_convert(i,16),strLenght);
				break;
			default:
				break;
			}
		}

	//Module 3: Closing argument list to necessary clean-up
	va_end(arg);
}

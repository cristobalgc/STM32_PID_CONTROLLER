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

#define UART_DECIMAL (10u)
#define UART_OCTAL (8u)
#define UART_HEX (16u)

static char str_buffer[UART_BUFF_LEN] = {0u};
static char* uart_convert(unsigned int, int);       //Convert integer number into octal, hex, etc.
static uint8_t uart_transmit(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint32_t timeout);

/**
 * @brief  Convert integer numbers in to a string to be represented in the LCD.
 * @note
 * @param[in]  num    	The number to convert
 * @param[in]  base   	The base to convert the number.
 * 					use 10 as base parameter to convert the number in to a decimal value,
 * 					16 to convert it in hexadecimal value and 8 to convert the number in octal.
 * @return  none
 */
static char *uart_convert(unsigned int num, int base)
{
    static char Representation[]= "0123456789ABCDEF";
    char *ptr;
    static char num_buffer[50];
    ptr = &num_buffer[49];
    *ptr = '\0';

    do
    {
        *--ptr = Representation[num%base];
        num /= base;
    }while(num != 0);

    return(ptr);
}

/**
  * @brief  Sends an amount of data in blocking mode or non blocking mode DMA mode.
  * @note   When UART parity is not enabled (PCE = 0), and Word Length is configured to 9 bits (M1-M0 = 01),
  *         the sent data is handled as a set of u16. In this case, Size must indicate the number
  *         of u16 provided through pData.
  * @param  huart Pointer to a UART_HandleTypeDef structure that contains
  *               the configuration information for the specified UART module.
  * @param  pData Pointer to data buffer (u8 or u16 data elements).
  * @param  Size  Amount of data elements (u8 or u16) to be sent
  * @param  Timeout Timeout duration (only when UART_USE_BLOCK_TRANSFER is used)
  * @retval HAL status
  */
static uint8_t uart_transmit(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint32_t timeout)
{
	uint8_t ret = 1u;
#ifdef UART_USE_IT_TRANSFER
	(void)timeout;
	if (HAL_UART_Transmit_IT(huart,pData, size) != HAL_OK)
	{
		ret = 0u;
	}
#endif
#ifdef UART_USE_BLOCK_TRANSFER
	if(HAL_UART_Transmit(huart,pData, size, timeout) != HAL_OK)
	{
		ret = 0u;
	}
#endif
#ifdef UART_USE_DMA_TRANSFER
	(void)timeout;
	if (HAL_UART_Transmit_DMA(huart, pData, Size) != HAL_OK)
	{
		ret = 0u;
	}
#endif

	return ret;
}

/*
 * @brief Print function to be used like printf.
 *
 * @param[in]: LCD_t *lcd - The lcd object to use.
 * @param[in]: char* format - The data string to be printed on the LCD.
 */
 void UART_print(UART_HandleTypeDef *huart, char* format,...)
{
	char *traverse = format;
	uint16_t strLenght = 0u;
	uint16_t strNumLenght = 0u;
	uint16_t messageSize = 0u;
	uint8_t go_out = 0u;
	int32_t i;
	char *s;
	//Module 1: Initializing Myprintf's arguments
	va_list arg;
	memset(str_buffer,'\0',sizeof(str_buffer));
	va_start(arg, format);

	for(; *traverse != '\0'; traverse++)
	{
		while( (*traverse != '%') && (*traverse != '\0'))
		{
			traverse++;
			strLenght++;
		}
		//Only print if any char detected before "%" special character
		if(strLenght > 0u)
		{
			if(strLenght < UART_BUFF_LEN)
			{
				(void*)strncat(str_buffer, format, strLenght);
			}
		}
		traverse++;
		//Module 2: Fetching and executing arguments
		switch(*traverse)
		{
		case 'c' :
			i = va_arg(arg,int);		//Fetch char argument
			if((strLenght + 1u) < UART_BUFF_LEN)
			{
				(void*)strncat(str_buffer, (char*)&i, 1u);
			}
			break;

		case 'd' :
			i = va_arg(arg,int);         //Fetch Decimal/Integer argument
			if(i<0)
			{
				i = -i;
				if((strLenght + 1u) < UART_BUFF_LEN)
				{
					(void*)strncat(str_buffer, (char *)"-", 1u);
				}
			}

			strNumLenght = strlen(uart_convert(i,UART_DECIMAL));
			if((strNumLenght + strLenght) < UART_BUFF_LEN)
			{
				(void*)strncat(str_buffer, (char*)uart_convert(i,UART_DECIMAL), strNumLenght);
			}
			break;

		case 'o':
			i = va_arg(arg,unsigned int); //Fetch Octal representation
			strNumLenght = strlen(uart_convert(i,UART_OCTAL));
			if((strNumLenght + strLenght) < UART_BUFF_LEN)
			{
				(void*)strncat(str_buffer, (char *)uart_convert(i,UART_OCTAL), strNumLenght);
			}
			break;

		case 's':
			s = va_arg(arg,char *);       //Fetch string
			strNumLenght = strlen(s);
			if((strNumLenght + strLenght) < UART_BUFF_LEN)
			{
				(void*)strncat(str_buffer, (char *)s, strNumLenght);
			}
			break;

		case 'x':
			i = va_arg(arg,unsigned int); //Fetch Hexadecimal representation
			strNumLenght = strlen(uart_convert(i,UART_HEX));

			if((strLenght + strNumLenght) < UART_BUFF_LEN)
			{
				(void*)strncat(str_buffer, (char *)uart_convert(i,UART_HEX), strNumLenght);
			}
			break;
		default:
		    go_out = 1u;
			break;
		}
		if(go_out){break;}
	}
	messageSize = strNumLenght + strLenght;
	if((messageSize) < UART_BUFF_LEN && (messageSize) > 0u)
	{
		uart_transmit(huart, (uint8_t *)str_buffer, messageSize, UART_TIMEOUT);
	}

	//Module 3: Closing argument list to necessary clean-up
	va_end(arg);
}

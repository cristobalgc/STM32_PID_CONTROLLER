/*
 * rtcm.c
 *
 *  Created on: 16 may. 2020
 *      Author: Cristobal
 */

#include "stm32f1xx_hal.h"
#include "rtcm.h"
#include <string.h>

static char const *rtc_days[]={RTC_SUNDAY, RTC_MONDAY, RTC_TUESDAY, RTC_WEDNESDAY, RTC_THURSDAY, RTC_FRIDAY, RTC_SATURDAY};
static char const *rtc_months[]={RTC_UNKNOWN, RTC_JAN, RTC_FEB, RTC_MAR, RTC_APR, RTC_MAY, RTC_JUN, RTC_JUL, RTC_AUG, RTC_SEP, RTC_OCT, RTC_NOV, RTC_DIC};

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
static void rtcm_Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

/**
  * @brief  Configure the current time and date.
  * @param  None
  * @retval None
  */
void RTCM_Init(rtc_t *me, RTC_HandleTypeDef *hrtc, rct_cfg_t *cfg)
{
	memcpy(&me->config, cfg, sizeof(rct_cfg_t));
	/*##-1- Configure the Date #################################################*/
	/* Set Date: Tuesday February 18th 2014 */
	if(HAL_RTC_SetDate(hrtc, &me->config.sdatestructure, RTC_FORMAT_BCD) != HAL_OK)
	{
		/* Initialization Error */
		rtcm_Error_Handler();
	}
	/*##-2- Configure the Time #################################################*/
	/* Set Time: 02:00:00 */
	if (HAL_RTC_SetTime(hrtc, &me->config.stimestructure, RTC_FORMAT_BCD) != HAL_OK)
	{
		/* Initialization Error */
		rtcm_Error_Handler();
	}

	/*##-3- Writes a data in a RTC Backup data Register1 #######################*/
	HAL_RTCEx_BKUPWrite(hrtc, RTC_BKP_DR1, 0x32F2);
}

/**
  * @brief  Display the current time and date.
  * @param  showtime : pointer to buffer
  * @param  showdate : pointer to buffer
  * @retval None
  */
void RTCM_GetDateTime(rtc_t *me, RTC_HandleTypeDef *hrtc)
{
  /* Get the RTC current Time */
  HAL_RTC_GetTime(hrtc, &me->data.stimestructureget, RTC_FORMAT_BIN);
  /* Get the RTC current Date */
  HAL_RTC_GetDate(hrtc, &me->data.sdatestructureget, RTC_FORMAT_BIN);
  me->data.date = rtc_days[me->data.sdatestructureget.WeekDay];
  me->data.month = rtc_months[me->data.sdatestructureget.Month];
}


/**
  * @brief  Display the current time and date.
  * @param  showtime : pointer to buffer
  * @param  showdate : pointer to buffer
  * @retval None
  */
void RTCM_SetAlarm(rtc_t *me, RTC_HandleTypeDef *hrtc)
{
	if(HAL_RTC_SetAlarm_IT(hrtc, &me->config.salarmstructure, RTC_FORMAT_BCD) != HAL_OK)
	{
		/* Initialization Error */
		rtcm_Error_Handler();
	}
}

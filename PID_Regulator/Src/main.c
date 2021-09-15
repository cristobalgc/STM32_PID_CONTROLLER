/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include "stm32f1xx_hal.h"
#include <../ds1302/ds1302.h>
#include <../menu/menu.h>
#include "../pid/pid.h"

#include "../lcd/bigFont_lcdI2c.h"
#include "../lcd/lcd_Hd44780I2C.h"

#include "../sdbg/sdbg.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define LCD_ROWS		(4U)
#define LCD_COLS		(20U)
#define LCD_I2C_ADDRESS	(0x4E)

#define SAMPLING_TIME (4u)
#define KI (1)
#define KD (1)
#define KP (7u)
#define AD_RES (5000000/4096)
#define INITDUTYVAL (500u)
#define SET_POINT_INIT_VAL (4217u)

#define HOUR_FORMAT	(0) // 0 = 0-24h; 1= 0-12h
#define HOUR 		(12U)
#define MINUTES 	(16U)
#define SECONDS 	(00U)
#define AM_PM		(1U)
#define WEEKDAY 	(1U)
#define MONTHDAY 	(25U)
#define MONTH 		(7U)
#define YEAR 		(2021U)

#define HOUR_ALARM_FORMAT	(0U) // 0 = 0-24h; 1= 0-12h
#define HOUR_ALARM			(15U)
#define MINUTES_ALARM		(10U)
#define SECONDS_ALARM		(10U)
#define AM_PM_ALARM			(1U)
#define ALARM_STATUS		(0U)// OFF

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* Exported functions ------------------------------------------------------- */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

I2C_HandleTypeDef hi2c1;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;

UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_tx;
DMA_HandleTypeDef hdma_usart1_rx;

/* USER CODE BEGIN PV */

/* Number to print in the LCD */
static uint32_t encoderPos = 0;
/* Lcd object */
static LCD_t Lcd_2;
/* PID object */
static pidc_t Pid;
/* DS1302 object */
static ds1302_T rtc;
/* Menu object */
static menu_T menu;

static uint8_t flag_timer3=0;

/*Pid configuration */
static const pid_cfg_t pidcfg = {
		KP,
		KI,
		KD,
		AD_RES,
		SAMPLING_TIME,
		INITDUTYVAL,
		SET_POINT_INIT_VAL
};

/* Pid configuration for menu */
static const menu_pidCfg_t menuPidCfg = {
		KP, KI, KD, SET_POINT_INIT_VAL
};

/* Clock configuration for menu */
static const menu_dateTimeCfg_t menu_dateTimeCfg ={
		HOUR_FORMAT, HOUR, MINUTES, SECONDS, AM_PM, WEEKDAY, MONTHDAY, MONTH, YEAR
};

/* Clock configuration for menu */
static const menu_alarmCfg_t menu_alarmCfg ={
		HOUR_ALARM_FORMAT, HOUR_ALARM, MINUTES_ALARM, SECONDS_ALARM, AM_PM_ALARM, ALARM_STATUS
};

/*Lcd configuration */
static const LCD_cfg_t Lcd_Hd44780I2cCfg =
{
	&hi2c1,   /* Depending on your active I2C configuration */
	LCD_COLS, /* Lcd columns */
	LCD_ROWS, /* Lcd rows */
	LCD_I2C_ADDRESS, /* Lcd address. Depending on your PCF8574T expander board it could change. */
	LCD_5x8DOTS /* Pending to remove, here you can set 0*/
};

static ds1302_cfg_T ds1302_config = {
	&hspi1,
	{GPIOA, GPIO_PIN_4}
};

static const menu_item_T menu1_cfg_options[6]={
        {0, menu_op1, menu_op1_sel, menu_PidSelected},
		{1, menu_op2, menu_op2_sel, menu_HourSelected},
		{2, menu_op3, menu_op3_sel, menu_enterPidOptions},
		{3, menu_op4, menu_op4_sel, menu_enterClockOptions},//
		{4, menu_op5, menu_op5_sel, menu_ChangePwmSelected},
		{5, menu_Exit, menu_Exit_sel, menu_ExitSelected},
};

/* MENU screen config */
static const menu_cfg_T menu1_cfg = {
		&Lcd_2,
		MENU_MAIN,
		MENU_SCREEN_NO_FREEZE,
		4,//max lines
		0,//line start
		5,//max options
		0,//min menu start option
		&menu1_cfg_options[0]
};

static const menu_item_T menu_defaultHourOptions[1]={
		{0, menu_empty, menu_empty, MENU_defaultHourToMainSel},
};

/* HOUR screen config */
static const menu_cfg_T menu_defaultHour = {
		&Lcd_2,
		MENU_DEFAULT_HOUR,
		MENU_SCREEN_NO_FREEZE,
		4,//max lines
		0,//line start
		0,//max options
		0,//min menu start option
		&menu_defaultHourOptions[0]
};

static const menu_item_T menu_defaultPidOptions[4]={
		{0, menu_defLine1Pid, menu_defLine1Pid, MENU_defaultPidToMainSel},
		{1, menu_defLine2Pid, menu_defLine2Pid, MENU_defaultPidToMainSel},
		{2, menu_defLine3Pid, menu_defLine3Pid, MENU_defaultPidToMainSel},
		{3, menu_defLine4Pid, menu_defLine4Pid, MENU_defaultPidToMainSel},
};

/* Pid screen config for PID*/
static const menu_cfg_T menu_defaultPid = {
		&Lcd_2,
		MENU_DEFAULT_PID,
		MENU_SCREEN_NO_FREEZE,
		4,
		0,
		0,
		0,
		&menu_defaultPidOptions[0]
};

static const menu_item_T menu_ChangePidSettingsOptions[11]={
		{0, submenu1_op1, submenu1_op1, NULL},
		{1, menu_empty, submenu1_op2, menu_ChangeKpEntireSel},
		{2, menu_empty, submenu1_op3, menu_ChangeKpDecimalSel},
		{3, menu_empty, submenu1_op4, menu_ChangeKiEntireSel},
		{4, menu_empty, submenu1_op5, menu_ChangeKiDecimalSel},
		{5, menu_empty, submenu1_op6, menu_ChangeKdEntireSel},
		{6, menu_empty, submenu1_op7, menu_ChangeKdDecimalSel},
		{7, menu_empty, submenu1_op8, menu_ChangeSetPointEntireSel},
		{8, menu_empty, submenu1_op9, menu_ChangeSetPointDecimalSel},
		{9, menu_empty, submenu1_op10, menu_ExitAndSavePid},
		{10, menu_empty, submenu1_op11, menu_exitAndNoSavePid},
};

/* Screen to Change PID settings*/
static const menu_cfg_T menu_ChangePidSettings = {
		&Lcd_2,
		MENU_PID_SETTINGS,
		MENU_SCREEN_NO_FREEZE,
		1, // Used Lines to print the menu
		1, // Print the entire menu rotating on this line
		10,// number of menu items
		1, //min menu start option
		&menu_ChangePidSettingsOptions[0]
};

static const menu_item_T menu_ChangeDateTimeOptions[12]={
		{0, submenu3_op1, submenu3_op1, NULL},
		{1, menu_empty, submenu3_op2, menu_ChangeHourFormatSel},
		{2, menu_empty, submenu3_op6, menu_ChangeAmPmSel},
		{3, menu_empty, submenu3_op3, menu_ChangeHourSel},
		{4, menu_empty, submenu3_op4, menu_ChangeMinSel},
		{5, menu_empty, submenu3_op5, menu_ChangeSecSel},
		{6, menu_empty, submenu3_op7, menu_ChangeWeekDaySel},
		{7, menu_empty, submenu3_op8, menu_ChangeMonthDaySel},
		{8, menu_empty, submenu3_op9, menu_ChangeMonthSel},
		{9, menu_empty, submenu3_op10, menu_ChangeYearSel},
		{10, menu_empty, submenu3_op11, menu_exitAndSaveDateTime},
		{11, menu_empty, submenu3_op12, menu_exitAndNoSaveDateTime},
};

/* Screen to Change Hours*/
static const menu_cfg_T menu_ChangeDateTime = {
		&Lcd_2,
		MENU_CLOCK_SETTINGS,
		MENU_SCREEN_NO_FREEZE,
		1, // Used Lines to print the menu
		1, // Print the entire menu rotating on this line
		11,// number of menu items
		1, //min menu start option
		&menu_ChangeDateTimeOptions[0]
};

static const menu_item_T menu_changeHourFormatOptions[1]={
		{0, menu_empty, menu_empty, menu_exitChangeClockSelected},
};

/* Screen to Change Hour format */
static const menu_cfg_T menu_changeHourFormat = {
		&Lcd_2,
		MENU_CHANGE_HOUR_FORMAT,
		MENU_SCREEN_FREEZE,
		1, // Used Lines to print the menu
		1, // Print the entire menu rotating on this line
		1,// number of menu items
		0, //min menu start option
		&menu_changeHourFormatOptions[0]
};

static const menu_item_T menu_changeHourAmPmOptions[1]={
		{0, menu_empty, menu_empty, menu_exitChangeClockSelected},
};

/* Screen to Change Hour mode */
static const  menu_cfg_T menu_changeHourAmPm = {
		&Lcd_2,
		MENU_CHANGE_HOUR_AM_PM,
		MENU_SCREEN_FREEZE,
		1, // Used Lines to print the menu
		1, // Print the entire menu rotating on this line
		1,// number of menu items
		0, //min menu start option
		&menu_changeHourAmPmOptions[0]
};

static const menu_item_T menu_changeHoursOptions[1]={
		{0, menu_empty, menu_empty, menu_exitChangeClockSelected},
};

/* Screen to Change Hours */
static const menu_cfg_T menu_changeHours = {
		&Lcd_2,
		MENU_CHANGE_HOUR,
		MENU_SCREEN_FREEZE,
		1, // Used Lines to print the menu
		1, // Print the entire menu rotating on this line
		23,// number of menu items
		0, //min menu start option
		&menu_changeHoursOptions[0]
};

static const menu_item_T menu_changeMinutesOptions[1]={
		{0, menu_empty, menu_empty, menu_exitChangeClockSelected},
};

/* Screen to Change Minutes */
static const menu_cfg_T menu_changeMinutes = {
		&Lcd_2,
		MENU_CHANGE_MINUTES,
		MENU_SCREEN_FREEZE,
		1, // Used Lines to print the menu
		1, // Print the entire menu rotating on this line
		59,// number of menu items
		0, //min menu start option
		&menu_changeMinutesOptions[0]
};


static const menu_item_T menu_changeSecondsOptions[1]={
		{0, menu_empty, menu_empty, menu_exitChangeClockSelected},
};


/* Screen to Change Seconds */
static const menu_cfg_T menu_changeSeconds = {
		&Lcd_2,
		MENU_CHANGE_SECONDS,
		MENU_SCREEN_FREEZE,
		1, // Used Lines to print the menu
		1, // Print the entire menu rotating on this line
		59,// number of menu items
		0, //min menu start option
		&menu_changeSecondsOptions[0]
};

static const menu_item_T menu_changeWeekDayOptions[1]={
		{0, menu_empty, menu_empty, menu_exitChangeClockSelected},
};

/* Screen to Change the day of the week */
static const menu_cfg_T menu_changeWeekDay = {
		&Lcd_2,
		MENU_CHANGE_WEEKDAY,
		MENU_SCREEN_FREEZE,
		1, // Used Lines to print the menu
		1, // Print the entire menu rotating on this line
		7, // number of menu items
		1, //min menu start option
		&menu_changeWeekDayOptions[0]
};

static const menu_item_T menu_changeMonthDayOptions[1]= {
		{0, menu_empty, menu_empty, menu_exitChangeClockSelected},
};


/* Screen to Change the day of the current month*/
static const menu_cfg_T menu_changeMonthDay = {
		&Lcd_2,
		MENU_CHANGE_MONTHDAY,
		MENU_SCREEN_FREEZE,
		1, // Used Lines to print the menu
		1, // Print the entire menu rotating on this line
		31,// number of menu items
		1, //min menu start option
		&menu_changeMonthDayOptions[0]
};

static const menu_item_T menu_changeMonthOptions[1]={
		{0, menu_empty, menu_empty, menu_exitChangeClockSelected},
};

/* Screen to Change the day of the current month*/
static const menu_cfg_T menu_changeMonth = {
		&Lcd_2,
		MENU_CHANGE_MONTH,
		MENU_SCREEN_FREEZE,
		1, // Used Lines to print the menu
		1, // Print the entire menu rotating on this line
		12,// number of menu items
		1, //min menu start option
		&menu_changeMonthOptions[0]
};

static const menu_item_T menu_changeYearOptions[1]={
	{0, menu_empty, menu_empty, menu_exitChangeClockSelected},
};

/* Screen to Change the day of the current month*/
static const menu_cfg_T menu_changeYear = {
		&Lcd_2,
		MENU_CHANGE_YEAR,
		MENU_SCREEN_FREEZE,
		1, // Used Lines to print the menu
		1, // Print the entire menu rotating on this line
		100,// number of menu items
		21, //min menu start option
		&menu_changeYearOptions[0]
};

static const menu_item_T menu_changeKpEntireOptions[1]={
		{0, menu_empty, menu_empty, menu_exitChangePidSelected},
};

/* Screen to Change the Entire part of the KP parameter*/
static const menu_cfg_T menu_changeKpEntire = {
		&Lcd_2,
		MENU_CHANGE_KP_ENTIRE,
		MENU_SCREEN_FREEZE,
		1, // Used Lines to print the menu
		1, // Print the entire menu rotating on this line
		100,// number of menu items
		0, //min menu start option
		&menu_changeKpEntireOptions[0]
};

static const menu_item_T menu_changeKpDecimalOptions[1]={
		{0, menu_empty, menu_empty, menu_exitChangePidSelected},
};

/* Screen to Change the decimal part of the KP parameter*/
static const menu_cfg_T menu_changeKpDecimal = {
		&Lcd_2,
		MENU_CHANGE_KP_DECIMAL,
		MENU_SCREEN_FREEZE,
		1, // Used Lines to print the menu
		1, // Print the entire menu rotating on this line
		100,// number of menu items
		0, //min menu start option
		&menu_changeKpDecimalOptions[0]
};

static const menu_item_T menu_changeKiEntireOptions[1]={
		{0, menu_empty, menu_empty, menu_exitChangePidSelected},
};

/* Screen to Change the Entire part of the KP parameter*/
static const menu_cfg_T menu_changeKiEntire = {
		&Lcd_2,
		MENU_CHANGE_KI_ENTIRE,
		MENU_SCREEN_FREEZE,
		1, // Used Lines to print the menu
		1, // Print the entire menu rotating on this line
		100,// number of menu items
		0, //min menu start option
		&menu_changeKiEntireOptions[0]
};

static const menu_item_T menu_changeKiDecimalOptions[1]={
		{0, menu_empty, menu_empty, menu_exitChangePidSelected},
};

/* Screen to Change the decimal part of the KP parameter*/
static menu_cfg_T menu_changeKiDecimal = {
		&Lcd_2,
		MENU_CHANGE_KI_DECIMAL,
		MENU_SCREEN_FREEZE,
		1, // Used Lines to print the menu
		1, // Print the entire menu rotating on this line
		100,// number of menu items
		0, //min menu start option
		&menu_changeKiDecimalOptions[0]
};

static const menu_item_T menu_changeKdEntireOptions[1]=		{
		{0, menu_empty, menu_empty, menu_exitChangePidSelected},
};

/* Screen to Change the Entire part of the KP parameter*/
static const menu_cfg_T menu_changeKdEntire = {
		&Lcd_2,
		MENU_CHANGE_KD_ENTIRE,
		MENU_SCREEN_FREEZE,
		1, // Used Lines to print the menu
		1, // Print the entire menu rotating on this line
		100,// number of menu items
		0, //min menu start option
		&menu_changeKdEntireOptions[0]
};

static const menu_item_T menu_changeKdDecimalOptions[1]=		{
		{0, menu_empty, menu_empty, menu_exitChangePidSelected},
};

/* Screen to Change the decimal part of the KP parameter*/
static const menu_cfg_T menu_changeKdDecimal = {
		&Lcd_2,
		MENU_CHANGE_KD_DECIMAL,
		MENU_SCREEN_FREEZE,
		1, // Used Lines to print the menu
		1, // Print the entire menu rotating on this line
		100,// number of menu items
		0, //min menu start option
		&menu_changeKdDecimalOptions[0]
};

static const menu_item_T menu_changeSetPointEntireOptions[1]={
		{0, menu_empty, menu_empty, menu_exitChangePidSelected},
};
/* Screen to Change the Entire part of the KP parameter*/
static const menu_cfg_T menu_changeSetPointEntire = {
		&Lcd_2,
		MENU_CHANGE_SET_POINT_ENTIRE,
		MENU_SCREEN_FREEZE,
		1, // Used Lines to print the menu
		1, // Print the entire menu rotating on this line
		100,// number of menu items
		0, //min menu start option
		&menu_changeSetPointEntireOptions[0]
};

static const menu_item_T menu_changeSetPointDecimalOptions[1]={
		{0, menu_empty, menu_empty, menu_exitChangePidSelected},
};

/* Screen to Change the decimal part of the KP parameter*/
static const menu_cfg_T menu_changeSetPointDecimal = {
		&Lcd_2,
		MENU_CHANGE_SET_POINT_DECIMAL,
		MENU_SCREEN_FREEZE,
		1, // Used Lines to print the menu
		1, // Print the entire menu rotating on this line
		100,// number of menu items
		0, //min menu start option
		&menu_changeSetPointDecimalOptions[0]
};

static const menu_item_T menu_PidOptionsOptions[7] ={
		{0, submenu4_title, submenu4_title, NULL},
		{1, submenu4_op1, submenu4_op1_sel , menu_enterChangePidSelected},
		{2, submenu4_op2, submenu4_op2_sel, MENU_showPidValues},
		{3, menu_Exit, menu_Exit_sel, MENU_pidOptionsExitToMain},
};

/* Screen to show all PID options */
static menu_cfg_T menu_PidOptions = {
		&Lcd_2,
		MENU_PID_OPTIONS,
		MENU_SCREEN_NO_FREEZE,
		3, // Used Lines to print the menu
		1, // start printing the menu on this line
		3,// number of menu items
		1, //min menu start option
		&menu_PidOptionsOptions[0]
};

static const menu_item_T menu_clockOptionsOptions[7]=		{
		{0, submenu5_title, submenu5_title, NULL},
		{1, submenu5_op1, submenu5_op1_sel , menu_enterChangeClockSelected},
		{2, submenu5_op2, submenu5_op2_sel, menu_enterChangeAlarmSelected},
		{3, menu_Exit, menu_Exit_sel, MENU_pidOptionsExitToMain},
};

/* Screen to show all PID options */
static menu_cfg_T menu_clockOptions = {
		&Lcd_2,
		MENU_CLOCK_OPTIONS,
		MENU_SCREEN_NO_FREEZE,
		3, // Used Lines to print the menu
		1, // Print the entire menu rotating on this line
		3,// number of menu items
		1, //min menu start option
		&menu_clockOptionsOptions[0]
};

static const menu_item_T menu_PidValuesOptions[4]={
		{0, submenu3_Kp, submenu3_Kp, menu_exitAndNoSavePid},
		{1, submenu3_Ki, submenu3_Ki, menu_exitAndNoSavePid},
		{2, submenu3_Kd, submenu3_Kd, menu_exitAndNoSavePid},
		{3, submenu3_Sp, submenu3_Sp, menu_exitAndNoSavePid},
};

/* Screen to show all PID options */
static const menu_cfg_T menu_PidValues = {
		&Lcd_2,
		MENU_PID_VALUES,
		MENU_SCREEN_NO_FREEZE,
		4, // Used Lines to print the menu
		0, // Print the entire menu rotating on this line
		3, // number of menu items
		0, //min menu start option
		&menu_PidValuesOptions[0]
};

static const menu_item_T menu_ChangeAlarmOptions[9]={
		{0, submenu6_op1, submenu6_op1, NULL},
		{1, menu_empty, submenu6_op2, menu_ChangeAlarmHourFormatSel},
		{2, menu_empty, submenu6_op3, menu_ChangeAlarmAmPmSel},
		{3, menu_empty, submenu6_op4, menu_ChangeAlarmHourSel},
		{4, menu_empty, submenu6_op5, menu_ChangeAlarmMinSel},
		{5, menu_empty, submenu6_op6, menu_ChangeAlarmSecSel},
		{6, menu_empty, submenu6_op7, menu_ChangeAlarmStatusSel},
		{7, menu_empty, submenu6_op8, menu_exitAndSaveAlarm},
		{8, menu_empty, submenu6_op9, menu_exitAndNoSaveAlarm}
};

/* Screen to Change Hours*/
static const menu_cfg_T menu_ChangeAlarm = {
		&Lcd_2,
		MENU_ALARM_SETTINGS,
		MENU_SCREEN_NO_FREEZE,
		1, // Used Lines to print the menu
		1, // Print the entire menu rotating on this line
		8,// number of menu items
		1, //min menu start option
		&menu_ChangeAlarmOptions[0]
};

static const menu_item_T menu_changeAlarmHourAmPmOptions[1]={
		{0, menu_empty, menu_empty, menu_exitChangeAlarmSelected},
};

/* Screen to Change Alarm Hour mode */
static const  menu_cfg_T menu_changeAlarmHourAmPm = {
		&Lcd_2,
		MENU_CHANGE_ALARM_HOUR_AM_PM,
		MENU_SCREEN_FREEZE,
		1, // Used Lines to print the menu
		1, // Print the entire menu rotating on this line
		1,// number of menu items
		0, //min menu start option
		&menu_changeAlarmHourAmPmOptions[0]
};

static const menu_item_T menu_changeAlarmHourFormatOptions[1]={
		{0, menu_empty, menu_empty, menu_exitChangeAlarmSelected},
};

/* Screen to Change Hour format */
static const menu_cfg_T menu_changeAlarmHourFormat = {
		&Lcd_2,
		MENU_CHANGE_ALARM_HOUR_FORMAT,
		MENU_SCREEN_FREEZE,
		1, // Used Lines to print the menu
		1, // Print the entire menu rotating on this line
		1,// number of menu items
		0, //min menu start option
		&menu_changeAlarmHourFormatOptions[0]
};

static const menu_item_T menu_changeAlarmHoursOptions[1]={
		{0, menu_empty, menu_empty, menu_exitChangeAlarmSelected},
};

/* Screen to Change Hours */
static const menu_cfg_T menu_changeAlarmHours = {
		&Lcd_2,
		MENU_CHANGE_ALARM_HOUR,
		MENU_SCREEN_FREEZE,
		1, // Used Lines to print the menu
		1, // Print the entire menu rotating on this line
		23,// number of menu items
		0, //min menu start option
		&menu_changeAlarmHoursOptions[0]
};

static const menu_item_T menu_changeAlarmMinutesOptions[1]={
		{0, menu_empty, menu_empty, menu_exitChangeAlarmSelected},
};

/* Screen to Change Hours */
static const menu_cfg_T menu_changeAlarmMinutes = {
		&Lcd_2,
		MENU_CHANGE_ALARM_MINUTES,
		MENU_SCREEN_FREEZE,
		1, // Used Lines to print the menu
		1, // Print the entire menu rotating on this line
		59,// number of menu items
		0, //min menu start option
		&menu_changeAlarmMinutesOptions[0]
};

static const menu_item_T menu_changeAlarmSecondsOptions[1]={
		{0, menu_empty, menu_empty, menu_exitChangeAlarmSelected},
};

/* Screen to Change Hours */
static const menu_cfg_T menu_changeAlarmSeconds = {
		&Lcd_2,
		MENU_CHANGE_ALARM_SECONDS,
		MENU_SCREEN_FREEZE,
		1, // Used Lines to print the menu
		1, // Print the entire menu rotating on this line
		59,// number of menu items
		0, //min menu start option
		&menu_changeAlarmSecondsOptions[0]
};

static const menu_item_T menu_changeAlarmStatusOptions[1]={
		{0, menu_empty, menu_empty, menu_exitChangeAlarmSelected},
};

/* Screen to Change Hours */
static const menu_cfg_T menu_changeAlarmStatus = {
		&Lcd_2,
		MENU_CHANGE_ALARM_STATUS,
		MENU_SCREEN_FREEZE,
		1, // Used Lines to print the menu
		1, // Print the entire menu rotating on this line
		1,// number of menu items
		0, //min menu start option
		&menu_changeAlarmStatusOptions[0]
};

static const menu_cfg_T *menu_configs[33]={
		&menu_defaultPid,			//0
		&menu_defaultPid,			//1
		&menu_defaultHour,			//2
		&menu1_cfg,					//3
		&menu_PidOptions,			//4
		&menu_ChangeDateTime,		//5
		&menu_changeHourFormat,		//6
		&menu_changeHourAmPm,		//7
		&menu_changeHours,			//8
		&menu_changeMinutes,		//9
		&menu_changeSeconds,		//10
		&menu_changeWeekDay,		//11
		&menu_changeMonthDay,		//12
		&menu_changeMonth,			//13
		&menu_changeYear,			//14
		&menu_ChangePidSettings,	//15
		&menu_changeKpEntire,		//16
		&menu_changeKpDecimal,		//17
		&menu_changeKiEntire,		//18
		&menu_changeKiDecimal,		//19
		&menu_changeKdEntire,		//20
		&menu_changeKdDecimal,		//21
		&menu_changeSetPointEntire,	//22
		&menu_changeSetPointDecimal,//23
		&menu_PidValues,			//24
		&menu_clockOptions,			//25
		&menu_ChangeAlarm,			//26
		&menu_changeAlarmHourFormat,//27
		&menu_changeAlarmHourAmPm,  //28
		&menu_changeAlarmHours,		//29
		&menu_changeAlarmMinutes,  //30
		&menu_changeAlarmSeconds,//31
		&menu_changeAlarmStatus,//32
};

static uint32_t i2c_TxInterrupts = 0;
static uint32_t i2c_RxInterrupts = 0;
static uint32_t Uart_TxInterrupts = 0;
uint8_t Rx_indx,Transfer_cplt;
char Rx_data[2],Rx_Buffer[100];


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM2_Init(void);
static void MX_DMA_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_TIM4_Init(void);
static void MX_SPI1_Init(void);
static void MX_ADC1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	menu_status_t menuStatus = MENU_DEFAULT_CFG;
	//check if the interrupt comes from TIM3
	if (htim->Instance == TIM3){
		menuStatus = MENU_GetStatus(&menu);
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
		SDBG_print(&huart1,"\r\nNumero: %d", encoderPos);
		menu.data.adc_val = HAL_ADC_GetValue(&hadc1);
		if ((menuStatus == MENU_DEFAULT_HOUR) || (menuStatus == MENU_DEFAULT_PID)){
			MENU_Task(&menu);
		}
		flag_timer3 = 1u;

	}
	//check if the interrupt comes from TIM4
	if(htim->Instance == TIM4){
		HAL_TIM_Base_Stop_IT(&htim4);
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_4);
		HAL_NVIC_EnableIRQ(EXTI4_IRQn);
	}
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	menu_status_t menuStatus = MENU_DEFAULT_CFG;

	if (htim->Instance==TIM2) //check if the interrupt comes from TIM2
	{
		if(HAL_TIM_Encoder_GetState(&htim2) == HAL_TIM_STATE_READY)
		{
			menuStatus = MENU_GetStatus(&menu);
			SDBG_print(&huart1,"\r\nEncoder: %d", TIM2->CNT);
			MENU_encoderOption (&menu, (int32_t)TIM2->CNT);

			if (( menuStatus!= MENU_DEFAULT_HOUR) && (menuStatus != MENU_DEFAULT_PID)){
				MENU_Task(&menu);
			}
//			encoderPos++;
//			LCD_setCursor(&Lcd_2,17,0);
//			LCD_print(&Lcd_2,"%d",encoderPos);
//			BIGFONT_printNumber(&Lcd_2,TIM2->CNT, 0 ,0);
		}
	}
}

void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	if(hi2c->Instance == I2C1)
	{
		i2c_TxInterrupts++;
	}
}

void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	if(hi2c->Instance == I2C1)
	{
		i2c_RxInterrupts++;
	}
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1)
	{
		Uart_TxInterrupts++;
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	uint8_t i;
	if(huart->Instance == USART1)//Current UART
	{
		if(Rx_indx == 0)
		{
			for(i=0; i<100; i++)
			{
				Rx_Buffer[i]=0; //Clear rx_Buffer prior to use
			}
			//reset the user led if previously set
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
		}
		if(Rx_data[0]!=13)
		{
			Rx_Buffer[Rx_indx++] = Rx_data[0]; //add data to rxbuffer
		}
		else // if received data =13 (carriage return)
		{
			Rx_indx =0;
			Transfer_cplt =1; // transfer complete, data is ready
			HAL_UART_Transmit_DMA(&huart1,(uint8_t*)Rx_Buffer,(uint16_t)strlen(Rx_Buffer));
			HAL_UART_Transmit_DMA(&huart1,(uint8_t*)"\n\r",2u);
			if(!strcmp(Rx_Buffer,"On"))// LED trigger phase
			{
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
			}
		}
		HAL_UART_Receive_DMA(&huart1,(uint8_t*)Rx_data,1u);// activate UART
		HAL_UART_Transmit_DMA(&huart1,(uint8_t*)Rx_data,(uint16_t)strlen(Rx_data));
	}
}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	static uint32_t counter = 0u;

	if (GPIO_Pin == GPIO_PIN_4){
		counter++;
		HAL_NVIC_DisableIRQ(EXTI4_IRQn);
		HAL_TIM_Base_Start_IT(&htim4);
		if((HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4) == GPIO_PIN_RESET) &&
				(counter == 1u)){
			/* Falling edge */
			MENU_keyPressed(&menu);
			MENU_Task(&menu);
		}else{
			/* Rising edge */
			counter = 0u;
		}
		//LCD_setCursor(&Lcd_2,17,0);
		//LCD_print(&Lcd_2,"%d",counter);
	}
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_TIM3_Init();
  MX_TIM2_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_TIM4_Init();
  MX_SPI1_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */
	(void)HAL_UART_Receive_DMA(&huart1, (uint8_t*)Rx_data, 1u);
	LCD_init(&Lcd_2, &Lcd_Hd44780I2cCfg); /* Initialize the LCD to print "normal characters"*/
	BIGFONT_init(&Lcd_2); /*Initialize Big font characters */
	LCD_setBacklight(&Lcd_2, 1u); /* Turn on the Backlight */
	DS1302_Init(&rtc, &ds1302_config);
	DS1302_setTime(&rtc, HOUR_FORMAT, HOUR, MINUTES, SECONDS, AM_PM, WEEKDAY, MONTHDAY, MONTH, YEAR);
	SDBG_print(&huart1,"\r\nHello World!");
	MENU_Init(&menu, &menu_configs, &menu_dateTimeCfg, &menu_alarmCfg, &menuPidCfg);
	PID_init(&Pid,&pidcfg);
	HAL_TIM_Base_Start_IT(&htim3);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1)
	{
		if(flag_timer3){
			DS1302_updateDateTime(&rtc);

			menu_SetSecondUnits(&menu, DS1302_getSecondsUnits(&rtc));
			menu_SetSecondDec(&menu, DS1302_getSecondsDec(&rtc));
			menu_SetMinutesUnits(&menu, DS1302_getMinutesUnits(&rtc));
			menu_SetMinutesDec(&menu, DS1302_getMinutesDec(&rtc));
			menu_SetHourUnits(&menu, DS1302_getHourUnits(&rtc));
			menu_SetHourDec(&menu, DS1302_getHourDec(&rtc));
			menu_SetAmPm(&menu, DS1302_getAmPmStatus(&rtc));
			menu_SetWeekday(&menu, DS1302_geWeekDay(&rtc));
			menu_SetMonth(&menu, DS1302_getMonth(&rtc));
			menu_SetMonthDay(&menu,DS1302_getMonthDay(&rtc));
			menu_SetYear(&menu, DS1302_getYear(&rtc));
			flag_timer3 = 0;
		}

		if(menu_GetChangesDataTime(&menu)){
			DS1302_setTime(&rtc, menu.data.dateTimeCfg.hourFormat, menu.data.dateTimeCfg.hour, menu.data.dateTimeCfg.minutes, menu.data.dateTimeCfg.seconds,
					menu.data.dateTimeCfg.amPm, menu.data.dateTimeCfg.dayOfWeek, menu.data.dateTimeCfg.dayOfMonth, menu.data.dateTimeCfg.month, menu.data.dateTimeCfg.year);
		}

		if(menu_GetChangesPid(&menu)){
			Pid.config.kp = menu.data.kpEntire;
			Pid.config.Ki = menu.data.kiEntire;
			Pid.config.kd = menu.data.kdEntire;
			Pid.data.setPoint = menu.data.SetPointEntire;
		}

		if(menu_checkAlarm(&menu)){

		}

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */
  /** Common config 
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel 
  */
  sConfig.Channel = ADC_CHANNEL_8;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */
	HAL_ADC_Start(&hadc1);
  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_1LINE;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_LSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_Encoder_InitTypeDef sConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_CENTERALIGNED1;
  htim2.Init.Period = 65535;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  sConfig.EncoderMode = TIM_ENCODERMODE_TI1;
  sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 15;
  sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 10;
  if (HAL_TIM_Encoder_Init(&htim2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */
  HAL_TIM_Encoder_Start_IT(&htim2, TIM_CHANNEL_1);
  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 7200;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 4999;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 3599;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 999;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */
  HAL_TIM_MspPostInit(&htim4);

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/** 
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void) 
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel4_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);
  /* DMA1_Channel5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 4, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PA4 */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PB4 */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI4_IRQn, 12, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

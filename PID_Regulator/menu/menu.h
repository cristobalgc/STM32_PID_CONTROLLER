/******************************************************************************/
/*   All rights reserved. Distribution or duplication without previous        */
/*   written agreement of the owner prohibited.                               */
/*                                                                            */
/******************************************************************************/

/** \file menu.h
 *
 * \brief Header file for menu module
 *
 * Header file for menu
 *
 * <table border="0" cellspacing="0" cellpadding="0">
 * <tr> <td> Author:   </td> <td> C Garcia Camoira  </td></tr>
 * <tr> <td> Date:     </td> <td> 24/01/2020             </td></tr>
 * </table>
 * \n
 * <table border="0" cellspacing="0" cellpadding="0">
 * <tr> <td> COMPONENT: </td> <td> menu   </td></tr>
 * <tr> <td> SCOPE:     </td> <td> Public      </td></tr>
 * <tr> <td> TARGET:    </td> <td> stm32f103c8t6         </td></tr>
 * </table>
 * \note
 *
 * \see
 */

#ifndef MENU_H_
#define MENU_H_

/******************************************************************************/
/*                         Project Includes                                   */
/******************************************************************************/
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "../lcd/bigFont_lcdI2c.h"
#include "../lcd/lcd_Hd44780I2C.h"

/******************************************************************************/
/*                 Definition of exported symbolic constants                  */
/******************************************************************************/
#define MENU_ESP_LANG

#define MENU_MAX_ROWS 		(4U)
#define MENU_MAX_COLS 		(21U)

#define MENU_TRUE 			(1U)
#define MENU_FALSE 			(0U)

#ifdef MENU_ENG_LANG
/* Days of a Week */
#define MENU_MONDAY "Mon"
#define MENU_TUESDAY "Tue"
#define MENU_WEDNESDAY "Wed"
#define MENU_THURSDAY "Thu"
#define MENU_FRIDAY "Fri"
#define MENU_SATURDAY "Sat"
#define MENU_SUNDAY "Sun"

/* Months of a year */
#define MENU_JAN	"Jan"
#define MENU_FEB "Feb"
#define MENU_MAR "Mar"
#define MENU_APR "Apr"
#define MENU_MAY "May"
#define MENU_JUN "Jun"
#define MENU_JUL "Jul"
#define MENU_AUG "Aug"
#define MENU_SEP "Sep"
#define MENU_OCT "Oct"
#define MENU_NOV "Nov"
#define MENU_DIC "Dec"
#endif

#ifdef MENU_ESP_LANG
/* Dias de la semana */
#define MENU_MONDAY "Lun"
#define MENU_TUESDAY "Mar"
#define MENU_WEDNESDAY "Mie"
#define MENU_THURSDAY "Jue"
#define MENU_FRIDAY "Vie"
#define MENU_SATURDAY "Sab"
#define MENU_SUNDAY "Dom"

/* Months of year */
#define MENU_JAN "Ene"
#define MENU_FEB "Feb"
#define MENU_MAR "Mar"
#define MENU_APR "Abr"
#define MENU_MAY "May"
#define MENU_JUN "Jun"
#define MENU_JUL "Jul"
#define MENU_AUG "Ago"
#define MENU_SEP "Sep"
#define MENU_OCT "Oct"
#define MENU_NOV "Nov"
#define MENU_DIC "Dic"
#endif


#define menu_op1  		"PID as default      "
#define menu_op1_sel 	"PID as default     <"
#define menu_op2  		"Clock as default    "
#define menu_op2_sel  	"Clock as default   <"
#define menu_op3 		"PID Settings        "
#define menu_op3_sel  	"PID Settings       <"
#define menu_op4  		"Clock Settings      "
#define menu_op4_sel 	"Clock Settings     <"
#define menu_op5  		"PWM settings        "
#define menu_op5_sel 	"PWM settings       <"
#define menu_Exit 		"Exit                "
#define menu_Exit_sel 	"Exit               <"
#define menu_empty		"                    "

#define submenu4_title		"--- PID OPTIONS ----"
#define submenu4_op1_sel	"Configure values   <"
#define submenu4_op1		"Configure values    "
#define submenu4_op2_sel	"Show PID values    <"
#define submenu4_op2		"Show PID values     "

#define submenu5_title		"-- CLOCK OPTIONS ---"
#define submenu5_op1_sel	"Change date & time <"
#define submenu5_op1		"Change date & time  "
#define submenu5_op2_sel	"Set alarm          <"
#define submenu5_op2		"Set alarm           "

#define submenu1_op1	"- CONFIG PID VALUES-"
#define submenu1_op2	"Change kp entire   <"
#define submenu1_op3	"Change kp decimal  <"
#define submenu1_op4	"Change ki entire   <"
#define submenu1_op5	"Change ki decimal  <"
#define submenu1_op6	"Change kd entire   <"
#define submenu1_op7	"Change kd decimal  <"
#define submenu1_op8	"Change setp entire <"
#define submenu1_op9	"Change setp dec    <"
#define submenu1_op10	"Exit & save        <"
#define submenu1_op11	"Exit & no save     <"

#define submenu3_op1	"-CHANGE DATE & TIME-"
#define submenu3_op2	"Change Hour Format <"
#define submenu3_op3	"Change Hour        <"
#define submenu3_op4	"Change Minutes     <"
#define submenu3_op5	"Change Seconds     <"
#define submenu3_op6	"Change AM/PM       <"
#define submenu3_op7	"Change Week Day    <"
#define submenu3_op8	"Change Month Day   <"
#define submenu3_op9	"Change Month       <"
#define submenu3_op10	"Change Year        <"
#define submenu3_op11	"Exit & save        <"
#define submenu3_op12	"Exit & no save     <"

#define submenu6_op1	"-CHANGE ALARM TIME -"
#define submenu6_op2	"Change Hour Format <"
#define submenu6_op3	"Change AM/PM       <"
#define submenu6_op4	"Change Hour        <"
#define submenu6_op5	"Change Minutes     <"
#define submenu6_op6	"Change Seconds     <"
#define submenu6_op7	"Activate alarm     <"
#define submenu6_op8	"Exit & save        <"
#define submenu6_op9	"Exit & no save     <"

#define submenu3_Kp	"KP:"
#define submenu3_Ki	"KI:"
#define submenu3_Kd	"KD:"
#define submenu3_Sp "SP:"

#define submenu2_op1 "- CHANGE PWM VALUES-"
#define submenu2_op2
#define submenu2_op3
#define submenu2_op4

#define menu_defLine1Pid " - PID REGULATOR - "
#define menu_defLine2Pid "Set Point:"
#define menu_defLine3Pid "Real val:"
#define menu_defLine4Pid "Duty:"

/******************************************************************************/
/*                Definition of exported function like macros                 */
/******************************************************************************/

/******************************************************************************/
/*         Definition of exported types (typedef, enum, struct, union)        */
/******************************************************************************/
typedef uint8_t (*menuSelected)( void* userData1, void* userData2);

typedef enum menu_status_e{
	MENU_DEFAULT_CFG,
	MENU_DEFAULT_PID,
	MENU_DEFAULT_HOUR,
	MENU_MAIN,
	MENU_PID_OPTIONS,
	MENU_CLOCK_OPTIONS,
	MENU_PWM_SETTINGS,
	MENU_EXIT,
	MENU_CLOCK_SETTINGS,
	MENU_CHANGE_HOUR_FORMAT,
	MENU_CHANGE_HOUR_AM_PM,
	MENU_CHANGE_HOUR,
	MENU_CHANGE_MINUTES,
	MENU_CHANGE_SECONDS,
	MENU_CHANGE_WEEKDAY,
	MENU_CHANGE_MONTHDAY,
	MENU_CHANGE_MONTH,
	MENU_CHANGE_YEAR,
	MENU_PID_SETTINGS,
	MENU_CHANGE_KP_ENTIRE,
	MENU_CHANGE_KP_DECIMAL,
	MENU_CHANGE_KI_ENTIRE,
	MENU_CHANGE_KI_DECIMAL,
	MENU_CHANGE_KD_ENTIRE,
	MENU_CHANGE_KD_DECIMAL,
	MENU_CHANGE_SET_POINT_ENTIRE,
	MENU_CHANGE_SET_POINT_DECIMAL,
	MENU_PID_VALUES,
	MENU_ALARM_SETTINGS,
	MENU_CHANGE_ALARM_HOUR_FORMAT,
	MENU_CHANGE_ALARM_HOUR_AM_PM,
	MENU_CHANGE_ALARM_HOUR,
	MENU_CHANGE_ALARM_MINUTES,
	MENU_CHANGE_ALARM_SECONDS,
	MENU_CHANGE_ALARM_STATUS,
}menu_status_t;

typedef enum menu_mode_e{
	MENU_SCREEN_NO_FREEZE, 	/**< when there are some options to show on the screen */
	MENU_SCREEN_FREEZE  	/**< when the menu only changes a number */
}menu_mode_T;

typedef struct{
	uint8_t position;
    char  msg_noSelected[MENU_MAX_COLS];
	char  msg_selected[MENU_MAX_COLS];
	menuSelected menuCallback;
}menu_item_T;

typedef struct menu_dateTimeCfg_s{
	uint8_t hourFormat; /**< Hour format. 0=24h ; 1=12h */
	uint8_t hour;		/**< Hour */
	uint8_t minutes;    /**< Minutes */
	uint8_t seconds;	/**< Seconds */
	uint8_t amPm; 		/**< 0=AM ; 1=PM */
	uint8_t dayOfWeek;  /**< Day of week */
	uint8_t dayOfMonth; /**< Day of Month */
	uint8_t month;		/**< Month */
	uint16_t year;
}menu_dateTimeCfg_t;

typedef struct menu_alarmCfg_s{
	uint8_t hourFormatAlarm;	/**< Hour format. 0=24h ; 1=12h */
	uint8_t hourAlarm;			/**< Hour */
	uint8_t minutesAlarm;   	/**< Minutes */
	uint8_t secondsAlarm;   	/**< Seconds */
	uint8_t amPmAlarm; 			/**< 0=AM ; 1=PM */
	uint8_t alarmStatus; 		/**< Alarm status. 0=OFF(Inactive); 1 = ON(Active) */
}menu_alarmCfg_t;

typedef struct menu_pidCfg_s{
	uint32_t kp;			/**< Kp parameter */
	uint32_t ki;			/**< Ki parameter */
	uint32_t kd;			/**< Kd parameter */
	uint32_t setPoint;		/**< Set point */
}menu_pidCfg_t;

typedef struct menu_cfg_s {
	LCD_t *lcd;
	menu_status_t status;
	menu_mode_T freezeScreen;
	uint8_t MenuMaxLines;// maximo numero de lineas que tiene el menu
	uint8_t MenuLinestart;// linea a partir de la cual se imprimira el menu
	uint8_t menu_maxOptions;// Maxima posicion que puede alcanzar el menu o numero maximo de posibilidades en el menu
	uint8_t menu_minOptions;// posicion donde empieza el menu o numero minimo de posibilidades del menu
	const menu_item_T *menu_items;
}menu_cfg_T;

typedef struct menu_data_s{
	menu_dateTimeCfg_t dateTimeCfg; /**< Contains the configuration parameters for the date & time configuration*/
	menu_alarmCfg_t alarmCfg;		/**< Contains the configuration parameters for the alarm configuration */
	menu_pidCfg_t pidCfg;			/**< Contains the configuration parameters for the pid configuration */
	uint8_t dateTimeChangesAvailable;
	uint8_t pidChangesAvailable;
	const char *lines[MENU_MAX_ROWS];
	uint8_t hour10;
	uint8_t hour;
	uint8_t minutes10;
	uint8_t minutes;
	uint8_t seconds10;
	uint8_t seconds;
	uint16_t year;
	uint8_t monthDay;
	const char *month;
	const char *weekDay;
	const char *amPm;
	uint16_t kpEntire;
	uint16_t kpDecimal;
	uint16_t kiEntire;
	uint16_t kiDecimal;
	uint16_t kdEntire;
	uint16_t kdDecimal;
	uint16_t SetPointEntire;
	uint16_t SetPointDecimal;
	uint16_t adc_val;
	uint16_t duty;
	uint32_t menu_CurrentIndex;
	uint32_t menu_PreviousIndexLevel1;
	uint32_t menu_PreviousIndexLevel2;
	uint32_t menu_PreviousIndexLevel3;
	uint32_t menu_PreviousIndexLevel4;
	uint32_t menu_PreviousIndexLevel5;
	uint32_t menu_PreviousIndexLevel6;
}menu_data_T;

typedef struct menu_s {
	menu_cfg_T *cfg;
	menu_cfg_T **globalCfg;
	menu_data_T data;
}menu_T;

/******************************************************************************/
/*                    Declaration of exported variables                       */
/******************************************************************************/

/******************************************************************************/
/*                  Declaration of exported constant data                     */
/******************************************************************************/

/******************************************************************************/
/*               Declaration of exported function prototypes                  */
/******************************************************************************/

/**
* Initialize the crc with processor expert parameters
*
* \return AHAL_HGEN_RESULT_SUCCESS if init function has correctly initialized, AHAL_HGEN_RESULT_ERROR otherwise
*
* \description
* Initialize the crc with processor expert parameters
*
**/
extern void MENU_Init(menu_T *menu, const menu_cfg_T *(*menuCfg)[], const menu_dateTimeCfg_t *dateTimeCfg, const menu_alarmCfg_t * alarmCfg, const menu_pidCfg_t *pidCfg);
extern void MENU_keyPressed(menu_T * menu);
extern void MENU_Task(menu_T * menu);
extern void MENU_encoderOption (menu_T *menu, int32_t current);
extern uint8_t menu_defaultScreen( void* userData1, void* userData2);
extern uint8_t menu_PidSelected( void* userData1, void* userData2);
extern uint8_t menu_HourSelected( void* userData1, void* userData2);
extern uint8_t menu_exitChangePidSelected( void* userData1, void* userData2);
extern uint8_t menu_enterChangeClockSelected( void* userData1, void* userData2);
extern uint8_t menu_exitChangeClockSelected( void* userData1, void* userData2);
extern uint8_t menu_ChangePwmSelected( void* userData1, void* userData2);
extern uint8_t menu_ExitSelected( void* userData1, void* userData2);
extern uint8_t MENU_defaultPidToMainSel(void* userData1, void* userData2);
extern uint8_t MENU_defaultHourToMainSel(void* userData1, void* userData2);
extern uint8_t menu_ChangeHourFormatSel( void* userData1, void* userData2);
extern uint8_t menu_ChangeHourSel( void* userData1, void* userData2);
extern uint8_t menu_ChangeMinSel( void* userData1, void* userData2);
extern uint8_t menu_ChangeSecSel( void* userData1, void* userData2);
extern uint8_t menu_ChangeAmPmSel( void* userData1, void* userData2);
extern uint8_t menu_ChangeWeekDaySel( void* userData1, void* userData2);
extern uint8_t menu_ChangeMonthDaySel( void* userData1, void* userData2);
extern uint8_t menu_ChangeMonthSel( void* userData1, void* userData2);
extern uint8_t menu_ChangeYearSel( void* userData1, void* userData2);
extern uint8_t menu_exitAndSaveDateTime( void* userData1, void* userData2);
extern uint8_t menu_exitAndNoSaveDateTime( void* userData1, void* userData2);
extern uint8_t menu_ChangeKpEntireSel( void* userData1, void* userData2);
extern uint8_t menu_ChangeKpEntireSel( void* userData1, void* userData2);
extern uint8_t menu_ChangeKpDecimalSel( void* userData1, void* userData2);
extern uint8_t menu_ChangeKiEntireSel( void* userData1, void* userData2);
extern uint8_t menu_ChangeKiDecimalSel( void* userData1, void* userData2);
extern uint8_t menu_ChangeKdEntireSel( void* userData1, void* userData2);
extern uint8_t menu_ChangeKdDecimalSel( void* userData1, void* userData2);
extern uint8_t menu_ChangeSetPointEntireSel( void* userData1, void* userData2);
extern uint8_t menu_ChangeSetPointDecimalSel( void* userData1, void* userData2);
extern uint8_t menu_ExitAndSavePid( void* userData1, void* userData2);
extern uint8_t menu_exitAndNoSavePid( void* userData1, void* userData2);
extern uint8_t MENU_pidOptionsExitToMain( void* userData1, void* userData2);
extern uint8_t menu_enterPidOptions( void* userData1, void* userData2);
extern uint8_t MENU_showPidValues( void* userData1, void* userData2);
extern uint8_t menu_enterClockOptions( void* userData1, void* userData2);
extern uint8_t menu_exitAndNoSaveAlarm( void* userData1, void* userData2);
extern uint8_t menu_exitAndSaveAlarm( void* userData1, void* userData2);
extern uint8_t menu_exitChangeAlarmSelected( void* userData1, void* userData2);
extern uint8_t menu_ChangeAlarmAmPmSel( void* userData1, void* userData2);
extern uint8_t menu_ChangeAlarmHourFormatSel( void* userData1, void* userData2);
extern uint8_t menu_ChangeAlarmHourSel( void* userData1, void* userData2);
extern uint8_t menu_ChangeAlarmMinSel( void* userData1, void* userData2);
extern uint8_t menu_ChangeAlarmSecSel( void* userData1, void* userData2);
extern uint8_t menu_ChangeAlarmStatusSel( void* userData1, void* userData2);
extern uint8_t menu_enterChangeAlarmSelected( void* userData1, void* userData2);
extern uint8_t menu_enterChangePidSelected( void* userData1, void* userData2);
extern uint8_t menu_GetChangesDataTime(menu_T *menu);
extern uint8_t menu_GetChangesPid(menu_T *menu);
extern uint8_t menu_checkAlarm(menu_T *menu);
extern void menu_SetSecondUnits(menu_T *menu, uint8_t Secondunits);
extern void menu_SetSecondDec(menu_T *menu, uint8_t SecondDec);
extern void menu_SetMinutesUnits(menu_T *menu, uint8_t minutesUnits);
extern void menu_SetMinutesDec(menu_T *menu, uint8_t minutesDec);
extern void menu_SetHourUnits(menu_T *menu, uint8_t hourUnits);
extern void menu_SetHourDec(menu_T *menu, uint8_t hourDec);
extern void menu_SetYear(menu_T *menu, uint16_t year);
extern void menu_SetMonthDay(menu_T *menu, uint8_t monthDay);
extern void menu_SetAmPm(menu_T *menu, const char* amPm);
extern void menu_SetWeekday(menu_T *menu, const char* weekDay);
extern void menu_SetMonth(menu_T *menu, const char* month);
extern menu_status_t MENU_GetStatus(menu_T *menu);
#endif /* MENU_H_ */


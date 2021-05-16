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
#define MENU_MAX_OPTIONS 	(15U)
#define MENU_TRUE 			(1U)
#define MENU_FALSE 			(0U)

#define MENU_SCREEN_NO_FREEZE	(0U)
#define MENU_SCREEN_FREEZE	(1U)
#define MENU_UNKNOWN	"Unknown"

#define MENU_AM "AM"
#define MENU_PM "PM"

#define MENU_12H "12h"
#define MENU_24H "24h"

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
/* Meses del año */
#define MENU_JAN	"Ene"
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
#define menu_op6 		"menu6               "
#define menu_op6_sel 	"menu6              <"
#define menu_op7  		"menu7               "
#define menu_op7_sel 	"menu7              <"
#define menu_op8  		"menu8               "
#define menu_op8_sel  	"menu8              <"
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
	MENU_CLOCK_SETTINGS,
	MENU_PWM_SETTINGS,
	MENU_EXIT,
	MENU_PID_SETTINGS,
	MENU_CHANGE_HOUR_FORMAT,
	MENU_CHANGE_HOUR_AM_PM,
	MENU_CHANGE_HOUR,
	MENU_CHANGE_MINUTES,
	MENU_CHANGE_SECONDS,
	MENU_CHANGE_WEEKDAY,
	MENU_CHANGE_MONTHDAY,
	MENU_CHANGE_MONTH,
	MENU_CHANGE_YEAR,
	MENU_CHANGE_KP_ENTIRE,
	MENU_CHANGE_KP_DECIMAL,
	MENU_CHANGE_KI_ENTIRE,
	MENU_CHANGE_KI_DECIMAL,
	MENU_CHANGE_KD_ENTIRE,
	MENU_CHANGE_KD_DECIMAL,
	MENU_CHANGE_SET_POINT_ENTIRE,
	MENU_CHANGE_SET_POINT_DECIMAL,
	MENU_PID_OPTIONS,
	MENU_PID_VALUES,
	MENU_CLOCK_OPTIONS,
}menu_status_t;

typedef struct menu_item_s {
	uint8_t position;
    char  msg_noSelected[MENU_MAX_COLS];
	char  msg_selected[MENU_MAX_COLS];
	menuSelected menuCallback;
}menu_item_T;

typedef struct menu_dateTimeCfg_s{
	uint8_t hourFormat; // 0=24h ; 1=12h
	uint8_t hour;
	uint8_t minutes;
	uint8_t seconds;
	uint8_t amPm; // 0=AM ; 1=PM
	uint8_t dayOfWeek;
	uint8_t dayOfMonth;
	uint8_t month;
	uint16_t year;
}menu_dateTimeCfg_t;

typedef struct menu_pidCfg_s{
	uint32_t kp;
	uint32_t ki;
	uint32_t kd;
	uint32_t consigna;
}menu_pidCfg_t;

typedef struct menu_cfg_s {
	LCD_t *lcd;
	menu_status_t status;
	uint8_t freezeScreen;
	uint8_t MenuMaxLines;
	uint8_t MenuLinestart;
	uint8_t menu_maxOptions;
	uint8_t menu_minOptions;
	menu_item_T menu_items[MENU_MAX_OPTIONS];
}menu_cfg_T;

typedef struct menu_data_s{
	menu_dateTimeCfg_t dateTimeCfg;
	menu_pidCfg_t pidCfg;
	uint8_t dateTimeChangesAvailable;
	uint8_t pidChangesAvailable;
	char *lines[MENU_MAX_ROWS];
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
	uint32_t menu_index;
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
extern void menu_Init(menu_T *menu, const menu_cfg_T *(*menuCfg)[], const menu_dateTimeCfg_t *dateTimeCfg, const menu_pidCfg_t *pidCfg);
extern void menu_keyPressed(menu_T * menu);
extern void menu_Task(menu_T * menu);
extern void menu_encoderOption (menu_T *menu, int32_t current);
extern uint8_t menu_defaultScreen( void* userData1, void* userData2);
extern uint8_t menu_PidSelected( void* userData1, void* userData2);
extern uint8_t menu_HourSelected( void* userData1, void* userData2);
extern uint8_t menu_ChangePidSelected( void* userData1, void* userData2);
extern uint8_t menu_ChangeClockSelected( void* userData1, void* userData2);
extern uint8_t menu_ChangePwmSelected( void* userData1, void* userData2);
extern uint8_t menu_ExitSelected( void* userData1, void* userData2);
extern uint8_t menu_defaultPidSel(void* userData1, void* userData2);
extern uint8_t menu_defaultHourSel(void* userData1, void* userData2);
extern uint8_t menu_ChangeHourFormatSel( void* userData1, void* userData2);
extern uint8_t menu_ChangeHourSel( void* userData1, void* userData2);
extern uint8_t menu_ChangeMinSel( void* userData1, void* userData2);
extern uint8_t menu_ChangeSecSel( void* userData1, void* userData2);
extern uint8_t menu_ChangeAmPmSel( void* userData1, void* userData2);
extern uint8_t menu_ChangeWeekDaySel( void* userData1, void* userData2);
extern uint8_t menu_ChangeMonthDaySel( void* userData1, void* userData2);
extern uint8_t menu_ChangeMonthSel( void* userData1, void* userData2);
extern uint8_t menu_ChangeYearSel( void* userData1, void* userData2);
extern uint8_t menu_exitAndSave( void* userData1, void* userData2);
extern uint8_t menu_exitAndNoSave( void* userData1, void* userData2);
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
extern uint8_t menu_showPidOptions( void* userData1, void* userData2);
extern uint8_t menu_showPidValues( void* userData1, void* userData2);
extern uint8_t menu_showClockOptions( void* userData1, void* userData2);

extern uint8_t menu_GetChangesDataTime(menu_T *menu);
extern uint8_t menu_GetChangesPid(menu_T *menu);
#endif /* MENU_H_ */


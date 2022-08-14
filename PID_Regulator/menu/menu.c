/******************************************************************************/
/*   All rights reserved. Distribution or duplication without previous        */
/*   written agreement of the owner prohibited.                               */
/*                                                                            */
/******************************************************************************/

/** \file menu.c
 *
 * \brief Source code file for menu
 *
 * <table border="0" cellspacing="0" cellpadding="0">
 * <tr> <td> Author:   </td> <td> C Garcia   </td></tr>
 * <tr> <td> Date:     </td> <td> 24/01/2021             </td></tr>
 * </table>
 * \n
 * <table border="0" cellspacing="0" cellpadding="0">
 * <tr> <td> COMPONENT: </td> <td> menu   </td></tr>
 * <tr> <td> TARGET:    </td> <td> STM32F103C8T6         </td></tr>
 * </table>
 * \note
 *
 * \see
 */

/******************************************************************************/
/*                Include common and project definition header                */
/******************************************************************************/

/******************************************************************************/
/*                      Include headers of the component                      */
/******************************************************************************/
#include "menu.h"
//#include "stm32f1xx_hal.h"
/******************************************************************************/
/*                            Include other headers                           */
/******************************************************************************/

/******************************************************************************/
/*                   Definition of local symbolic constants                   */
/******************************************************************************/
#define menu_WEEK_DAYS_MAX		(8U)
#define menu_MONTHS_MAX			(13U)
#define menu_AMPM_MAX			(2U)
#define menu_HOUR_FORMAT_MAX	(2U)
#define menu_ALARM_FORMAT_MAX	(2U)
#define menu_12H				(12U)
#define menu_1H					(1U)
#define menu_24H_FORMAT			(0U)
#define menu_12H_FORMAT			(1U)
#define menu_24H_FORMAT			(0U)
#define menu_12H_FORMAT			(1U)
#define menu_AM_CFG				(0U)
#define menu_PM_CFG				(1U)

#define menu_UNKNOWN	"Unknown"
#define menu_AM 		"AM"
#define menu_PM 		"PM"
#define MENU_12H 		"12h"
#define MENU_24H 		"24h"
#define menu_ALARM_ON 	"On"
#define menu_ALARM_OFF  "Off"

#define menu_LINE0	(0U)
#define menu_LINE1	(1U)
#define menu_LINE2	(2U)
#define menu_LINE3	(3U)

#define menu_DECIMAL 	(10U)
#define menu_CENTIMAL 	(100U)
#define menu_MILESIMAL 	(1000U)
#define menu_MILENIUM	(2000U)

/******************************************************************************/
/*                  Definition of local function like macros                  */
/******************************************************************************/
//  Macros to convert the bcd values of the registers to normal integer variables.  //
//  The code uses separate variables for the high byte and the low byte of the bcd, //
//  so these macros handle both bytes separately.//
#define MENU_BCD2BIN(h,l)   (((h)*10) + (l))
#define MENU_BIN2BCD_H(x)   ((x)/10)
#define MENU_BIN2BCD_L(x)   ((x)%10)
/******************************************************************************/
/*          Definition of local types (typedef, enum, struct, union)          */
/******************************************************************************/

/******************************************************************************/
/*                       Definition of local variables                        */
/******************************************************************************/
static int32_t previous_val = 0;
/* Days of a week */
static char const *menu_days[menu_WEEK_DAYS_MAX]={menu_UNKNOWN, MENU_SUNDAY, MENU_MONDAY, MENU_TUESDAY, MENU_WEDNESDAY, MENU_THURSDAY, MENU_FRIDAY, MENU_SATURDAY};
/* Months of a year */
static char const *menu_months[menu_MONTHS_MAX]={menu_UNKNOWN, MENU_JAN, MENU_FEB, MENU_MAR, MENU_APR, MENU_MAY, MENU_JUN, MENU_JUL, MENU_AUG, MENU_SEP, MENU_OCT, MENU_NOV, MENU_DIC};
/* AM/PM */
static char const *menu_AMPM[menu_AMPM_MAX]={menu_AM, menu_PM};
/* Hour format 24H/12H */
static char const *menu_hourFormat[menu_HOUR_FORMAT_MAX]={MENU_24H, MENU_12H};
/* Hour format 24H/12H */
static char const *menu_AlarmOnOff[menu_ALARM_FORMAT_MAX]={menu_ALARM_OFF, menu_ALARM_ON};

//extern TIM_HandleTypeDef htim2;
/******************************************************************************/
/*                     Definition of local constant data                      */
/******************************************************************************/

/******************************************************************************/
/*                      Definition of exported variables                      */
/******************************************************************************/

/******************************************************************************/
/*                    Definition of exported constant data                    */
/******************************************************************************/

/******************************************************************************/
/*                  Declaration of local function prototypes                  */
/******************************************************************************/
static void menu_applyCfg(menu_T *menu, menu_status_t selcCfg);
static void menu_decimalPartAdapter(menu_T *menu, uint8_t line ,const char* parameter, uint16_t entire, uint16_t decimal);
static void menu_PrintDefaultPid(menu_T *menu);
static void menu_printDateTime(menu_T *menu);
static void menu_PrintMainMenu(const menu_T *menu);
static void menu_PrintChangePid( menu_T *menu);
static void menu_PrintChangeDateTime(menu_T *menu);
static void menu_PrintChangeHour(menu_T *menu);
static void menu_PrintChangeMinutes(menu_T *menu);
static void menu_PrintChangeSeconds(menu_T *menu);
static void menu_PrintChangeWeekday(menu_T *menu);
static void menu_PrintChangeMonthDay(menu_T *menu);
static void menu_PrintChangeMonth(menu_T *menu);
static void menu_PrintChangeYear(menu_T *menu);
static void menu_PrintChangeHourFormat(menu_T *menu);
static void menu_PrintChangeHourAmPm(menu_T *menu);
static void menu_PrintChangeKpEntire(menu_T *menu);
static void menu_PrintChangeKpDecimal(menu_T *menu);
static void menu_PrintChangeKiEntire(menu_T *menu);
static void menu_PrintChangeKiDecimal(menu_T *menu);
static void menu_PrintChangeKdEntire(menu_T *menu);
static void menu_PrintChangeKdDecimal(menu_T *menu);
static void menu_PrintPidOptions(menu_T *menu);
static void menu_PrintPidValues(menu_T *menu);
static void menu_PrintClockOptions(menu_T *menu);
static void menu_PrintChangeAlarmSettings(menu_T *menu);
static void menu_PrintChangeSetPointEntire(menu_T *menu);
static void menu_PrintChangeSetPointDecimal(menu_T *menu);
static void menu_PrintChangeAlarmHourAmPm(menu_T *menu);
static void menu_PrintChangeAlarmFormat(menu_T *menu);
static void menu_PrintChangeAlarmHour(menu_T *menu);
static void menu_PrintChangeAlarmMinutes(menu_T *menu);
static void menu_PrintChangeAlarmSeconds(menu_T *menu);
static void menu_PrintChangeAlarmStatus(menu_T *menu);

static void menu_SetChangeDateTimeFlag(menu_T *menu, uint8_t flagVal);
static void menu_SetChangePidFlag(menu_T *menu, uint8_t flagVal);

static void menu_cleanLcd(menu_T *menu);
static uint8_t menu_checkConstraints(const menu_cfg_T *(*cfg)[], const menu_dateTimeCfg_t *dateTimeCfg, const menu_pidCfg_t *pidCfg);
static uint8_t menu_searchNum(uint8_t number, uint8_t* arr);
/******************************************************************************/
/*                       Definition of local functions                        */
/******************************************************************************/

static uint8_t menu_checkConstraints(const menu_cfg_T *(*cfg)[], const menu_dateTimeCfg_t *dateTimeCfg, const menu_pidCfg_t *pidCfg){
	uint8_t retVal = MENU_FALSE;
	uint8_t i;
	for(i=0; i<4; i++){
		//(*cfg)[1]->MenuLinestart =1;
		if((*cfg)[i] != NULL){
			if(((((*cfg)[i]->MenuMaxLines == 4) && ((*cfg)[i]->MenuLinestart == 0)) ||
					(((*cfg)[i]->MenuMaxLines == 3) && (((*cfg)[i]->MenuLinestart == 0) || ((*cfg)[i]->MenuLinestart == 1))) ||
					(((*cfg)[i]->MenuMaxLines == 2) && (((*cfg)[i]->MenuLinestart == 0) || ((*cfg)[i]->MenuLinestart == 1) || ((*cfg)[i]->MenuLinestart == 2))) ||
					(((*cfg)[i]->MenuMaxLines == 1) && (((*cfg)[i]->MenuLinestart == 0) || ((*cfg)[i]->MenuLinestart == 1) || ((*cfg)[i]->MenuLinestart == 2) || ((*cfg)[i]->MenuLinestart == 3)))) &&
					((*cfg)[i]->menu_maxOptions >= (*cfg)[i]->menu_minOptions)){//pueden ser iguales el maximo y el minimo
				retVal = MENU_TRUE;
			}else{
				retVal = MENU_FALSE;
				break;
			}
		}else{
			retVal = MENU_FALSE;
			break;
		}
	}

	if((retVal == MENU_TRUE) && (dateTimeCfg != NULL) && (pidCfg != NULL)){
		retVal = MENU_TRUE;
	} else {
		retVal = MENU_FALSE;
	}

	return retVal;
}

static void menu_decimalPartAdapter(menu_T *menu, uint8_t line ,const char* parameter, uint16_t entire, uint16_t decimal){

	if((line < MENU_MAX_ROWS) && (menu!= NULL)){
		if(decimal < menu_DECIMAL){
			snprintf(menu->data.lineContent[line], 20, "%s %d.00%d      ", parameter, entire, decimal);
		}else if((decimal >= menu_DECIMAL) && (decimal < menu_CENTIMAL)){
			snprintf(menu->data.lineContent[line], 20, "%s %d.0%d      ", parameter, entire, decimal);
		}else if ((decimal >= menu_CENTIMAL) && (decimal < menu_MILESIMAL)){
			snprintf(menu->data.lineContent[line], 20, "%s %d.%d      ", parameter, entire, decimal);
		}else{
			/* Nothing to do */
		}
	}

}

static uint8_t menu_searchNum(uint8_t number, uint8_t* arr){
	uint8_t i = 0;
	uint8_t ret = 0;
	for (i=0; i<MENU_MAX_ROWS; i++){
		if(number == arr[i]){
			ret = 1;
			break;
		}
	}
	return ret;
}

static void menu_cleanLcd(menu_T *menu)
{
	memcpy(menu->data.lineContent[menu_LINE0], "                    ",20);
	memcpy(menu->data.lineContent[menu_LINE1], "                    ",20);
	memcpy(menu->data.lineContent[menu_LINE2], "                    ",20);
	memcpy(menu->data.lineContent[menu_LINE3], "                    ",20);
}

static void menu_PrintDefaultPid(menu_T *menu){
	snprintf(menu->data.lineContent[menu_LINE0], 20, "%s", menu->data.lines[menu_LINE0]);
	menu->data.lines[menu_LINE0] = menu->data.lineContent[menu_LINE0];
	snprintf(menu->data.lineContent[menu_LINE1], 20, "%s%d.%d", menu->data.lines[menu_LINE1], menu->data.SetPointEntire, menu->data.SetPointDecimal);
	menu->data.lines[menu_LINE1] = menu->data.lineContent[menu_LINE1];
	snprintf(menu->data.lineContent[menu_LINE2], 20, "%s%d       ", menu->data.lines[menu_LINE2], menu->data.adc_val);
	menu->data.lines[menu_LINE2] = menu->data.lineContent[menu_LINE2];
	snprintf(menu->data.lineContent[menu_LINE3], 20, "%s%d            ", menu->data.lines[menu_LINE3], menu->data.duty);
	menu->data.lines[menu_LINE3] = menu->data.lineContent[menu_LINE3];
}

static void menu_PrintChangePid( menu_T *menu){
	const char* menu_pidLine2[8]={"KP:", "KP:", "KI:", "KI:", "KD:", "KD:", "SP:", "SP:"};
	uint16_t menu_PidValues[8][2]={{menu->data.kpEntire, menu->data.kpDecimal},
			{menu->data.kpEntire, menu->data.kpDecimal},
			{menu->data.kiEntire, menu->data.kiDecimal},
			{menu->data.kiEntire, menu->data.kiDecimal},
			{menu->data.kdEntire, menu->data.kdDecimal},
			{menu->data.kdEntire, menu->data.kdDecimal},
			{menu->data.SetPointEntire, menu->data.SetPointDecimal},
			{menu->data.SetPointEntire, menu->data.SetPointDecimal}};
	if(menu->data.menu_CurrentIndex-menu->cfg->menu_minOptions < 8){

		menu_decimalPartAdapter(menu, menu_LINE2, menu_pidLine2[menu->data.menu_CurrentIndex-menu->cfg->menu_minOptions],
				menu_PidValues[menu->data.menu_CurrentIndex-menu->cfg->menu_minOptions][0],
				menu_PidValues[menu->data.menu_CurrentIndex-menu->cfg->menu_minOptions][1]);

		menu->data.lines[menu_LINE2] = menu->data.lineContent[menu_LINE2];
	}
}

static void menu_PrintPidOptions(menu_T *menu){

}

static void menu_PrintClockOptions(menu_T *menu){

}

static void menu_PrintPidValues (menu_T *menu){
	menu_decimalPartAdapter(menu, menu_LINE0, menu->data.lines[menu_LINE0], menu->data.kpEntire, menu->data.kpDecimal);
	menu_decimalPartAdapter(menu, menu_LINE1, menu->data.lines[menu_LINE1], menu->data.kiEntire, menu->data.kiDecimal);
	menu_decimalPartAdapter(menu, menu_LINE2, menu->data.lines[menu_LINE2], menu->data.kdEntire, menu->data.kdDecimal);
	menu_decimalPartAdapter(menu, menu_LINE3, menu->data.lines[menu_LINE3], menu->data.SetPointEntire, menu->data.SetPointDecimal);
	menu->data.lines[menu_LINE0] = menu->data.lineContent[menu_LINE0];
	menu->data.lines[menu_LINE1] = menu->data.lineContent[menu_LINE1];
	menu->data.lines[menu_LINE2] = menu->data.lineContent[menu_LINE2];
	menu->data.lines[menu_LINE3] = menu->data.lineContent[menu_LINE3];

}

static void menu_PrintMainMenu(const menu_T *menu){

}

static void menu_printDateTime(menu_T *menu){
	snprintf(menu->data.lineContent[menu_LINE3], 20, "- %s/%d/%s/%d -", menu->data.weekDay, menu->data.monthDay, menu->data.month, menu->data.year);
	menu->data.lines[menu_LINE3] = menu->data.lineContent[menu_LINE3];
}

static void menu_PrintChangeAlarmSettings(menu_T *menu){
	snprintf(menu->data.lineContent[menu_LINE2], 20, "%s/%s   %d:%d:%d   ", menu_hourFormat[menu->data.alarmCfg.hourFormatAlarm], menu_AMPM[menu->data.alarmCfg.amPmAlarm], menu->data.alarmCfg.hourAlarm, menu->data.alarmCfg.minutesAlarm, menu->data.alarmCfg.secondsAlarm);
	menu->data.lines[menu_LINE2] = menu->data.lineContent[menu_LINE2];
	snprintf(menu->data.lineContent[menu_LINE3], 20, "Alarm Status: %s   ", menu_AlarmOnOff[menu->data.alarmCfg.alarmStatus]);
	menu->data.lines[menu_LINE3] = menu->data.lineContent[menu_LINE3];
}

static void menu_PrintChangeDateTime(menu_T *menu){

	snprintf(menu->data.lineContent[menu_LINE2], 20, "%s/%s   %d:%d:%d   ", menu_hourFormat[menu->data.dateTimeCfg.hourFormat], menu_AMPM[menu->data.dateTimeCfg.amPm], menu->data.dateTimeCfg.hour, menu->data.dateTimeCfg.minutes, menu->data.dateTimeCfg.seconds);
	menu->data.lines[menu_LINE2] = menu->data.lineContent[menu_LINE2];
	snprintf(menu->data.lineContent[menu_LINE3], 20, "  %s/%d/%s/%d   ", menu_days[menu->data.dateTimeCfg.dayOfWeek], menu->data.dateTimeCfg.dayOfMonth, menu_months[menu->data.dateTimeCfg.month], menu->data.dateTimeCfg.year);
	menu->data.lines[menu_LINE3] = menu->data.lineContent[menu_LINE3];

}

static void menu_PrintChangeAlarmFormat(menu_T *menu){
	static uint8_t hformatprevious;
	menu->data.alarmCfg.hourFormatAlarm = menu->data.menu_CurrentIndex;

	if(menu->data.alarmCfg.hourFormatAlarm != hformatprevious ){
		if((menu->data.alarmCfg.hourFormatAlarm == menu_12H_FORMAT) && (menu->data.alarmCfg.hourAlarm >= menu_12H)){
			menu->data.alarmCfg.hourAlarm -= menu_12H;
		}else{
			if(menu->data.alarmCfg.amPmAlarm){
				// if is configured as PM
				menu->data.alarmCfg.hourAlarm += menu_12H;
			}
		}
	}
	hformatprevious = menu->data.alarmCfg.hourFormatAlarm;

	snprintf(menu->data.lineContent[menu_LINE2], 20, "%s/%s   %d:%d:%d   ", menu_hourFormat[menu->data.alarmCfg.hourFormatAlarm], menu_AMPM[menu->data.alarmCfg.amPmAlarm], menu->data.alarmCfg.hourAlarm, menu->data.alarmCfg.minutesAlarm, menu->data.alarmCfg.secondsAlarm);
	menu->data.lines[menu_LINE2] = menu->data.lineContent[menu_LINE2];

}

static void menu_PrintChangeHourFormat(menu_T *menu){

	static uint8_t hformatprevious;
	menu->data.dateTimeCfg.hourFormat = menu->data.menu_CurrentIndex;

	if(menu->data.dateTimeCfg.hourFormat != hformatprevious ){
		if((menu->data.dateTimeCfg.hourFormat == menu_12H_FORMAT) && (menu->data.dateTimeCfg.hour >= menu_12H)){
			menu->data.dateTimeCfg.hour -= menu_12H;
		}else{
			if(menu->data.dateTimeCfg.amPm){
				// if is configured as PM
				menu->data.dateTimeCfg.hour += menu_12H;
			}
		}
	}
	hformatprevious = menu->data.dateTimeCfg.hourFormat;

	snprintf(menu->data.lineContent[menu_LINE2], 20, "%s/%s   %d:%d:%d   ", menu_hourFormat[menu->data.dateTimeCfg.hourFormat], menu_AMPM[menu->data.dateTimeCfg.amPm], menu->data.dateTimeCfg.hour, menu->data.dateTimeCfg.minutes, menu->data.dateTimeCfg.seconds);
	menu->data.lines[menu_LINE2] = menu->data.lineContent[menu_LINE2];

}

static void menu_PrintChangeAlarmHourAmPm(menu_T *menu){
	/* The AM/PM mode can be changed only if the hour format mode is equal to 0-12h and the hours are less than 12*/
	if((menu->data.alarmCfg.hourFormatAlarm == menu_12H_FORMAT) && (menu->data.alarmCfg.hourAlarm < menu_12H)){
		menu->data.alarmCfg.amPmAlarm = menu->data.menu_CurrentIndex;
		snprintf(menu->data.lineContent[menu_LINE2], 20, "%s/%s   %d:%d:%d   ", menu_hourFormat[menu->data.alarmCfg.hourFormatAlarm], menu_AMPM[menu->data.alarmCfg.amPmAlarm], menu->data.alarmCfg.hourAlarm, menu->data.alarmCfg.minutesAlarm, menu->data.alarmCfg.secondsAlarm);
		menu->data.lines[menu_LINE2] = menu->data.lineContent[menu_LINE2];

	}
}

static void menu_PrintChangeHourAmPm(menu_T *menu){
	/* The AM/PM mode can be changed only if the hour format mode is equal to 0-12h and the hours are less than 12*/
	if((menu->data.dateTimeCfg.hourFormat == menu_12H_FORMAT) && (menu->data.dateTimeCfg.hour < menu_12H)){
		menu->data.dateTimeCfg.amPm = menu->data.menu_CurrentIndex;
		snprintf(menu->data.lineContent[menu_LINE2], 20, "%s/%s   %d:%d:%d   ", menu_hourFormat[menu->data.dateTimeCfg.hourFormat], menu_AMPM[menu->data.dateTimeCfg.amPm], menu->data.dateTimeCfg.hour, menu->data.dateTimeCfg.minutes, menu->data.dateTimeCfg.seconds);
		menu->data.lines[menu_LINE2] = menu->data.lineContent[menu_LINE2];

	}
}

static void menu_PrintChangeHour(menu_T *menu){
	if((menu->data.dateTimeCfg.hourFormat == menu_24H_FORMAT) && ((menu->data.menu_CurrentIndex >= menu_12H))){
		menu->data.dateTimeCfg.amPm = menu_PM_CFG;
	}else{
		menu->data.dateTimeCfg.amPm = menu_AM_CFG;
	}
	menu->data.dateTimeCfg.hour = menu->data.menu_CurrentIndex;
	snprintf(menu->data.lineContent[menu_LINE2], 20, "%s/%s   %d:%d:%d   ", menu_hourFormat[menu->data.dateTimeCfg.hourFormat], menu_AMPM[menu->data.dateTimeCfg.amPm], menu->data.dateTimeCfg.hour, menu->data.dateTimeCfg.minutes, menu->data.dateTimeCfg.seconds);

	menu->data.lines[menu_LINE2] = menu->data.lineContent[menu_LINE2];

}

static void menu_PrintChangeAlarmHour(menu_T *menu){

	if((menu->data.alarmCfg.hourFormatAlarm == menu_24H_FORMAT) && ((menu->data.menu_CurrentIndex >= menu_12H))){
		menu->data.alarmCfg.amPmAlarm = menu_PM_CFG;
	}else{
		menu->data.alarmCfg.amPmAlarm = menu_AM_CFG;
	}
	menu->data.alarmCfg.hourAlarm = menu->data.menu_CurrentIndex;
	snprintf(menu->data.lineContent[menu_LINE2], 20, "%s/%s   %d:%d:%d   ", menu_hourFormat[menu->data.alarmCfg.hourFormatAlarm], menu_AMPM[menu->data.alarmCfg.amPmAlarm], menu->data.alarmCfg.hourAlarm, menu->data.alarmCfg.minutesAlarm, menu->data.alarmCfg.secondsAlarm);
	menu->data.lines[menu_LINE2] = menu->data.lineContent[menu_LINE2];

}

static void menu_PrintChangeMinutes(menu_T *menu){
	menu->data.dateTimeCfg.minutes = menu->data.menu_CurrentIndex;
	snprintf(menu->data.lineContent[2], 20, "%s/%s   %d:%d:%d   ", menu_hourFormat[menu->data.dateTimeCfg.hourFormat], menu_AMPM[menu->data.dateTimeCfg.amPm], menu->data.dateTimeCfg.hour, menu->data.dateTimeCfg.minutes, menu->data.dateTimeCfg.seconds);
	menu->data.lines[menu_LINE2] = menu->data.lineContent[menu_LINE2];

}

static void menu_PrintChangeAlarmMinutes(menu_T *menu){
	menu->data.alarmCfg.minutesAlarm = menu->data.menu_CurrentIndex;
	snprintf(menu->data.lineContent[menu_LINE2], 20, "%s/%s   %d:%d:%d   ", menu_hourFormat[menu->data.alarmCfg.hourFormatAlarm], menu_AMPM[menu->data.alarmCfg.amPmAlarm], menu->data.alarmCfg.hourAlarm, menu->data.alarmCfg.minutesAlarm, menu->data.alarmCfg.secondsAlarm);
	menu->data.lines[menu_LINE2] = menu->data.lineContent[menu_LINE2];
}

static void menu_PrintChangeSeconds(menu_T *menu){
	menu->data.dateTimeCfg.seconds = menu->data.menu_CurrentIndex;
	snprintf(menu->data.lineContent[menu_LINE2], 20, "%s/%s   %d:%d:%d   ", menu_hourFormat[menu->data.dateTimeCfg.hourFormat], menu_AMPM[menu->data.dateTimeCfg.amPm], menu->data.dateTimeCfg.hour, menu->data.dateTimeCfg.minutes, menu->data.dateTimeCfg.seconds);
	menu->data.lines[menu_LINE2] = menu->data.lineContent[2];
}

static void menu_PrintChangeAlarmSeconds(menu_T *menu){
	menu->data.alarmCfg.secondsAlarm = menu->data.menu_CurrentIndex;
	snprintf(menu->data.lineContent[menu_LINE2], 20, "%s/%s   %d:%d:%d   ", menu_hourFormat[menu->data.alarmCfg.hourFormatAlarm], menu_AMPM[menu->data.alarmCfg.amPmAlarm], menu->data.alarmCfg.hourAlarm, menu->data.alarmCfg.minutesAlarm, menu->data.alarmCfg.secondsAlarm);
	menu->data.lines[menu_LINE2] = menu->data.lineContent[menu_LINE2];
}

static void menu_PrintChangeAlarmStatus(menu_T *menu){
	menu->data.alarmCfg.alarmStatus = menu->data.menu_CurrentIndex;
	snprintf(menu->data.lineContent[menu_LINE3], 20, "Alarm Status: %s   ", menu_AlarmOnOff[menu->data.alarmCfg.alarmStatus]);
	menu->data.lines[menu_LINE3] = menu->data.lineContent[menu_LINE3];
}

static void menu_PrintChangeWeekday(menu_T *menu){
	menu->data.dateTimeCfg.dayOfWeek = menu->data.menu_CurrentIndex;
	if(menu->data.menu_CurrentIndex < menu_WEEK_DAYS_MAX){
		snprintf(menu->data.lineContent[menu_LINE3], 20, "  %s/%d/%s/%d   ", menu_days[menu->data.dateTimeCfg.dayOfWeek], menu->data.dateTimeCfg.dayOfMonth, menu_months[menu->data.dateTimeCfg.month], menu->data.dateTimeCfg.year);
		menu->data.lines[menu_LINE3] = menu->data.lineContent[menu_LINE3];
	}
}

static void menu_PrintChangeMonthDay(menu_T *menu){
	menu->data.dateTimeCfg.dayOfMonth = menu->data.menu_CurrentIndex;
	snprintf(menu->data.lineContent[menu_LINE3], 20, "  %s/%d/%s/%d   ", menu_days[menu->data.dateTimeCfg.dayOfWeek], menu->data.dateTimeCfg.dayOfMonth, menu_months[menu->data.dateTimeCfg.month], menu->data.dateTimeCfg.year);
	menu->data.lines[menu_LINE3] = menu->data.lineContent[3];
}

static void menu_PrintChangeMonth(menu_T *menu){
	menu->data.dateTimeCfg.month = menu->data.menu_CurrentIndex;
	if(menu->data.menu_CurrentIndex < menu_MONTHS_MAX){
		snprintf(menu->data.lineContent[menu_LINE3], 20, "  %s/%d/%s/%d   ", menu_days[menu->data.dateTimeCfg.dayOfWeek], menu->data.dateTimeCfg.dayOfMonth, menu_months[menu->data.dateTimeCfg.month], menu->data.dateTimeCfg.year);
		menu->data.lines[menu_LINE3] = menu->data.lineContent[menu_LINE3];
	}
}

static void menu_PrintChangeYear(menu_T *menu){
	menu->data.dateTimeCfg.year = menu->data.menu_CurrentIndex + menu_MILENIUM;
	snprintf(menu->data.lineContent[menu_LINE3], 20, "  %s/%d/%s/%d   ", menu_days[menu->data.dateTimeCfg.dayOfWeek], menu->data.dateTimeCfg.dayOfMonth, menu_months[menu->data.dateTimeCfg.month], menu->data.dateTimeCfg.year);
	menu->data.lines[menu_LINE3] = menu->data.lineContent[menu_LINE3];
}

static void menu_PrintChangeKpEntire(menu_T *menu){
	menu->data.kpEntire = menu->data.menu_CurrentIndex;
	menu_decimalPartAdapter(menu, menu_LINE2, "KP:", menu->data.kpEntire, menu->data.kpDecimal);
	menu->data.lines[menu_LINE2] = menu->data.lineContent[menu_LINE2];
}

static void menu_PrintChangeKpDecimal(menu_T *menu){
	menu->data.kpDecimal = menu->data.menu_CurrentIndex;
	menu_decimalPartAdapter(menu, menu_LINE2, "KP:", menu->data.kpEntire, menu->data.kpDecimal);
	menu->data.lines[menu_LINE2] = menu->data.lineContent[menu_LINE2];
}

static void menu_PrintChangeKiEntire(menu_T *menu){
	menu->data.kiEntire = menu->data.menu_CurrentIndex;
	menu_decimalPartAdapter(menu, menu_LINE2, "KI:", menu->data.kiEntire, menu->data.kiDecimal);
	menu->data.lines[menu_LINE2] = menu->data.lineContent[menu_LINE2];
}

static void menu_PrintChangeKiDecimal(menu_T *menu){
	menu->data.kiDecimal = menu->data.menu_CurrentIndex;
	menu_decimalPartAdapter(menu, menu_LINE2, "KI:", menu->data.kiEntire, menu->data.kiDecimal);
	menu->data.lines[menu_LINE2] = menu->data.lineContent[menu_LINE2];
}

static void menu_PrintChangeKdEntire(menu_T *menu){
	menu->data.kdEntire = menu->data.menu_CurrentIndex;
	menu_decimalPartAdapter(menu, menu_LINE2, "KD:", menu->data.kdEntire, menu->data.kdDecimal);
	menu->data.lines[menu_LINE2] = menu->data.lineContent[menu_LINE2];
}

static void menu_PrintChangeKdDecimal(menu_T *menu){
	menu->data.kdDecimal = menu->data.menu_CurrentIndex;
	menu_decimalPartAdapter(menu, menu_LINE2, "KD:", menu->data.kdEntire, menu->data.kdDecimal);
	menu->data.lines[menu_LINE2] = menu->data.lineContent[menu_LINE2];
}

static void menu_PrintChangeSetPointEntire(menu_T *menu){
	menu->data.SetPointEntire = menu->data.menu_CurrentIndex;
	menu_decimalPartAdapter(menu, menu_LINE2, "SP:", menu->data.SetPointEntire, menu->data.SetPointDecimal);
	menu->data.lines[menu_LINE2] = menu->data.lineContent[menu_LINE2];
}

static void menu_PrintChangeSetPointDecimal(menu_T *menu){
	menu->data.SetPointDecimal = menu->data.menu_CurrentIndex;
	menu_decimalPartAdapter(menu, menu_LINE2, "SP:", menu->data.SetPointEntire, menu->data.SetPointDecimal);
	menu->data.lines[menu_LINE2] = menu->data.lineContent[menu_LINE2];
}

static void menu_SetChangeDateTimeFlag(menu_T *menu, uint8_t flagVal){
	menu->data.dateTimeChangesAvailable = flagVal;
}

static void menu_SetChangePidFlag(menu_T *menu, uint8_t flagVal){
	menu->data.pidChangesAvailable = flagVal;
}

static void menu_applyCfg(menu_T *menu, menu_status_t selcCfg){
    if((menu != NULL) )
    {
    	switch (selcCfg){
    	case MENU_DEFAULT_CFG:
    		menu->cfg = menu->globalCfg[0];
    		break;
    	case MENU_DEFAULT_PID:
    		menu->cfg = menu->globalCfg[1];
    		break;
    	case MENU_DEFAULT_HOUR:
    		menu->cfg = menu->globalCfg[2];
    		break;
    	case MENU_MAIN:
    		menu->cfg = menu->globalCfg[3];
    		break;
    	case MENU_PID_OPTIONS:
    		menu->cfg = menu->globalCfg[4];
    		break;
    	case MENU_CLOCK_SETTINGS:
    		menu->cfg = menu->globalCfg[5];
    		break;
    	case MENU_PWM_SETTINGS:
    		//menu->cfg = menu->globalCfg[6];
    		break;
    	case MENU_EXIT:
    		break;
    	case MENU_CHANGE_HOUR_FORMAT:
    		menu->cfg = menu->globalCfg[6];
    		break;
    	case MENU_CHANGE_HOUR_AM_PM:
    		menu->cfg = menu->globalCfg[7];
    		break;
    	case MENU_CHANGE_HOUR:
    		menu->cfg = menu->globalCfg[8];
    		break;
    	case MENU_CHANGE_MINUTES:
    		menu->cfg = menu->globalCfg[9];
    		break;
    	case MENU_CHANGE_SECONDS:
    		menu->cfg = menu->globalCfg[10];
    		break;
    	case MENU_CHANGE_WEEKDAY:
    		menu->cfg = menu->globalCfg[11];
    		break;
    	case MENU_CHANGE_MONTHDAY:
    		menu->cfg = menu->globalCfg[12];
    		break;
    	case MENU_CHANGE_MONTH:
    		menu->cfg = menu->globalCfg[13];
    		break;
    	case MENU_CHANGE_YEAR:
    		menu->cfg = menu->globalCfg[14];
    		break;
    	case MENU_PID_SETTINGS:
    		menu->cfg = menu->globalCfg[15];
    		break;
    	case MENU_CHANGE_KP_ENTIRE:
    		menu->cfg = menu->globalCfg[16];
    		break;
    	case MENU_CHANGE_KP_DECIMAL:
    		menu->cfg = menu->globalCfg[17];
    		break;
    	case MENU_CHANGE_KI_ENTIRE:
    		menu->cfg = menu->globalCfg[18];
    		break;
    	case MENU_CHANGE_KI_DECIMAL:
    		menu->cfg = menu->globalCfg[19];
    		break;
    	case MENU_CHANGE_KD_ENTIRE:
    		menu->cfg = menu->globalCfg[20];
    		break;
    	case MENU_CHANGE_KD_DECIMAL:
    		menu->cfg = menu->globalCfg[21];
    		break;
    	case MENU_CHANGE_SET_POINT_ENTIRE:
    		menu->cfg = menu->globalCfg[22];
    		break;
    	case MENU_CHANGE_SET_POINT_DECIMAL:
    		menu->cfg = menu->globalCfg[23];
    		break;
    	case MENU_PID_VALUES:
    		menu->cfg = menu->globalCfg[24];
    		break;
    	case MENU_CLOCK_OPTIONS:
    		menu->cfg = menu->globalCfg[25];
    		break;
    	case MENU_ALARM_SETTINGS:
    		menu->cfg = menu->globalCfg[26];
    		break;
    	case MENU_CHANGE_ALARM_HOUR_FORMAT:
    		menu->cfg = menu->globalCfg[27];
    		break;
    	case MENU_CHANGE_ALARM_HOUR_AM_PM:
    		menu->cfg = menu->globalCfg[28];
    		break;
    	case MENU_CHANGE_ALARM_HOUR:
    		menu->cfg = menu->globalCfg[29];
    		break;
    	case MENU_CHANGE_ALARM_MINUTES:
    		menu->cfg = menu->globalCfg[30];
    		break;
    	case MENU_CHANGE_ALARM_SECONDS:
    		menu->cfg = menu->globalCfg[31];
    		break;
    	case MENU_CHANGE_ALARM_STATUS:
    		menu->cfg = menu->globalCfg[32];
    		break;
    	};
    }
}

/******************************************************************************/
/*                      Definition of exported functions                      */
/******************************************************************************/

void MENU_Init(menu_T *menu, const menu_cfg_T *(*menuCfg)[], const menu_dateTimeCfg_t *dateTimeCfg, const menu_alarmCfg_t * alarmCfg, const menu_pidCfg_t *pidCfg){

	uint16_t i,j;
	if(menu_checkConstraints(menuCfg, dateTimeCfg, pidCfg)){
		menu->globalCfg = menuCfg;
		menu->cfg = menu->globalCfg[0];
		memcpy(&menu->data.dateTimeCfg, dateTimeCfg, sizeof(menu_dateTimeCfg_t));
		memcpy(&menu->data.alarmCfg, alarmCfg, sizeof(menu_alarmCfg_t));
		memcpy(&menu->data.pidCfg, pidCfg, sizeof(menu_pidCfg_t));

		/* Initialize LCD parameters */
		for(i=0; i < MENU_MAX_ROWS; i++){
			menu->data.lines[i] = NULL;
			for(j=0; j<20; j++){
				menu->data.lineContent[i][j] = 0U;
			}
		}
		menu->data.dateTimeChangesAvailable = 0U;
		menu->data.pidChangesAvailable = 0U;
		menu->data.hour10 = MENU_BIN2BCD_L(menu->data.dateTimeCfg.hour);
		menu->data.hour = MENU_BIN2BCD_H(menu->data.dateTimeCfg.hour);
		menu->data.minutes10 = MENU_BIN2BCD_L(menu->data.dateTimeCfg.minutes);
		menu->data.minutes = MENU_BIN2BCD_H(menu->data.dateTimeCfg.minutes);
		menu->data.seconds10 = MENU_BIN2BCD_L(menu->data.dateTimeCfg.seconds);
		menu->data.seconds = MENU_BIN2BCD_H(menu->data.dateTimeCfg.seconds);
		menu->data.year = menu->data.dateTimeCfg.year;
		menu->data.monthDay = menu->data.dateTimeCfg.dayOfMonth;
		menu->data.weekDay = NULL;
		menu->data.month = NULL;
		menu->data.amPm = NULL;
		menu->data.kpEntire = menu->data.pidCfg.kpCfgEntire;
		menu->data.kpDecimal = menu->data.pidCfg.kpCfgDec;
		menu->data.kiEntire = menu->data.pidCfg.kiCfgEntire;
		menu->data.kiDecimal = menu->data.pidCfg.kiCfgDec;
		menu->data.kdEntire = menu->data.pidCfg.kdCfgEntire;
		menu->data.kdDecimal = menu->data.pidCfg.kdCfgDec;
		menu->data.SetPointEntire = menu->data.pidCfg.setPointCfgEntire;
		menu->data.SetPointDecimal = menu->data.pidCfg.setPointCfgDec;
		menu->data.duty = 0U; //Todo
		menu->data.menu_CurrentIndex = menu->cfg->menu_minOptions;
		menu->data.menu_PreviousIndexLevel1 = menu->globalCfg[3]->menu_minOptions;//MENU_MAIN
		menu->data.menu_PreviousIndexLevel2 = menu->globalCfg[4]->menu_minOptions;//MENU_PID_OPTIONS
		menu->data.menu_PreviousIndexLevel3 = menu->globalCfg[25]->menu_minOptions;// MENU_CLOCK_OPTIONS
		menu->data.menu_PreviousIndexLevel4 = menu->globalCfg[5]->menu_minOptions;// MENU_CLOCK_SETTINGS
		menu->data.menu_PreviousIndexLevel5 = menu->globalCfg[26]->menu_minOptions;// MENU_ALARM_SETTINGS
		menu->data.menu_PreviousIndexLevel6 = menu->globalCfg[15]->menu_minOptions;// MENU_PID_SETTINGS
	}
}

void MENU_encoderOption (menu_T *menu, int32_t current){
	if(((current-previous_val)>(menu->cfg->menu_maxOptions - menu->cfg->menu_minOptions)) &&
			((current-previous_val)> 0)){
		menu->data.menu_CurrentIndex = menu->cfg->menu_maxOptions;
	}else if(((current-previous_val) < (menu->cfg->menu_minOptions - menu->cfg->menu_maxOptions)) &&
			((current-previous_val)< 0)){
		menu->data.menu_CurrentIndex = menu->cfg->menu_minOptions;
	}else if(current > previous_val){
		if((menu->data.menu_CurrentIndex >= menu->cfg->menu_maxOptions)){
			menu->data.menu_CurrentIndex = menu->cfg->menu_maxOptions;
		} else {
			menu->data.menu_CurrentIndex++;
		}

		if((menu->cfg->status == MENU_CHANGE_HOUR) && (menu->data.dateTimeCfg.hourFormat == menu_12H_FORMAT) && (menu->data.menu_CurrentIndex > menu_12H)){
			menu->data.menu_CurrentIndex = menu_12H;
		}

		if((menu->cfg->status == MENU_CHANGE_ALARM_HOUR) && (menu->data.alarmCfg.hourFormatAlarm == menu_12H_FORMAT) && (menu->data.menu_CurrentIndex > menu_12H)){
			menu->data.menu_CurrentIndex = menu_12H;
		}

	} else if (current < previous_val){

		if((menu->data.menu_CurrentIndex <= menu->cfg->menu_minOptions)){
				menu->data.menu_CurrentIndex = menu->cfg->menu_minOptions;
		}else{
			menu->data.menu_CurrentIndex--;
		}

		if((menu->cfg->status == MENU_CHANGE_HOUR) && (menu->data.dateTimeCfg.hourFormat == menu_12H_FORMAT) && (menu->data.menu_CurrentIndex < menu_1H)){
			menu->data.menu_CurrentIndex = menu_1H;
		}

		if((menu->cfg->status == MENU_CHANGE_ALARM_HOUR) && (menu->data.alarmCfg.hourFormatAlarm == menu_12H_FORMAT) && (menu->data.menu_CurrentIndex < menu_1H)){
			menu->data.menu_CurrentIndex = menu_1H;
		}

	} else {
		/* Nothing to do */
	}
	previous_val = current;
}


void MENU_keyPressed(menu_T *menu){
	if(menu != NULL){
		// if the menu is not numeric
		switch (menu->cfg->freezeScreen){
		case MENU_SCREEN_NO_FREEZE:
			if((menu->data.menu_CurrentIndex <= menu->cfg->menu_maxOptions) &&
					(menu->cfg->menu_items[menu->data.menu_CurrentIndex].menuCallback != NULL) ){
				menu->cfg->menu_items[menu->data.menu_CurrentIndex].menuCallback(menu, NULL);
			}
			break;
		case MENU_SCREEN_FREEZE:
			menu->cfg->menu_items[0].menuCallback(menu, NULL);
			break;
		default:
			break;
		}
	}
	return;
}

void MENU_Task(menu_T * menu){
	uint8_t line = 0u;
	uint8_t reject_lines[MENU_MAX_ROWS] = {80, 80, 80, 80};
	uint8_t rejected_index = 0;

	if(!menu->cfg->freezeScreen){
		/*El numero 4 indica la posicion donde empieza el menu de verdad*/
		if((menu->cfg->MenuMaxLines)>(menu->data.menu_CurrentIndex - menu->cfg->menu_minOptions)){
			for(line = menu->cfg->MenuLinestart; line < (menu->cfg->MenuMaxLines + menu->cfg->MenuLinestart) ; line++){
				if(menu->data.menu_CurrentIndex == menu->cfg->menu_items[line+menu->cfg->menu_minOptions-menu->cfg->MenuLinestart].position){
					menu->data.lines[line] = menu->cfg->menu_items[line+menu->cfg->menu_minOptions-menu->cfg->MenuLinestart].msg_selected;
					reject_lines[rejected_index]=line;
					rejected_index++;
				} else {
					menu->data.lines[line] = menu->cfg->menu_items[line+menu->cfg->menu_minOptions-menu->cfg->MenuLinestart].msg_noSelected;
					reject_lines[rejected_index]=line;
					rejected_index++;
				}
			}
		} else if((menu->data.menu_CurrentIndex <= menu->cfg->menu_maxOptions) && (menu->data.menu_CurrentIndex > menu->cfg->menu_minOptions)){
			/* Avoid undesired options */
			menu->data.lines[menu->cfg->MenuMaxLines-1+menu->cfg->MenuLinestart] = menu->cfg->menu_items[menu->data.menu_CurrentIndex].msg_selected;
			reject_lines[rejected_index]= menu->cfg->MenuMaxLines-1+menu->cfg->MenuLinestart;
			rejected_index++;

			for(line = menu->cfg->MenuLinestart; line < menu->cfg->MenuMaxLines-1+ menu->cfg->MenuLinestart; line++){
				menu->data.lines[line] = menu->cfg->menu_items[menu->data.menu_CurrentIndex -(menu->cfg->MenuMaxLines-1)+(line-menu->cfg->MenuLinestart)].msg_noSelected;
				reject_lines[rejected_index]= line;
				rejected_index++;
			}
		} else {
			reject_lines[0] = 0;
			reject_lines[1] = 1;
			reject_lines[2] = 2;
			reject_lines[3] = 3;
		}
//		rellena con los titulos de las 4 primeras lineas las lineas vacias
		for(line=0; line < MENU_MAX_ROWS; line++){
			if(!menu_searchNum(line, reject_lines)){
				menu->data.lines[line] = menu->cfg->menu_items[line].msg_noSelected;
			}
		}
	}

	switch (menu->cfg->status){
	case MENU_DEFAULT_PID:
		menu_PrintDefaultPid(menu);
		break;
	case MENU_MAIN:
		menu_PrintMainMenu(menu);
		break;
	case MENU_DEFAULT_HOUR:
		menu_printDateTime(menu);
		break;
	case MENU_PID_SETTINGS:
		menu_PrintChangePid(menu);
		break;
	case MENU_CLOCK_SETTINGS:
		menu_PrintChangeDateTime(menu);
		break;
	case MENU_CHANGE_HOUR_FORMAT:
		menu_PrintChangeHourFormat(menu);
		break;
	case MENU_CHANGE_HOUR_AM_PM:
		menu_PrintChangeHourAmPm(menu);
		break;
	case MENU_CHANGE_HOUR:
		menu_PrintChangeHour(menu);
		break;
	case MENU_CHANGE_MINUTES:
		menu_PrintChangeMinutes(menu);
		break;
	case MENU_CHANGE_SECONDS:
		menu_PrintChangeSeconds(menu);
		break;
	case MENU_CHANGE_WEEKDAY:
		menu_PrintChangeWeekday(menu);
		break;
	case MENU_CHANGE_MONTHDAY:
		menu_PrintChangeMonthDay(menu);
		break;
	case MENU_CHANGE_MONTH:
		menu_PrintChangeMonth(menu);
		break;
	case MENU_CHANGE_YEAR:
		menu_PrintChangeYear(menu);
		break;
	case MENU_CHANGE_KP_ENTIRE:
		menu_PrintChangeKpEntire(menu);
		break;
	case MENU_CHANGE_KP_DECIMAL:
		menu_PrintChangeKpDecimal(menu);
		break;
	case MENU_CHANGE_KI_ENTIRE:
		menu_PrintChangeKiEntire(menu);
		break;
	case MENU_CHANGE_KI_DECIMAL:
		menu_PrintChangeKiDecimal(menu);
		break;
	case MENU_CHANGE_KD_ENTIRE:
		menu_PrintChangeKdEntire(menu);
		break;
	case MENU_CHANGE_KD_DECIMAL:
		menu_PrintChangeKdDecimal(menu);
		break;
	case MENU_CHANGE_SET_POINT_ENTIRE:
		menu_PrintChangeSetPointEntire(menu);
		break;
	case MENU_CHANGE_SET_POINT_DECIMAL:
		menu_PrintChangeSetPointDecimal(menu);
		break;
	case MENU_PID_OPTIONS:
		menu_PrintPidOptions(menu);
		break;
	case MENU_PID_VALUES:
		menu_PrintPidValues(menu);
		break;
	case MENU_CLOCK_OPTIONS:
		menu_PrintClockOptions(menu);
		break;
	case MENU_ALARM_SETTINGS:
		menu_PrintChangeAlarmSettings(menu);
		break;
	case MENU_CHANGE_ALARM_HOUR_FORMAT:
		menu_PrintChangeAlarmFormat(menu);
		break;
	case MENU_CHANGE_ALARM_HOUR_AM_PM:
		menu_PrintChangeAlarmHourAmPm(menu);
		break;
	case MENU_CHANGE_ALARM_HOUR:
		menu_PrintChangeAlarmHour(menu);
		break;
	case MENU_CHANGE_ALARM_MINUTES:
		menu_PrintChangeAlarmMinutes(menu);
		break;
	case MENU_CHANGE_ALARM_SECONDS:
		menu_PrintChangeAlarmSeconds(menu);
		break;
	case MENU_CHANGE_ALARM_STATUS:
		menu_PrintChangeAlarmStatus(menu);
		break;
	case MENU_PWM_SETTINGS:
		break;

	case MENU_EXIT:
	case MENU_DEFAULT_CFG:
		//Nothing to do
		break;
	}
	return;
}

uint8_t MENU_PidSelected( void* userData1, void* userData2){
	uint8_t ret_val = 0;
	menu_T *menu = (menu_T*)userData1;
	if((userData1 != NULL) ){
		/* Clean the LCD */
		menu_cleanLcd((menu_T*)userData1);
		/* When button is pressed apply the config for the menu_defaultPid */
		menu_applyCfg((menu_T*)userData1, MENU_DEFAULT_PID);
		menu->data.menu_PreviousIndexLevel1 = menu->data.menu_CurrentIndex;
		menu->data.menu_CurrentIndex = menu->cfg->menu_minOptions;
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}

uint8_t MENU_HourSelected( void* userData1, void* userData2){
	uint8_t ret_val = 0;
	menu_T *menu = (menu_T*)userData1;
	if((userData1 != NULL) ){
		/* Clean the LCD */
		menu_cleanLcd((menu_T*)userData1);
		/* When button is pressed apply the config for the menu_defaultHour*/
		menu_applyCfg((menu_T*)userData1, MENU_DEFAULT_HOUR);
		menu->data.menu_PreviousIndexLevel1 = menu->data.menu_CurrentIndex;
		menu->data.menu_CurrentIndex = menu->cfg->menu_minOptions;
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}

//entra en las opciones de pid
uint8_t MENU_enterPidOptions( void* userData1, void* userData2){
	uint8_t ret_val = 0u;
	menu_T *menu = (menu_T*)userData1;
	if(userData1 != NULL){
		menu_cleanLcd((menu_T*)userData1);
		/* When button is pressed apply the config for the menu 1*/
		menu_applyCfg((menu_T*)userData1, MENU_PID_OPTIONS);
		menu->data.menu_PreviousIndexLevel1 = menu->data.menu_CurrentIndex;
		menu->data.menu_CurrentIndex = menu->data.menu_PreviousIndexLevel2;
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}

//entra en las opciones del reloj
uint8_t MENU_enterClockOptions( void* userData1, void* userData2){
	uint8_t ret_val = 0u;
	menu_T *menu = (menu_T*)userData1;
	if(userData1 != NULL){
		menu_cleanLcd((menu_T*)userData1);
		/* When button is pressed apply the config for the menu 1*/
		menu_applyCfg((menu_T*)userData1, MENU_CLOCK_OPTIONS);
		menu->data.menu_PreviousIndexLevel1 = menu->data.menu_CurrentIndex;
		menu->data.menu_CurrentIndex = menu->data.menu_PreviousIndexLevel3;
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}

//retorna del menu de la alarma a las opciones del reloj
uint8_t MENU_exitAndNoSaveAlarm( void* userData1, void* userData2){
	uint8_t ret_val = 0u;
	menu_T *menu = (menu_T*)userData1;
	if(userData1 != NULL){
		menu_cleanLcd((menu_T*)userData1);
		/* When button is pressed apply the config for the menu 1*/
		menu_applyCfg((menu_T*)userData1, MENU_CLOCK_OPTIONS);
		menu->data.menu_CurrentIndex = menu->data.menu_PreviousIndexLevel3;
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}

//Vuelve al menu main despues de estar en pid
uint8_t MENU_defaultPidToMainSel(void* userData1, void* userData2){
	uint8_t ret_val = 0u;
	menu_T *menu = (menu_T*)userData1;
	if(userData1 != NULL){
		menu_cleanLcd((menu_T*)userData1);
		/* When button is pressed apply the config for the menu 1*/
		menu_applyCfg((menu_T*)userData1, MENU_MAIN);
		menu->data.menu_CurrentIndex = menu->data.menu_PreviousIndexLevel1;
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}

//Vuelve al menu main despues de estar en default hour
uint8_t MENU_defaultHourToMainSel(void* userData1, void* userData2){
	uint8_t ret_val = 0u;
	menu_T *menu = (menu_T*)userData1;
	if(userData1 != NULL){
		menu_cleanLcd((menu_T*)userData1);
		/* When button is pressed apply the config for the menu 1*/
		menu_applyCfg((menu_T*)userData1, MENU_MAIN);
		menu->data.menu_CurrentIndex = menu->data.menu_PreviousIndexLevel1;
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}

uint8_t MENU_pidOptionsExitToMain( void* userData1, void* userData2){
	uint8_t ret_val = 0u;
	menu_T *menu = (menu_T*)userData1;
	if(userData1 != NULL){
		menu_cleanLcd((menu_T*)userData1);
		/* When button is pressed apply the config for the menu 1*/
		menu_applyCfg((menu_T*)userData1, MENU_MAIN);
		menu->data.menu_CurrentIndex = menu->data.menu_PreviousIndexLevel1;
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}


uint8_t MENU_ChangeKpEntireSel( void* userData1, void* userData2){
	uint8_t ret_val = 0;
	menu_T *menu = (menu_T*)userData1;
	/* When button is pressed apply the config for the menu kp entire */
	if((userData1 != NULL) ){
		menu_applyCfg((menu_T*)userData1, MENU_CHANGE_KP_ENTIRE);
		menu->data.menu_PreviousIndexLevel6 = menu->data.menu_CurrentIndex;
		menu->data.menu_CurrentIndex = menu->data.kpEntire;
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}

uint8_t MENU_ChangeKpDecimalSel( void* userData1, void* userData2){
	uint8_t ret_val = 0;
	menu_T *menu = (menu_T*)userData1;
	/* When button is pressed apply the config for the menu_defaultPid*/
	if((userData1 != NULL) ){
		menu_applyCfg((menu_T*)userData1, MENU_CHANGE_KP_DECIMAL);
		menu->data.menu_PreviousIndexLevel6 = menu->data.menu_CurrentIndex;
		menu->data.menu_CurrentIndex = menu->data.kpDecimal;
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}
uint8_t MENU_ChangeKiEntireSel( void* userData1, void* userData2){
	uint8_t ret_val = 0;
	menu_T *menu = (menu_T*)userData1;
	/* When button is pressed apply the config for the menu_defaultPid*/
	if((userData1 != NULL) ){
		menu_applyCfg((menu_T*)userData1, MENU_CHANGE_KI_ENTIRE);
		menu->data.menu_PreviousIndexLevel6 = menu->data.menu_CurrentIndex;
		menu->data.menu_CurrentIndex = menu->data.kiEntire;
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}
uint8_t MENU_ChangeKiDecimalSel( void* userData1, void* userData2){
	uint8_t ret_val = 0;
	menu_T *menu = (menu_T*)userData1;
	/* When button is pressed apply the config for the menu_defaultPid*/
	if((userData1 != NULL) ){
		menu_applyCfg((menu_T*)userData1, MENU_CHANGE_KI_DECIMAL);
		menu->data.menu_PreviousIndexLevel6 = menu->data.menu_CurrentIndex;
		menu->data.menu_CurrentIndex = menu->data.kiDecimal;
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}

uint8_t MENU_ChangeKdEntireSel( void* userData1, void* userData2){
	uint8_t ret_val = 0;
	menu_T *menu = (menu_T*)userData1;
	if((userData1 != NULL) ){
		/* When button is pressed apply the config for the menu_defaultPid*/
		menu_applyCfg((menu_T*)userData1, MENU_CHANGE_KD_ENTIRE);
		menu->data.menu_PreviousIndexLevel6 = menu->data.menu_CurrentIndex;
		menu->data.menu_CurrentIndex = menu->data.kdEntire;
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}

uint8_t MENU_ChangeKdDecimalSel( void* userData1, void* userData2){
	uint8_t ret_val = 0;
	menu_T *menu = (menu_T*)userData1;
	if((userData1 != NULL) ){
		/* When button is pressed apply the config for the menu_defaultPid*/
		menu_applyCfg((menu_T*)userData1, MENU_CHANGE_KD_DECIMAL);
		menu->data.menu_PreviousIndexLevel6 = menu->data.menu_CurrentIndex;
		menu->data.menu_CurrentIndex = menu->data.kdDecimal;
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}

uint8_t MENU_ChangeSetPointEntireSel( void* userData1, void* userData2){
	uint8_t ret_val = 0;
	menu_T *menu = (menu_T*)userData1;
	if((userData1 != NULL) ){
		/* When button is pressed apply the config for the menu_defaultPid*/
		menu_applyCfg((menu_T*)userData1, MENU_CHANGE_SET_POINT_ENTIRE);
		menu->data.menu_PreviousIndexLevel6 = menu->data.menu_CurrentIndex;
		menu->data.menu_CurrentIndex = menu->data.SetPointEntire;
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}

uint8_t MENU_ChangeSetPointDecimalSel( void* userData1, void* userData2){
	uint8_t ret_val = 0;
	menu_T *menu = (menu_T*)userData1;
	if((userData1 != NULL) ){
		/* When button is pressed apply the config for the menu_defaultPid*/
		menu_applyCfg((menu_T*)userData1, MENU_CHANGE_SET_POINT_DECIMAL);
		menu->data.menu_PreviousIndexLevel6 = menu->data.menu_CurrentIndex;
		menu->data.menu_CurrentIndex = menu->data.SetPointDecimal;
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}

//enta en las opciones de cambio de hora y fecha
uint8_t MENU_enterChangeClockSelected( void* userData1, void* userData2){
	uint8_t ret_val = 0;
	menu_T *menu = (menu_T*)userData1;
	if((userData1 != NULL) ){
		/* Clean the LCD */
		menu_cleanLcd((menu_T*)userData1);
		/* When button is pressed apply the config for the menu_defaultHour*/
		menu_applyCfg((menu_T*)userData1, MENU_CLOCK_SETTINGS);
		menu->data.menu_PreviousIndexLevel3 = menu->data.menu_CurrentIndex;
		menu->data.menu_CurrentIndex = menu->cfg->menu_minOptions;
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}


//Sale de las opciones de cambio de hora y fecha
uint8_t MENU_exitChangeClockSelected( void* userData1, void* userData2){
	uint8_t ret_val = 0;
	menu_T *menu = (menu_T*)userData1;
	if((userData1 != NULL) ){
		/* Clean the LCD */
		menu_cleanLcd((menu_T*)userData1);
		/* When button is pressed apply the config for the menu_defaultHour*/
		menu_applyCfg((menu_T*)userData1, MENU_CLOCK_SETTINGS);
		menu->data.menu_CurrentIndex = menu->data.menu_PreviousIndexLevel4;
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}

uint8_t MENU_enterChangeAlarmSelected( void* userData1, void* userData2){
	uint8_t ret_val = 0;
	menu_T *menu = (menu_T*)userData1;
	if((userData1 != NULL) ){
		/* Clean the LCD */
		menu_cleanLcd((menu_T*)userData1);
		/* When button is pressed apply the config for the menu_defaultHour*/
		menu_applyCfg((menu_T*)userData1, MENU_ALARM_SETTINGS);
		menu->data.menu_PreviousIndexLevel3 = menu->data.menu_CurrentIndex;
		menu->data.menu_CurrentIndex = menu->cfg->menu_minOptions;
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}


uint8_t MENU_exitChangeAlarmSelected( void* userData1, void* userData2){
	uint8_t ret_val = 0;
	menu_T *menu = (menu_T*)userData1;
	if((userData1 != NULL) ){
		/* Clean the LCD */
		menu_cleanLcd((menu_T*)userData1);
		/* When button is pressed apply the config for the menu_defaultHour*/
		menu_applyCfg((menu_T*)userData1, MENU_ALARM_SETTINGS);
		menu->data.menu_CurrentIndex = menu->data.menu_PreviousIndexLevel5;
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}

uint8_t MENU_enterChangePidSelected( void* userData1, void* userData2){
	uint8_t ret_val = 0;
	menu_T *menu = (menu_T*)userData1;
	if((userData1 != NULL) ){
		/* Clean the LCD */
		menu_cleanLcd((menu_T*)userData1);
		/* When button is pressed apply the config for the menu_defaultHour*/
		menu_applyCfg((menu_T*)userData1, MENU_PID_SETTINGS);
		menu->data.menu_PreviousIndexLevel2 = menu->data.menu_CurrentIndex;
		menu->data.menu_CurrentIndex = menu->cfg->menu_minOptions;
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}


uint8_t MENU_exitChangePidSelected( void* userData1, void* userData2){
	uint8_t ret_val = 0;
	menu_T *menu = (menu_T*)userData1;
	if((userData1 != NULL) ){
		/* Clean the LCD */
		menu_cleanLcd((menu_T*)userData1);
		/* When button is pressed apply the config for the menu_defaultHour*/
		menu_applyCfg((menu_T*)userData1, MENU_PID_SETTINGS);
		menu->data.menu_CurrentIndex = menu->data.menu_PreviousIndexLevel6;
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}

uint8_t MENU_ChangePwmSelected( void* userData1, void* userData2){
	/* pWM settings */
	return 0;
}


uint8_t MENU_ChangeHourFormatSel( void* userData1, void* userData2){
	uint8_t ret_val = 0u;
	menu_T *menu = (menu_T*)userData1;
	if(userData1 != NULL){
		/* When button is pressed apply the config for the menu 1*/
		menu_applyCfg((menu_T*)userData1, MENU_CHANGE_HOUR_FORMAT);
		menu->data.menu_PreviousIndexLevel4 = menu->data.menu_CurrentIndex;
		menu->data.menu_CurrentIndex = menu->data.dateTimeCfg.hourFormat;
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}


uint8_t MENU_ChangeAmPmSel( void* userData1, void* userData2){
	uint8_t ret_val = 0u;
	menu_T *menu = (menu_T*)userData1;
	if(userData1 != NULL){
		/* When button is pressed apply the config for the menu 1*/
		menu_applyCfg((menu_T*)userData1, MENU_CHANGE_HOUR_AM_PM);
		menu->data.menu_PreviousIndexLevel4 = menu->data.menu_CurrentIndex;
		menu->data.menu_CurrentIndex = menu->data.dateTimeCfg.amPm;
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}

uint8_t MENU_ChangeHourSel( void* userData1, void* userData2){
	uint8_t ret_val = 0u;
	menu_T *menu = (menu_T*)userData1;
	if(userData1 != NULL){
		/* When button is pressed apply the config for the menu 1*/
		menu_applyCfg((menu_T*)userData1, MENU_CHANGE_HOUR);
		menu->data.menu_PreviousIndexLevel4 = menu->data.menu_CurrentIndex;
		menu->data.menu_CurrentIndex = menu->data.dateTimeCfg.hour;
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}

uint8_t MENU_ChangeMinSel( void* userData1, void* userData2){
	uint8_t ret_val = 0u;
	menu_T *menu = (menu_T*)userData1;
	if(userData1 != NULL){
		/* When button is pressed apply the config for the menu 1*/
		menu_applyCfg((menu_T*)userData1, MENU_CHANGE_MINUTES);
		menu->data.menu_PreviousIndexLevel4 = menu->data.menu_CurrentIndex;
		menu->data.menu_CurrentIndex = menu->data.dateTimeCfg.minutes;
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}

uint8_t MENU_ChangeSecSel( void* userData1, void* userData2){
	uint8_t ret_val = 0u;
	menu_T *menu = (menu_T*)userData1;
	if(userData1 != NULL){
		/* When button is pressed apply the config for the menu 1*/
		menu_applyCfg((menu_T*)userData1, MENU_CHANGE_SECONDS);
		menu->data.menu_PreviousIndexLevel4 = menu->data.menu_CurrentIndex;
		menu->data.menu_CurrentIndex = menu->data.dateTimeCfg.seconds;
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}

uint8_t MENU_ChangeWeekDaySel( void* userData1, void* userData2){
	uint8_t ret_val = 0u;
	menu_T *menu = (menu_T*)userData1;
	if(userData1 != NULL){
		/* When button is pressed apply the config for the menu 1*/
		menu_applyCfg((menu_T*)userData1, MENU_CHANGE_WEEKDAY);
		menu->data.menu_PreviousIndexLevel4 = menu->data.menu_CurrentIndex;
		menu->data.menu_CurrentIndex = menu->data.dateTimeCfg.dayOfWeek;
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}

uint8_t MENU_ChangeMonthDaySel( void* userData1, void* userData2){
	uint8_t ret_val = 0u;
	menu_T *menu = (menu_T*)userData1;
	if(userData1 != NULL){
		/* When button is pressed apply the config for the menu 1*/
		menu_applyCfg((menu_T*)userData1, MENU_CHANGE_MONTHDAY);
		menu->data.menu_PreviousIndexLevel4 = menu->data.menu_CurrentIndex;
		menu->data.menu_CurrentIndex = menu->data.dateTimeCfg.dayOfMonth;
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}

uint8_t MENU_ChangeMonthSel( void* userData1, void* userData2){
	uint8_t ret_val = 0u;
	menu_T *menu = (menu_T*)userData1;
	if(userData1 != NULL){
		/* When button is pressed apply the config for the menu 1*/
		menu_applyCfg((menu_T*)userData1, MENU_CHANGE_MONTH);
		menu->data.menu_PreviousIndexLevel4 = menu->data.menu_CurrentIndex;
		menu->data.menu_CurrentIndex = menu->data.dateTimeCfg.month;
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}

uint8_t MENU_ChangeYearSel( void* userData1, void* userData2){
	uint8_t ret_val = 0u;
	menu_T *menu = (menu_T*)userData1;
	if(userData1 != NULL){
		/* When button is pressed apply the config for the menu 1*/
		menu_applyCfg((menu_T*)userData1, MENU_CHANGE_YEAR);
		menu->data.menu_PreviousIndexLevel4 = menu->data.menu_CurrentIndex;
		menu->data.menu_CurrentIndex = menu->data.dateTimeCfg.year - menu_MILENIUM;
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}

uint8_t MENU_ChangeAlarmHourFormatSel( void* userData1, void* userData2){
	uint8_t ret_val = 0u;
	menu_T *menu = (menu_T*)userData1;
	if(userData1 != NULL){
		/* When button is pressed apply the config for the menu 1*/
		menu_applyCfg((menu_T*)userData1, MENU_CHANGE_ALARM_HOUR_FORMAT);
		menu->data.menu_PreviousIndexLevel5 = menu->data.menu_CurrentIndex;
		menu->data.menu_CurrentIndex = menu->data.alarmCfg.hourFormatAlarm;
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}

uint8_t MENU_ChangeAlarmAmPmSel( void* userData1, void* userData2){
	uint8_t ret_val = 0u;
	menu_T *menu = (menu_T*)userData1;
	if(userData1 != NULL){
		/* When button is pressed apply the config for the menu 1*/
		menu_applyCfg((menu_T*)userData1, MENU_CHANGE_ALARM_HOUR_AM_PM);
		menu->data.menu_PreviousIndexLevel5 = menu->data.menu_CurrentIndex;
		menu->data.menu_CurrentIndex = menu->data.alarmCfg.amPmAlarm;
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}

uint8_t MENU_ChangeAlarmHourSel( void* userData1, void* userData2){
	uint8_t ret_val = 0u;
	menu_T *menu = (menu_T*)userData1;
	if(userData1 != NULL){
		/* When button is pressed apply the config for the menu 1*/
		menu_applyCfg((menu_T*)userData1, MENU_CHANGE_ALARM_HOUR);
		menu->data.menu_PreviousIndexLevel5 = menu->data.menu_CurrentIndex;
		menu->data.menu_CurrentIndex = menu->data.alarmCfg.hourAlarm;
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}

uint8_t MENU_ChangeAlarmMinSel( void* userData1, void* userData2){
	uint8_t ret_val = 0u;
	menu_T *menu = (menu_T*)userData1;
	if(userData1 != NULL){
		/* When button is pressed apply the config for the menu 1*/
		menu_applyCfg((menu_T*)userData1, MENU_CHANGE_ALARM_MINUTES);
		menu->data.menu_PreviousIndexLevel5 = menu->data.menu_CurrentIndex;
		menu->data.menu_CurrentIndex = menu->data.alarmCfg.minutesAlarm;
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}

uint8_t MENU_ChangeAlarmSecSel( void* userData1, void* userData2){
	uint8_t ret_val = 0u;
	menu_T *menu = (menu_T*)userData1;
	if(userData1 != NULL){
		/* When button is pressed apply the config for the menu 1*/
		menu_applyCfg((menu_T*)userData1, MENU_CHANGE_ALARM_SECONDS);
		menu->data.menu_PreviousIndexLevel5 = menu->data.menu_CurrentIndex;
		menu->data.menu_CurrentIndex = menu->data.alarmCfg.secondsAlarm;
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}

uint8_t MENU_ChangeAlarmStatusSel( void* userData1, void* userData2){
	uint8_t ret_val = 0u;
	menu_T *menu = (menu_T*)userData1;
	if(userData1 != NULL){
		/* When button is pressed apply the config for the menu 1*/
		menu_applyCfg((menu_T*)userData1, MENU_CHANGE_ALARM_STATUS);
		menu->data.menu_PreviousIndexLevel5 = menu->data.menu_CurrentIndex;
		menu->data.menu_CurrentIndex = menu->data.alarmCfg.alarmStatus;
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}

//retorna del menu de la alarma guardada
uint8_t MENU_exitAndSaveAlarm( void* userData1, void* userData2){
	uint8_t ret_val = 0u;
	menu_T *menu = (menu_T*)userData1;
	if(userData1 != NULL){
		/* Clean the LCD */
		menu_cleanLcd((menu_T*)userData1);
		/* When button is pressed apply the config for the menu 1*/
		menu_applyCfg((menu_T*)userData1, MENU_CLOCK_OPTIONS);
		menu->data.menu_CurrentIndex = menu->data.menu_PreviousIndexLevel3;
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}

uint8_t MENU_ExitSelected( void* userData1, void* userData2){
	uint8_t ret_val = 0u;
	menu_T *menu = (menu_T*)userData1;
	if(userData1 != NULL){
		/* Clean the LCD */
		menu_cleanLcd((menu_T*)userData1);
		/* When button is pressed apply the default config for the menu 1*/
		menu_applyCfg((menu_T*)userData1, MENU_DEFAULT_CFG);
		menu->data.menu_PreviousIndexLevel1 = menu->data.menu_CurrentIndex;
		menu->data.menu_CurrentIndex = menu->cfg->menu_minOptions;
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}

// return to clock options
uint8_t MENU_exitAndSaveDateTime( void* userData1, void* userData2){
	uint8_t ret_val = 0u;
	menu_T *menu = (menu_T*)userData1;
	if(userData1 != NULL){
		/* When button is pressed apply the config for the menu 1*/
		menu_applyCfg((menu_T*)userData1, MENU_CLOCK_OPTIONS);
		menu_SetChangeDateTimeFlag((menu_T*)userData1, 1U);
		menu->data.menu_CurrentIndex = menu->data.menu_PreviousIndexLevel3;
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}

//return to PID options
uint8_t MENU_ExitAndSavePid( void* userData1, void* userData2){
	uint8_t ret_val = 0u;
	menu_T *menu = (menu_T*)userData1;
	if(userData1 != NULL){
		/* When button is pressed apply the config for the menu 1*/
		menu_applyCfg((menu_T*)userData1, MENU_PID_OPTIONS);
		menu_SetChangePidFlag((menu_T*)userData1, 1U);
		menu->data.menu_CurrentIndex = menu->data.menu_PreviousIndexLevel2;
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}

//return to PID options
uint8_t MENU_exitAndNoSavePid( void* userData1, void* userData2){
	uint8_t ret_val = 0u;
	menu_T *menu = (menu_T*)userData1;
	if(userData1 != NULL){
		/* When button is pressed apply the config for the menu 1*/
		menu_applyCfg((menu_T*)userData1, MENU_PID_OPTIONS);
		menu->data.menu_CurrentIndex = menu->data.menu_PreviousIndexLevel2;
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}

//return to clock options
uint8_t MENU_exitAndNoSaveDateTime( void* userData1, void* userData2){
	uint8_t ret_val = 0u;
	menu_T *menu = (menu_T*)userData1;
	if(userData1 != NULL){
		/* When button is pressed apply the config for the menu 1*/
		menu_applyCfg((menu_T*)userData1, MENU_CLOCK_OPTIONS);
		menu->data.menu_CurrentIndex = menu->data.menu_PreviousIndexLevel3;
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}

uint8_t MENU_showPidValues( void* userData1, void* userData2){
	uint8_t ret_val = 0u;
	menu_T *menu = (menu_T*)userData1;
	if(userData1 != NULL){
		/* Clean the LCD */
		menu_cleanLcd((menu_T*)userData1);
		/* When button is pressed apply the config for the menu 1*/
		menu_applyCfg((menu_T*)userData1, MENU_PID_VALUES);
		menu->data.menu_PreviousIndexLevel2 = menu->data.menu_CurrentIndex;
		menu->data.menu_CurrentIndex = menu->cfg->menu_minOptions;
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}

uint8_t MENU_checkAlarm(menu_T *menu){
	uint8_t retVal;
	if((menu->data.alarmCfg.alarmStatus == 1u) && (menu->data.alarmCfg.hourAlarm == MENU_BCD2BIN(menu->data.hour10,menu->data.hour)) &&
			(menu->data.alarmCfg.minutesAlarm == MENU_BCD2BIN(menu->data.minutes10, menu->data.minutes)) &&
			(menu->data.alarmCfg.secondsAlarm == MENU_BCD2BIN(menu->data.seconds10, menu->data.seconds))){
		retVal = 1;
	}else{
		retVal = 0;
	}
	return retVal;
}

uint8_t MENU_GetChangesDataTime(menu_T *menu){
	uint8_t ret_val = menu->data.dateTimeChangesAvailable;
	menu->data.dateTimeChangesAvailable = 0u;
	return ret_val;
}

uint8_t MENU_GetChangesPid(menu_T *menu){
	uint8_t ret_val = menu->data.pidChangesAvailable;
	menu->data.pidChangesAvailable = 0u;
	return ret_val;
}

menu_status_t MENU_GetStatus(menu_T *menu){
	return menu->cfg->status;
}

void MENU_SetSecondUnits(menu_T *menu, uint8_t Secondunits){
	if(menu != NULL){
		menu->data.seconds = Secondunits;
	}
}

void MENU_SetSecondDec(menu_T* menu, uint8_t SecondDec) {
	if (menu != NULL) {
		menu->data.seconds10 = SecondDec;
	}
}

void MENU_SetMinutesUnits(menu_T* menu, uint8_t minutesUnits) {
	if (menu != NULL) {
		menu->data.minutes = minutesUnits;
	}
}

void MENU_SetMinutesDec(menu_T *menu, uint8_t minutesDec){
	if(menu != NULL){
		menu->data.minutes10 = minutesDec;
	}
}

void MENU_SetHourUnits(menu_T *menu, uint8_t hourUnits){
	if(menu != NULL){
		menu->data.hour = hourUnits;
	}
}

void MENU_SetHourDec(menu_T *menu, uint8_t hourDec){
	if(menu != NULL){
		menu->data.hour10 = hourDec;
	}
}

void MENU_SetYear(menu_T *menu, uint16_t year){
	if(menu != NULL){
		menu->data.year = year;
	}
}

void MENU_SetMonthDay(menu_T *menu, uint8_t monthDay){
	if(menu != NULL){
		menu->data.monthDay = monthDay;
	}
}

void MENU_SetAmPm(menu_T *menu, const char* amPm){
	if((menu != NULL) && (amPm != NULL)){
		menu->data.amPm = amPm;
	}
}

void MENU_SetWeekday(menu_T *menu, const char* weekDay){
	if((menu != NULL) && (weekDay != NULL)){
		menu->data.weekDay = weekDay;
	}
}

void MENU_SetMonth(menu_T *menu, const char* month){
	if((menu != NULL) && (month != NULL)){
		menu->data.month = month;
	}
}

void MENU_SetDutyCycle(menu_T *menu, const uint16_t dutyCycle){
	if((menu != NULL)){
		menu->data.duty = dutyCycle;
	}	
}

void MENU_SetAdcValue(menu_T *menu, const uint16_t adcVal){
	if((menu != NULL)){
		menu->data.adc_val = adcVal;
	}
}

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
#include <menu.h>
#include "stm32f1xx_hal.h"
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
#define menu_12H				(12U)
#define menu_1H					(1U)
#define menu_24H_FORMAT			(0U)
#define menu_12H_FORMAT			(1U)
#define menu_24H_FORMAT			(0U)
#define menu_12H_FORMAT			(1U)
#define menu_AM_CFG				(0U)
#define menu_PM_CFG				(1U)
/******************************************************************************/
/*                  Definition of local function like macros                  */
/******************************************************************************/

/******************************************************************************/
/*          Definition of local types (typedef, enum, struct, union)          */
/******************************************************************************/

/******************************************************************************/
/*                       Definition of local variables                        */
/******************************************************************************/
static int32_t previous_val = 0;
/* Days of a week */
static char const *menu_days[menu_WEEK_DAYS_MAX]={MENU_UNKNOWN, MENU_SUNDAY, MENU_MONDAY, MENU_TUESDAY, MENU_WEDNESDAY, MENU_THURSDAY, MENU_FRIDAY, MENU_SATURDAY};
/* Months of a year */
static char const *menu_months[menu_MONTHS_MAX]={MENU_UNKNOWN, MENU_JAN, MENU_FEB, MENU_MAR, MENU_APR, MENU_MAY, MENU_JUN, MENU_JUL, MENU_AUG, MENU_SEP, MENU_OCT, MENU_NOV, MENU_DIC};
/* AM/PM */
static char const *menu_AMPM[menu_AMPM_MAX]={MENU_AM, MENU_PM};
/* Hour format 24H/12H */
static char const *menu_hourFormat[menu_HOUR_FORMAT_MAX]={MENU_24H, MENU_12H};

static char menu_Line0[20]={0};
static char menu_Line1[20]={0};
static char menu_Line2[20]={0};
static char menu_Line3[20]={0};

extern TIM_HandleTypeDef htim2;
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
static void menu_PrintDefaultPid(const menu_T *menu);
static void menu_printDateTime(const menu_T *menu);
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

static void menu_PrintChangeSetPointEntire(menu_T *menu);
static void menu_PrintChangeSetPointDecimal(menu_T *menu);

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
	for(i=0; i<5; i++){
		//(*cfg)[1]->MenuLinestart =1;
		if((*cfg)[i] != NULL){
			if(((((*cfg)[i]->MenuMaxLines == 4) && ((*cfg)[i]->MenuLinestart == 0)) ||
					(((*cfg)[i]->MenuMaxLines == 3) && (((*cfg)[i]->MenuLinestart == 0) || ((*cfg)[i]->MenuLinestart == 1))) ||
					(((*cfg)[i]->MenuMaxLines == 2) && (((*cfg)[i]->MenuLinestart == 0) || ((*cfg)[i]->MenuLinestart == 1) || ((*cfg)[i]->MenuLinestart == 2))) ||
					(((*cfg)[i]->MenuMaxLines == 1) && (((*cfg)[i]->MenuLinestart == 0) || ((*cfg)[i]->MenuLinestart == 1) || ((*cfg)[i]->MenuLinestart == 2) || ((*cfg)[i]->MenuLinestart == 3)))) &&
					((*cfg)[i]->menu_maxOptions > (*cfg)[i]->menu_minOptions) && ((*cfg)[i]->lcd != NULL)){
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
	LCD_clear(menu->cfg->lcd);
}

static void menu_PrintDefaultPid(const menu_T *menu){
	LCD_setCursor(menu->cfg->lcd, 0, 0);
	LCD_print(menu->cfg->lcd, "%s", menu->data.lines[0]);
	LCD_setCursor(menu->cfg->lcd, 0, 1);
	LCD_print(menu->cfg->lcd, "%s%d.%d", menu->data.lines[1], menu->data.SetPointEntire, menu->data.SetPointDecimal);
	LCD_setCursor(menu->cfg->lcd, 0, 2);
	LCD_print(menu->cfg->lcd, "%s%d", menu->data.lines[2], menu->data.adc_val);
	LCD_setCursor(menu->cfg->lcd, 0, 3);
	LCD_print(menu->cfg->lcd, "%s%d", menu->data.lines[3], menu->data.duty);
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
	if(menu->data.menu_index-4 < 8){
		snprintf(menu_Line2, 20, "%s %d.%d   ", menu_pidLine2[menu->data.menu_index-4], menu_PidValues[menu->data.menu_index-4][0],
				menu_PidValues[menu->data.menu_index-4][1]);
		menu->data.lines[2] = menu_Line2;
	}

	LCD_setCursor(menu->cfg->lcd, 0, 0);
	LCD_print(menu->cfg->lcd, "%s", menu->data.lines[0]);
	LCD_setCursor(menu->cfg->lcd, 0, 1);
	LCD_print(menu->cfg->lcd, "%s", menu->data.lines[1]);
	LCD_setCursor(menu->cfg->lcd, 0, 2);
	LCD_print(menu->cfg->lcd, "%s", menu->data.lines[2]);
	LCD_setCursor(menu->cfg->lcd, 0, 3);
	LCD_print(menu->cfg->lcd, "%s", menu->data.lines[3]);
}

static void menu_PrintPidOptions(menu_T *menu){
	LCD_setCursor(menu->cfg->lcd, 0, 0);
	LCD_print(menu->cfg->lcd, "%s", menu->data.lines[0]);
	LCD_setCursor(menu->cfg->lcd, 0, 1);
	LCD_print(menu->cfg->lcd, "%s", menu->data.lines[1]);
	LCD_setCursor(menu->cfg->lcd, 0, 2);
	LCD_print(menu->cfg->lcd, "%s", menu->data.lines[2]);
	LCD_setCursor(menu->cfg->lcd, 0, 3);
	LCD_print(menu->cfg->lcd, "%s", menu->data.lines[3]);
}

static void menu_PrintClockOptions(menu_T *menu){
	LCD_setCursor(menu->cfg->lcd, 0, 0);
	LCD_print(menu->cfg->lcd, "%s", menu->data.lines[0]);
	LCD_setCursor(menu->cfg->lcd, 0, 1);
	LCD_print(menu->cfg->lcd, "%s", menu->data.lines[1]);
	LCD_setCursor(menu->cfg->lcd, 0, 2);
	LCD_print(menu->cfg->lcd, "%s", menu->data.lines[2]);
	LCD_setCursor(menu->cfg->lcd, 0, 3);
	LCD_print(menu->cfg->lcd, "%s", menu->data.lines[3]);
}



static void menu_PrintPidValues (menu_T *menu){
	snprintf(menu_Line0, 20, "%s %d.%d   ", menu->data.lines[0], menu->data.kpEntire, menu->data.kpDecimal);
	snprintf(menu_Line1, 20, "%s %d.%d   ", menu->data.lines[1], menu->data.kiEntire, menu->data.kiDecimal);
	snprintf(menu_Line2, 20, "%s %d.%d   ", menu->data.lines[2], menu->data.kdEntire, menu->data.kdDecimal);
	snprintf(menu_Line3, 20, "%s %d.%d   ", menu->data.lines[3], menu->data.SetPointEntire, menu->data.SetPointDecimal);

	menu->data.lines[0] = menu_Line0;
	menu->data.lines[1] = menu_Line1;
	menu->data.lines[2] = menu_Line2;
	menu->data.lines[3] = menu_Line3;

	LCD_setCursor(menu->cfg->lcd, 0, 0);
	LCD_print(menu->cfg->lcd, "%s", menu->data.lines[0]);
	LCD_setCursor(menu->cfg->lcd, 0, 1);
	LCD_print(menu->cfg->lcd, "%s", menu->data.lines[1]);
	LCD_setCursor(menu->cfg->lcd, 0, 2);
	LCD_print(menu->cfg->lcd, "%s", menu->data.lines[2]);
	LCD_setCursor(menu->cfg->lcd, 0, 3);
	LCD_print(menu->cfg->lcd, "%s", menu->data.lines[3]);
}

static void menu_PrintMainMenu(const menu_T *menu){
	LCD_setCursor(menu->cfg->lcd, 0, 0);
	LCD_print(menu->cfg->lcd, "%s", menu->data.lines[0]);
	LCD_setCursor(menu->cfg->lcd, 0, 1);
	LCD_print(menu->cfg->lcd, "%s", menu->data.lines[1]);
	LCD_setCursor(menu->cfg->lcd, 0, 2);
	LCD_print(menu->cfg->lcd, "%s", menu->data.lines[2]);
	LCD_setCursor(menu->cfg->lcd, 0, 3);
	LCD_print(menu->cfg->lcd, "%s", menu->data.lines[3]);
}

static void menu_printDateTime(const menu_T *menu){
	BIGFONT_printChar(menu->cfg->lcd,':', 7, 0);

	BIGFONT_printNumber(menu->cfg->lcd, menu->data.hour10, 0, 0);
	BIGFONT_printNumber(menu->cfg->lcd, menu->data.hour, 4, 0);
	BIGFONT_printNumber(menu->cfg->lcd, menu->data.minutes10, 10, 0);
	BIGFONT_printNumber(menu->cfg->lcd, menu->data.minutes, 14, 0);

	LCD_setCursor(menu->cfg->lcd, 18, 0);
	LCD_print(menu->cfg->lcd,"%d", menu->data.seconds10);
	LCD_setCursor(menu->cfg->lcd, 19, 0);
	LCD_print(menu->cfg->lcd,"%d", menu->data.seconds);

	LCD_setCursor(menu->cfg->lcd, 0, 3);
	LCD_print(menu->cfg->lcd,"%s/%d/%s/%d", menu->data.weekDay, menu->data.monthDay, menu->data.month,
			menu->data.year);
}

static void menu_PrintChangeDateTime(menu_T *menu){
	LCD_setCursor(menu->cfg->lcd, 0, 0);
	LCD_print(menu->cfg->lcd, "%s", menu->data.lines[0]);
	LCD_setCursor(menu->cfg->lcd, 0, 1);
	LCD_print(menu->cfg->lcd, "%s", menu->data.lines[1]);
	LCD_setCursor(menu->cfg->lcd, 0, 2);
	snprintf(menu_Line2, 20, "%s/%s   %d:%d:%d   ", menu_hourFormat[menu->data.dateTimeCfg.hourFormat], menu_AMPM[menu->data.dateTimeCfg.amPm], menu->data.dateTimeCfg.hour, menu->data.dateTimeCfg.minutes, menu->data.dateTimeCfg.seconds);
	menu->data.lines[2] = menu_Line2;
	LCD_print(menu->cfg->lcd, "%s", menu->data.lines[2]);
	LCD_setCursor(menu->cfg->lcd, 0, 3);
	snprintf(menu_Line3, 20, "  %s/%d/%s/%d   ", menu_days[menu->data.dateTimeCfg.dayOfWeek], menu->data.dateTimeCfg.dayOfMonth, menu_months[menu->data.dateTimeCfg.month], menu->data.dateTimeCfg.year);
	menu->data.lines[3] = menu_Line3;
	LCD_print(menu->cfg->lcd, "%s", menu->data.lines[3]);
}

static void menu_PrintChangeHourFormat(menu_T *menu){

	static uint8_t hformatprevious;
	menu->data.dateTimeCfg.hourFormat = menu->data.menu_index;

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

	snprintf(menu_Line2, 20, "%s/%s   %d:%d:%d   ", menu_hourFormat[menu->data.dateTimeCfg.hourFormat], menu_AMPM[menu->data.dateTimeCfg.amPm], menu->data.dateTimeCfg.hour, menu->data.dateTimeCfg.minutes, menu->data.dateTimeCfg.seconds);
	menu->data.lines[2] = menu_Line2;
	LCD_setCursor(menu->cfg->lcd, 0, 2);
	LCD_print(menu->cfg->lcd, "%s", menu->data.lines[2]);
}

static void menu_PrintChangeHourAmPm(menu_T *menu){
	menu->data.dateTimeCfg.amPm = menu->data.menu_index;
	snprintf(menu_Line2, 20, "%s/%s   %d:%d:%d   ", menu_hourFormat[menu->data.dateTimeCfg.hourFormat], menu_AMPM[menu->data.dateTimeCfg.amPm], menu->data.dateTimeCfg.hour, menu->data.dateTimeCfg.minutes, menu->data.dateTimeCfg.seconds);
	menu->data.lines[2] = menu_Line2;
	LCD_setCursor(menu->cfg->lcd, 0, 2);
	LCD_print(menu->cfg->lcd, "%s", menu->data.lines[2]);
}

static void menu_PrintChangeHour(menu_T *menu){
	if((menu->data.dateTimeCfg.hourFormat == menu_24H_FORMAT) && ((menu->data.menu_index >= menu_12H))){
		menu->data.dateTimeCfg.amPm = menu_PM_CFG;
	}else{
		menu->data.dateTimeCfg.amPm = menu_AM_CFG;
	}
	menu->data.dateTimeCfg.hour = menu->data.menu_index;
	snprintf(menu_Line2, 20, "%s/%s   %d:%d:%d   ", menu_hourFormat[menu->data.dateTimeCfg.hourFormat], menu_AMPM[menu->data.dateTimeCfg.amPm], menu->data.dateTimeCfg.hour, menu->data.dateTimeCfg.minutes, menu->data.dateTimeCfg.seconds);

	menu->data.lines[2] = menu_Line2;
	LCD_setCursor(menu->cfg->lcd, 0, 2);
	LCD_print(menu->cfg->lcd, "%s", menu->data.lines[2]);
}

static void menu_PrintChangeMinutes(menu_T *menu){
	menu->data.dateTimeCfg.minutes = menu->data.menu_index;
	snprintf(menu_Line2, 20, "%s/%s   %d:%d:%d   ", menu_hourFormat[menu->data.dateTimeCfg.hourFormat], menu_AMPM[menu->data.dateTimeCfg.amPm], menu->data.dateTimeCfg.hour, menu->data.dateTimeCfg.minutes, menu->data.dateTimeCfg.seconds);
	menu->data.lines[2] = menu_Line2;
	LCD_setCursor(menu->cfg->lcd, 0, 2);
	LCD_print(menu->cfg->lcd, "%s", menu->data.lines[2]);
}

static void menu_PrintChangeSeconds(menu_T *menu){
	menu->data.dateTimeCfg.seconds = menu->data.menu_index;
	snprintf(menu_Line2, 20, "%s/%s   %d:%d:%d   ", menu_hourFormat[menu->data.dateTimeCfg.hourFormat], menu_AMPM[menu->data.dateTimeCfg.amPm], menu->data.dateTimeCfg.hour, menu->data.dateTimeCfg.minutes, menu->data.dateTimeCfg.seconds);
	menu->data.lines[2] = menu_Line2;
	LCD_setCursor(menu->cfg->lcd, 0, 2);
	LCD_print(menu->cfg->lcd, "%s", menu->data.lines[2]);
}

static void menu_PrintChangeWeekday(menu_T *menu){
	menu->data.dateTimeCfg.dayOfWeek = menu->data.menu_index;
	if(menu->data.menu_index < menu_WEEK_DAYS_MAX){
		snprintf(menu_Line3, 20, "  %s/%d/%s/%d   ", menu_days[menu->data.dateTimeCfg.dayOfWeek], menu->data.dateTimeCfg.dayOfMonth, menu_months[menu->data.dateTimeCfg.month], menu->data.dateTimeCfg.year);
		menu->data.lines[3] = menu_Line3;
		LCD_setCursor(menu->cfg->lcd, 0, 3);
		LCD_print(menu->cfg->lcd, "%s", menu->data.lines[3]);
	}
}

static void menu_PrintChangeMonthDay(menu_T *menu){
	menu->data.dateTimeCfg.dayOfMonth = menu->data.menu_index;
	snprintf(menu_Line3, 20, "  %s/%d/%s/%d   ", menu_days[menu->data.dateTimeCfg.dayOfWeek], menu->data.dateTimeCfg.dayOfMonth, menu_months[menu->data.dateTimeCfg.month], menu->data.dateTimeCfg.year);
	menu->data.lines[3] = menu_Line3;
	LCD_setCursor(menu->cfg->lcd, 0, 3);
	LCD_print(menu->cfg->lcd, "%s", menu->data.lines[3]);
}

static void menu_PrintChangeMonth(menu_T *menu){
	menu->data.dateTimeCfg.month = menu->data.menu_index;
	if(menu->data.menu_index < menu_MONTHS_MAX){
		snprintf(menu_Line3, 20, "  %s/%d/%s/%d   ", menu_days[menu->data.dateTimeCfg.dayOfWeek], menu->data.dateTimeCfg.dayOfMonth, menu_months[menu->data.dateTimeCfg.month], menu->data.dateTimeCfg.year);
		menu->data.lines[3] = menu_Line3;
		LCD_setCursor(menu->cfg->lcd, 0, 3);
		LCD_print(menu->cfg->lcd, "%s", menu->data.lines[3]);
	}
}

static void menu_PrintChangeYear(menu_T *menu){
	menu->data.dateTimeCfg.year  = menu->data.menu_index + 2000;
	snprintf(menu_Line3, 20, "  %s/%d/%s/%d   ", menu_days[menu->data.dateTimeCfg.dayOfWeek], menu->data.dateTimeCfg.dayOfMonth, menu_months[menu->data.dateTimeCfg.month], menu->data.dateTimeCfg.year);
	menu->data.lines[3] = menu_Line3;
	LCD_setCursor(menu->cfg->lcd, 0, 3);
	LCD_print(menu->cfg->lcd, "%s", menu->data.lines[3]);
}

static void menu_PrintChangeKpEntire(menu_T *menu){
	menu->data.kpEntire = menu->data.menu_index;
	snprintf(menu_Line2, 20, "%s %d.%d   ", "KP:", menu->data.kpEntire,
			menu->data.kpDecimal);
	menu->data.lines[2] = menu_Line2;
	LCD_setCursor(menu->cfg->lcd, 0, 2);
	LCD_print(menu->cfg->lcd, "%s", menu->data.lines[2]);
}

static void menu_PrintChangeKpDecimal(menu_T *menu){
	menu->data.kpDecimal = menu->data.menu_index;
	snprintf(menu_Line2, 20, "%s %d.%d   ", "KP:", menu->data.kpEntire,
			menu->data.kpDecimal);
	menu->data.lines[2] = menu_Line2;
	LCD_setCursor(menu->cfg->lcd, 0, 2);
	LCD_print(menu->cfg->lcd, "%s", menu->data.lines[2]);
}

static void menu_PrintChangeKiEntire(menu_T *menu){
	menu->data.kiEntire = menu->data.menu_index;
	snprintf(menu_Line2, 20, "%s %d.%d   ", "KI:", menu->data.kiEntire,
			menu->data.kiDecimal);
	menu->data.lines[2] = menu_Line2;
	LCD_setCursor(menu->cfg->lcd, 0, 2);
	LCD_print(menu->cfg->lcd, "%s", menu->data.lines[2]);
}

static void menu_PrintChangeKiDecimal(menu_T *menu){
	menu->data.kiDecimal = menu->data.menu_index;
	snprintf(menu_Line2, 20, "%s %d.%d   ", "KI:", menu->data.kiEntire,
			menu->data.kiDecimal);
	menu->data.lines[2] = menu_Line2;
	LCD_setCursor(menu->cfg->lcd, 0, 2);
	LCD_print(menu->cfg->lcd, "%s", menu->data.lines[2]);
}

static void menu_PrintChangeKdEntire(menu_T *menu){
	menu->data.kdEntire = menu->data.menu_index;
	snprintf(menu_Line2, 20, "%s %d.%d   ", "KD:", menu->data.kdEntire,
			menu->data.kdDecimal);
	menu->data.lines[2] = menu_Line2;
	LCD_setCursor(menu->cfg->lcd, 0, 2);
	LCD_print(menu->cfg->lcd, "%s", menu->data.lines[2]);
}

static void menu_PrintChangeKdDecimal(menu_T *menu){
	menu->data.kdDecimal = menu->data.menu_index;
	snprintf(menu_Line2, 20, "%s %d.%d   ", "KD:", menu->data.kdEntire,
			menu->data.kdDecimal);
	menu->data.lines[2] = menu_Line2;
	LCD_setCursor(menu->cfg->lcd, 0, 2);
	LCD_print(menu->cfg->lcd, "%s", menu->data.lines[2]);
}

static void menu_PrintChangeSetPointEntire(menu_T *menu){
	menu->data.SetPointEntire = menu->data.menu_index;
	snprintf(menu_Line2, 20, "%s %d.%d   ", "SP:", menu->data.SetPointEntire,
			menu->data.SetPointDecimal);
	menu->data.lines[2] = menu_Line2;
	LCD_setCursor(menu->cfg->lcd, 0, 2);
	LCD_print(menu->cfg->lcd, "%s", menu->data.lines[2]);
}

static void menu_PrintChangeSetPointDecimal(menu_T *menu){
	menu->data.SetPointDecimal = menu->data.menu_index;
	snprintf(menu_Line2, 20, "%s %d.%d   ", "SP:", menu->data.SetPointEntire,
			menu->data.SetPointDecimal);
	menu->data.lines[2] = menu_Line2;
	LCD_setCursor(menu->cfg->lcd, 0, 2);
	LCD_print(menu->cfg->lcd, "%s", menu->data.lines[2]);
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
    		menu->data.menu_index = menu->cfg->menu_minOptions;
    		break;
    	case MENU_DEFAULT_PID:
    		menu->cfg = menu->globalCfg[1];
    		menu->data.menu_index = menu->cfg->menu_minOptions;
    		break;
    	case MENU_DEFAULT_HOUR:
    		menu->cfg = menu->globalCfg[2];
    		menu->data.menu_index = menu->cfg->menu_minOptions;
    		break;
    	case MENU_MAIN:
    		menu->cfg = menu->globalCfg[3];
    		menu->data.menu_index = menu->cfg->menu_minOptions;
    		break;
    	case MENU_PID_SETTINGS:
    		menu->cfg = menu->globalCfg[4];
    		menu->data.menu_index = menu->cfg->menu_minOptions;
    		break;
    	case MENU_CLOCK_SETTINGS:
    		menu->cfg = menu->globalCfg[5];
    		menu->data.menu_index = menu->cfg->menu_minOptions;
    		break;
    	case MENU_PWM_SETTINGS:
    		//menu->cfg = menu->globalCfg[6];
    		menu->data.menu_index = menu->cfg->menu_minOptions;
    		break;

    	case MENU_EXIT:
    		break;
    	case MENU_CHANGE_HOUR_FORMAT:
    		menu->cfg = menu->globalCfg[6];
    		menu->data.menu_index = menu->data.dateTimeCfg.hourFormat;
    		break;
    	case MENU_CHANGE_HOUR_AM_PM:
    		menu->cfg = menu->globalCfg[7];
    		menu->data.menu_index = menu->data.dateTimeCfg.amPm;
    		break;
    	case MENU_CHANGE_HOUR:
    		menu->cfg = menu->globalCfg[8];
    		menu->data.menu_index = menu->data.dateTimeCfg.hour;
    		break;
    	case MENU_CHANGE_MINUTES:
    		menu->cfg = menu->globalCfg[9];
    		menu->data.menu_index = menu->data.dateTimeCfg.minutes;
    		break;
    	case MENU_CHANGE_SECONDS:
    		menu->cfg = menu->globalCfg[10];
    		menu->data.menu_index = menu->data.dateTimeCfg.seconds;
    		break;
    	case MENU_CHANGE_WEEKDAY:
    		menu->cfg = menu->globalCfg[11];
    		menu->data.menu_index = menu->data.dateTimeCfg.dayOfWeek;
    		break;
    	case MENU_CHANGE_MONTHDAY:
    		menu->cfg = menu->globalCfg[12];
    		menu->data.menu_index = menu->data.dateTimeCfg.dayOfMonth;
    		break;
    	case MENU_CHANGE_MONTH:
    		menu->cfg = menu->globalCfg[13];
    		menu->data.menu_index = menu->data.dateTimeCfg.month;
    		break;
    	case MENU_CHANGE_YEAR:
    		menu->cfg = menu->globalCfg[14];
    		menu->data.menu_index = menu->data.dateTimeCfg.year;
    		break;
    	case MENU_PID_OPTIONS:
    		menu->cfg = menu->globalCfg[15];
    		menu->data.menu_index = menu->cfg->menu_minOptions;
    		break;
    	case MENU_CHANGE_KP_ENTIRE:
    		menu->cfg = menu->globalCfg[16];
    		menu->data.menu_index = menu->data.kpEntire;
    		break;
    	case MENU_CHANGE_KP_DECIMAL:
    		menu->cfg = menu->globalCfg[17];
    		menu->data.menu_index = menu->data.kpDecimal;
    		break;
    	case MENU_CHANGE_KI_ENTIRE:
    		menu->cfg = menu->globalCfg[18];
    		menu->data.menu_index = menu->data.kiEntire;
    		break;
    	case MENU_CHANGE_KI_DECIMAL:
    		menu->cfg = menu->globalCfg[19];
    		menu->data.menu_index = menu->data.kiDecimal;
    		break;
    	case MENU_CHANGE_KD_ENTIRE:
    		menu->cfg = menu->globalCfg[20];
    		menu->data.menu_index = menu->data.kdEntire;
    		break;
    	case MENU_CHANGE_KD_DECIMAL:
    		menu->cfg = menu->globalCfg[21];
    		menu->data.menu_index = menu->data.kdDecimal;
    		break;
    	case MENU_CHANGE_SET_POINT_ENTIRE:
    		menu->cfg = menu->globalCfg[22];
    		menu->data.menu_index = menu->data.SetPointEntire;
    		break;
    	case MENU_CHANGE_SET_POINT_DECIMAL:
    		menu->cfg = menu->globalCfg[23];
    		menu->data.menu_index = menu->data.SetPointDecimal;
    		break;
    	case MENU_PID_VALUES:
    		menu->cfg = menu->globalCfg[24];
    		menu->data.menu_index = menu->cfg->menu_minOptions;
    		break;
    	case MENU_CLOCK_OPTIONS:
    		menu->cfg = menu->globalCfg[25];
    		menu->data.menu_index = menu->cfg->menu_minOptions;
    		break;
    	};
    }
}

/******************************************************************************/
/*                      Definition of exported functions                      */
/******************************************************************************/

void menu_Init(menu_T *menu, const menu_cfg_T *(*menuCfg)[], const menu_dateTimeCfg_t *dateTimeCfg, const menu_pidCfg_t *pidCfg){

	if(menu_checkConstraints(menuCfg, dateTimeCfg, pidCfg)){
		menu->globalCfg = menuCfg;
		menu->cfg = menu->globalCfg[0];
		memcpy(&menu->data.dateTimeCfg, dateTimeCfg, sizeof(menu_dateTimeCfg_t));
		memcpy(&menu->data.pidCfg, pidCfg, sizeof(menu_pidCfg_t));
		menu->data.dateTimeChangesAvailable = 0;
		menu->data.pidChangesAvailable = 0;
		menu->data.lines[0] = NULL;
		menu->data.lines[1] = NULL;
		menu->data.lines[2] = NULL;
		menu->data.lines[3] = NULL;
		menu->data.hour10 = 1;
		menu->data.hour = 2;
		menu->data.minutes10 = 4;
		menu->data.minutes = 5;
		menu->data.seconds10 = 3;
		menu->data.seconds = 2;
		menu->data.year = 2021;
		menu->data.monthDay = 21;
		menu->data.weekDay = NULL;
		menu->data.month = NULL;
		menu->data.amPm = NULL;
		menu->data.kpEntire = 50;
		menu->data.kpDecimal = 500;
		menu->data.kiEntire = 0;
		menu->data.kiDecimal = 2;
		menu->data.kdEntire = 10;
		menu->data.kdDecimal = 103;
		menu->data.SetPointEntire = 1000;
		menu->data.SetPointDecimal = 789;
		menu->data.duty = 0;
		menu->data.menu_index = menu->cfg->menu_minOptions;
	}
}

void menu_encoderOption (menu_T *menu, int32_t current){
	if(((current-previous_val)>(menu->cfg->menu_maxOptions - menu->cfg->menu_minOptions)) &&
			((current-previous_val)> 0)){
		menu->data.menu_index = menu->cfg->menu_maxOptions;
	}else if(((current-previous_val) < (menu->cfg->menu_minOptions - menu->cfg->menu_maxOptions)) &&
			((current-previous_val)< 0)){
		menu->data.menu_index = menu->cfg->menu_minOptions;
	}else if(current > previous_val){
		if((menu->data.menu_index >= menu->cfg->menu_maxOptions)){
			menu->data.menu_index = menu->cfg->menu_maxOptions;
		} else {
			menu->data.menu_index++;
		}

		if((menu->cfg->status == MENU_CHANGE_HOUR) && (menu->data.dateTimeCfg.hourFormat == menu_12H_FORMAT) && (menu->data.menu_index > menu_12H)){
			menu->data.menu_index = menu_12H;
		}

	} else if (current < previous_val){

		if((menu->data.menu_index <= menu->cfg->menu_minOptions)){
				menu->data.menu_index = menu->cfg->menu_minOptions;
		}else{
			menu->data.menu_index--;
		}

		if((menu->cfg->status == MENU_CHANGE_HOUR) && (menu->data.dateTimeCfg.hourFormat == menu_12H_FORMAT) && (menu->data.menu_index < menu_1H)){
			menu->data.menu_index = menu_1H;
		}

	} else {
		/* Nothing to do */
	}
	previous_val = current;
}


void menu_keyPressed(menu_T *menu){
	if(menu != NULL){
		if((menu->data.menu_index < MENU_MAX_OPTIONS) && (menu->cfg->menu_items[menu->data.menu_index].menuCallback != NULL)){
			menu->cfg->menu_items[menu->data.menu_index].menuCallback(menu, NULL);
		}else{
			menu->cfg->menu_items[10].menuCallback(menu, NULL);
		}
	}
	return;
}

void menu_Task(menu_T * menu){
	uint8_t line = 0u;
	uint8_t reject_lines[MENU_MAX_ROWS] = {80, 80, 80, 80};
	uint8_t rejected_index = 0;

	if(!menu->cfg->freezeScreen){
		/*El numero 4 indica la posicion donde empieza el menu de verdad*/
		if((menu->cfg->MenuMaxLines)>(menu->data.menu_index - 4)){
			for(line = menu->cfg->MenuLinestart; line < (menu->cfg->MenuMaxLines + menu->cfg->MenuLinestart) ; line++){
				if(menu->data.menu_index == menu->cfg->menu_items[line+4-menu->cfg->MenuLinestart].position){
					menu->data.lines[line] = menu->cfg->menu_items[line+4-menu->cfg->MenuLinestart].msg_selected;
					reject_lines[rejected_index]=line;
					rejected_index++;
				} else {
					menu->data.lines[line] = menu->cfg->menu_items[line+4-menu->cfg->MenuLinestart].msg_noSelected;
					reject_lines[rejected_index]=line;
					rejected_index++;
				}
			}
		} else if((menu->data.menu_index <= menu->cfg->menu_maxOptions) && (menu->data.menu_index > menu->cfg->menu_minOptions)){
			/* Avoid undesired options */
			menu->data.lines[menu->cfg->MenuMaxLines-1+menu->cfg->MenuLinestart] = menu->cfg->menu_items[menu->data.menu_index].msg_selected;
			reject_lines[rejected_index]= menu->cfg->MenuMaxLines-1+menu->cfg->MenuLinestart;
			rejected_index++;

			for(line = menu->cfg->MenuLinestart; line < menu->cfg->MenuMaxLines-1+ menu->cfg->MenuLinestart; line++){
				menu->data.lines[line] = menu->cfg->menu_items[menu->data.menu_index -(menu->cfg->MenuMaxLines-1)+(line-menu->cfg->MenuLinestart)].msg_noSelected;
				reject_lines[rejected_index]= line;
				rejected_index++;
			}
		} else {
			reject_lines[0] = 0;
			reject_lines[1] = 1;
			reject_lines[2] = 2;
			reject_lines[3] = 3;
		}
		//rellena con los titulos de las 4 primeras lineas las lineas vacias
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
	}
	return;
}


uint8_t menu_defaultScreen( void* userData1, void* userData2){
	uint8_t ret_val = 0;
	if((userData1 != NULL) ){
		/* When button is pressed apply the config for the main menu */
	menu_applyCfg((menu_T*)userData1, MENU_MAIN);
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}

uint8_t menu_PidSelected( void* userData1, void* userData2){
	uint8_t ret_val = 0;
	if((userData1 != NULL) ){
		/* Clean the LCD */
		menu_cleanLcd((menu_T*)userData1);
		/* When button is pressed apply the config for the menu_defaultPid */
		menu_applyCfg((menu_T*)userData1, MENU_DEFAULT_PID);
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}

uint8_t menu_ChangeKpEntireSel( void* userData1, void* userData2){
	uint8_t ret_val = 0;
	/* When button is pressed apply the config for the menu kp entire */
	if((userData1 != NULL) ){
		menu_applyCfg((menu_T*)userData1, MENU_CHANGE_KP_ENTIRE);
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}

uint8_t menu_ChangeKpDecimalSel( void* userData1, void* userData2){
	uint8_t ret_val = 0;
	/* When button is pressed apply the config for the menu_defaultPid*/
	if((userData1 != NULL) ){
		menu_applyCfg((menu_T*)userData1, MENU_CHANGE_KP_DECIMAL);
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}
uint8_t menu_ChangeKiEntireSel( void* userData1, void* userData2){
	uint8_t ret_val = 0;
	/* When button is pressed apply the config for the menu_defaultPid*/
	if((userData1 != NULL) ){
		menu_applyCfg((menu_T*)userData1, MENU_CHANGE_KI_ENTIRE);
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}
uint8_t menu_ChangeKiDecimalSel( void* userData1, void* userData2){
	uint8_t ret_val = 0;
	/* When button is pressed apply the config for the menu_defaultPid*/
	if((userData1 != NULL) ){
		menu_applyCfg((menu_T*)userData1, MENU_CHANGE_KI_DECIMAL);
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}

uint8_t menu_ChangeKdEntireSel( void* userData1, void* userData2){
	uint8_t ret_val = 0;
	if((userData1 != NULL) ){
		/* When button is pressed apply the config for the menu_defaultPid*/
		menu_applyCfg((menu_T*)userData1, MENU_CHANGE_KD_ENTIRE);
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}

uint8_t menu_ChangeKdDecimalSel( void* userData1, void* userData2){
	uint8_t ret_val = 0;
	if((userData1 != NULL) ){
		/* When button is pressed apply the config for the menu_defaultPid*/
		menu_applyCfg((menu_T*)userData1, MENU_CHANGE_KD_DECIMAL);
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}

uint8_t menu_ChangeSetPointEntireSel( void* userData1, void* userData2){
	uint8_t ret_val = 0;
	if((userData1 != NULL) ){
		/* When button is pressed apply the config for the menu_defaultPid*/
		menu_applyCfg((menu_T*)userData1, MENU_CHANGE_SET_POINT_ENTIRE);
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}

uint8_t menu_ChangeSetPointDecimalSel( void* userData1, void* userData2){
	uint8_t ret_val = 0;
	if((userData1 != NULL) ){
		/* When button is pressed apply the config for the menu_defaultPid*/
		menu_applyCfg((menu_T*)userData1, MENU_CHANGE_SET_POINT_DECIMAL);
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}

uint8_t menu_HourSelected( void* userData1, void* userData2){
	uint8_t ret_val = 0;

	if((userData1 != NULL) ){
		/* Clean the LCD */
		menu_cleanLcd((menu_T*)userData1);
		/* When button is pressed apply the config for the menu_defaultHour*/
		menu_applyCfg((menu_T*)userData1, MENU_DEFAULT_HOUR);
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}

uint8_t menu_ChangeClockSelected( void* userData1, void* userData2){
	uint8_t ret_val = 0;

	if((userData1 != NULL) ){
		/* Clean the LCD */
		menu_cleanLcd((menu_T*)userData1);
		/* When button is pressed apply the config for the menu_defaultHour*/
		menu_applyCfg((menu_T*)userData1, MENU_CLOCK_SETTINGS);
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}

uint8_t menu_ChangePidSelected( void* userData1, void* userData2){
	uint8_t ret_val = 0;
	if((userData1 != NULL) ){
		/* Clean the LCD */
		menu_cleanLcd((menu_T*)userData1);
		/* When button is pressed apply the config for the menu_defaultHour*/
		menu_applyCfg((menu_T*)userData1, MENU_PID_SETTINGS);
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}

uint8_t menu_ChangePwmSelected( void* userData1, void* userData2){
	/* pWM settings */
	return 0;
}

uint8_t menu_ExitSelected( void* userData1, void* userData2){
	uint8_t ret_val = 0u;

	if(userData1 != NULL){
		/* Clean the LCD */
		menu_cleanLcd((menu_T*)userData1);
		/* When button is pressed apply the default config for the menu 1*/
		menu_applyCfg((menu_T*)userData1, MENU_DEFAULT_CFG);
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}

uint8_t menu_defaultPidSel(void* userData1, void* userData2){
	uint8_t ret_val = 0u;
	if(userData1 != NULL){
		/* When button is pressed apply the config for the menu 1*/
		menu_applyCfg((menu_T*)userData1, MENU_MAIN);
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}

uint8_t menu_defaultHourSel(void* userData1, void* userData2){
	uint8_t ret_val = 0u;
	if(userData1 != NULL){
		/* When button is pressed apply the config for the menu 1*/
		menu_applyCfg((menu_T*)userData1, MENU_MAIN);
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}

uint8_t menu_ChangeHourFormatSel( void* userData1, void* userData2){

	uint8_t ret_val = 0u;
	if(userData1 != NULL){
		/* When button is pressed apply the config for the menu 1*/
		menu_applyCfg((menu_T*)userData1, MENU_CHANGE_HOUR_FORMAT);
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}

uint8_t menu_ChangeAmPmSel( void* userData1, void* userData2){
	uint8_t ret_val = 0u;
	if(userData1 != NULL){
		/* When button is pressed apply the config for the menu 1*/
		menu_applyCfg((menu_T*)userData1, MENU_CHANGE_HOUR_AM_PM);
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}

uint8_t menu_ChangeHourSel( void* userData1, void* userData2){
	uint8_t ret_val = 0u;
	if(userData1 != NULL){
		/* When button is pressed apply the config for the menu 1*/
		menu_applyCfg((menu_T*)userData1, MENU_CHANGE_HOUR);
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}

uint8_t menu_ChangeMinSel( void* userData1, void* userData2){
	uint8_t ret_val = 0u;
	if(userData1 != NULL){
		/* When button is pressed apply the config for the menu 1*/
		menu_applyCfg((menu_T*)userData1, MENU_CHANGE_MINUTES);
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}

uint8_t menu_ChangeSecSel( void* userData1, void* userData2){
	uint8_t ret_val = 0u;
	if(userData1 != NULL){
		/* When button is pressed apply the config for the menu 1*/
		menu_applyCfg((menu_T*)userData1, MENU_CHANGE_SECONDS);
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}

uint8_t menu_ChangeWeekDaySel( void* userData1, void* userData2){
	uint8_t ret_val = 0u;
	if(userData1 != NULL){
		/* When button is pressed apply the config for the menu 1*/
		menu_applyCfg((menu_T*)userData1, MENU_CHANGE_WEEKDAY);
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}

uint8_t menu_ChangeMonthDaySel( void* userData1, void* userData2){
	uint8_t ret_val = 0u;
	if(userData1 != NULL){
		/* When button is pressed apply the config for the menu 1*/
		menu_applyCfg((menu_T*)userData1, MENU_CHANGE_MONTHDAY);
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}

uint8_t menu_ChangeMonthSel( void* userData1, void* userData2){
	uint8_t ret_val = 0u;
	if(userData1 != NULL){
		/* When button is pressed apply the config for the menu 1*/
		menu_applyCfg((menu_T*)userData1, MENU_CHANGE_MONTH);
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}

uint8_t menu_ChangeYearSel( void* userData1, void* userData2){
	uint8_t ret_val = 0u;
	if(userData1 != NULL){
		/* When button is pressed apply the config for the menu 1*/
		menu_applyCfg((menu_T*)userData1, MENU_CHANGE_YEAR);
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}

uint8_t menu_exitAndSave( void* userData1, void* userData2){
	uint8_t ret_val = 0u;
	if(userData1 != NULL){
		/* When button is pressed apply the config for the menu 1*/
		menu_applyCfg((menu_T*)userData1, MENU_MAIN);
		menu_SetChangeDateTimeFlag((menu_T*)userData1, 1U);
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}

uint8_t menu_ExitAndSavePid( void* userData1, void* userData2){
	uint8_t ret_val = 0u;
	if(userData1 != NULL){
		/* When button is pressed apply the config for the menu 1*/
		menu_applyCfg((menu_T*)userData1, MENU_PID_OPTIONS);
		menu_SetChangePidFlag((menu_T*)userData1, 1U);
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}

uint8_t menu_exitAndNoSavePid( void* userData1, void* userData2){
	uint8_t ret_val = 0u;
	if(userData1 != NULL){
		/* When button is pressed apply the config for the menu 1*/
		menu_applyCfg((menu_T*)userData1, MENU_PID_OPTIONS);
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}

uint8_t menu_exitAndNoSave( void* userData1, void* userData2){
	uint8_t ret_val = 0u;
	if(userData1 != NULL){
		/* When button is pressed apply the config for the menu 1*/
		menu_applyCfg((menu_T*)userData1, MENU_MAIN);
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}

uint8_t menu_showPidOptions( void* userData1, void* userData2){
	uint8_t ret_val = 0u;
	if(userData1 != NULL){
		menu_cleanLcd((menu_T*)userData1);
		/* When button is pressed apply the config for the menu 1*/
		menu_applyCfg((menu_T*)userData1, MENU_PID_OPTIONS);
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}

uint8_t menu_showClockOptions( void* userData1, void* userData2){
	uint8_t ret_val = 0u;
	if(userData1 != NULL){
		menu_cleanLcd((menu_T*)userData1);
		/* When button is pressed apply the config for the menu 1*/
		menu_applyCfg((menu_T*)userData1, MENU_CLOCK_OPTIONS);
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}

uint8_t menu_showPidValues( void* userData1, void* userData2){
	uint8_t ret_val = 0u;
	if(userData1 != NULL){
		menu_cleanLcd((menu_T*)userData1);
		/* When button is pressed apply the config for the menu 1*/
		menu_applyCfg((menu_T*)userData1, MENU_PID_VALUES);
		ret_val = 1u;
	} else {
		ret_val = 0u;
	}
	return ret_val;
}


uint8_t menu_GetChangesDataTime(menu_T *menu){
	uint8_t ret_val = menu->data.dateTimeChangesAvailable;
	menu->data.dateTimeChangesAvailable = 0;
	return ret_val;
}

uint8_t menu_GetChangesPid(menu_T *menu){
	uint8_t ret_val = menu->data.pidChangesAvailable;
	menu->data.pidChangesAvailable = 0;
	return ret_val;
}


//static void menu_printDateTime(menu_T *menu){
//	BIGFONT_printChar(menu->cfg->lcd,':', 7, 0);
//
//	if(menu->data.hour < 10){
//		BIGFONT_printNumber(menu->cfg->lcd, 0, 0, 0);
//		BIGFONT_printNumber(menu->cfg->lcd, menu->data.hour, 4, 0);
//	} else {
//		BIGFONT_printNumber(menu->cfg->lcd, menu->data.hour, 0, 0);
//	}
//
//	if(menu->data.minutes < 10){
//		BIGFONT_printNumber(menu->cfg->lcd, 0, 10, 0);
//		BIGFONT_printNumber(menu->cfg->lcd, menu->data.minutes, 14, 0);
//	} else {
//		BIGFONT_printNumber(menu->cfg->lcd, menu->data.minutes, 10, 0);
//	}
//
//	if(menu->data.seconds < 10){
//		LCD_setCursor(menu->cfg->lcd, 18, 0);
//		LCD_print(menu->cfg->lcd,"%d%d", 0, menu->data.seconds);
//	} else {
//		LCD_setCursor(menu->cfg->lcd, 18, 0);
//		LCD_print(menu->cfg->lcd, "%d", menu->data.seconds);
//	}
//
//	LCD_setCursor(menu->cfg->lcd, 0, 3);
//	LCD_print(menu->cfg->lcd,"%s/%d/%s/%d", menu->data.weekDay, menu->data.monthDay, menu->data.month,
//			menu->data.year+2000);
//}








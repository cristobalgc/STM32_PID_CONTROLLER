/******************************************************************************/
/*                                                                            */
/*   All rights reserved. Distribution or duplication without previous        */
/*   written agreement of the owner prohibited.                               */
/*                                                                            */
/******************************************************************************/

/** \file sw_component.h
 *
 * \brief Header file for SW_COMPONENT component
 *
 * Header file for SW_COMPONENT manager
 *
 * <table border="0" cellspacing="0" cellpadding="0">
 * <tr> <td> Author:   </td> <td> C.Garcia   </td></tr>
 * <tr> <td> Date:     </td> <td> 04/09/2018             </td></tr>
 * </table>
 * \n
 * <table border="0" cellspacing="0" cellpadding="0">
 * <tr> <td> COMPONENT: </td> <td> SW_COMPONENT   </td></tr>
 * <tr> <td> SCOPE:     </td> <td> Public      </td></tr>
 * <tr> <td> TARGET:    </td> <td> MCU        </td></tr>
 * </table>
 * \note
 *
 * \see
 */

#ifndef PID_H_
#define PID_H_

/******************************************************************************/
/*                         Project Includes                                   */
/******************************************************************************/

/******************************************************************************/
/*                 Definition of exported symbolic constants                  */
/******************************************************************************/
#define PID_ON (1U)
#define PID_OFF (0U)

#define PID_MAX_DUTYCYCLE_VAL (100u)
#define PID_MIN_DUTYCYCLE_VAL (0u)
/******************************************************************************/
/*                Definition of exported function like macros                 */
/******************************************************************************/

/******************************************************************************/
/*         Definition of exported types (typedef, enum, struct, union)        */
/******************************************************************************/
typedef struct pid_cfg_s
{
	uint16_t kpEntireCfg; 		/**< Kp parameter entire part */
	uint16_t kpDecCfg;			/**< Kp parameter decimal part */
	uint16_t kiEntireCfg;		/**< Ki parameter entire part */
	uint16_t kiDecCfg;			/**< Ki parameter decimal part */
	uint16_t kdEntireCfg;		/**< Kd parameter entire part */
	uint16_t kdDecCfg;			/**< Kd parameter decimal part */
	uint32_t ADresolution;		/**< Vref(uV)/adcRes: (2^10)10b or (2^12)12b */
	uint16_t samplingTimeCfg;	/**< Sampling time in milliseconds */
	uint16_t dutycycleInitVal;
	uint16_t setPointInitVal; 	/**< Set point in mV */
}pid_cfg_t;

typedef struct pid_data_s
{
	double kp;				/**< Kp parameter */
	double Ki;
	double kd;
	double h2; 				/**<The semi-period of sampling time. Sampling time divided by 2*/
	double samplingTime;	/**< Sampling time in milliseconds */
	double setPoint; 		/**< Set point in mV */
	uint16_t adcVal;		/**< The adc value that has been read in mV after counts to mV conversion */
	uint16_t dutycycle;
	uint8_t OnOffFlag;		/**< Flag to power on the PID */
}pid_data_t;

typedef struct pid_s
{
	pid_cfg_t config;
	pid_data_t data;
}pidc_t;

/******************************************************************************/
/*                    Declaration of exported variables                       */
/******************************************************************************/

/******************************************************************************/
/*                  Declaration of exported constant data                     */
/******************************************************************************/

/******************************************************************************/
/*               Declaration of exported function prototypes                  */
/******************************************************************************/

extern void PID_init(pidc_t *pid, const pid_cfg_t *cfg);
extern void PID_SetParameters(pidc_t *pid, uint16_t kpEntire, uint16_t kpDec,
		uint16_t kiEntire, uint16_t kiDec, uint16_t kdEntire, uint16_t KdDec);
extern void PID_SetSetPointVal(pidc_t *pid, uint16_t setPointEntire, uint16_t SetPointDec);
extern void PID_control(pidc_t *pid, uint32_t measuredAdc);
extern uint16_t PID_GetDutyValule(const pidc_t *pid);
extern uint16_t PID_GetAdcValule(const pidc_t *pid);

#endif /* PID_H_ */

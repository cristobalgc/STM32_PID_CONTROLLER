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

#define PID_MAX_DUTYCYCLE_VAL (500u)
#define PID_MIN_DUTYCYCLE_VAL (0u)
/******************************************************************************/
/*                Definition of exported function like macros                 */
/******************************************************************************/

/******************************************************************************/
/*         Definition of exported types (typedef, enum, struct, union)        */
/******************************************************************************/
typedef struct pid_cfg_s
{
	int32_t kp;
	int32_t Ki;
	int32_t kd;
	uint32_t ADresolution;
	uint32_t samplingTime;
	uint32_t dutycycleInitVal;
	uint32_t setPointInitVal;
}pid_cfg_t;

typedef struct pid_data_s
{
	uint32_t samplingTime2;
	uint32_t dutycycle;
	uint32_t setPoint;
	uint8_t OnOffFlag;
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
extern void PID_SetParameters(pidc_t *pid, float kp, float ki, float kd);
extern void PID_SetSetPointVal(pidc_t *pid, uint32_t setPoint);
extern void PID_control(pidc_t *pid, uint32_t measuredAdc);

#endif /* PID_H_ */

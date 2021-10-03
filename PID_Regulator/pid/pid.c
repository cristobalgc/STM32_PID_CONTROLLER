/******************************************************************************/
/*                                                                            */
/*   All rights reserved. Distribution or duplication without previous        */
/*   written agreement of the owner prohibited.                               */
/*                                                                            */
/******************************************************************************/

/** \file pid.c
 *
 * \brief Source code file for PID
 *
 * Source code file for PID manager
 *
 * <table border="0" cellspacing="0" cellpadding="0">
 * <tr> <td> Author:   </td> <td> C.Garcia   </td></tr>
 * <tr> <td> Date:     </td> <td> 04/09/2018             </td></tr>
 * </table>
 * \n
 * <table border="0" cellspacing="0" cellpadding="0">
 * <tr> <td> COMPONENT: </td> <td> PID  </td></tr>
 * <tr> <td> TARGET:    </td> <td> MCU        </td></tr>
 * </table>
 * \note
 *
 * \see
 */

/******************************************************************************/
/*                Include common and project definition header                */
/******************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
/******************************************************************************/
/*                      Include headers of the component                      */
/******************************************************************************/
#include "../pid/pid.h"
/******************************************************************************/
/*                            Include other headers                           */
/******************************************************************************/

/******************************************************************************/
/*                   Definition of local symbolic constants                   */
/******************************************************************************/
#define pid_DECIMAL		(10U)
#define pid_CENTIMAL	(100U)
#define pid_MILESIMAL	(1000U)
#define pid_10MILESIMAL	(10000U)
/******************************************************************************/
/*                  Definition of local function like macros                  */
/******************************************************************************/

/******************************************************************************/
/*          Definition of local types (typedef, enum, struct, union)          */
/******************************************************************************/

/******************************************************************************/
/*                       Definition of local variables                        */
/******************************************************************************/

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

/******************************************************************************/
/*                       Definition of local functions                        */
/******************************************************************************/

/******************************************************************************/
/*                      Definition of exported functions                      */
/******************************************************************************/
/*
 * @brief Initialize Pid parameters
 *
 * @param[in] pidc_t *pid - The pid object.
 * @param[in] pid_cfg_t *cfg - The configutation parameters
 * */
void PID_init(pidc_t *pid, const pid_cfg_t *cfg)
{
	/* Copy the cfg parameters */
	memcpy(&pid->config, cfg, sizeof(pid_cfg_t));
	/* Apply the configuration parameters into the data */
	PID_SetParameters(pid, pid->config.kpEntireCfg, pid->config.kpDecCfg,
			pid->config.kiEntireCfg, pid->config.kiDecCfg,
			pid->config.kdEntireCfg, pid->config.kdDecCfg);
	PID_SetSetPointVal(pid, pid->config.setPointInitVal, 0);

	pid->data.samplingTime = ((double)pid->config.samplingTimeCfg / 1000);//0.002
	pid->data.h2 = pid->data.samplingTime / 2u;
	pid->data.dutycycle = pid->config.dutycycleInitVal;
	pid->data.adcVal = 0U;
	pid->data.OnOffFlag = PID_ON;
}

/*
 * @brief Initialize Pid parameters
 *
 * @param[in] pidc_t *pid - The pid object.
 * */
void PID_DeInit(pidc_t *pid)
{
	pid->data.OnOffFlag = PID_OFF;
}

/*
 * @brief To change PID parameters on runtime
 *
 * @param[in] pidc_t *pid - The Pid object.
 * @param[in] uint32_t kp - Proportional parameter.
 * @param[in] uint32_t ki - Integral parameter.
 * @param[in] uint32_t kd - Derivative parameter.
 * */
void PID_SetParameters(pidc_t *pid, uint16_t kpEntire, uint16_t kpDec,
		uint16_t kiEntire, uint16_t kiDec, uint16_t kdEntire, uint16_t KdDec){

	pid->data.kp = (double)kpEntire +((double)kpDec/pid_MILESIMAL);
	pid->data.Ki = (double)kiEntire +((double)kiDec/pid_MILESIMAL);
	pid->data.kd = (double)kdEntire +((double)KdDec/pid_MILESIMAL);
}

/*
 * @brief Change set pont parameter on runtime
 *
 * @param[in] pidc_t *pid - The Pid object.
 * @param[in] uint32_t setPoint - Proportional parameter.
 * */
void PID_SetSetPointVal(pidc_t *pid, uint16_t setPointEntire, uint16_t SetPointDec){
	/*Update Set Point values*/
	pid->data.setPoint = (double)setPointEntire +((double)SetPointDec/pid_MILESIMAL);
}

/*
 * @brief: This task must be executed after each adc measurement
 * or at least at double of ADC sampling frequency.
 *
 * @param[in]: pidc_t *pid - The pid object.
 * @param[in]: uint32_t setPoint - Set point in millivolts.
 * @param[in]: uint32_t measuredAdc - Measured ADC value (in ADC counts).
 *  */
void PID_control(pidc_t *pid, uint32_t measuredAdc)
{
	double	E0 = 0 , IE = 0 ;
	double	Er,Ur,U1,U2,U3, YR=0, Y1=0, DE;

	if(pid->data.OnOffFlag)
	{
		YR = pid->data.setPoint/pid_MILESIMAL;
		Y1 = (double)(measuredAdc * pid->config.ADresolution)/1000000;//in V
		pid->data.adcVal = (measuredAdc * pid->config.ADresolution)/pid_MILESIMAL;// in mV

		Er = YR - Y1;
		DE = (Er - E0)/pid->data.samplingTime;

		if (pid->data.dutycycle != PID_MAX_DUTYCYCLE_VAL) //Duty cycle
		{
			IE = IE + (pid->data.h2 * (Er + E0));
		}

		E0 = Er;
		U1 = pid->data.kp * Er ;
		U2 = IE * pid->data.kp/pid->data.Ki;
		U3 = pid->data.kp * pid->data.kd * DE;
		Ur = U1 + U2 + U3 ;

		if (Ur >= 12){
			pid->data.dutycycle = PID_MAX_DUTYCYCLE_VAL; // Max value of duty cycle
		}else{
			if ( Ur <= 0 ){
				pid->data.dutycycle = PID_MIN_DUTYCYCLE_VAL; // Min value of duty cycle
			}else{
				Ur = (Ur * PID_MAX_DUTYCYCLE_VAL)/12 ;
				pid->data.dutycycle = Ur;
			}
		}
	}
}
/*
 * @brief: Get the applied PID value.
 * @param[in]: pidc_t *pid - The pid object.
 * */
uint16_t PID_GetDutyValule(const pidc_t *pid){
	return pid->data.dutycycle;
}

/*
 * @brief: Get the converted ADC value in mV.
 * @param[in]: pidc_t *pid - The pid object.
 * */
uint16_t PID_GetAdcValule(const pidc_t *pid){
	return pid->data.adcVal;
}


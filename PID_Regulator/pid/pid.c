/*
 * pid.c
 *
 *  Created on: 4 nov. 2019
 *      Author: CGC
 */

#include "../pid/pid.h"

/*
 * @brief Initialize Pid parameters
 *
 * @param[in] pidc_t *pid - The pid object.
 * @param[in] pid_cfg_t *cfg - The configutation parameters
 * */
void PID_init(pidc_t *pid, const pid_cfg_t *cfg)
{
	memcpy(&pid->config, cfg, sizeof(pid_cfg_t));
	//cfg->ADresolution; // in volts
	// cfg->samplingTime; // in microseconds
	pid->data.setPoint = pid->config.setPointInitVal;
	pid->data.samplingTime2 = pid->config.samplingTime / 2u;//The semi-period of sampling time.
	pid->data.dutycycle = pid->config.dutycycleInitVal;
	pid->data.OnOffFlag = PID_ON;
}

/*
 * @brief To change PID parameters on runtime
 *
 * @param[in] pidc_t *pid - The Pid object.
 * @param[in] uint32_t kp - Proportional parameter.
 * @param[in] uint32_t ki - Integral parameter.
 * @param[in] uint32_t kd - Derivative parameter.
 * */
void PID_SetParameters(pidc_t *pid, float kp, float ki, float kd)
{
	pid->config.Ki = ki;
	pid->config.kp = kp;
	pid->config.kd = kd;
}

/*
 * @brief Change set pont parameter on runtime
 *
 * @param[in] pidc_t *pid - The Pid object.
 * @param[in] uint32_t setPoint - Proportional parameter.
 * */
void PID_SetSetPointVal(pidc_t *pid, uint32_t setPoint)
{
	pid->data.setPoint = setPoint;
}


/*
 * @brief: This task must be executed after each adc measurement
 * or at least at double of ADC sampling frequency.
 *
 * @param[in]: pidc_t *pid - The pid object.
 * @param[in]: uint32_t setPoint - Set point in milivolts.
 * @param[in]: uint32_t measuredAdc - Measured ADC value (in ADC counts).
 *  */
void PID_control(pidc_t *pid, uint32_t measuredAdc)
{
	float	E0 = 0 , IE = 0 ;
	float	Er,Ur,U1,U2,U3, YR=0, Y1=0, DE;

	while(pid->data.OnOffFlag)
	{
		YR = pid->data.setPoint;//convert_AD(5) * pid->data.ADresolution;	// entrada analógica potenciómetro
		//Y1 = convert_AD(9) * pid->data.ADresolution;  // entrada analógica tacodinamo   //AN7 Vtaco AN9 Vgen AN10 Iload CORRIENTE POR LA CARGA= 9 => REGULA EN TENSION.
		Y1 = (measuredAdc * pid->config.ADresolution)/1000;

		Er = YR - Y1;
		DE = (Er - E0)/pid->config.samplingTime;

		if (pid->data.dutycycle != 125) //
		{
			IE = IE + (pid->data.samplingTime2 * (Er + E0)); // h/2 esn el periodo de muestreo dividido por 2
		}

		E0 = Er;
		U1 = pid->config.kp * Er ;
		U2 = IE * pid->config.kp/pid->config.Ki;
		U3 = pid->config.kp * pid->config.kd * DE;
		Ur = U1 + U2 + U3 ;

		if (Ur >= 12000 )
		{
			pid->data.dutycycle = PID_MAX_DUTYCYCLE_VAL; // Max value of duty cycle
		}
		else
		{
			if ( Ur <= 0 )
			{
				pid->data.dutycycle = PID_MIN_DUTYCYCLE_VAL; // Min value of duty cycle
			}
			else
			{
				Ur = (Ur * 500)/12 ;
				pid->data.dutycycle = Ur;
			}
		}
	}
}


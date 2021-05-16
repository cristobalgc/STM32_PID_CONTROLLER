/*
 * pid.h
 *
 *  Created on: 4 nov. 2019
 *      Author: CGC
 */

#ifndef PID_H_
#define PID_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define PID_ON (1U)
#define PID_OFF (0U)

#define PID_MAX_DUTYCYCLE_VAL (500u)
#define PID_MIN_DUTYCYCLE_VAL (0u)

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

extern void PID_init(pidc_t *pid, const pid_cfg_t *cfg);
extern void PID_SetParameters(pidc_t *pid, float kp, float ki, float kd);
extern void PID_SetSetPointVal(pidc_t *pid, uint32_t setPoint);
extern void PID_control(pidc_t *pid, uint32_t measuredAdc);

#endif /* PID_H_ */

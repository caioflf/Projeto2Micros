/*
 * pseudoMain.h
 *
 *  Created on: Feb 3, 2024
 *      Author: caiof
 */
#include "variaveisPseudoMain.h"

#ifndef SRC_PSEUDOMAIN_H_
#define SRC_PSEUDOMAIN_H_

void cortinaAutomatica(flag *flag, char perfil, ADC_HandleTypeDef *hadc1);

void homicros(char *perfil, flag *flag, RTC_HandleTypeDef *hrtc, RTC_TimeTypeDef *sTime, RTC_DateTypeDef *DateToUpdate,
		unsigned char *segundo_ant, ADC_HandleTypeDef *hadc1,TIM_HandleTypeDef *htim3, ADC_HandleTypeDef *hadc2);


void login (flag *flag, RTC_HandleTypeDef *hrtc, RTC_TimeTypeDef *sTime, RTC_DateTypeDef *DateToUpdate,
			unsigned char *segundo_ant, ADC_HandleTypeDef *hadc1, TIM_HandleTypeDef *htim3, ADC_HandleTypeDef *hadc2);

int pseudoMain(RTC_HandleTypeDef *hrtc, RTC_TimeTypeDef *sTime, RTC_DateTypeDef *DateToUpdate, ADC_HandleTypeDef *hadc1,
		TIM_HandleTypeDef *htim3, ADC_HandleTypeDef *hadc2);

#endif /* SRC_PSEUDOMAIN_H_ */

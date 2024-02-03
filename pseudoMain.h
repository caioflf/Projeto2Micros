/*
 * pseudoMain.h
 *
 *  Created on: Feb 3, 2024
 *      Author: caiof
 */
#include "variaveisPseudoMain.h"

#ifndef SRC_PSEUDOMAIN_H_
#define SRC_PSEUDOMAIN_H_


void homicros(char *perfil, flag *flag);

void login (flag *flag);

int pseudoMain(RTC_HandleTypeDef *hrtc, RTC_TimeTypeDef *sTime, RTC_DateTypeDef *DateToUpdate);

#endif /* SRC_PSEUDOMAIN_H_ */

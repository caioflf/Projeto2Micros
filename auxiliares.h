/*=================================================================================================*/
//	UNIVERSIDADE FEDERAL DO RIO GRANDE DO SUL
//	ENG04475 - Microprocessadores I (2023/2)
//	Projeto 2 - HOMICROS
//
// Alunos:
// Bruno Gevehr Fernandes (00335299)
// Caio Fernando Leite França (00330098)
// Thiago Arndt Schimit (00333710)
/*=================================================================================================*/

#ifndef SRC_AUXILIARES_H_
#define SRC_AUXILIARES_H_

void converteASCII (unsigned short valor, char *stringConvertida);

void imprimeASCII (unsigned short valor);

char compara_string(char* a, char* b);

void desligaSistema (flag *flag);

void ligaSistema(flag *flag);

void imprimeZero(unsigned short valor);

void telaRepouso(RTC_HandleTypeDef *hrtc, RTC_TimeTypeDef *sTime, RTC_DateTypeDef *DateToUpdate,
				unsigned char *segundo_ant, ADC_HandleTypeDef *hadc1);

void inicia();

#endif


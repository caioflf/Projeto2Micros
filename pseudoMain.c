
#include "main.h"
#include "variaveisPseudoMain.h"
#include "controleLCD.h"
#include "teclado.h"
#include "auxiliares.h"


//loop principal
void homicros(char *perfil, flag *flag, RTC_HandleTypeDef *hrtc, RTC_TimeTypeDef *sTime, RTC_DateTypeDef *DateToUpdate,
		unsigned char *segundo_ant, ADC_HandleTypeDef *hadc1, TIM_HandleTypeDef *htim3, ADC_HandleTypeDef *hadc2){
	char letra, verificacao, i = 1;
	indice indice;
	indice.menu = 0;
	indice.info = 0;
	indice.ultimoMenu = 0;
	indice.ultimoInfo = 0;
	if (*perfil == 0 || *perfil == 'd'){
		return;
	}
	while(1){
		letra = scan(i);
		verificacao = verifica_logoff();
		if (verificacao == '#'){
			limpa_lcd();
			escreve_lcd("Logoff Realizado");
			HAL_Delay(2000);
			*perfil = 0;
			flag->atualizarTela = 1;
			return;
		}
		if (verificacao == '*'){
			desligaSistema(flag);
			*perfil = 'd';
			return;
		}

		navegacaoMenu(flag, &indice, letra, *perfil);
		menu(flag, &indice, hrtc, sTime, DateToUpdate, segundo_ant, hadc1, letra, htim3, hadc2, *perfil);

		indice.ultimoMenu = indice.menu;
		indice.ultimoInfo = indice.info;

		i++;
		if (i > 4) 	i = 1;
	}

}

void login (flag *flag, RTC_HandleTypeDef *hrtc, RTC_TimeTypeDef *sTime, RTC_DateTypeDef *DateToUpdate,
			unsigned char *segundo_ant, ADC_HandleTypeDef *hadc1, TIM_HandleTypeDef *htim3, ADC_HandleTypeDef *hadc2){
	char perfil = 0;
	lcdSolicitaSenha();
	HAL_Delay(1000);
	while(1){
		perfil = ler_senha();

		if (perfil == 0){
					limpa_lcd();
					escreve_lcd("Senha Invalida!");
					HAL_Delay(2000);				// atraso em que o usuario espera pra poder digitar novamente a senha
		}
		if (perfil == 1){
			limpa_lcd();
			if (flag->ativaPerfil1){
				escreve_lcd("Perfil 1 Logado!");
			} else if (!flag->ativaPerfil1){
				escreve_lcd("Perfil 1 Bloq.!");
				perfil = 0;
			}
			HAL_Delay(2000);

		}
		if (perfil == 2){
			limpa_lcd();
			if (flag->ativaPerfil2){
				escreve_lcd("Perfil 2 Logado!");
			} else if (!flag->ativaPerfil2){
				escreve_lcd("Perfil 2 Bloq.!");
				perfil = 0;
			}
			HAL_Delay(2000);

		}
		if (perfil == ADM){
			limpa_lcd();
			escreve_lcd("Perfil ADM Logado!");
			HAL_Delay(2000);

		}


		homicros(&perfil, flag, hrtc, sTime, DateToUpdate, segundo_ant, hadc1, htim3, hadc2); // loop principal

		if (perfil == 'd'){				//comando de desligar o sistema
			desligaSistema(flag);
			return;
		}

	}
}


int pseudoMain(RTC_HandleTypeDef *hrtc, RTC_TimeTypeDef *sTime, RTC_DateTypeDef *DateToUpdate, ADC_HandleTypeDef *hadc1,
		TIM_HandleTypeDef *htim3, ADC_HandleTypeDef *hadc2){

  DateToUpdate->Date= 22;
  DateToUpdate->Month = 10;
  DateToUpdate->Year = 24;
  HAL_RTC_SetDate(hrtc, DateToUpdate, RTC_FORMAT_BIN);

  sTime->Hours = 9;
  sTime->Minutes = 15;
  HAL_RTC_SetTime(hrtc, sTime, RTC_FORMAT_BIN);

  HAL_ADC_Start(hadc1);

  unsigned char verificacao, segundo_ant = 0;
  flag flag;
  flag.sistema = 0;
  flag.ativaPerfil1 = 1;
  flag.ativaPerfil2 = 1;
  flag.atualizarTela = 1;
  inicia(htim3, hadc1);


  while (1) {

	telaRepouso(hrtc, sTime, DateToUpdate, &segundo_ant, hadc1);
  	verificacao = verifica_logoff();
  	if (verificacao == '*'){
  		desligaSistema(&flag);
  	}
  	if (verificacao == '#' && flag.sistema == 0){
  		ligaSistema(&flag);
  		login(&flag, hrtc, sTime, DateToUpdate, &segundo_ant, hadc1, htim3, hadc2);
  	}
  }
}


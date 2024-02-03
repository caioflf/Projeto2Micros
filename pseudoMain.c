
#include "main.h"
#include "variaveisPseudoMain.h"
#include "controleLCD.h"
#include "teclado.h"
#include "auxiliares.h"

//loop principal
void homicros(char *perfil, flag *flag){
	char letra, verificacao, i = 1;
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
			return;
		}
		if (verificacao == '*'){
			desligaSistema(flag);
			*perfil = 'd';
			return;
		}




		i++;
		if (i > 4) 	i = 1;
	}

}

void login (flag *flag){
	char perfil = 0;
	while(1){
		perfil = ler_senha();

		if (perfil == 1){
			limpa_lcd();
			escreve_lcd("Perfil 1 Logado!");
			HAL_Delay(2000);

		}
		if (perfil == 2){
			limpa_lcd();
			escreve_lcd("Perfil 2 Logado!");
			HAL_Delay(2000);

		}
		if (perfil == 3){
			limpa_lcd();
			escreve_lcd("Perfil ADM Logado!");
			HAL_Delay(2000);

		}
		if (perfil == 0){
			limpa_lcd();
			escreve_lcd("Senha Invalida!");
			HAL_Delay(2000);				// atraso em que o usuario espera pra poder digitar novamente a senha
		}

		homicros(&perfil, flag); // loop principal

		if (perfil == 'd'){				//comando de desligar o sistema
			desligaSistema(flag);
			return;
		}

	}
}


int pseudoMain(RTC_HandleTypeDef *hrtc, RTC_TimeTypeDef *sTime, RTC_DateTypeDef *DateToUpdate, ADC_HandleTypeDef *hadc1){



  DateToUpdate->Date= 22;
  DateToUpdate->Month = 10;
  DateToUpdate->Year = 24;
  HAL_RTC_SetDate(hrtc, DateToUpdate, RTC_FORMAT_BIN);

  sTime->Hours = 9;
  sTime->Minutes = 15;
  HAL_RTC_SetTime(hrtc, sTime, RTC_FORMAT_BIN);

  uint16_t valorLidoTemp;
  short tCelsius;
  HAL_ADC_Start(hadc1);

  unsigned char verificacao, segundo_ant = 0;
  flag flag;
  flag.sistema = 0;

  inicia();


  while (1) {

	telaRepouso(hrtc, sTime, DateToUpdate, &segundo_ant, hadc1);
  	verificacao = verifica_logoff();
  	if (verificacao == '*'){
  		desligaSistema(&flag);
  	}
  	if (verificacao == '#' && flag.sistema == 0){
  		ligaSistema(&flag);
  		login(&flag);
  	}
  }
}


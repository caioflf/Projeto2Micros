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

#include "main.h"
#include "variaveisPseudoMain.h"
#include "pseudoMain.h"
#include "controleLCD.h"
#include "teclado.h"
#include "auxiliares.h"


/*====================================  Funçoes Auxiliares =======================================*/

extern RTC_HandleTypeDef hrtc;
extern RTC_TimeTypeDef sTime;
extern RTC_DateTypeDef DateToUpdate;

void converteASCII (unsigned short valor, char *stringConvertida){
	char i = 0, b = 0;
	if (valor > 65534){
		stringConvertida[0] = '*';
		stringConvertida[1] = '\0';
		return;
	}
	if (valor == 0){
		stringConvertida[0] = '0';
		stringConvertida[1] = '\0';
		return;
	}

	unsigned short divisor = 10;
	unsigned short auxiliar = 0;
	// associa cada caractere a seu respectivo valor em ASCII
	for (i = 0; i < 5; i++){
		auxiliar = valor%(divisor);
		valor -= auxiliar;
		valor/=10;
		auxiliar += 48;
		stringConvertida[i]=auxiliar;
		auxiliar = 0;
	}
	while (stringConvertida[i-1] == '0'){
		stringConvertida[i-1] = '\0';
		i--;
	}
	i--;

	char inicio = 0;
	char fim = i;
	// Reordena a string
	while (inicio < fim){
		char letraAux = stringConvertida[inicio];
		stringConvertida[inicio] = stringConvertida[fim];
		stringConvertida[fim] = letraAux;
		inicio++;
		fim--;
	}
	if (stringConvertida[0]=='\0')
	stringConvertida[0] = '0';

	stringConvertida[5] = '\0';
	return;
}

void imprimeASCII (unsigned short valor){
	char string[6];
	converteASCII(valor, string);
	escreve_lcd(string);
}

char compara_string(char* a, char* b){ // 0 se igual, 1 se diferente
	char flag = 0;
	while (*a != '\0' || *b != '\0') {
		if (*a == *b) {
			a++;
			b++;
		}
		else if ((*a == '\0' && *b != '\0') || (*a != '\0' && *b == '\0') || *a != *b){// se forem diferentes
			flag = 1;
			return 1;
		}
	}
	if (flag == 0) { // se forem iguais
		return 0;
	}
	return 0;
}


void desligaSistema (flag *flag) {
	if (flag->sistema == 1){
		flag->sistema =0;
	}
}

// Funcao para ligar o sistema quando solicitado
void ligaSistema(flag *flag) {
	if (flag->sistema == 0){
		flag->sistema = 1;
		HAL_Delay(1000);		// atraso de 1seg pra nao ser lido o que estiver sendo pressionado logo apos iniciar
	}
}

void imprimeZero(unsigned short valor){
	if(valor<10)
	letra_lcd('0');
}

void telaRepouso(RTC_HandleTypeDef *hrtc, RTC_TimeTypeDef *sTime, RTC_DateTypeDef *DateToUpdate,
				unsigned char *segundo_ant, ADC_HandleTypeDef *hadc1){
	int valorLidoTemperatura, temperaturaCelsius;
	float aux;

	//adquirindo tempo e data
	HAL_RTC_GetTime(hrtc, sTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(hrtc, DateToUpdate, RTC_FORMAT_BIN);

	if (sTime->Seconds == *segundo_ant)
		return;
	*segundo_ant = sTime->Seconds;

	// adquirindo temperatura
	HAL_ADC_PollForConversion(hadc1,1000);
	valorLidoTemperatura = HAL_ADC_GetValue(hadc1);
	/*aux = 332.558 - 0.187364 * valorLidoTemperatura; // (V25 - 3.3)/4096 * AVG_SLOPE
	temperaturaCelsius = aux;*/
	aux = ((((float)valorLidoTemperatura * 3.3 / 4096) - 0.76)/0.0025 + 25)/10;
	temperaturaCelsius = (int)aux;

	limpa_lcd();
	escreve_lcd(" ");
	imprimeZero(sTime->Hours);
	imprimeASCII(sTime->Hours);
	escreve_lcd(":");

	imprimeZero(sTime->Minutes);
	imprimeASCII(sTime->Minutes);
	escreve_lcd(":");

	imprimeZero(sTime->Seconds);
	imprimeASCII(sTime->Seconds);
	escreve_lcd(" ");

	imprimeZero(DateToUpdate->Date);
	imprimeASCII(DateToUpdate->Date);
	escreve_lcd("/");

	imprimeZero(DateToUpdate->Month);
	imprimeASCII(DateToUpdate->Month);
	comando_lcd(0xC0);

	escreve_lcd("      ");
	imprimeZero(temperaturaCelsius);
	imprimeASCII(temperaturaCelsius);
	letra_lcd(223);
	escreve_lcd("C");

}

void inicia(){
	inicia_lcd_4bits();

	  //setando as linhas do teclado como 1
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 1);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, 1);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, 1);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, 1);
}

char ler_senha(){
	char perfil = 0; //perfil 0 (senha invalida), perfil 1 , perfil 2, perfil 3 (ADM)
	char senha[10]={'\0'}, i=0, j=0, letra = '\0';
	lcdSolicitaSenha();
	while(letra!='#'&&(j<10)){
		for(i=1;i<=4;i++){
			letra=scan(i);
			if (letra == 'd')
			return letra;			//se scan retornar valor atribuido ao comando de desligamento, interrompe a funcao aqui

			if('\0'!=letra && '*' != letra && '#' != letra){
				letra_lcd('*');
				senha[j] = letra;
				j++;
			}
			else if (letra == '*'){
				j = 0;
				while (senha[j+1]!='\0'){
					senha[j] = '\0';
					j++;
				}
				j=0;
				lcdSolicitaSenha();
			}
		}
	}
	if (!compara_string(senha, SENHA1)){
		perfil = 1;
		return perfil;
	}
	if (!compara_string(senha, SENHA2)){
		perfil = 2;
		return perfil;
	}
	if (!compara_string(senha, SENHAADM)){
		perfil = ADM;
		return perfil;
	}
	else {
		perfil = 0;
		return perfil;
	}
	return perfil;
}

// Menu interativo para o usuario
void navegacaoMenu(flag *flag, indice *indice, char letra, char perfil){

	if (letra == '2'){			// ir para "cima"
		if (!(indice->menu))			// nao faz nada
		return;

		if (indice->menu == 1){			// ja estava mostrando o cliente no topo, agora mostra a direção a ser seguida
			indice->menu -= 1;
			indice->info = 0;
			return;
		}

		indice->menu -= 1;
		indice->info = 0;
		return;
	}

	if (letra == '5'){			// ir para "baixo"
		if ((indice->menu == INDICE_MAX_USUARIO) && (perfil != ADM))
		return;

		if (indice->menu == INDICE_MAX_ADM)
		return;

		indice->menu += 1;
		indice->info = 0;
		return;
	}

	if (letra == '4'){			// ir para "esquerda"
		if (!(indice->info))
		return;

		indice->info-=1;
		return;
	}

	if (letra == '6'){		// ir para "direita"
		if (indice->menu == 3){
			 if(indice->info == 1)
				 return;

			 indice->info += 1;
			return;
		}

		if (indice->menu == 4){
				if(perfil != ADM)
				return;

				if(indice->info == 2)
				return;

				indice->info += 1;
				return;
		}

		if ((indice->menu == 5)||(indice->menu == 6)){
				if(indice->info == 1)
				return;

				indice->info += 1;
				return;
		}
		return;
	}

	return;
}

void menu(flag *flag, indice *indice, RTC_HandleTypeDef *hrtc, RTC_TimeTypeDef *sTime, RTC_DateTypeDef *DateToUpdate,
		unsigned char *segundo_ant, ADC_HandleTypeDef *hadc1, char letra){
	if (flag->atualizarTela)
		flag->atualizarTela = 0;
	else if ((indice->menu == indice->ultimoMenu || indice->info == indice->ultimoInfo) && letra == '\0')
		return;


	switch (indice->menu){
		case 0: telaRepouso(hrtc, sTime, DateToUpdate, segundo_ant, hadc1);
				break;

		case 1: limpa_lcd();  					// luz 1
				escreve_lcd("Luz1: ");
				if (flag->luz1){				// avisa o status da luz
					escreve_lcd("ON");
					comando_lcd(0xC0);
					escreve_lcd("Desligar: (1)");
					if (letra == '1'){			// desliga
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, 0);
						flag->luz1 = 0;
						flag->atualizarTela = 1;
					}
				} else
				if (!flag->luz1){				// avisa o status da luz
					escreve_lcd("OFF");
					comando_lcd(0xC0);
					escreve_lcd("Ligar: (3)");
					if (letra == '3'){			// liga
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, 1);
						flag->luz1 = 1;
						flag->atualizarTela = 1;
					}
				}
				break;

		case 2: limpa_lcd(); 					 // luz 2
				escreve_lcd("Luz2: ");
				if (flag->luz2){				// avisa o status da luz
					escreve_lcd("ON");
					comando_lcd(0xC0);
					escreve_lcd("Desligar: (1)");
					if (letra == '1'){			// desliga
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, 0);
						flag->luz2 = 0;
						flag->atualizarTela = 1;
					}
				} else
				if (!flag->luz2){				// avisa o status da luz
						escreve_lcd("OFF");
						comando_lcd(0xC0);
						escreve_lcd("Ligar: (3)");
						if (letra == '3'){		// liga
							HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, 1);
							flag->luz2 = 1;
							flag->atualizarTela = 1;
						}
				}
				break;

		case 3: //luz quarto PWM
				break;

		case 4: // cortina
				break;

		case 5: // desabilita usuario
				break;

		case 6: // muda data ou hora
				break;

		default: break;
	}

}





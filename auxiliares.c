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
#include "controleLCD.h"
#include "auxiliares.h"


/*====================================  Funçoes Auxiliares =======================================*/

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
		desliga_lcd_4bits();
		flag->sistema =0;
	}
}

// Funcao para ligar o sistema quando solicitado
void ligaSistema(flag *flag) {
	if (flag->sistema == 0){
		inicia_lcd_4bits();
		escreve_lcd("Insira a senha:");
		comando_lcd(0xC0); // nova linha
		flag->sistema = 1;
		HAL_Delay(1000);		// atraso de 1seg pra nao ser lido o que estiver sendo pressionado logo apos iniciar
	}
}

void inicia(){
	inicia_lcd_4bits();
	limpa_lcd();

	  //setando as linhas do teclado como 1
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 1);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, 1);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, 1);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, 1);
}

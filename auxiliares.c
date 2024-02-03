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
#include "teclado.h"
#include "auxiliares.h"


/*====================================  Funçoes Auxiliares =======================================*/

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

void telaRepouso(){
	HAL_RTC_GetTime(RtcHandle, &sTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(RtcHandle, &sDate, RTC_FORMAT_BIN);
	limpa_lcd();
	escreve_lcd("   ");
	imprimeZero(sTime.Hours);
	imprimeASCII(sTime.Hours);
	escreve_lcd(":");

	imprimeZero(sTime.Minutes);
	imprimeASCII(sTime.Minutes);
	escreve_lcd(" ");

	imprimeZero(sDate.WeekDay);
	imprimeASCII(sDate.WeekDay);
	escreve_lcd("/");

	imprimeZero(sDate.Month);
	impimeASCII(sDate.Month);
	comando_lcd(0xC0);
	escreve_lcd("      ");

	imprimeZero(98);
	imprimeASCII(98);
	escreve_lcd("°F");

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
		perfil = 3;
		return perfil;
	}
	else {
		perfil = 0;
		return perfil;
	}
	return perfil;
}

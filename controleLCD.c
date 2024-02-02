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


/*====================================  Configuracoes do LCD 16x2 =======================================*/



void comando_lcd (unsigned char comando){ // comando em 4bits
	HAL_GPIO_WritePin(GPIOA, RS, 0); // RS = 0
	HAL_GPIO_WritePin(GPIOA, EN, 1); // EN = 1

	//Parte alta do comando
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, comando&(1<<4));
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, comando&(1<<5));
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, comando&(1<<6));
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, comando&(1<<7));

	//atualiza
	HAL_GPIO_WritePin(GPIOA, EN, 0);// EN = 0
	HAL_GPIO_WritePin(GPIOA, EN, 1);// EN = 1

	// Parte baixa do comando
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, comando&(1<<0));
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, comando&(1<<1));
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, comando&(1<<2));
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, comando&(1<<3));

	HAL_GPIO_WritePin(GPIOA, EN, 0);// EN = 0

	HAL_Delay(1);

}
void letra_lcd (unsigned char letra){ // comando em 4bits
	HAL_GPIO_WritePin(GPIOA, RS, 1); // RS = 0
	HAL_GPIO_WritePin(GPIOA, EN, 1); // EN = 1
	//Parte alta do comando
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, letra&(1<<4));
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, letra&(1<<5));
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, letra&(1<<6));
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, letra&(1<<7));
	//atualiza
	HAL_GPIO_WritePin(GPIOA, EN, 0);// EN = 0
	HAL_GPIO_WritePin(GPIOA, EN, 1);// EN = 1
	// Parte baixa do comando
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, letra&(1<<0));
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, letra&(1<<1));
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, letra&(1<<2));
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, letra&(1<<3));

	HAL_GPIO_WritePin(GPIOA, EN, 0);// EN = 0

	HAL_Delay(1);

}
void escreve_lcd (char  *msg){ // escreve um string no lcd
	unsigned char i=0;
	while (msg[i] != 0){
		letra_lcd(msg[i]);
		i++;
		HAL_Delay(1); // atraso 40us
	}
}
void limpa_lcd(){
	comando_lcd(0x01);
	HAL_Delay(2);
}
void inicia_lcd_4bits(){ // inicializa em 4bits o lcd
	HAL_Delay(15);
	comando_lcd (0x28);
	comando_lcd (0x0C);
	comando_lcd (0x06);
	comando_lcd (0x01);
	HAL_Delay(2);//atraso_1ms64();
}
void desliga_lcd_4bits() {
	HAL_Delay(15);
	comando_lcd (0x08);
	HAL_Delay(2);
}

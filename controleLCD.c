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

#include "controleLCD.h"


/*====================================  Configuracoes do LCD 16x2 =======================================*/

void comando_lcd (unsigned char comando){ // comando em 4bits
	PORTC &= ~(1<<RS);			// RS = 0
	PORTC |= (1<<EN);			// EN = 1
	PORTC &= 0x30;				// Limpa porta -> 0011 0000
	PORTC |= ((comando)/16);	//Parte alta do comando
	PORTC &= ~(1<<EN);			// EN = 0
	PORTC |= (1<<EN);
	PORTC &= 0x30;
	PORTC |= (comando&0x0F);	// Parte baixa do comando
	PORTC &= ~(1<<EN);
	atraso_40us();
}

void letra_lcd (unsigned char comando){ // letra em 4bits
	PORTC |= (1<<RS);					// RS = 1
	PORTC |= (1<<EN);					// EN = 1
	PORTC &= 0x30;						// Limpa porta -> 0011 0000
	PORTC |= ((comando)/16);			//Parte alta do comando
	PORTC &= ~(1<<EN);					// EN = 0
	PORTC |= (1<<EN);
	PORTC &= 0X30;
	PORTC |= (comando&0x0F);			// Parte baixa do comando
	PORTC &= ~(1<<EN);
	atraso_40us();
}

void escreve_lcd (char  *msg){ // escreve um string no lcd
	unsigned char i=0;
	while (msg[i] != 0){
		letra_lcd(msg[i]);
		i++;
		atraso_40us();
	}
}

void limpa_lcd(){
	comando_lcd(0x01);
	atraso_1ms64();
}

void inicia_lcd_4bits(){ // inicializa em 4bits o lcd
	atraso_15ms();
	comando_lcd (0x28);
	comando_lcd (0x0C);
	comando_lcd (0x06);
	comando_lcd (0x01);
	atraso_1ms64();
}

void desliga_lcd_4bits() {
	atraso_15ms();
	comando_lcd (0x08);
	atraso_1ms64();
}

void lcdEscreverSenha(){
	limpa_lcd();
	escreve_lcd("Insira a senha:");
	comando_lcd(0xC0); // nova linha
}


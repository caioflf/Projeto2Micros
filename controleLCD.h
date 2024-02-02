/*=================================================================================================*/
//	UNIVERSIDADE FEDERAL DO RIO GRANDE DO SUL
//	ENG04475 - Microprocessadores I (2023/2)
//	Projeto 1 - UBERGS
//
// Alunos:
// Bruno Gevehr Fernandes (00335299)
// Caio Fernando Leite França (00330098)
// Thiago Arndt Schimit (00333710)
/*=================================================================================================*/

#ifndef _CONTROLE_LCD_H_
#define _CONTROLE_LCD_H_

// Display (PORTC)
#define RS 4						// Pino RS do display na PORTC4
#define EN 5						// Pino EN do display na PORTC5

void comando_lcd (unsigned char comando);

void letra_lcd (unsigned char comando);

void escreve_lcd (char  *msg);

void limpa_lcd();

void inicia_lcd_4bits();

void desliga_lcd_4bits();

void lcdEscreverSenha();

#endif

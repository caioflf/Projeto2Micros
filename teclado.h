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

#ifndef _TECLADO_H_
#define _TECLADO_H_

// Senhas
#define SENHA1 "1253"					//Senha do operador 1
#define SENHA2 "8759"					//Senha do operador 2

// Teclado (PORTD e PORTB)
#define linha_max 7					// Ultimo pino em que as linhas estao conectadas (PORTD7)
#define botao1 2					// Coluna 1 do teclado
#define botao2 1					// Coluna 2 do teclado
#define botao3 0					// Coluna 3 do teclado

// Teclas do teclado
unsigned char teclado[4][3]={'1','2','3',
	                '4','5','6',
                        '7','8','9',
                        '*','0','#'
};

unsigned char debounce(unsigned char num_bit);

// Funcao que verifica se é pressionada a tecla '*' por tempo suficiente
unsigned char verificacao_tecla1(char tempo);

// Funcao que verifica se é pressionada a tecla '#' por tempo suficiente
unsigned char verificacao_tecla2(char tempo);

//Funcao que verifia se é solicitado o logout, o desligue do sistema, ou a ligacao do sistema
unsigned char verifica_login();

// Funcao qu escaneia as linhas 1, 2, 3 e 4
unsigned char scan(unsigned char linha);

// Funcao para controlar o carro
void movimento_manual (char letra);

#endif

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


// Funcao que verifica se é pressionada a tecla '*' por tempo suficiente
unsigned char verificacao_asterisco(char tempo);

// Funcao que verifica se é pressionada a tecla '#' por tempo suficiente
unsigned char verificacao_hashtag(char tempo);

//Funcao que verifia se é solicitado o logout, o desligue do sistema, ou a ligacao do sistema
unsigned char verifica_logoff();

//Funcao debounce
unsigned char debounce(unsigned char num_bit);

// Funcao qu escaneia as linhas 1, 2, 3 e 4
unsigned char scan(unsigned char linha);

unsigned char debounceDimming(unsigned char num_bit);

unsigned char scanDimming(unsigned char linha);

unsigned char scanTemp(unsigned char linha);

unsigned char scanData(unsigned char linha);

unsigned char scanHora(unsigned char linha);

#endif

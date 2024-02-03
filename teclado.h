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
#define SENHA1 		"12345"					//Senha do operador 1
#define SENHA2 		"54321"					//Senha do operador 2
#define SENHAADM	"00000"


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


#endif

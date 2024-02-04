/*
 * variaveisPseudoMain.h
 *
 *  Created on: Feb 3, 2024
 *      Author: caiof
 */

#ifndef SRC_VARIAVEISPSEUDOMAIN_H_
#define SRC_VARIAVEISPSEUDOMAIN_H_

typedef struct flag{				// Struct para flags
	char sistema;
}flag;

typedef struct indices{				// Struct para os indices
	char menu;		// indice da vertical
	char info;		// indice da horizontal
}indice;

// Senhas
#define SENHA1 				"12345"					//Senha do operador 1
#define SENHA2 				"54321"					//Senha do operador 2
#define SENHAADM			"00000"					//Senha do ADM
#define INDICE_MAX_USUARIO 		4						//Máximo de indices acessados por nao adm no menu
#define INDICE_MAX_ADM 			7						//Máximo de indices acessados por nao adm no menu
#define ADM 					3						//Perfil numero 3 == ADM

#endif /* SRC_VARIAVEISPSEUDOMAIN_H_ */

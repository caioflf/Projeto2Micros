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
	char luz1;
	char luz2;
	char luz3;
	char cortina;
	char cortinaAuto;
	char atualizarTela;
	char ativaPerfil1;
	char ativaPerfil2;
	unsigned short valor_ad1;
	unsigned short valor_ad2;
	unsigned short valor_ad3;
	char valorTemperatura1;
	char valorTemperatura2;
	char valorTemperatura3;
	char presencaNoturna;
	char ativadoNoturno;
}flag;

typedef struct indices{				// Struct para os indices
	char menu;		// indice da vertical
	char info;		// indice da horizontal
	char ultimoMenu;
	char ultimoInfo;
}indice;

// Senhas
#define SENHA1 				"12345"					//Senha do operador 1
#define SENHA2 				"54321"					//Senha do operador 2
#define SENHAADM			"00000"					//Senha do ADM
#define INDICE_MAX_USUARIO 		4						//Máximo de indices acessados por nao adm no menu
#define INDICE_MAX_ADM 			7						//Máximo de indices acessados por nao adm no menu
#define ADM 					3						//Perfil numero 3 == ADM

//LUZ QUARTO B0
//POTENCIOMETRO B1
// LUZ1 B10
// LUZ2 B11

#endif /* SRC_VARIAVEISPSEUDOMAIN_H_ */

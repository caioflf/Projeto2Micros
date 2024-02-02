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
#include "teclado.h"

unsigned char teclado[4][3]={
		{'1','2','3'},
		{'4','5','6'},
		{'7','8','9'},
		{'*','0','#'}
};

// Funcao que verifica se é pressionada a tecla '*' por tempo suficiente
unsigned char verificacao_tecla1(char tempo){ //tempo multiplicado por 2 em seg por conta de usar atraso de 0,5seg
}

// Funcao que verifica se é pressionada a tecla '#' por tempo suficiente
unsigned char verificacao_tecla2(char tempo){  // tempo em segundos (aproximado

}

//Funcao que verifia se é solicitado o logout, o desligue do sistema, ou a ligacao do sistema
unsigned char verifica_login(){

}


unsigned char debounce(unsigned char num_bit){
    unsigned char cont=0;
    unsigned char ultimo=0;

    while(cont<7){
        HAL_Delay(2);
        if(ultimo==(HAL_GPIO_ReadPin(GPIOB, 1<<(num_bit)))){
            cont++;
        }
        else{
            ultimo=(HAL_GPIO_ReadPin(GPIOB, 1<<(num_bit)));
            cont=0;
        }

    }
    return ultimo;
}
// Funcao qu escaneia as linhas 1, 2, 3 e 4
//LINHAS  PORT B3 B4 B5 B6
//COLUNAS PORT B7 B8 B9
unsigned char scan(unsigned char linha){

	HAL_GPIO_WritePin(GPIOB, 1<<(linha+2), 0);

	if(!debounce(7)){
		while(!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7));
		HAL_GPIO_WritePin(GPIOB, 1<<(linha+2), 1);
		return teclado[linha-1][0];
	}
	if(!debounce(8)){
		while(!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_8));
		HAL_GPIO_WritePin(GPIOB, 1<<(linha+2), 1);
		return teclado[linha-1][1];
	}
	if(!debounce(9)){
		while(!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9));
		HAL_GPIO_WritePin(GPIOB, 1<<(linha+2), 1);
		return teclado[linha-1][2];
	}
	HAL_GPIO_WritePin(GPIOB, 1<<(linha+2), 1);
	return '\0';
}



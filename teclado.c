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
#include "variaveisPseudoMain.h"
#include "teclado.h"

// Teclas do Teclado
unsigned char teclado[4][3]={
		{'1','2','3'},
		{'4','5','6'},
		{'7','8','9'},
		{'*','0','#'}
};

// Funcao que verifica se é pressionada a tecla '*' por tempo suficiente
unsigned char verificacao_asterisco(char tempo){ //tempo multiplicado por 2 em seg por conta de usar atraso de 0,5seg
	unsigned char cont=0;
	unsigned char ultimo=0;
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, 0);
	while(cont<tempo){
		if(ultimo){
			HAL_Delay(1);
			cont++;
		}
		else{
			if(ultimo==HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7)){
				HAL_Delay(500);
				cont++;
			}
			else{
				ultimo=HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7);
				cont=0;
			}
		}
	}
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, 1);
	return ultimo;
}

// Funcao que verifica se é pressionada a tecla '#' por tempo suficiente
unsigned char verificacao_hashtag(char tempo){ //tempo multiplicado por 2 em seg
	unsigned char cont=0;
	unsigned char ultimo=0;
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, 0);
	while(cont<tempo){
			if(ultimo){
				HAL_Delay(1);
				cont++;
			}
			else{
				if(ultimo==HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9)){
					HAL_Delay(500);
					cont++;
				}
				else{
					ultimo=HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9);
					cont=0;
				}
			}
		}
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, 1);
		return ultimo;
}

//Funcao que verifia se é solicitado o logout, o desligue do sistema, ou a ligacao do sistema
unsigned char verifica_logoff(){

	if(!verificacao_asterisco(6)){
		return teclado[3][0];
	}
	if(!verificacao_hashtag(4)){
		return teclado[3][2];
	}
	return '\0';
}


unsigned char debounce(unsigned char num_bit){
    unsigned char cont=0;
    unsigned char ultimo=0;

    while(cont<7){
        HAL_Delay(1);
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
		if(linha == 4){
			if(!verificacao_asterisco(6)){		//verifica se vem comando de desligar o sistema
				return 'd';
			}
		}
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

unsigned char debounceDimming(unsigned char num_bit){
    unsigned char cont=0;
    unsigned char ultimo=0;

    while(cont<2){
        HAL_Delay(1);
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

unsigned char scanDimming(unsigned char linha){

	HAL_GPIO_WritePin(GPIOB, 1<<(linha+2), 0);

	if(!debounceDimming(7)){
		HAL_GPIO_WritePin(GPIOB, 1<<(linha+2), 1);
		return teclado[linha-1][0];
	}
	if(!debounceDimming(8)){
		HAL_GPIO_WritePin(GPIOB, 1<<(linha+2), 1);
		return teclado[linha-1][1];
	}
	if(!debounceDimming(9)){
		HAL_GPIO_WritePin(GPIOB, 1<<(linha+2), 1);
		return teclado[linha-1][2];
	}
	HAL_GPIO_WritePin(GPIOB, 1<<(linha+2), 1);
	return '\0';
}

unsigned char scanTemp(unsigned char linha){

	HAL_GPIO_WritePin(GPIOB, 1<<(linha+2), 0);

	if(!debounceDimming(7)){
		HAL_GPIO_WritePin(GPIOB, 1<<(linha+2), 1);
		return teclado[linha-1][0];
	}
	if(!debounceDimming(8)){
		while(!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_8));
		HAL_GPIO_WritePin(GPIOB, 1<<(linha+2), 1);
		return teclado[linha-1][1];
	}
	if(!debounceDimming(9)){
		HAL_GPIO_WritePin(GPIOB, 1<<(linha+2), 1);
		return teclado[linha-1][2];
	}
	HAL_GPIO_WritePin(GPIOB, 1<<(linha+2), 1);
	return '\0';
}

unsigned char scanData(unsigned char linha){

	HAL_GPIO_WritePin(GPIOB, 1<<(linha+2), 0);

	if(!debounceDimming(7)){
		HAL_GPIO_WritePin(GPIOB, 1<<(linha+2), 1);
		return teclado[linha-1][0];
	}
	if(!debounceDimming(8)){
		HAL_GPIO_WritePin(GPIOB, 1<<(linha+2), 1);
		return teclado[linha-1][1];
	}
	if(!debounceDimming(9)){
		HAL_GPIO_WritePin(GPIOB, 1<<(linha+2), 1);
		return teclado[linha-1][2];
	}
	HAL_GPIO_WritePin(GPIOB, 1<<(linha+2), 1);
	return '\0';
}


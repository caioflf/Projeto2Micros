/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

unsigned char teclado[4][3]={
	{'1','2','3'},
	{'4','5','6'},
	{'7','8','9'},
	{'*','0','#'}
};

#define RS GPIO_PIN_4						// Pino RS do display na PORTA4
#define EN GPIO_PIN_5						// Pino EN do display na PORTA5

void SystemClock_Config(void);
static void MX_GPIO_Init(void);

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


//LINHAS  PORT B3 B4 B5 B6
//COLUNAS PORT B7 B8 B9
unsigned char scan(unsigned char linha){
	//linhas de 1 a 4
	HAL_GPIO_WritePin(GPIOB, 1<<(linha+2), 0);

	if(!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7)){
		while(!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7));
		HAL_GPIO_WritePin(GPIOB, 1<<(linha+2), 1);
		return teclado[linha-1][0];
	}
	if(!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_8)){
		while(!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_8));
		HAL_GPIO_WritePin(GPIOB, 1<<(linha+2), 1);
		return teclado[linha-1][1];
	}
	if(!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9)){
		while(!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9));
		HAL_GPIO_WritePin(GPIOB, 1<<(linha+2), 1);
		return teclado[linha-1][2];
	}
	HAL_GPIO_WritePin(GPIOB, 1<<(linha+2), 1);
	return '\0';
}


int main(void){
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();

  char letra, ultima, i;

  inicia_lcd_4bits();
  limpa_lcd();

  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 1);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, 1);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, 1);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, 1);

  while (1)
  {

	  for (i = 1; i<=4; i++){
		  letra = scan(i);
		  if ((letra != '\0') && (letra != ultima)){
			  letra_lcd(letra);
		  }
		  ultima = letra;
	  }
  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA0 PA1 PA2 PA3
                           PA4 PA5 PA6 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB3 PB4 PB5 PB6 */
  GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PB7 PB8 PB9 */
  GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

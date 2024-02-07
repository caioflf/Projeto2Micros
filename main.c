
#include "main.h"
#include "variaveisPseudoMain.h"
#include "controleLCD.h"
#include "teclado.h"
#include "auxiliares.h"


ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc2;
RTC_HandleTypeDef hrtc;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;


void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_RTC_Init(void);
static void MX_ADC1_Init(void);
static void MX_TIM3_Init(void);
static void MX_ADC2_Init(void);
static void MX_TIM2_Init(void);


void cortinaAutomatica(flag *flag, char perfil){

	if(!(flag->cortinaAuto))
	return;

	int valorLidoTemperatura, temperaturaCelsius;
	float aux;

	HAL_ADC_PollForConversion(&hadc1,1000);
	valorLidoTemperatura = HAL_ADC_GetValue(&hadc1);
	/*aux = 332.558 - 0.187364 * valorLidoTemperatura; // (V25 - 3.3)/4096 * AVG_SLOPE
	temperaturaCelsius = aux;*/
	aux = ((((float)valorLidoTemperatura * 3.3 / 4096) - 0.76)/0.0025 + 25)/10;
	temperaturaCelsius = (int)aux;


	if(perfil == 1){
		if(flag->cortina){
			if(temperaturaCelsius >= (flag->valorTemperatura1+1)){
				flag->cortina = 0;
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, 1);
				return;
			}
			return;
		}
		if(temperaturaCelsius <= (flag->valorTemperatura1-1)){
			flag->cortina = 1;
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, 0);
			return;
		}
		return;
	}
	if(perfil == 2){
		if(flag->cortina){
			if(temperaturaCelsius >= (flag->valorTemperatura2+1)){
				flag->cortina = 0;
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, 1);
				return;
			}
			return;
		}
		if(temperaturaCelsius <= (flag->valorTemperatura2-1)){
			flag->cortina = 1;
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, 0);
			return;
		}
		return;
	}
	if(perfil == 3){
		if(flag->cortina){
			if(temperaturaCelsius >= (flag->valorTemperatura3+1)){
				flag->cortina = 0;
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, 1);
				return;
			}
			return;
		}
		if(temperaturaCelsius <= (flag->valorTemperatura3-1)){
			flag->cortina = 1;
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, 0);
			return;
		}
		return;
	}
}

void simulaPresencaNoturna(flag *flag){
	if (!(flag->presencaNoturna))
		return;

	HAL_TIM_Base_Start_IT(&htim2);

	char letra = 0, perfil = 0, status = 0;

	while(!perfil){
		while(status != '*'){

		}


	}
}

//loop principal
void homicros(char *perfil, flag *flag, RTC_TimeTypeDef *sTime, RTC_DateTypeDef *DateToUpdate, unsigned char *segundo_ant){
	char letra, verificacao, i = 1;
	indice indice;
	indice.menu = 0;
	indice.info = 0;
	indice.ultimoMenu = 0;
	indice.ultimoInfo = 0;
	if (*perfil == 0 || *perfil == 'd'){
		return;
	}
	while(1){


		for (i = 1; i <= 2; i++){
			letra = scan(i);
			navegacaoMenu(flag, &indice, letra, *perfil);
			menu(flag, &indice, &hrtc, &sTime, DateToUpdate, segundo_ant, &hadc1, letra, &htim3, &hadc2, *perfil);

			indice.ultimoMenu = indice.menu;
			indice.ultimoInfo = indice.info;
		}

		cortinaAutomatica(flag, *perfil);
	//	simulaPresecaNoturna(&flag);

		for (i = 3; i <= 4; i++){
			letra = scan(i);
			verificacao = verifica_logoff();
			if (verificacao == '#'){
				limpa_lcd();
				escreve_lcd("Logoff Realizado");
				HAL_Delay(2000);
				*perfil = 0;
				flag->atualizarTela = 1;
				return;
			}
			if (verificacao == '*'){
				desligaSistema(flag);
				*perfil = 'd';
				return;
			}
		}
	}

}

void login (flag *flag, RTC_TimeTypeDef *sTime, RTC_DateTypeDef *DateToUpdate, unsigned char *segundo_ant){
	char perfil = 0;
	lcdSolicitaSenha();
	HAL_Delay(1000);
	while(1){
		perfil = ler_senha();

		if (perfil == 0){
					limpa_lcd();
					escreve_lcd("Senha Invalida!");
					HAL_Delay(2000);				// atraso em que o usuario espera pra poder digitar novamente a senha
		}
		if (perfil == 1){
			limpa_lcd();
			if (flag->ativaPerfil1){
				escreve_lcd("Perfil 1 Logado!");
			} else if (!flag->ativaPerfil1){
				escreve_lcd("Perfil 1 Bloq.!");
				perfil = 0;
			}
			HAL_Delay(2000);

		}
		if (perfil == 2){
			limpa_lcd();
			if (flag->ativaPerfil2){
				escreve_lcd("Perfil 2 Logado!");
			} else if (!flag->ativaPerfil2){
				escreve_lcd("Perfil 2 Bloq.!");
				perfil = 0;
			}
			HAL_Delay(2000);

		}
		if (perfil == ADM){
			limpa_lcd();
			escreve_lcd("Perfil ADM Logado!");
			HAL_Delay(2000);

		}


		homicros(&perfil, flag, sTime, DateToUpdate, segundo_ant); // loop principal

		if (perfil == 'd'){				//comando de desligar o sistema
			desligaSistema(flag);
			return;
		}

	}
}


int main(){

 HAL_Init();

  SystemClock_Config();

  MX_GPIO_Init();
  MX_RTC_Init();
  MX_ADC1_Init();
  MX_TIM3_Init();
  MX_ADC2_Init();
  MX_TIM2_Init();

  RTC_TimeTypeDef sTime;
  RTC_DateTypeDef DateToUpdate;

  RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

  GPIOC->CRH  &= ~GPIO_CRH_CNF13;
  GPIOC->CRH  |=  GPIO_CRH_MODE13_0;

  GPIOC->BSRR  = GPIO_BSRR_BR13;

  DateToUpdate.Date= 22;
  DateToUpdate.Month = 10;
  DateToUpdate.Year = 24;

  HAL_RTC_SetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BIN);

  sTime.Hours = 9;
  sTime.Minutes = 15;

  HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);

  HAL_ADC_Start(&hadc1);

  unsigned char verificacao, segundo_ant = 0;

  flag flag;
  flag.sistema = 0;
  flag.ativaPerfil1 = 1;
  flag.ativaPerfil2 = 1;
  flag.atualizarTela = 1;
  flag.luz1 = 0;
  flag.luz2 = 0;
  flag.luz3 = 0;
  flag.cortina = 0;
  flag.cortinaAuto = 0;
  flag.valorTemperatura1 = 25;
  flag.valorTemperatura2 = 25;
  flag.valorTemperatura3 = 25;
  flag.valor_ad1 = 0;
  flag.valor_ad2 = 0;
  flag.valor_ad3 = 0;
  flag.presencaNoturna = 0;
  flag.contagemLuzesPresenca = 0;

  inicia(&htim3, &hadc2);


  while (1) {

	telaRepouso(&hrtc, &sTime, &DateToUpdate, &segundo_ant, &hadc1);
  	verificacao = verifica_logoff();
  	if (verificacao == '*'){
  		desligaSistema(&flag);
  	}
  	if (verificacao == '#' && flag.sistema == 0){
  		ligaSistema(&flag);
  		login(&flag, &sTime, &DateToUpdate, &segundo_ant);
  	}
  }
}



void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	HAL_TIM_Base_Start_IT(&htim2);

}
/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_ADC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_HSE_DIV128;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_55CYCLES_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief ADC2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC2_Init(void)
{

  /* USER CODE BEGIN ADC2_Init 0 */

  /* USER CODE END ADC2_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC2_Init 1 */

  /* USER CODE END ADC2_Init 1 */

  /** Common config
  */
  hadc2.Instance = ADC2;
  hadc2.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc2.Init.ContinuousConvMode = DISABLE;
  hadc2.Init.DiscontinuousConvMode = DISABLE;
  hadc2.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc2.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&hadc2) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_9;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC2_Init 2 */

  /* USER CODE END ADC2_Init 2 */

}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef DateToUpdate = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
  hrtc.Init.OutPut = RTC_OUTPUTSOURCE_NONE;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */

  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 0x0;
  sTime.Minutes = 0x0;
  sTime.Seconds = 0x0;

  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  DateToUpdate.WeekDay = RTC_WEEKDAY_MONDAY;
  DateToUpdate.Month = RTC_MONTH_JANUARY;
  DateToUpdate.Date = 0x1;
  DateToUpdate.Year = 0x0;

  if (HAL_RTC_SetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 57600;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 6250-1;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 65535;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

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
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
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

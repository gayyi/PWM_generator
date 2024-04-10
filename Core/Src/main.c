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
#include "adc.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "pid.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define shutdown 00
#define buck 01
#define boost 10
#define working 11
#define noise 700
#define exp_buck_in 2482
#define exp_buck_out 1034
#define v_exp_buck_out 0.8333

#define exp_boost_in 1034
#define exp_boost_out 2482
#define v_exp_boost_out 2.000
#define s_exp_boost_out 791
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint16_t  terminal_12V,terminal_5V;
uint16_t  ADC_terminal_0,ADC_terminal_1;
uint8_t   work_mode=00,flag = 0;
double    ADC_terminal_12V,ADC_terminal_5V;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void ADC_Process()
{
	HAL_ADC_Start(&hadc1);
	ADC_terminal_0 = HAL_ADC_GetValue(&hadc1);
	ADC_terminal_12V = ADC_terminal_0/4096.0f*3.3f;
	HAL_Delay(10);
	HAL_ADC_Start(&hadc1);
	ADC_terminal_1 = HAL_ADC_GetValue(&hadc1);
	ADC_terminal_5V = ADC_terminal_1/4096.0f*3.3f;
}

void PWM_Process(double collect_voltage,double exp_voltage)
{	
	PID_Control(collect_voltage,exp_voltage);
	if(flag == 0)
	{
	HAL_TIMEx_PWMN_Start(&htim1,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
	flag = 1;
	}
	else return;
}

void Workmode_Select(void)
{
	if((ADC_terminal_0 <= noise)&&(ADC_terminal_1 <= noise))
		{work_mode = shutdown;}
	else if((((ADC_terminal_0 <= (exp_buck_in+noise))&&(ADC_terminal_0 >= (exp_buck_in-noise)))&&(ADC_terminal_1 <= noise))&& (flag == 0))
		{work_mode = buck;}
	else if((((ADC_terminal_1 <= (exp_boost_in+noise))&&(ADC_terminal_1 >= (exp_boost_in-noise)))&&((ADC_terminal_0 <= (s_exp_boost_out+noise))&&(ADC_terminal_0 >= (s_exp_boost_out-noise)))) && (flag ==0))
  //else if(((ADC_terminal_1 <= (exp_boost_in+noise))&&(ADC_terminal_1 >= (exp_boost_in-noise)))&&((ADC_terminal_0 <= (noise))))
		{work_mode = boost;}
	else return;
}

void Workmode_Process(void)
{
	if(work_mode == shutdown)
	{
		PID_Clear();
		HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_1);
		HAL_TIMEx_PWMN_Stop(&htim1,TIM_CHANNEL_1);
		flag = 0;
	}
	else if(work_mode == buck)
	{
	PWM_Process(ADC_terminal_5V,v_exp_buck_out);
	}
	else if(work_mode == boost)
	{
	PWM_Process(v_exp_boost_out,ADC_terminal_12V);
	}
}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM1_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */
//	TIM1->CCR1 = 336;
//	HAL_TIMEx_PWMN_Start(&htim1,TIM_CHANNEL_1);
//	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  ADC_Process();
	  Workmode_Select();
	  Workmode_Process();
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 80;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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

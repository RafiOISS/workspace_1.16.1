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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "fonts.h"
#include "ssd1306.h"
#include "stdio.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;

TIM_HandleTypeDef htim1;

/* USER CODE BEGIN PV */

#define TRIG_PIN GPIO_PIN_9
#define TRIG_PORT GPIOA
#define ECHO_PIN GPIO_PIN_8
#define ECHO_PORT GPIOA

uint32_t pMillis = 0;
uint32_t cMillis = 0;

uint32_t Value1 = 0;
uint32_t Value2 = 0;
uint16_t Distance  = 0;  // cm

GPIO_InitTypeDef GPIO_InitStructPrivate = {0};
uint8_t keyPressed = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM1_Init(void);
static void MX_I2C2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

#define DHT11_PORT GPIOB
#define DHT11_PIN GPIO_PIN_9
uint8_t RHI, RHD, TCI, TCD, SUM;
//uint32_t pMillis, cMillis;
float tCelsius = 0;
float tFahrenheit = 0;
float RH = 0;
uint8_t TFI = 0;
uint8_t TFD = 0;
char strCopy[15];

void microDelay (uint16_t delay)
{
  __HAL_TIM_SET_COUNTER(&htim1, 0);
  while (__HAL_TIM_GET_COUNTER(&htim1) < delay);
}

uint8_t DHT11_Start (void)
{
  uint8_t Response = 0;
  GPIO_InitTypeDef GPIO_InitStructPrivate = {0};

  GPIO_InitStructPrivate.Pin = DHT11_PIN;
  GPIO_InitStructPrivate.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructPrivate.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStructPrivate.Pull = GPIO_NOPULL;

  HAL_GPIO_Init(DHT11_PORT, &GPIO_InitStructPrivate); // set the pin as output
  HAL_GPIO_WritePin (DHT11_PORT, DHT11_PIN, 0);   // pull the pin low
  HAL_Delay(20);   // wait for 20ms
  HAL_GPIO_WritePin (DHT11_PORT, DHT11_PIN, 1);   // pull the pin high
  microDelay (30);   // wait for 30us

  GPIO_InitStructPrivate.Mode = GPIO_MODE_INPUT;
  GPIO_InitStructPrivate.Pull = GPIO_PULLUP;

  HAL_GPIO_Init(DHT11_PORT, &GPIO_InitStructPrivate); // set the pin as input
  microDelay (40);

  if (!(HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)))
  {
    microDelay (80);
    if ((HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN))) Response = 1;
  }

  pMillis = HAL_GetTick();
  cMillis = HAL_GetTick();
  while ((HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)) && pMillis + 2 > cMillis)
  {
    cMillis = HAL_GetTick();
  }
  return Response;
}

uint8_t DHT11_Read (void)
{
  uint8_t a,b;
  for (a=0;a<8;a++)
  {
    pMillis = HAL_GetTick();
    cMillis = HAL_GetTick();
    while (!(HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)) && pMillis + 2 > cMillis)
    {  // wait for the pin to go high
      cMillis = HAL_GetTick();
    }
    microDelay (40);   // wait for 40 us
    if (!(HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)))   // if the pin is low
      b&= ~(1<<(7-a));
    else
      b|= (1<<(7-a));
    pMillis = HAL_GetTick();
    cMillis = HAL_GetTick();
    while ((HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)) && pMillis + 2 > cMillis)
    {  // wait for the pin to go low
      cMillis = HAL_GetTick();
    }
  }
  return b;
}

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
  MX_I2C1_Init();
  MX_TIM1_Init();
  MX_I2C2_Init();
  /* USER CODE BEGIN 2 */

  HAL_TIM_Base_Start(&htim1);
  SSD1306_Init();
  HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_RESET);  // pull the TRIG pin low
  char snum[5];

  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, 1);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, 1);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, 1);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, 1);

//  SSD1306_GotoXY (0,0);
//  SSD1306_Puts ("Rafi", &Font_7x10, 1);
//  SSD1306_GotoXY (0, 30);
//  SSD1306_Puts ("CSE", &Font_7x10, 1);
//  SSD1306_UpdateScreen();
//  HAL_Delay (1000);
////
//  SSD1306_ScrollRight(0,7);
//  HAL_Delay(3000);
//  SSD1306_ScrollLeft(0,7);
//  HAL_Delay(3000);
//  SSD1306_Stopscroll();
//  SSD1306_Clear();
//  SSD1306_GotoXY (35,0);
//  SSD1306_Puts ("SCORE", &Font_7x10, 1);

//  SSD1306_GotoXY (0,0);
//  SSD1306_Puts ("0123456789abcdefgh", &Font_7x10, 1);
//
//  SSD1306_GotoXY (0,12);
//  SSD1306_Puts ("0123456789a", &Font_11x18, 1);
//
//  SSD1306_GotoXY (0, 32);
//  SSD1306_Puts ("0123456", &Font_16x26, 1);
//  SSD1306_GotoXY (0,20);
//  SSD1306_Puts ("2 BCDEFGH jklmnopq", &Font_7x10, 1);
//  SSD1306_GotoXY (0, 30);
//  SSD1306_Puts ("3 bcdefgh JKLMNOPQ", &Font_7x10, 1);
//
//  SSD1306_GotoXY (0,40);
//  SSD1306_Puts ("4 BCDEFGH jklmnopq", &Font_7x10, 1);
//  SSD1306_GotoXY (0, 50);
//  SSD1306_Puts ("5 bcdefgh JKLMNOPQ", &Font_7x10, 1);

//  	SSD1306_GotoXY (0,0);
//    SSD1306_Puts ("0", &Font_7x10, 1);
//    SSD1306_GotoXY (6, 0);
//    SSD1306_Puts ("0", &Font_7x10, 1);
//    SSD1306_GotoXY (20,0);
//    SSD1306_Puts ("2", &Font_7x10, 1);
//    SSD1306_GotoXY (30, 0);
//    SSD1306_Puts ("3", &Font_7x10, 1);
//
//    SSD1306_GotoXY (40,0);
//    SSD1306_Puts ("4", &Font_7x10, 1);
//    SSD1306_GotoXY (50, 0);
//    SSD1306_Puts ("5", &Font_7x10, 1);
//    SSD1306_GotoXY (60, 0);
//    SSD1306_Puts ("6", &Font_7x10, 1);
//    SSD1306_GotoXY (70, 0);
//    SSD1306_Puts ("7", &Font_7x10, 1);
//
//    SSD1306_UpdateScreen();



  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
//	  for(int i=0; i<20; i++){
//		  itoa(i, snum, 21);
//		  SSD1306_GotoXY (i*7, 10);
//		  SSD1306_Puts (snum, &Font_11x18, 1);
//		  SSD1306_UpdateScreen();
//		  HAL_Delay (100);
//	  }
//	  for ( int x = 1; x <= 10000 ; x++ )
//	  	{
//	  		itoa(x, snum, 10);
//	  		SSD1306_GotoXY (0, 30);
//	  		SSD1306_Puts ("             ", &Font_16x26, 1);
//	  		SSD1306_UpdateScreen();
//	  		if(x < 10) {
//	  			SSD1306_GotoXY (53, 30);  // 1 DIGIT
//	  		}
//	  		else if (x < 100 ) {
//	  			SSD1306_GotoXY (45, 30);  // 2 DIGITS
//	  		}
//	  		else if (x < 1000 ) {
//	  			SSD1306_GotoXY (37, 30);  // 3 DIGITS
//	  		}
//	  		else {
//	  			SSD1306_GotoXY (30, 30);  // 4 DIGIS
//	  		}
//	  		SSD1306_Puts (snum, &Font_16x26, 1);
//	  		SSD1306_UpdateScreen();
//	  		HAL_Delay (500);
//	  	}

	  HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_SET);  // pull the TRIG pin HIGH
	  __HAL_TIM_SET_COUNTER(&htim1, 0);
	  while (__HAL_TIM_GET_COUNTER (&htim1) < 10);  // wait for 10 us
	  HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_RESET);  // pull the TRIG pin low

	  pMillis = HAL_GetTick(); // used this to avoid infinite while loop  (for timeout)
	  // wait for the echo pin to go high
	  while (!(HAL_GPIO_ReadPin (ECHO_PORT, ECHO_PIN)) && pMillis + 10 >  HAL_GetTick());
	  Value1 = __HAL_TIM_GET_COUNTER (&htim1);

	  pMillis = HAL_GetTick(); // used this to avoid infinite while loop (for timeout)
	  // wait for the echo pin to go low
	  while ((HAL_GPIO_ReadPin (ECHO_PORT, ECHO_PIN)) && pMillis + 50 > HAL_GetTick());
	  Value2 = __HAL_TIM_GET_COUNTER (&htim1);

	  Distance = (Value2-Value1)* 0.034/2;

	  itoa(Distance, snum, 10);

	  SSD1306_GotoXY (84, 0);
	  SSD1306_Puts (snum, &Font_7x10, 1);
//	  SSD1306_UpdateScreen();
//	  HAL_Delay(50);

	  if(DHT11_Start())
	      {
	        RHI = DHT11_Read(); // Relative humidity integral
	        RHD = DHT11_Read(); // Relative humidity decimal
	        TCI = DHT11_Read(); // Celsius integral
	        TCD = DHT11_Read(); // Celsius decimal
	        SUM = DHT11_Read(); // Check sum
	        if (RHI + RHD + TCI + TCD == SUM)
	        {
	          // Can use RHI and TCI for any purposes if whole number only needed
	          tCelsius = (float)TCI + (float)(TCD/10.0);
	          tFahrenheit = tCelsius * 9/5 + 32;
	          RH = (float)RHI + (float)(RHD/10.0);
	          // Can use tCelsius, tFahrenheit and RH for any purposes
	          TFI = tFahrenheit;  // Fahrenheit integral
	          TFD = tFahrenheit*10-TFI*10; // Fahrenheit decimal
	          sprintf(strCopy,"%d.%d C   ", TCI, TCD);
	          SSD1306_GotoXY (0, 0);
	          SSD1306_Puts (strCopy, &Font_11x18, 1);
	          sprintf(strCopy,"%d.%d F   ", TFI, TFD);
	          SSD1306_GotoXY (0, 20);
	          SSD1306_Puts (strCopy, &Font_11x18, 1);
	          sprintf(strCopy,"%d.%d %%  ", RHI, RHD);
	          SSD1306_GotoXY (0, 40);
	          SSD1306_Puts (strCopy, &Font_11x18, 1);
//	          SSD1306_UpdateScreen();
	        }
	      }
	  	  SSD1306_UpdateScreen();
//	      HAL_Delay(50);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 400000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.ClockSpeed = 400000;
  hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 71;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 65535;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

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
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, R1_Pin|R2_Pin|R3_Pin|R4_Pin
                          |GPIO_PIN_9, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);

  /*Configure GPIO pins : R1_Pin R2_Pin R3_Pin R4_Pin
                           PA9 */
  GPIO_InitStruct.Pin = R1_Pin|R2_Pin|R3_Pin|R4_Pin
                          |GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : C1_Pin C2_Pin C3_Pin C4_Pin
                           PA8 */
  GPIO_InitStruct.Pin = C1_Pin|C2_Pin|C3_Pin|C4_Pin
                          |GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PB9 */
  GPIO_InitStruct.Pin = GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
//{
//  cMillis = HAL_GetTick();
//  if (cMillis - pMillis > 10) {
//    /*Configure GPIO pins : PB6 PB7 PB8 PB9 to GPIO_INPUT*/
//    GPIO_InitStructPrivate.Pin = GPIO_PIN_4|GPIO_PIN_4|GPIO_PIN_6|GPIO_PIN_7;
//    GPIO_InitStructPrivate.Mode = GPIO_MODE_INPUT;
//    GPIO_InitStructPrivate.Pull = GPIO_NOPULL;
//    GPIO_InitStructPrivate.Speed = GPIO_SPEED_FREQ_LOW;
//    HAL_GPIO_Init(GPIOA, &GPIO_InitStructPrivate);
//
//    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, 1);
//    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, 0);
//    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, 0);
//    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, 0);
//    if(GPIO_Pin == GPIO_PIN_4 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4))
//    {
//      keyPressed = 68; //ASCII value of D
//    }
//    else if(GPIO_Pin == GPIO_PIN_5 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5))
//    {
//      keyPressed = 67; //ASCII value of C
//    }
//    else if(GPIO_Pin == GPIO_PIN_6 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6))
//    {
//      keyPressed = 66; //ASCII value of B
//    }
//    else if(GPIO_Pin == GPIO_PIN_7 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7))
//    {
//      keyPressed = 65; //ASCII value of A
//    }
//
//    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, 0);
//    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, 1);
//    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, 0);
//    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 0);
//    if(GPIO_Pin == GPIO_PIN_4 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4))
//    {
//      keyPressed = 35; //ASCII value of #
//    }
//    else if(GPIO_Pin == GPIO_PIN_5 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5))
//    {
//      keyPressed = 57; //ASCII value of 9
//    }
//    else if(GPIO_Pin == GPIO_PIN_6 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6))
//    {
//      keyPressed = 54; //ASCII value of 6
//    }
//    else if(GPIO_Pin == GPIO_PIN_7 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7))
//    {
//      keyPressed = 51; //ASCII value of 3
//    }
//
//    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, 0);
//    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, 0);
//    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, 1);
//    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 0);
//    if(GPIO_Pin == GPIO_PIN_4 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4))
//    {
//      keyPressed = 48; //ASCII value of 0
//    }
//    else if(GPIO_Pin == GPIO_PIN_5 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5))
//    {
//      keyPressed = 56; //ASCII value of 8
//    }
//    else if(GPIO_Pin == GPIO_PIN_6 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6))
//    {
//      keyPressed = 53; //ASCII value of 5
//    }
//    else if(GPIO_Pin == GPIO_PIN_7 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7))
//    {
//      keyPressed = 50; //ASCII value of 2
//    }
//
//    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, 0);
//    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, 0);
//    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, 0);
//    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 1);
//    if(GPIO_Pin == GPIO_PIN_4 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4))
//    {
//      keyPressed = 42; //ASCII value of *
//    }
//    else if(GPIO_Pin == GPIO_PIN_5 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5))
//    {
//      keyPressed = 55; //ASCII value of 7
//    }
//    else if(GPIO_Pin == GPIO_PIN_6 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6))
//    {
//      keyPressed = 52; //ASCII value of 4
//    }
//    else if(GPIO_Pin == GPIO_PIN_7 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7))
//    {
//      keyPressed = 49; //ASCII value of 1
//    }
//
//    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, 1);
//    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, 1);
//    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, 1);
//    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 1);
//    /*Configure GPIO pins : PB6 PB7 PB8 PB9 back to EXTI*/
//    GPIO_InitStructPrivate.Mode = GPIO_MODE_IT_RISING;
//    GPIO_InitStructPrivate.Pull = GPIO_PULLDOWN;
//    HAL_GPIO_Init(GPIOA, &GPIO_InitStructPrivate);
//    pMillis = cMillis;
//  }
//}

void Keys_Detect(void)
{
	/* Setting Row 1 ad High and others LOW */
	HAL_GPIO_WritePin(GPIOA, R1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, R2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, R3_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, R4_Pin, GPIO_PIN_RESET);

	if(HAL_GPIO_ReadPin(GPIOA, C1_Pin) == GPIO_PIN_SET)
	{
		/* 1 */
		keyPressed = 49; //ASCII value of 1
		while (HAL_GPIO_ReadPin (GPIOA, C1_Pin));
	}

	if(HAL_GPIO_ReadPin(GPIOA, C2_Pin) == GPIO_PIN_SET)
	{
		/* 2 */
		keyPressed = 50; //ASCII value of 2
		while (HAL_GPIO_ReadPin (GPIOA, C2_Pin));
	}

	if(HAL_GPIO_ReadPin(GPIOA, C3_Pin) == GPIO_PIN_SET)
	{
		/* 3 */
		keyPressed = 51; //ASCII value of 2
		while (HAL_GPIO_ReadPin (GPIOA, C3_Pin));
	}

	if(HAL_GPIO_ReadPin(GPIOA, C4_Pin) == GPIO_PIN_SET)
	{
		/* A */
		keyPressed = 65; //ASCII value of 2
		while (HAL_GPIO_ReadPin (GPIOA, C4_Pin));
	}

	/* Setting Row 2 ad High and others LOW */
	HAL_GPIO_WritePin(GPIOA, R1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, R2_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, R3_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, R4_Pin, GPIO_PIN_RESET);

	if(HAL_GPIO_ReadPin(GPIOA, C1_Pin) == GPIO_PIN_SET)
	{
		/* 4 */
		keyPressed = 52; //ASCII value of 2
		while (HAL_GPIO_ReadPin (GPIOA, C1_Pin));
	}

	if(HAL_GPIO_ReadPin(GPIOA, C2_Pin) == GPIO_PIN_SET)
	{
		/* 5 */
		keyPressed = 53; //ASCII value of 2
		while (HAL_GPIO_ReadPin (GPIOA, C2_Pin));
	}

	if(HAL_GPIO_ReadPin(GPIOA, C3_Pin) == GPIO_PIN_SET)
	{
		/* 6 */
		keyPressed = 54; //ASCII value of 2
		while (HAL_GPIO_ReadPin (GPIOA, C3_Pin));
	}

	if(HAL_GPIO_ReadPin(GPIOA, C4_Pin) == GPIO_PIN_SET)
	{
		/* B */
		keyPressed = 66; //ASCII value of 2
		while (HAL_GPIO_ReadPin (GPIOA, C4_Pin));
	}

	/* Setting Row 3 ad High and others LOW */
	HAL_GPIO_WritePin(GPIOA, R1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, R2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, R3_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, R4_Pin, GPIO_PIN_RESET);

	if(HAL_GPIO_ReadPin(GPIOA, C1_Pin) == GPIO_PIN_SET)
	{
		/* 7 */
		keyPressed = 55; //ASCII value of 2
		while (HAL_GPIO_ReadPin (GPIOA, C1_Pin));
	}

	if(HAL_GPIO_ReadPin(GPIOA, C2_Pin) == GPIO_PIN_SET)
	{
		/* 8 */
		keyPressed = 56; //ASCII value of 2
		while (HAL_GPIO_ReadPin (GPIOA, C2_Pin));
	}

	if(HAL_GPIO_ReadPin(GPIOA, C3_Pin) == GPIO_PIN_SET)
	{
		/* 9 */
		keyPressed = 57; //ASCII value of 2
		while (HAL_GPIO_ReadPin (GPIOA, C3_Pin));
	}

	if(HAL_GPIO_ReadPin(GPIOA, C4_Pin) == GPIO_PIN_SET)
	{
		/* C */
		keyPressed = 67; //ASCII value of 2
		while (HAL_GPIO_ReadPin (GPIOA, C4_Pin));
	}

	/* Setting Row 4 ad High and others LOW */
	HAL_GPIO_WritePin(GPIOA, R1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, R2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, R3_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, R4_Pin, GPIO_PIN_SET);

	if(HAL_GPIO_ReadPin(GPIOA, C1_Pin) == GPIO_PIN_SET)
	{
		/* * */
		keyPressed = 42; //ASCII value of 2
		while (HAL_GPIO_ReadPin (GPIOA, C1_Pin));
	}

	if(HAL_GPIO_ReadPin(GPIOA, C2_Pin) == GPIO_PIN_SET)
	{
		/* 0 */
		keyPressed = 48; //ASCII value of 2
		while (HAL_GPIO_ReadPin (GPIOA, C2_Pin));
	}

	if(HAL_GPIO_ReadPin(GPIOA, C3_Pin) == GPIO_PIN_SET)
	{
		/* # */
		keyPressed = 35; //ASCII value of 2
		while (HAL_GPIO_ReadPin (GPIOA, C3_Pin));
	}

	if(HAL_GPIO_ReadPin(GPIOA, C4_Pin) == GPIO_PIN_SET)
	{
		/* D */
		keyPressed = 68; //ASCII value of 2
		while (HAL_GPIO_ReadPin (GPIOA, C4_Pin));
	}
}

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
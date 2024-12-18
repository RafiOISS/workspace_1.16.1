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

// Debounce delay (adjust as needed)
//#define DEBOUNCE_DELAY 50 // 50 ms

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;

TIM_HandleTypeDef htim1;

/* USER CODE BEGIN PV */

//uint8_t keyPressed = 0;
// Global variable to store the pressed key
//volatile uint8_t keyPressed = 0;

GPIO_InitTypeDef GPIO_InitStructPrivate = {0};
uint32_t previousMillis = 0;
uint32_t currentMillis = 0;
uint8_t keyPressed = 0;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM1_Init(void);
static void MX_I2C2_Init(void);
/* USER CODE BEGIN PFP */

//void Keys_Detect(void);
void oledPrintChar(int asciiValue);
void DashboardScreen();

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
  MX_I2C1_Init();
  MX_TIM1_Init();
  MX_I2C2_Init();
  /* USER CODE BEGIN 2 */

  SSD1306_Init();

  SSD1306_GotoXY (24,0);
  SSD1306_Puts ("DEM-C", &Font_16x26, 1);
  SSD1306_GotoXY (25, 30);
  SSD1306_Puts ("Environment", &Font_7x10, 1);
  SSD1306_GotoXY (39, 42);
  SSD1306_Puts ("Monitor", &Font_7x10, 1);
  SSD1306_UpdateScreen();
  HAL_Delay(3000);
  SSD1306_Clear();
//  SSD1306_GotoXY (0,0);

  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, 1);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, 1);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, 1);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, 1);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

//	  DashboardScreen();
//	  Keys_Detect();
	  HAL_Delay(100);


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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL2;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
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
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_9, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA0 PA1 PA2 PA3
                           PA9 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PA4 PA5 PA6 PA7 */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PA8 */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PB9 */
  GPIO_InitStruct.Pin = GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  currentMillis = HAL_GetTick();
  if (currentMillis - previousMillis > 10) {
    /*Configure GPIO pins : PB6 PB7 PB8 PB9 to GPIO_INPUT*/
    GPIO_InitStructPrivate.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStructPrivate.Mode = GPIO_MODE_INPUT;
    GPIO_InitStructPrivate.Pull = GPIO_NOPULL;
    GPIO_InitStructPrivate.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructPrivate);

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, 1);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, 0);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, 0);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, 0);
    if(GPIO_Pin == GPIO_PIN_4 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4))
    {
      keyPressed = 68; //ASCII value of D
      oledPrintChar(keyPressed);
    }
    else if(GPIO_Pin == GPIO_PIN_5 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5))
    {
      keyPressed = 67; //ASCII value of C
      oledPrintChar(keyPressed);
    }
    else if(GPIO_Pin == GPIO_PIN_6 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6))
    {
      keyPressed = 66; //ASCII value of B
      oledPrintChar(keyPressed);
    }
    else if(GPIO_Pin == GPIO_PIN_7 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7))
    {
      keyPressed = 65; //ASCII value of A
      oledPrintChar(keyPressed);
    }

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, 0);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, 1);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, 0);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, 0);
    if(GPIO_Pin == GPIO_PIN_4 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4))
    {
      keyPressed = 35; //ASCII value of #
      oledPrintChar(keyPressed);
    }
    else if(GPIO_Pin == GPIO_PIN_5 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5))
    {
      keyPressed = 57; //ASCII value of 9
      oledPrintChar(keyPressed);
    }
    else if(GPIO_Pin == GPIO_PIN_6 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6))
    {
      keyPressed = 54; //ASCII value of 6
      oledPrintChar(keyPressed);
    }
    else if(GPIO_Pin == GPIO_PIN_7 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7))
    {
      keyPressed = 51; //ASCII value of 3
      oledPrintChar(keyPressed);
    }

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, 0);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, 0);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, 1);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, 0);
    if(GPIO_Pin == GPIO_PIN_4 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4))
    {
      keyPressed = 48; //ASCII value of 0
      oledPrintChar(keyPressed);
    }
    else if(GPIO_Pin == GPIO_PIN_5 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5))
    {
      keyPressed = 56; //ASCII value of 8
      oledPrintChar(keyPressed);
    }
    else if(GPIO_Pin == GPIO_PIN_6 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6))
    {
      keyPressed = 53; //ASCII value of 5
      oledPrintChar(keyPressed);
    }
    else if(GPIO_Pin == GPIO_PIN_7 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7))
    {
      keyPressed = 50; //ASCII value of 2
      oledPrintChar(keyPressed);
    }

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, 0);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, 0);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, 0);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, 1);
    if(GPIO_Pin == GPIO_PIN_4 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4))
    {
      keyPressed = 42; //ASCII value of *
      oledPrintChar(keyPressed);
    }
    else if(GPIO_Pin == GPIO_PIN_5 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5))
    {
      keyPressed = 55; //ASCII value of 7
      oledPrintChar(keyPressed);
    }
    else if(GPIO_Pin == GPIO_PIN_6 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6))
    {
      keyPressed = 52; //ASCII value of 4
      oledPrintChar(keyPressed);
    }
    else if(GPIO_Pin == GPIO_PIN_7 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7))
    {
      keyPressed = 49; //ASCII value of 1
      oledPrintChar(keyPressed);
    }

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, 1);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, 1);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, 1);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, 1);
    /*Configure GPIO pins : PB6 PB7 PB8 PB9 back to EXTI*/
    GPIO_InitStructPrivate.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStructPrivate.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructPrivate);
    previousMillis = currentMillis;
  }
}

//void Keys_Detect(void)
//{
//	/* Setting Row 1 ad High and others LOW */
//	HAL_GPIO_WritePin(GPIOA, R1_Pin, GPIO_PIN_SET);
//	HAL_GPIO_WritePin(GPIOA, R2_Pin, GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(GPIOA, R3_Pin, GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(GPIOA, R4_Pin, GPIO_PIN_RESET);
//
//	if(HAL_GPIO_ReadPin(GPIOA, C1_Pin) == GPIO_PIN_SET)
//	{
//		/* 1 */
////		  LCD_command(0x01);
////			LCD_string((uint8_t *)"You Pressed");
////
////		  LCD_command(0x38);                       // Set 8 bit mode
////	    LCD_command(0XC0);                       // Moving the cursor to second line
////	    LCD_string((uint8_t *)"Key 1");
//
////		SSD1306_Clear();
////		SSD1306_GotoXY (0,0);
////
////		SSD1306_Puts ("1", &Font_16x26, 1);
////		SSD1306_UpdateScreen();
//
//		keyPressed = 49; //ASCII value of 1
//		oledPrintChar(keyPressed);
//		while (HAL_GPIO_ReadPin (GPIOA, C1_Pin));
//	}
//
//	if(HAL_GPIO_ReadPin(GPIOA, C2_Pin) == GPIO_PIN_SET)
//	{
//		/* 2 */
////		  LCD_command(0x01);
////			LCD_string((uint8_t *)"You Pressed");
////
////		  LCD_command(0x38);                       // Set 8 bit mode
////	    LCD_command(0XC0);                       // Moving the cursor to second line
////	    LCD_string((uint8_t *)"Key 2");
//
////		SSD1306_Clear();
////		SSD1306_GotoXY (0,0);
////		SSD1306_Puts ("2", &Font_16x26, 1);
////		SSD1306_UpdateScreen();
//
//		keyPressed = 50; //ASCII value of 2
//		oledPrintChar(keyPressed);
//		while (HAL_GPIO_ReadPin (GPIOA, C2_Pin));
//	}
//
//	if(HAL_GPIO_ReadPin(GPIOA, C3_Pin) == GPIO_PIN_SET)
//	{
//		/* 3 */
////		  LCD_command(0x01);
////			LCD_string((uint8_t *)"You Pressed");
////
////		  LCD_command(0x38);                       // Set 8 bit mode
////	    LCD_command(0XC0);                       // Moving the cursor to second line
////	    LCD_string((uint8_t *)"Key 3");
//
////		SSD1306_Clear();
////		SSD1306_GotoXY (0,0);
////		SSD1306_Puts ("3", &Font_16x26, 1);
////		SSD1306_UpdateScreen();
//
//		keyPressed = 51; //ASCII value of 2
//		oledPrintChar(keyPressed);
//		while (HAL_GPIO_ReadPin (GPIOA, C3_Pin));
//	}
//
//	if(HAL_GPIO_ReadPin(GPIOA, C4_Pin) == GPIO_PIN_SET)
//	{
//		/* A */
////		  LCD_command(0x01);
////			LCD_string((uint8_t *)"You Pressed");
////
////		  LCD_command(0x38);                       // Set 8 bit mode
////	    LCD_command(0XC0);                       // Moving the cursor to second line
////	    LCD_string((uint8_t *)"Key A");
////
////		SSD1306_Clear();
////		SSD1306_GotoXY (0,0);
////		SSD1306_Puts ("A", &Font_16x26, 1);
////		SSD1306_UpdateScreen();
//
//		keyPressed = 65; //ASCII value of 2
//		oledPrintChar(keyPressed);
//		while (HAL_GPIO_ReadPin (GPIOA, C4_Pin));
//	}
//
//	/* Setting Row 2 ad High and others LOW */
//	HAL_GPIO_WritePin(GPIOA, R1_Pin, GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(GPIOA, R2_Pin, GPIO_PIN_SET);
//	HAL_GPIO_WritePin(GPIOA, R3_Pin, GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(GPIOA, R4_Pin, GPIO_PIN_RESET);
//
//	if(HAL_GPIO_ReadPin(GPIOA, C1_Pin) == GPIO_PIN_SET)
//	{
//		/* 4 */
////		  LCD_command(0x01);
////			LCD_string((uint8_t *)"You Pressed");
////
////		  LCD_command(0x38);                       // Set 8 bit mode
////	    LCD_command(0XC0);                       // Moving the cursor to second line
////	    LCD_string((uint8_t *)"Key 4");
//
////		SSD1306_Clear();
////		SSD1306_GotoXY (0,0);
////		SSD1306_Puts ("4", &Font_16x26, 1);
////		SSD1306_UpdateScreen();
//
//		keyPressed = 52; //ASCII value of 2
//		oledPrintChar(keyPressed);
//		while (HAL_GPIO_ReadPin (GPIOA, C1_Pin));
//	}
//
//	if(HAL_GPIO_ReadPin(GPIOA, C2_Pin) == GPIO_PIN_SET)
//	{
//		/* 5 */
////		  LCD_command(0x01);
////			LCD_string((uint8_t *)"You Pressed");
////
////		  LCD_command(0x38);                       // Set 8 bit mode
////	    LCD_command(0XC0);                       // Moving the cursor to second line
////	    LCD_string((uint8_t *)"Key 5");
//
////		SSD1306_Clear();
////		SSD1306_GotoXY (0,0);
////		SSD1306_Puts ("5", &Font_16x26, 1);
////		SSD1306_UpdateScreen();
//
//		keyPressed = 53; //ASCII value of 2
//		oledPrintChar(keyPressed);
//		while (HAL_GPIO_ReadPin (GPIOA, C2_Pin));
//	}
//
//	if(HAL_GPIO_ReadPin(GPIOA, C3_Pin) == GPIO_PIN_SET)
//	{
//		/* 6 */
////		  LCD_command(0x01);
////			LCD_string((uint8_t *)"You Pressed");
////
////		  LCD_command(0x38);                       // Set 8 bit mode
////	    LCD_command(0XC0);                       // Moving the cursor to second line
////	    LCD_string((uint8_t *)"Key 6");
////
////		SSD1306_Clear();
////		SSD1306_GotoXY (0,0);
////		SSD1306_Puts ("6", &Font_16x26, 1);
////		SSD1306_UpdateScreen();
//
//		keyPressed = 54; //ASCII value of 2
//		oledPrintChar(keyPressed);
//		while (HAL_GPIO_ReadPin (GPIOA, C3_Pin));
//	}
//
//	if(HAL_GPIO_ReadPin(GPIOA, C4_Pin) == GPIO_PIN_SET)
//	{
//		/* B */
////		  LCD_command(0x01);
////			LCD_string((uint8_t *)"You Pressed");
////
////		  LCD_command(0x38);                       // Set 8 bit mode
////	    LCD_command(0XC0);                       // Moving the cursor to second line
////	    LCD_string((uint8_t *)"Key B");
//
////		SSD1306_Clear();
////		SSD1306_GotoXY (0,0);
////		SSD1306_Puts ("B", &Font_16x26, 1);
////		SSD1306_UpdateScreen();
//
//		keyPressed = 66; //ASCII value of 2
//		oledPrintChar(keyPressed);
//		while (HAL_GPIO_ReadPin (GPIOA, C4_Pin));
//	}
//
//	/* Setting Row 3 ad High and others LOW */
//	HAL_GPIO_WritePin(GPIOA, R1_Pin, GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(GPIOA, R2_Pin, GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(GPIOA, R3_Pin, GPIO_PIN_SET);
//	HAL_GPIO_WritePin(GPIOA, R4_Pin, GPIO_PIN_RESET);
//
//	if(HAL_GPIO_ReadPin(GPIOA, C1_Pin) == GPIO_PIN_SET)
//	{
//		/* 7 */
////		  LCD_command(0x01);
////			LCD_string((uint8_t *)"You Pressed");
////
////		  LCD_command(0x38);                       // Set 8 bit mode
////	    LCD_command(0XC0);                       // Moving the cursor to second line
////	    LCD_string((uint8_t *)"Key 7");
//
////		SSD1306_Clear();
////		SSD1306_GotoXY (0,0);
////		SSD1306_Puts ("7", &Font_16x26, 1);
////		SSD1306_UpdateScreen();
//
//		keyPressed = 55; //ASCII value of 2
//		oledPrintChar(keyPressed);
//		while (HAL_GPIO_ReadPin (GPIOA, C1_Pin));
//	}
//
//	if(HAL_GPIO_ReadPin(GPIOA, C2_Pin) == GPIO_PIN_SET)
//	{
//		/* 8 */
////		  LCD_command(0x01);
////			LCD_string((uint8_t *)"You Pressed");
////
////		  LCD_command(0x38);                       // Set 8 bit mode
////	    LCD_command(0XC0);                       // Moving the cursor to second line
////	    LCD_string((uint8_t *)"Key 8");
////
////		SSD1306_Clear();
////		SSD1306_GotoXY (0,0);
////		SSD1306_Puts ("8", &Font_16x26, 1);
////		SSD1306_UpdateScreen();
//
//		keyPressed = 56; //ASCII value of 2
//		oledPrintChar(keyPressed);
//		while (HAL_GPIO_ReadPin (GPIOA, C2_Pin));
//	}
//
//	if(HAL_GPIO_ReadPin(GPIOA, C3_Pin) == GPIO_PIN_SET)
//	{
//		/* 9 */
////		  LCD_command(0x01);
////			LCD_string((uint8_t *)"You Pressed");
////
////		  LCD_command(0x38);                       // Set 8 bit mode
////	    LCD_command(0XC0);                       // Moving the cursor to second line
////	    LCD_string((uint8_t *)"Key 9");
////
////		SSD1306_Clear();
////		SSD1306_GotoXY (0,0);
////		SSD1306_Puts ("9", &Font_16x26, 1);
////		SSD1306_UpdateScreen();
//
//		keyPressed = 57; //ASCII value of 2
//		oledPrintChar(keyPressed);
//		while (HAL_GPIO_ReadPin (GPIOA, C3_Pin));
//	}
//
//	if(HAL_GPIO_ReadPin(GPIOA, C4_Pin) == GPIO_PIN_SET)
//	{
//		/* C */
////		  LCD_command(0x01);
////			LCD_string((uint8_t *)"You Pressed");
////
////		  LCD_command(0x38);                       // Set 8 bit mode
////	    LCD_command(0XC0);                       // Moving the cursor to second line
////	    LCD_string((uint8_t *)"Key C");
//
////		SSD1306_Clear();
////		SSD1306_GotoXY (0,0);
////		SSD1306_Puts ("C", &Font_16x26, 1);
////		SSD1306_UpdateScreen();
//
//		keyPressed = 67; //ASCII value of 2
//		oledPrintChar(keyPressed);
//		while (HAL_GPIO_ReadPin (GPIOA, C4_Pin));
//	}
//
//	/* Setting Row 4 ad High and others LOW */
//	HAL_GPIO_WritePin(GPIOA, R1_Pin, GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(GPIOA, R2_Pin, GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(GPIOA, R3_Pin, GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(GPIOA, R4_Pin, GPIO_PIN_SET);
//
//	if(HAL_GPIO_ReadPin(GPIOA, C1_Pin) == GPIO_PIN_SET)
//	{
//		/* * */
////		  LCD_command(0x01);
////			LCD_string((uint8_t *)"You Pressed");
////
////		  LCD_command(0x38);                       // Set 8 bit mode
////	    LCD_command(0XC0);                       // Moving the cursor to second line
////	    LCD_string((uint8_t *)"Key *");
//
////		SSD1306_Clear();
////		SSD1306_GotoXY (0,0);
////		SSD1306_Puts ("*", &Font_16x26, 1);
////		SSD1306_UpdateScreen();
//
//		keyPressed = 42; //ASCII value of 2
//		oledPrintChar(keyPressed);
//		while (HAL_GPIO_ReadPin (GPIOA, C1_Pin));
//	}
//
//	if(HAL_GPIO_ReadPin(GPIOA, C2_Pin) == GPIO_PIN_SET)
//	{
//		/* 0 */
////		  LCD_command(0x01);
////			LCD_string((uint8_t *)"You Pressed");
////
////		  LCD_command(0x38);                       // Set 8 bit mode
////	    LCD_command(0XC0);                       // Moving the cursor to second line
////	    LCD_string((uint8_t *)"Key 0");
//
////		SSD1306_Clear();
////		SSD1306_GotoXY (0,0);
////		SSD1306_Puts ("0", &Font_16x26, 1);
////		SSD1306_UpdateScreen();
//
//		keyPressed = 48; //ASCII value of 2
//		oledPrintChar(keyPressed);
//		while (HAL_GPIO_ReadPin (GPIOA, C2_Pin));
//	}
//
//	if(HAL_GPIO_ReadPin(GPIOA, C3_Pin) == GPIO_PIN_SET)
//	{
//		/* # */
////		  LCD_command(0x01);
////			LCD_string((uint8_t *)"You Pressed");
////
////		  LCD_command(0x38);                       // Set 8 bit mode
////	    LCD_command(0XC0);                       // Moving the cursor to second line
////	    LCD_string((uint8_t *)"Key #");
//
////		SSD1306_Clear();
////		SSD1306_GotoXY (0,0);
////		SSD1306_Puts ("#", &Font_16x26, 1);
////		SSD1306_UpdateScreen();
//
//		keyPressed = 35; //ASCII value of 2
//		oledPrintChar(keyPressed);
//		while (HAL_GPIO_ReadPin (GPIOA, C3_Pin));
//	}
//
//	if(HAL_GPIO_ReadPin(GPIOA, C4_Pin) == GPIO_PIN_SET)
//	{
//		/* D */
////		  LCD_command(0x01);
////			LCD_string((uint8_t *)"You Pressed");
////
////		  LCD_command(0x38);                       // Set 8 bit mode
////	    LCD_command(0XC0);                       // Moving the cursor to second line
////	    LCD_string((uint8_t *)"Key D");
//
////		SSD1306_Clear();
////		SSD1306_GotoXY (0,0);
////		SSD1306_Puts ("D", &Font_16x26, 1);
////		SSD1306_UpdateScreen();
//
//		keyPressed = 68; //ASCII value of 2
//		oledPrintChar(keyPressed);
//		while (HAL_GPIO_ReadPin (GPIOA, C4_Pin));
//	}
//}

//void Keys_Detect(void)
//{
//    // Reset keyPressed before detection
//    keyPressed = 0;
//
//    // 4x4 Keypad Layout
//    const uint8_t keyMap[4][4] = {
//        {'1', '2', '3', 'A'},
//        {'4', '5', '6', 'B'},
//        {'7', '8', '9', 'C'},
//        {'*', '0', '#', 'D'}
//    };
//
//    // Scan through rows
//    for (int row = 0; row < 4; row++)
//    {
//        // Reset all rows to low
//        HAL_GPIO_WritePin(GPIOA, R1_Pin, GPIO_PIN_RESET);
//        HAL_GPIO_WritePin(GPIOA, R2_Pin, GPIO_PIN_RESET);
//        HAL_GPIO_WritePin(GPIOA, R3_Pin, GPIO_PIN_RESET);
//        HAL_GPIO_WritePin(GPIOA, R4_Pin, GPIO_PIN_RESET);
//
//        // Set current row high
//        switch(row)
//        {
//            case 0: HAL_GPIO_WritePin(GPIOA, R1_Pin, GPIO_PIN_SET); break;
//            case 1: HAL_GPIO_WritePin(GPIOA, R2_Pin, GPIO_PIN_SET); break;
//            case 2: HAL_GPIO_WritePin(GPIOA, R3_Pin, GPIO_PIN_SET); break;
//            case 3: HAL_GPIO_WritePin(GPIOA, R4_Pin, GPIO_PIN_SET); break;
//        }
//
//        // Small delay to stabilize
//        HAL_Delay(10);
//
//        // Check columns
//        for (int col = 0; col < 4; col++)
//        {
//            GPIO_PinState colState;
//            switch(col)
//            {
//                case 0: colState = HAL_GPIO_ReadPin(GPIOA, C1_Pin); break;
//                case 1: colState = HAL_GPIO_ReadPin(GPIOA, C2_Pin); break;
//                case 2: colState = HAL_GPIO_ReadPin(GPIOA, C3_Pin); break;
//                case 3: colState = HAL_GPIO_ReadPin(GPIOA, C4_Pin); break;
//            }
//
//            // Key is pressed
//            if (colState == GPIO_PIN_SET)
//            {
//                // Debounce
//                HAL_Delay(DEBOUNCE_DELAY);
//
//                // Re-check to confirm key press
//                switch(col)
//                {
//                    case 0: colState = HAL_GPIO_ReadPin(GPIOA, C1_Pin); break;
//                    case 1: colState = HAL_GPIO_ReadPin(GPIOA, C2_Pin); break;
//                    case 2: colState = HAL_GPIO_ReadPin(GPIOA, C3_Pin); break;
//                    case 3: colState = HAL_GPIO_ReadPin(GPIOA, C4_Pin); break;
//                }
//
//                // Confirm key press after debounce
//                if (colState == GPIO_PIN_SET)
//                {
//                    // Store the key
//                    keyPressed = keyMap[row][col];
//
//                    // Optional: Print the key
//                    oledPrintChar(keyPressed);
//
//                    // Wait for key release
//                    while(colState == GPIO_PIN_SET)
//                    {
//                        switch(col)
//                        {
//                            case 0: colState = HAL_GPIO_ReadPin(GPIOA, C1_Pin); break;
//                            case 1: colState = HAL_GPIO_ReadPin(GPIOA, C2_Pin); break;
//                            case 2: colState = HAL_GPIO_ReadPin(GPIOA, C3_Pin); break;
//                            case 3: colState = HAL_GPIO_ReadPin(GPIOA, C4_Pin); break;
//                        }
//                    }
//
//                    // Exit after first key press
//                    return;
//                }
//            }
//        }
//    }
//}

void oledPrintChar(int asciiValue){
	char character[2];
	character[0] = (char)asciiValue;
	character[1] = '\0';

	SSD1306_Clear();
	SSD1306_GotoXY (0,0);
	SSD1306_Puts (character, &Font_16x26, 1);
	SSD1306_UpdateScreen();
}

void DashboardScreen(){
	SSD1306_GotoXY (29,0);
	SSD1306_Puts ("20.5 C", &Font_11x18, 1);
	SSD1306_GotoXY (29, 20);
	SSD1306_Puts ("50.9 %", &Font_11x18, 1);

	SSD1306_DrawLine(0, 45, SSD1306_WIDTH - 1, 45, 1);

	SSD1306_GotoXY (6, 52);
	SSD1306_Puts ("A", &Font_7x10, 1);
	SSD1306_DrawCircle(9, 56, 7, 1);
	SSD1306_GotoXY (20, 52);
	SSD1306_Puts ("MENU", &Font_7x10, 1);

	SSD1306_GotoXY (113, 52);
	SSD1306_Puts ("ON", &Font_7x10, 1);
	SSD1306_UpdateScreen();
//	  HAL_Delay(3000);
//	  SSD1306_Clear();
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

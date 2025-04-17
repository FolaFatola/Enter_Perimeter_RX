/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include <cstdio>
#include "rtc.hpp"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
//register commands
constexpr uint8_t write = 0x20;
constexpr uint8_t read = 0x0;

constexpr uint8_t config_reg = 0x0;
constexpr uint8_t feature_reg = 0x1D;
constexpr uint8_t aw_reg = 0x03;
constexpr uint8_t rx_addr_p0_reg = 0x0A;
constexpr uint8_t tx_addr_reg = 0x10;
constexpr uint8_t setup_retr_reg = 0x04;
constexpr uint8_t rf_setup_reg = 0x06;
constexpr uint8_t rf_ch_reg = 0x05;

//set of spi commands
constexpr uint8_t WRITE_CONFIG_REG = write | config_reg;
constexpr uint8_t READ_CONFIG_REG = read | config_reg;
constexpr uint8_t WRITE_FEATURE_REG = write | feature_reg;
constexpr uint8_t READ_FEATURE_REG = read | feature_reg;
constexpr uint8_t WRITE_AW_REG = write | aw_reg;
constexpr uint8_t READ_AW_REG = read | aw_reg;
constexpr uint8_t WRITE_RX_ADDR_P0_REG = write | rx_addr_p0_reg;
constexpr uint8_t READ_RX_ADDR_P0_REG = read | rx_addr_p0_reg;
constexpr uint8_t WRITE_TX_ADDR = write | tx_addr_reg;
constexpr uint8_t READ_TX_ADDR = read | tx_addr_reg;
constexpr uint8_t WRITE_SETUP_RETR_REG = write | setup_retr_reg;
constexpr uint8_t READ_SETUP_RETR_REG = read | setup_retr_reg;
constexpr uint8_t WRITE_RF_SETUP_REG = write | rf_setup_reg;
constexpr uint8_t READ_RF_SETUP_REG = read | rf_setup_reg;
constexpr uint8_t WRITE_RF_CH_REG = write | rf_ch_reg;
constexpr uint8_t READ_RF_CH_REG = read | rf_ch_reg;

//register bits.
constexpr uint8_t PWR_UP_BIT = (1 << 1);
constexpr uint8_t EN_DPL_BIT = (1 << 2);

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c2;

SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C2_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_SPI1_Init(void);
/* USER CODE BEGIN PFP */

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
  MX_I2C2_Init();
  MX_USART2_UART_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */
  bool use_tfh_time = true;
  uint8_t seconds = 2;
  uint8_t minutes = 0;
  uint8_t hours = 6;
  uint8_t weekday = SATURDAY;
  uint8_t date_day = 12;
  uint8_t month = APRIL;
  uint8_t year = 25;

  Time_RTC rtc_sensor_time{&hi2c2, use_tfh_time, seconds, minutes, hours, weekday, date_day, month, year};
  rtc_sensor_time.rtc_init();

  char message[100];

  //when due to voltage being above 1.9V, we are currently in power down mode.

  //PB6 is the CS PIN. PC7 is CE pin, and PA9 is for the external interrupt.
  //Set the PWR_UP bit in the config register.
  uint8_t command = WRITE_CONFIG_REG;
  uint8_t config_reg_bits = PWR_UP_BIT;
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
  HAL_SPI_Transmit(&hspi1, &command, 1, 100);
  HAL_SPI_Transmit(&hspi1, &config_reg_bits, 1, 100);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);

  //Read back the PWR_UP bit in the config register.
  command = READ_CONFIG_REG;
  uint8_t read_config_bits = 0;
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);

  HAL_SPI_Transmit(&hspi1, &command, 1, 100);
  HAL_SPI_Receive(&hspi1, &read_config_bits, 1, 100);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
  HAL_Delay(3);

  //Here you should be in standby mode.
  //enable DPL by setting the DPL bit in the feature register high.
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
  command = WRITE_FEATURE_REG;
  uint8_t feature_reg_bits = EN_DPL_BIT;
  HAL_SPI_Transmit(&hspi1, &command, 1, 100);
  HAL_SPI_Transmit(&hspi1, &feature_reg_bits, 1, 100);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);

  //Read back the EN_DPL bit in the feature register
  uint8_t read_feature_reg_bits = 24;
  command = READ_FEATURE_REG;
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
  HAL_SPI_Transmit(&hspi1, &command, 1, 100);
  HAL_SPI_Receive(&hspi1, &read_feature_reg_bits, 1, 100);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);

  //Set the AW register
  command = WRITE_AW_REG;
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
  uint8_t three_bytes = 1;
  uint8_t four_bytes = 2;
  uint8_t five_bytes = 3;
  HAL_SPI_Transmit(&hspi1, &command, 1, 100);
  HAL_SPI_Transmit(&hspi1, &three_bytes, 1, 100);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);

  //read back the AW register
  uint8_t read_aw_reg_bits = 0;
  command = READ_AW_REG;
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
  HAL_SPI_Transmit(&hspi1, &command, 1, 100);
  HAL_SPI_Receive(&hspi1, &read_aw_reg_bits, 1, 100);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);

  //Set the receiver address.
  uint8_t rx_addr_byte = 0x78;
  command = WRITE_RX_ADDR_P0_REG;
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
  HAL_SPI_Transmit(&hspi1, &command, 1, 100);
  HAL_SPI_Transmit(&hspi1, &rx_addr_byte, 1, 100);
  HAL_SPI_Transmit(&hspi1, &rx_addr_byte, 1, 100);
  HAL_SPI_Transmit(&hspi1, &rx_addr_byte, 1, 100);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);

  //Read back the receiver address
  uint8_t data_byte = 0;
  int rx_address = 0;
  command = READ_RX_ADDR_P0_REG;
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
  HAL_SPI_Transmit(&hspi1, &command, 1, 100);
  HAL_SPI_Receive(&hspi1, &data_byte, 1, 100);
  rx_address |= data_byte;
  HAL_SPI_Receive(&hspi1, &data_byte, 1, 100);
  rx_address |= data_byte << 8;
  HAL_SPI_Receive(&hspi1, &data_byte, 1, 100);
  rx_address |= data_byte << 16;
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);

  //set the transmitter address
  command = WRITE_TX_ADDR;
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
  HAL_SPI_Transmit(&hspi1, &command, 1, 100);
  HAL_SPI_Transmit(&hspi1, &rx_addr_byte, 1, 100);
  HAL_SPI_Transmit(&hspi1, &rx_addr_byte, 1, 100);
  HAL_SPI_Transmit(&hspi1, &rx_addr_byte, 1, 100);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);

  //Read back the transmitter address
   int tx_address = 0;
   command = READ_RX_ADDR_P0_REG;
   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
   HAL_SPI_Transmit(&hspi1, &command, 1, 100);
   HAL_SPI_Receive(&hspi1, &data_byte, 1, 100);
   tx_address |= data_byte;
   HAL_SPI_Receive(&hspi1, &data_byte, 1, 100);
   tx_address |= data_byte << 8;
   HAL_SPI_Receive(&hspi1, &data_byte, 1, 100);
   tx_address |= data_byte << 16;
   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);

   //set up the number of retransmits and the auto retransmit delay
   uint8_t arc_bits = 0x03;
   uint8_t ard_bits = 0x01;
   uint8_t set_retr_reg_bits = (ard_bits << 4) | arc_bits;
   command = WRITE_SETUP_RETR_REG;
   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
   HAL_SPI_Transmit(&hspi1, &command, 1, 100);
   HAL_SPI_Transmit(&hspi1, &set_retr_reg_bits, 1, 100);
   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);

   //read back the status of the set_retr register.
   uint8_t read_set_retr_reg_bits = 0;
   command = READ_SETUP_RETR_REG;
   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
   HAL_SPI_Transmit(&hspi1, &command, 1, 100);
   HAL_SPI_Receive(&hspi1, &read_set_retr_reg_bits, 1, 100);
   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);

   //Setting up the air data rate and PA control stuff.
   uint8_t rf_setup_reg_bits = 0x24;
   command = WRITE_RF_SETUP_REG;
   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
   HAL_SPI_Transmit(&hspi1, &command, 1, 100);
   HAL_SPI_Transmit(&hspi1, &rf_setup_reg_bits, 1, 100);
   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);

   //read back the rf_setup bits.
   uint8_t read_rf_setup_bits = 0;
   command = READ_RF_SETUP_REG;
   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
   HAL_SPI_Transmit(&hspi1, &command, 1, 100);
   HAL_SPI_Receive(&hspi1, &read_rf_setup_bits, 1, 100);
   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);

//   //setting the rf_ch bits
   uint8_t rf_channel_bits = 0x04;
   command = WRITE_RF_CH_REG;
   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
   HAL_SPI_Transmit(&hspi1, &command, 1, 100);
   HAL_SPI_Transmit(&hspi1, &rf_channel_bits, 1, 100);
   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);

   //reading the rf channel bits
   uint8_t read_rf_channel_bits = 0;
   command = READ_RF_CH_REG;
   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
   HAL_SPI_Transmit(&hspi1, &command, 1, 100);
   HAL_SPI_Receive(&hspi1, &read_rf_channel_bits, 1, 100);
   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);








  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
//	  rtc_sensor_time.rtc_get_time_unit(SECONDS, seconds);
//	  rtc_sensor_time.rtc_get_time_unit(MINUTES, minutes);
//	  rtc_sensor_time.rtc_get_time_unit(HOURS, hours);
//	  rtc_sensor_time.rtc_get_time_unit(WEEK_DAY, weekday);
//	  rtc_sensor_time.rtc_get_time_unit(DATE_DAY, date_day);
//	  rtc_sensor_time.rtc_get_time_unit(MONTH, month);
//	  rtc_sensor_time.rtc_get_time_unit(YEAR, year);
//
//	  HAL_UART_Transmit(&huart2, (uint8_t *)message, sprintf(message, "Seconds: %d, Minutes: %d, Hours: %d, "
//			  "Week_Day: %d, Date_Day %d, Month %d, Year %d\r\n",
//			  seconds, minutes, hours, weekday, date_day, month, year+millennium), 100);

	  HAL_UART_Transmit(&huart2, (uint8_t *)message, sprintf(message, "The config reg status is %d\r\n",
			  read_config_bits), 100);

	  HAL_UART_Transmit(&huart2, (uint8_t *)message, sprintf(message, "The feature reg status is %d\r\n",
			  read_feature_reg_bits), 100);

	  HAL_UART_Transmit(&huart2, (uint8_t *)message, sprintf(message, "The AW reg status is %d\r\n",
			  read_aw_reg_bits), 100);

	  HAL_UART_Transmit(&huart2, (uint8_t *)message, sprintf(message, "The RX_ADDR reg status is %d\r\n",
			  rx_address), 100);

	  HAL_UART_Transmit(&huart2, (uint8_t *)message, sprintf(message, "The TX_ADDR reg status is %d\r\n",
	 			  tx_address), 100);

	  HAL_UART_Transmit(&huart2, (uint8_t *)message, sprintf(message, "The set_retr reg status is %d\r\n",
			  read_set_retr_reg_bits), 100);

	  HAL_UART_Transmit(&huart2, (uint8_t *)message, sprintf(message, "The rf_setup_reg value is %d\r\n",
			  read_rf_setup_bits), 100);

	  HAL_UART_Transmit(&huart2, (uint8_t *)message, sprintf(message, "The rf_ch reg value is %d\r\n",
			  read_rf_channel_bits), 100);


	  HAL_Delay(1000);
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
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
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
  hi2c2.Init.ClockSpeed = 100000;
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
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

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
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC7 */
  GPIO_InitStruct.Pin = GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PA9 */
  GPIO_InitStruct.Pin = GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PB6 */
  GPIO_InitStruct.Pin = GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
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

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
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <cstdio>
#include <stdio.h>
#include "nrf2401.hpp"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
//register commands
constexpr uint8_t write = 0x20;
constexpr uint8_t read = 0x00;


/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void write_register(SPI_HandleTypeDef *spi_handle, uint8_t *write_data, uint8_t write_bytes);
void read_register(SPI_HandleTypeDef *spi_handle, uint8_t *read_data, uint8_t read_bytes);

void write_register(SPI_HandleTypeDef *spi_handle, uint8_t reg, uint8_t *write_data, uint8_t write_bytes) {
	uint8_t write_reg = write | reg;
  	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
	HAL_SPI_Transmit(spi_handle, &write_reg, 1, 100);
	HAL_SPI_Transmit(spi_handle, write_data, write_bytes, 100);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
}

void read_register(SPI_HandleTypeDef *spi_handle, uint8_t reg, uint8_t *read_data, uint8_t read_bytes) {
	uint8_t read_reg = read | reg;
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
	HAL_SPI_Transmit(spi_handle, &read_reg, 1, 100);
	HAL_SPI_Receive(spi_handle, read_data, read_bytes, 100);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
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
  MX_SPI1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

	//when due to voltage being above 1.9V, we are currently in power down mode.

	//PB6 is the CS PIN. PC7 is CE pin, and PA9 is for the external interrupt.

    HAL_Delay(10);
    uint8_t command = 0;

    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);

	//reset all registers
	uint8_t reset_value = 0x08;
	write_register(&hspi1, config_reg, &reset_value, 1);

	reset_value = 0x3F;
	write_register(&hspi1, en_aa_reg, &reset_value, 1);

	reset_value = 0x03;
	write_register(&hspi1, en_rxaddr_reg, &reset_value, 1);

	reset_value = 0x03;
	write_register(&hspi1, aw_reg, &reset_value, 1);

	reset_value = 0x03;
	write_register(&hspi1, setup_retr_reg, &reset_value, 1);

	reset_value = 0x02;
	write_register(&hspi1, rf_ch_reg, &reset_value, 1);

	reset_value = 0x07;
	write_register(&hspi1, rf_setup_reg, &reset_value, 1);

	reset_value = 0x7E;
	write_register(&hspi1, status_reg, &reset_value, 1);

	uint8_t address_reset_val_p0[5] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};
	write_register(&hspi1, rx_addr_p0_reg, address_reset_val_p0, 5);
	write_register(&hspi1, tx_addr_reg, address_reset_val_p0, 5);
	uint8_t address_reset_val_p1[5] = {0xC2, 0xC2, 0xC2, 0xC2, 0xC2};
	write_register(&hspi1, rx_addr_p1_reg, address_reset_val_p1, 5);
	reset_value = 0xC3;
	write_register(&hspi1, rx_addr_p2_reg, &reset_value, 1);
	reset_value = 0xC4;
	write_register(&hspi1, rx_addr_p3_reg, &reset_value, 1);
	reset_value = 0xC5;
	write_register(&hspi1, rx_addr_p4_reg, &reset_value, 1);
	reset_value = 0xC6;
	write_register(&hspi1, rx_addr_p5_reg, &reset_value, 1);

	reset_value = 0x00;
	write_register(&hspi1, rx_pw_p0_reg, &reset_value, 1);
	write_register(&hspi1, rx_pw_p1_reg, &reset_value, 1);
	write_register(&hspi1, rx_pw_p2_reg, &reset_value, 1);
	write_register(&hspi1, rx_pw_p3_reg, &reset_value, 1);
	write_register(&hspi1, rx_pw_p4_reg, &reset_value, 1);
	write_register(&hspi1, rx_pw_p5_reg, &reset_value, 1);

	reset_value = 0x11;
	write_register(&hspi1, fifo_status_reg, &reset_value, 1);

	reset_value = 0x00;
	write_register(&hspi1, dynpd_reg, &reset_value, 1);
	write_register(&hspi1, feature_reg, &reset_value, 1);







	uint8_t reg_value = 0;


	uint8_t tx_addr[5] = {0x34, 0x35, 0xF0, 0xD3, 0xE4};


	rf_module.nrf_init(2480, KBPS_250, FIVE_BYTES);
	rf_module.setup_tx_mode(tx_addr, 5);

	rf_module.send_spi_command(flush_rx_fifo);
	rf_module.send_spi_command(flush_tx_fifo);


	read_register(&hspi1, status_reg, &reg_value, 1);
	printf("The status_reg is %d\n", reg_value);

	read_register(&hspi1, config_reg, &reg_value, 1);
	printf("The config register is %d\n", reg_value);

	read_register(&hspi1, aw_reg, &reg_value, 1);
	printf("The aw_reg is %d\n", reg_value);

	read_register(&hspi1, setup_retr_reg, &reg_value, 1);
	printf("The setup_retr_reg is %d\n", reg_value);

	read_register(&hspi1, rf_ch_reg, &reg_value, 1);
	printf("The rf_ch_reg is %d\n", reg_value);

	read_register(&hspi1, rf_setup_reg, &reg_value, 1);
	printf("The rf_setup_reg is %d\n", reg_value);

	rf_module.check_fifo_status(reg_value);
	printf("The value of the fifo_status is %d\n", reg_value);

	uint8_t rx_address[5];
	read_register(&hspi1, rx_addr_p0_reg, rx_address, 5);
	printf("Rx_address\n");
	for (int i = 0; i < 5; i++) {
		printf("The byte is %d\n", rx_address[i]);
	}


	uint8_t tx_address[5];
	read_register(&hspi1, tx_addr_reg, tx_address, 5);
	printf("Tx_address\n");
	for (int i = 0; i < 5; i++) {
		printf("The byte is %d\n", tx_address[i]);
	}







	uint8_t tx_data[3] = {1, 2, 3};
	uint8_t fifo_stat = 0;




  /* USER CODE END 2 */


  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1) {
	  for (int i = 0; i < 3; ++i) {
		  tx_data[i] += 1;
		  printf("The value of tx_data[i] is %d\n", tx_data[i]);
	  }
	  rf_module.send_data_tx_to_fifo(tx_data, 3);


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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

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

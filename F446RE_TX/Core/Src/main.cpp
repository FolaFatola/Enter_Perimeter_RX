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

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
//register commands
constexpr uint8_t write = 0x20;
constexpr uint8_t read = 0x0;

constexpr uint8_t config_reg = 0x00;
constexpr uint8_t en_aa_reg = 0x01;
constexpr uint8_t aw_reg = 0x03;
constexpr uint8_t setup_retr_reg = 0x04;
constexpr uint8_t rf_ch_reg = 0x05;
constexpr uint8_t rf_setup_reg = 0x06;
constexpr uint8_t rx_addr_p0_reg = 0x0A;
constexpr uint8_t tx_addr_reg = 0x10;
constexpr uint8_t fifo_status_reg = 0x17;
constexpr uint8_t dynpd_reg = 0x1C;
constexpr uint8_t feature_reg = 0x1D;
constexpr uint8_t status_reg = 0x07;
constexpr uint8_t en_rxaddr = 0x02;


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
constexpr uint8_t WRITE_EN_AA_REG = write | en_aa_reg;
constexpr uint8_t READ_EN_AA_REG = read | en_aa_reg;
constexpr uint8_t WRITE_DYNPD_REG =  write | dynpd_reg;
constexpr uint8_t READ_DYNPD_REG = read | dynpd_reg;
constexpr uint8_t READ_FIFO_STATUS = read | fifo_status_reg;
constexpr uint8_t READ_STATUS_REG = read | status_reg;
constexpr uint8_t WRITE_STATUS_REG = write | status_reg;
constexpr uint8_t WRITE_EN_RX_ADDR_REG = write | en_rxaddr;


constexpr uint8_t SEND_DATA = 0xA0;
constexpr uint8_t FLUSH_TX = 0xE1;

//config register.
constexpr uint8_t EN_CRC = (1 << 3);
constexpr uint8_t CRCO = (1 << 2);
constexpr uint8_t PWR_UP_BIT = (1 << 1);
constexpr uint8_t PRIM_RX = (1 << 0);

//feature reg
constexpr uint8_t EN_ACK_PAY = (1 << 1);



//ENAA registers (enable auto acknowledgement)
constexpr uint8_t ENAA_P0 = (1 << 0);
constexpr uint8_t ENAA_P1 = (1 << 1);
constexpr uint8_t ENAA_P2 = (1 << 2);
constexpr uint8_t ENAA_P3 = (1 << 3);
constexpr uint8_t ENAA_P4 = (1 << 4);
constexpr uint8_t ENAA_P5 = (1 << 5);

constexpr uint8_t DPL_P0 = (1 << 0);
constexpr uint8_t DPL_P1 = (1 << 1);
constexpr uint8_t DPL_P2 = (1 << 2);
constexpr uint8_t DPL_P3 = (1 << 3);
constexpr uint8_t DPL_P4 = (1 << 4);
constexpr uint8_t DPL_P5 = (1 << 5);



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

    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);



	//reset all registers
    uint8_t command = WRITE_CONFIG_REG;
	uint8_t reset_value = 0x08;
	write_register(&hspi1, config_reg, &reset_value, 1);

	reset_value = 0x3F;
	write_register(&hspi1, en_aa_reg, &reset_value, 1);

	reset_value = 0x03;
	write_register(&hspi1, en_rxaddr, &reset_value, 1);

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

//	uint8_t tx_ds = (1 << 5);
//	uint8_t max_rt = (1 << 4);
//	uint8_t rx_dr = (1 << 6);
//
//	uint8_t status_register_bits = 0;
//
//	status_register_bits |= tx_ds; //TX_DS clear
//	status_register_bits |= max_rt;
//	status_register_bits |= rx_dr;
//	write_register(&hspi1, status_reg, &status_register_bits, 1);
//	write_register(&hspi1, status_reg, &status_register_bits, 1);
//	write_register(&hspi1, status_reg, &status_register_bits, 1);




	//CONFIG settings
  	uint8_t config_reg_bits = 0;
  	read_register(&hspi1, config_reg, &config_reg_bits, 1);

  	printf("Reading config reg %d...\n", config_reg_bits);

//	config_reg_bits |= 0x70; //clear interrupt flags
//	write_register(&hspi1, config_reg, &config_reg_bits, 1);

	//Set the PWR_UP bit in the config register.
	config_reg_bits |= PWR_UP_BIT;	//ENTER standby mode.
	write_register(&hspi1, config_reg, &config_reg_bits, 1);
	HAL_Delay(5);

	//GO into TX mode.
	read_register(&hspi1, config_reg, &config_reg_bits, 1);
	config_reg_bits &= ~PRIM_RX; //SET PRIM_RX to 0;
	write_register(&hspi1, config_reg, &config_reg_bits, 1);
	HAL_Delay(5);

	read_register(&hspi1, config_reg, &config_reg_bits, 1);

	printf("Reading config reg after setting pwr, mode and more %d...\n", config_reg_bits);



	//RF CH settings
	uint8_t rf_channel_bits = 0;
	command = READ_RF_CH_REG;
	read_register(&hspi1, rf_ch_reg, &rf_channel_bits, 1);

	//setting the rf_ch bits
	rf_channel_bits |= 0x64;
	write_register(&hspi1, rf_ch_reg, &rf_channel_bits, 1);


	//AIR DATA RATE
	uint8_t rf_setup_reg_bits = 0;
	read_register(&hspi1, rf_setup_reg, &rf_setup_reg_bits, 1);

	//air data rate.
	rf_setup_reg_bits |= 0x06;
	write_register(&hspi1, rf_setup_reg, &rf_setup_reg_bits, 1);

	//setting up crc
	config_reg_bits |= CRCO | EN_CRC;
	write_register(&hspi1, config_reg, &config_reg_bits, 1);

	//Set the AW register
	uint8_t three_bytes = 1;
	uint8_t four_bytes = 2;
	uint8_t five_bytes = 3;
	write_register(&hspi1, aw_reg, &five_bytes, 1);

	//SET THE TX address
	uint8_t tx_addr[5] = {0x77, 0x35, 0xF0, 0xD3, 0xE7};
	write_register(&hspi1, tx_addr_reg, tx_addr, 5);

	//SET RX address
	write_register(&hspi1, rx_addr_p0_reg, tx_addr, 5);


	uint8_t set_retr_reg_bits = 0;
	read_register(&hspi1, setup_retr_reg, &set_retr_reg_bits, 1);

//	//set up the number of retransmits and the auto retransmit delay
	uint8_t arc_bits = 0x03;
	uint8_t ard_bits = 0x00;
	set_retr_reg_bits |= (ard_bits << 4) | arc_bits;
	command = WRITE_SETUP_RETR_REG;
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, &command, 1, 100);
	HAL_SPI_Transmit(&hspi1, &set_retr_reg_bits, 1, 100);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);

//
	command = FLUSH_TX;
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, &command, 1, 100);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);

//	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);



	uint8_t tx_full = 0;
	uint8_t tx_empty = 0;
	uint8_t fifo_status = 0;

	uint8_t aw_reg_bits = 0;
	read_register(&hspi1, aw_reg, &aw_reg_bits, 1);
	read_register(&hspi1, setup_retr_reg, &set_retr_reg_bits, 1);
	read_register(&hspi1, rf_ch_reg, &rf_channel_bits, 1);
	read_register(&hspi1, rf_setup_reg, &rf_setup_reg_bits, 1);
	uint8_t status_reg_bits = 0;
	read_register(&hspi1, status_reg, &status_reg_bits, 1);

	uint8_t tx_address_read[5];
	read_register(&hspi1, tx_addr_reg, tx_address_read, 5);

	for (int i = 0; i < 5; i++) {
		printf("Tx Address Byte: %d\n", tx_address_read[i]);
	}

	uint8_t rx_address_read[5];
	read_register(&hspi1, rx_addr_p0_reg, rx_address_read, 5);
	for (int i = 0; i < 5; i++) {
		printf("RX Address Byte %d\n", rx_address_read[i]);
	}

	printf("AW reg bits is %d\n", aw_reg_bits);
	printf("SETUP_RETR reg bits %d\n", set_retr_reg_bits);
	printf("RF CH bits %d\n", rf_channel_bits);
	printf("RF_SETUP reg is %d\n", rf_setup_reg_bits);
	printf("Status reg %d\n", status_reg_bits);
	printf("The config register is %d\n", &config_reg_bits);

	uint8_t data_bytes[3] = {5, 6, 10}; //31
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1) {

	  command = SEND_DATA;
	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
	  HAL_SPI_Transmit(&hspi1, &command, 1, 100);
	  HAL_SPI_Transmit(&hspi1, data_bytes, 3, 100);
	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);


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
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	uint8_t tx_ds_irq = (1 << 5);
	uint8_t max_rt_irq = (1 << 4);
	uint8_t rx_dr_irq = (1 << 6);

	uint8_t status_register_bits_irq = 0;

//	status_register_bits_irq |= tx_ds_irq; //TX_DS clear
//	status_register_bits_irq |= max_rt_irq;
//	status_register_bits_irq |= rx_dr_irq;
//	write_register(&hspi1, status_reg, &status_register_bits_irq, 1);
//	write_register(&hspi1, status_reg, &status_register_bits_irq, 1);
//	write_register(&hspi1, status_reg, &status_register_bits_irq, 1);



//	status_register_bits &= tx_ds;
//	status_register_bits &= max_rt;

	if(GPIO_Pin == GPIO_PIN_9) {
		read_register(&hspi1, status_reg, &status_register_bits_irq, 1);
//		printf("Sending\n");

		if (status_register_bits_irq & tx_ds_irq) {
			status_register_bits_irq |= tx_ds_irq; //TX_DS clear
			write_register(&hspi1, status_reg, &status_register_bits_irq, 1);
			printf("Data sent\n");
		}
		if (status_register_bits_irq & max_rt_irq) { //MAX_RT clear
			status_register_bits_irq |= max_rt_irq;
			write_register(&hspi1, status_reg, &status_register_bits_irq, 1);
//			printf("Max number of retransmits sent\n");
		}

		if (status_register_bits_irq & rx_dr_irq) {
			printf("rx dr\n");
			status_register_bits_irq |= rx_dr_irq;
			write_register(&hspi1, status_reg, &status_register_bits_irq, 1);
		}

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

#include "nrf2401.hpp"


volatile uint8_t rx_data_received = 0;

NRF2401 rf_module(&hspi1, GPIOB, GPIOC, GPIOA, GPIO_PIN_6, GPIO_PIN_7, GPIO_PIN_9);

NRF2401::NRF2401(SPI_HandleTypeDef *spi_handle, GPIO_TypeDef *cs_pin_port, GPIO_TypeDef *ce_pin_port,
		GPIO_TypeDef *irq_pin_port, uint16_t cs_pin, uint16_t ce_pin, uint16_t irq_pin) :
		spi_handle_{spi_handle}, cs_pin_port_{cs_pin_port}, ce_pin_port_{ce_pin_port},
		irq_pin_port_{irq_pin_port}, cs_pin_{cs_pin}, ce_pin_{ce_pin}, irq_pin_{irq_pin}
		{}


void NRF2401::write_register(uint8_t reg, uint8_t *write_data, uint8_t num_write_bytes) {
	uint8_t write_reg = write_command_prefix | reg;
	HAL_GPIO_WritePin(cs_pin_port_, cs_pin_, GPIO_PIN_RESET);
	HAL_SPI_Transmit(spi_handle_, &write_reg, 1, 100);
	HAL_SPI_Transmit(spi_handle_, write_data, num_write_bytes, 100);
	HAL_GPIO_WritePin(cs_pin_port_, cs_pin_, GPIO_PIN_SET);
}

void NRF2401::read_register(uint8_t reg, uint8_t *read_data, uint8_t num_read_bytes) {
	uint8_t read_reg = read_command_prefix | reg;
	HAL_GPIO_WritePin(cs_pin_port_, cs_pin_, GPIO_PIN_RESET);
	HAL_SPI_Transmit(spi_handle_, &read_reg, 1, 100);
	HAL_SPI_Receive(spi_handle_, read_data, num_read_bytes, 100);
	HAL_GPIO_WritePin(cs_pin_port_, cs_pin_, GPIO_PIN_SET);
}

void NRF2401::send_spi_command(uint8_t nrf2401_command) {
	HAL_GPIO_WritePin(cs_pin_port_, cs_pin_, GPIO_PIN_RESET);
	HAL_SPI_Transmit(spi_handle_, &nrf2401_command, 1, 100);
	HAL_GPIO_WritePin(cs_pin_port_, cs_pin_, GPIO_PIN_SET);
}

void NRF2401::nrf_power_on() {
	const uint8_t PWR_UP_BIT = 1;
	uint8_t config_reg_bits = 0;
	read_register(config_reg, &config_reg_bits, 1);
	config_reg_bits |= 1 << PWR_UP_BIT;
	write_register(config_reg, &config_reg_bits, 1);
	HAL_Delay(5);
}

void NRF2401::nrf_set_prim_rx() {
	const uint8_t PRIM_RX_BIT = 0;
	uint8_t config_reg_bits = 0;
	read_register(config_reg, &config_reg_bits, 1);
	if (operating_mode_ == true) {	//PRIM_RX is cleared
		config_reg_bits &= ~(1 << PRIM_RX_BIT);
	} else {				//PRIM_RX bit is set
		config_reg_bits |= 1 << PRIM_RX_BIT;
	}
	write_register(config_reg, &config_reg_bits, 1);
	HAL_Delay(5);
}

void NRF2401::enable_crc(bool two_bytes) {
	const uint8_t EN_CRC_BIT = 3;
	const uint8_t CRCO_BIT = 2;
	uint8_t config_reg_bits = 0;
	read_register(config_reg, &config_reg_bits, 1);
	config_reg_bits |= 1 << EN_CRC_BIT;
	if (two_bytes == true) {
		config_reg_bits |= 1 << CRCO_BIT;
	} else {
		config_reg_bits &= ~(1 << CRCO_BIT);
	}
	write_register(config_reg, &config_reg_bits, 1);
}


void NRF2401::set_rf_channel_frequency(uint16_t ch_freq_mhz) {
	if (ch_freq_mhz > HIGHEST_RF_CHANNEL_FREQUENCY || ch_freq_mhz < LOWEST_RF_CHANNEL_FREQUENCY) {
		return;
	}
	uint8_t rf_ch_value = ch_freq_mhz - LOWEST_RF_CHANNEL_FREQUENCY;
	printf("The rf_ch_value is %d\n", rf_ch_value);
	uint8_t rf_ch_reg_bits = 0;
	read_register(rf_ch_reg, &rf_ch_reg_bits, 1);
	rf_ch_reg_bits = rf_ch_value;
	write_register(rf_ch_reg, &rf_ch_reg_bits, 1);
}

void NRF2401::set_air_data_rate(DATA_RATE rate) {

	//TODO: Write a function for RF_PWR
	const uint8_t RF_DR_LOW_BIT = 5;
	const uint8_t RF_DR_HIGH_BIT = 3;
	uint8_t rf_setup_reg_bits = 0;

	read_register(rf_setup_reg, &rf_setup_reg_bits, 1);
	rf_setup_reg_bits |= 0x06; //0 dbm

	//determine the data rate.
	switch(rate) {
		case MBPS_ONE:
			rf_setup_reg_bits &= ~(1 << RF_DR_LOW_BIT);
			rf_setup_reg_bits &= ~(1 << RF_DR_HIGH_BIT);
			break;
		case MBPS_TWO:
			rf_setup_reg_bits &= ~(1 << RF_DR_LOW_BIT);
			rf_setup_reg_bits |= (1 << RF_DR_HIGH_BIT);
			break;
		case KBPS_250:
			rf_setup_reg_bits |= (1 << RF_DR_LOW_BIT);
			rf_setup_reg_bits &= ~(1 << RF_DR_HIGH_BIT);
			break;
		default:
			break;
	};

	write_register(rf_setup_reg, &rf_setup_reg_bits, 1);
}


void NRF2401::set_address_width(ADDR_WIDTH width) {
	const uint8_t AW_BIT_ZERO = 0;
	const uint8_t AW_BIT_ONE = 1;

	uint8_t aw_reg_bits = 0;
	read_register(aw_reg, &aw_reg_bits, 1);

	switch(width) {
		case THREE_BYTES:
			aw_reg_bits |= (1 << AW_BIT_ZERO);
			break;
		case FOUR_BYTES:
			aw_reg_bits |= (1 << AW_BIT_ONE);
			break;
		case FIVE_BYTES:
			aw_reg_bits |= (1 << AW_BIT_ONE) | (1 << AW_BIT_ZERO);
			break;
		default:
			break;
	};

	write_register(aw_reg, &aw_reg_bits, 1);
}


void NRF2401::set_auto_retransmit(uint8_t auto_retransmit_count, uint16_t auto_retransmit_delay_us) {

	if (auto_retransmit_delay_us > 4000 || auto_retransmit_delay_us == 0|| auto_retransmit_delay_us % 250) {
		printf("auto retransmit delay error\n");
		return;
	}

	if (auto_retransmit_count > 15) {
		printf("auto retransmit count error\n");
		return;
	}

	uint8_t setup_retr_reg_bits = 0;
	const uint8_t auto_retransmit_delay_step = 250;
	read_register(setup_retr_reg, &setup_retr_reg_bits, 1);

	setup_retr_reg_bits |= auto_retransmit_count; //set the arc bits.
	setup_retr_reg_bits |= (auto_retransmit_delay_us / auto_retransmit_delay_step - 1) << 4; //set ard bits.

	write_register(setup_retr_reg, &setup_retr_reg_bits, 1);
}

void NRF2401::nrf_init(uint16_t rf_ch_frequency, DATA_RATE data_rate, ADDR_WIDTH width) {
	nrf_power_on();
	enable_crc(false);
	set_rf_channel_frequency(rf_ch_frequency);
	set_air_data_rate(KBPS_250);
	set_address_width(width);
	set_auto_retransmit(10, 500);
}

void NRF2401::setup_tx_mode(uint8_t *address, uint8_t address_length_bytes) {
	operating_mode_ = true;
//	set_tx_address(address, address_length_bytes);
	nrf_set_prim_rx();
	HAL_GPIO_WritePin(ce_pin_port_, ce_pin_, GPIO_PIN_SET);
//	send_spi_command(flush_tx_fifo);
}

void NRF2401::setup_receiver_channel(uint8_t enable_auto_acknowledge_pipe_x, uint8_t enable_rx_addr_pipe_x) {
	uint8_t en_rxaddr_bits = 0;
	read_register(en_rxaddr_reg, &en_rxaddr_bits, 1);

	en_rxaddr_bits |= (1 << enable_rx_addr_pipe_x);
	write_register(en_rxaddr_reg, &en_rxaddr_bits, 1);

	uint8_t en_aa_reg_bits = 0;
	read_register(en_aa_reg, &en_aa_reg_bits, 1);

	en_aa_reg_bits |= (1 << enable_auto_acknowledge_pipe_x);
	write_register(en_aa_reg, &en_aa_reg_bits, 1);
}

void NRF2401::set_tx_address(uint8_t *address, uint8_t address_length_bytes) {
	printf("The value of address_length_bytes is %d\n", address_length_bytes);

	printf("set_tx_address\n");
	for (int i = 0; i < 5; i++) {
		printf("The byte is %d\n", address[i]);
	}

	write_register(tx_addr_reg, address, address_length_bytes);
	write_register(rx_addr_p0_reg, address, address_length_bytes);
}

void NRF2401::set_rx_address(uint8_t *address, uint8_t address_length_bytes, uint8_t data_pipe) {
	if (data_pipe > 5) {
		printf("Max data pipe exceeded\n");
		return;
	}
	uint8_t rx_addr_px_reg = rx_addr_p0_reg + data_pipe;
	write_register(rx_addr_px_reg, address, address_length_bytes);
}


void NRF2401::setup_rx_mode(uint8_t *address, uint8_t address_length_bytes, uint8_t enable_auto_acknowledge_pipe_x,
		uint8_t enable_rx_addr_pipe_x, uint8_t payload_length) {
//	setup_receiver_channel(enable_auto_acknowledge_pipe_x, enable_rx_addr_pipe_x);
//	set_rx_address(address, address_length_bytes, enable_rx_addr_pipe_x);
	set_data_pipe_payload_length(payload_length, enable_rx_addr_pipe_x);
	nrf_set_prim_rx();
	HAL_GPIO_WritePin(ce_pin_port_, ce_pin_, GPIO_PIN_SET);
}

void NRF2401::set_data_pipe_payload_length(uint8_t payload_length, uint8_t rx_data_pipe) {
	if (payload_length > 32) {
		printf("Payload length exceeded\n");
	}

	payload_length_ = payload_length;
	uint8_t rx_pw_px_reg = rx_pw_p0_reg;
	rx_pw_px_reg += rx_data_pipe;
	write_register(rx_pw_px_reg, &payload_length, 1);
}

void NRF2401::set_status(uint8_t bits) {
	write_register(status_reg, &bits, 1);
}

void NRF2401::get_status(uint8_t &status_reg) {
	read_register(status_reg, &status_reg, 1);
}

void NRF2401::check_fifo_status(uint8_t &fifo_reg) {
	read_register(fifo_status_reg, &fifo_reg, 1);
}

void NRF2401::receive_data_from_rx_fifo(uint8_t *rx_data) {
	HAL_GPIO_WritePin(cs_pin_port_, cs_pin_, GPIO_PIN_RESET);
	HAL_SPI_Transmit(spi_handle_, &read_rx_fifo, 1, 100);
	HAL_SPI_Transmit(spi_handle_, rx_data, payload_length_, 100);
	HAL_GPIO_WritePin(cs_pin_port_, cs_pin_, GPIO_PIN_SET);

	printf("The payload length is %d\n", payload_length_);
}

void NRF2401::send_data_tx_to_fifo(uint8_t *tx_data, uint8_t payload_length) {
	HAL_GPIO_WritePin(cs_pin_port_, cs_pin_, GPIO_PIN_RESET);
	HAL_SPI_Transmit(spi_handle_, &write_tx_fifo, 1, 100);
	HAL_SPI_Transmit(spi_handle_, tx_data, payload_length, 100);
	HAL_GPIO_WritePin(cs_pin_port_, cs_pin_, GPIO_PIN_SET);
}

bool NRF2401::get_operating_mode() {
	return operating_mode_;
}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	printf("Hi\n");
	if(GPIO_Pin == GPIO_PIN_9) {
		uint8_t rx_dr_irq = (1 << 6);
		uint8_t tx_ds_irq = (1 << 5);
		uint8_t max_rt_irq = (1 << 4);

		uint8_t status_register_byte = 0;
		printf("Entering interrupt\n");
		if (rf_module.get_operating_mode() == true) { //tx mode

			rf_module.read_register(status_reg, &status_register_byte, 1);
			if (status_register_byte & tx_ds_irq) {
				status_register_byte |= tx_ds_irq; //TX_DS clear
				printf("TX_DS clear\n");
			} else {
				 status_register_byte |= max_rt_irq;
				 printf("Max RT\n");
			}
			rf_module.write_register(status_reg, &status_register_byte, 1);
		} else {						//false
			printf("Receiver\n");
			rf_module.read_register(status_reg, &status_register_byte, 1);
			uint8_t rx_data[3] = {1, 1, 1};
			if (status_register_byte & rx_dr_irq) { 	//data is ready.
				rf_module.receive_data_from_rx_fifo(rx_data);
				printf("The first I need to do is %d %d %d\n", rx_data[0], rx_data[1], rx_data[2]);
				status_register_byte |= rx_dr_irq;
			}

			if (status_register_byte & tx_ds_irq) {
				status_register_byte |= tx_ds_irq; //TX_DS clear
				printf("TX_DS clear\n");
			} else if (status_register_byte & max_rt_irq) {
				 status_register_byte |= max_rt_irq;

				 printf("Max RT\n");
			}

			rf_module.write_register(status_reg, &status_register_byte, 1);
			rf_module.read_register(status_reg, &status_register_byte, 1);
			printf("Register is %d\n", status_register_byte);
		}

	}
}

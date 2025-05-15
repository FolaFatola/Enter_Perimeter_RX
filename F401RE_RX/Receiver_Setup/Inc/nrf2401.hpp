#ifndef NRF2401_HPP
#define NRF2401_HPP

#include "spi.h"
#include "gpio.h"
#include "nrf2401.hpp"
#include "nrf_constants.hpp"
#include <stdio.h>


typedef enum DATA_RATE {
	MBPS_ONE,
	MBPS_TWO,
	KBPS_250
} DATA_RATE;

typedef enum ADDR_WIDTH {
	THREE_BYTES,
	FOUR_BYTES,
	FIVE_BYTES
} ADDR_WIDTH;

extern volatile uint8_t rx_data_received;
extern volatile uint8_t tx_interrupt;

class NRF2401;

extern NRF2401 rf_module;

class NRF2401 {
public:
	NRF2401(SPI_HandleTypeDef *spi_handle, GPIO_TypeDef *cs_pin_port, GPIO_TypeDef *ce_pin_port,
			GPIO_TypeDef *irq_pin_port, uint16_t cs_pin, uint16_t ce_pin,
			uint16_t irq_pin);

	void nrf_init(uint16_t rf_ch_frequency, DATA_RATE data_rate, ADDR_WIDTH width);
	void nrf_reset();
	void setup_tx_mode(uint8_t *address, uint8_t address_length_bytes);
	void setup_rx_mode(uint8_t *address, uint8_t address_length_bytes,
			uint8_t enable_rx_addr_pipe_x, uint8_t payload_length);



	void nrf_power_on();
	void nrf_set_prim_rx();
	void enable_crc(bool two_bytes);
	void set_rf_channel_frequency(uint16_t ch_freq_mhz);
	void set_air_data_rate(DATA_RATE rate);
	void set_address_width(ADDR_WIDTH width);
	void set_tx_address(uint8_t *address, uint8_t address_length_bytes);
	void set_rx_address(uint8_t *address, uint8_t address_length_bytes, uint8_t data_pipe);
	void set_auto_retransmit(uint8_t auto_retransmit_count, uint16_t auto_retransmit_delay_us);
	void setup_receiver_channel(uint8_t enable_rx_addr_pipe_x);
	void set_data_pipe_payload_length(uint8_t payload_length, uint8_t rx_data_pipe);
	void set_status(uint8_t bits);
	void get_status(uint8_t &status_reg);

	void check_fifo_status(uint8_t &fifo_reg);

	void send_data_tx_to_fifo(uint8_t *tx_data, uint8_t payload_length);
	void receive_data_from_rx_fifo(uint8_t *rx_data);
	bool get_operating_mode();

	//HELPERS
	void write_register(uint8_t reg, uint8_t *write_data, uint8_t num_write_bytes);
	void read_register(uint8_t reg, uint8_t *read_data, uint8_t num_read_bytes);

	void send_spi_command(uint8_t nrf2401_command);

private:
	SPI_HandleTypeDef *spi_handle_;
	GPIO_TypeDef* cs_pin_port_;
	GPIO_TypeDef* ce_pin_port_;
	GPIO_TypeDef* irq_pin_port_;
	uint16_t cs_pin_;
	uint16_t ce_pin_;
	uint16_t irq_pin_;

	bool operating_mode_;
	uint8_t payload_length_;
};

#endif

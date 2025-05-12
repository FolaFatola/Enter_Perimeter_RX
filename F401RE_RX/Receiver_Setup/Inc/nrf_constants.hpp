#ifndef INC_NRF_CONSTANTS_HPP_
#define INC_NRF_CONSTANTS_HPP_

#include <stdint.h>

/*
 * REGISTER:
 */
constexpr uint8_t config_reg = 0x00;
constexpr uint8_t en_aa_reg = 0x01;
constexpr uint8_t aw_reg = 0x03;
constexpr uint8_t setup_retr_reg = 0x04;
constexpr uint8_t rf_ch_reg = 0x05;
constexpr uint8_t rf_setup_reg = 0x06;
constexpr uint8_t rx_addr_p0_reg = 0x0A;
constexpr uint8_t rx_addr_p1_reg = 0x0B;
constexpr uint8_t rx_addr_p2_reg = 0x0C;
constexpr uint8_t rx_addr_p3_reg = 0x0D;
constexpr uint8_t rx_addr_p4_reg = 0x0E;
constexpr uint8_t rx_addr_p5_reg = 0x0F;
constexpr uint8_t tx_addr_reg = 0x10;
constexpr uint8_t fifo_status_reg = 0x17;
constexpr uint8_t dynpd_reg = 0x1C;
constexpr uint8_t feature_reg = 0x1D;
constexpr uint8_t status_reg = 0x07;
constexpr uint8_t en_rxaddr_reg = 0x02;


/*
 * COMMANDS:
 */

//for read and write commands
constexpr uint8_t write_command_prefix = 0x20;
constexpr uint8_t read_command_prefix = 0x0;


constexpr uint8_t read_rx_fifo = 0x61;		//read data from the RX_FIFO
constexpr uint8_t write_tx_fifo = 0xA0;		//write data to the TX_FIFO
constexpr uint8_t flush_tx_fifo = 0xE1;		//flush TX FIFO
constexpr uint8_t flush_rx_fifo = 0xE2;		//flush RX FIFO

//lowest and highest rf channel operational frequency.
constexpr uint16_t LOWEST_RF_CHANNEL_FREQUENCY = 2400;
constexpr uint16_t HIGHEST_RF_CHANNEL_FREQUENCY = 2525;

constexpr uint8_t rx_pw_p0_reg = 0x11;
constexpr uint8_t rx_pw_p1_reg = 0x12;
constexpr uint8_t rx_pw_p2_reg = 0x13;
constexpr uint8_t rx_pw_p3_reg = 0x14;
constexpr uint8_t rx_pw_p4_reg = 0x15;
constexpr uint8_t rx_pw_p5_reg = 0x16;


//ENAA registers (enable auto acknowledgement)
//constexpr uint8_t ENAA_P0 = (1 << 0);
//constexpr uint8_t ENAA_P1 = (1 << 1);
//constexpr uint8_t ENAA_P2 = (1 << 2);
//constexpr uint8_t ENAA_P3 = (1 << 3);
//constexpr uint8_t ENAA_P4 = (1 << 4);
//constexpr uint8_t ENAA_P5 = (1 << 5);




#endif /* INC_NRF_CONSTANTS_HPP_ */

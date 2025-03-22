/*
 * rtc.hpp
 *
 *  Created on: Mar 14, 2025
 *      Author: Fola Fatola
 */

#ifndef SECURITY_INC_RTC_HPP_
#define SECURITY_INC_RTC_HPP_

#include "time.hpp"
#include "rtc_error_codes.hpp"
#include "stdint.h"
#include "stm32f4xx_hal.h"

#define DEVICE_ADDR 0xD0 			//device address of the ds1307

//Register addresses within the Ds1307 RTC
#define SECONDS_REG 	0x00
#define MINUTES_REG 	0x01
#define HOURS_REG 		0x02
#define WEEK_DAY_REG 	0x03
#define DATE_DAY_REG	0x04
#define MONTH_REG		0x05
#define	YEAR_REG		0x06
#define	CONTROL_REG		0x07

#define MEM_ADDR_SIZE 	0x01		//8-bit or byte addresses

#define TIMEOUT 		100 		//100 ms timeout


class Time_RTC {
public:
	Time_RTC(I2C_HandleTypeDef *i2c_handle, bool use_military_time,
			uint8_t seconds, uint8_t minutes,
			uint8_t hours, uint8_t week_day,
			uint8_t date_day, uint8_t month, uint8_t year);
	RTC_Status_E rtc_set_reg(uint8_t register_address, uint8_t transmit_data);
	RTC_Status_E rtc_init();
	RTC_Status_E rtc_read_reg(uint8_t register_address, uint8_t *recv_data);
	RTC_Status_E rtc_get_time();

private:
	I2C_HandleTypeDef *i2c_handle_;
	bool military_time_;
	bool time_init_;
	TimeManager time_;
};



#endif /* SECURITY_INC_RTC_HPP_ */

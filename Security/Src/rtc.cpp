/*
 * rtc.cpp
 *
 *  Created on: Mar 14, 2025
 *      Author: Fola Fatola
 */

#include "rtc.hpp"



Time_RTC::Time_RTC(I2C_HandleTypeDef *i2c_handle, bool use_military_time,
		uint8_t seconds, uint8_t minutes,
		uint8_t hours, uint8_t week_day,
		uint8_t date_day, uint8_t month, uint8_t year) :
	i2c_handle_{i2c_handle}, military_time_{use_military_time}, time_init_{true},
	time_{TimeManager(seconds, minutes, hours, week_day, date_day, month, year, use_military_time)}{

	}


RTC_Status_E Time_RTC::rtc_set_reg(uint8_t register_address, uint8_t transmit_data) {
	if (i2c_handle_ == nullptr) {
		return RTC_NULL_I2C_HANDLE;
	}

	if (HAL_I2C_Mem_Write(i2c_handle_, DEVICE_ADDR, register_address, MEM_ADDR_SIZE, &transmit_data, 1, TIMEOUT) != HAL_OK) {
		return RTC_INIT_FAIL;
	}

	return RTC_OK;
}

RTC_Status_E Time_RTC::rtc_read_reg(uint8_t register_address, uint8_t *recv_data) {
	if (i2c_handle_ == nullptr) {
		return RTC_NULL_I2C_HANDLE;
	}

	if (HAL_I2C_Mem_Read(i2c_handle_, DEVICE_ADDR, register_address, MEM_ADDR_SIZE, recv_data, 1, TIMEOUT) != HAL_OK) {
		return RTC_INIT_FAIL;
	}

	return RTC_OK;
}


RTC_Status_E Time_RTC::rtc_init() {
	RTC_Status_E status = RTC_OK;

	status = time_.convert_bcd_time_to_decimal();

	if (status != RTC_OK) {
		return status;
	}

	TimeUnit time = time_.getTimeUnit();

	Time_TypeDef decimal_time[7] = {time.seconds_, time.minutes_, time.hours_,
			time.week_day_, time.date_day_, time.month_, time.year_};

	uint8_t time_registers[7] = {SECONDS_REG, MINUTES_REG, HOURS_REG,
			WEEK_DAY_REG, DATE_DAY_REG, MONTH_REG, YEAR_REG};

	for (int time_unit_idx = 0; time_unit_idx < sizeof(Time_TypeDef); ++time_unit_idx) {
		status = rtc_set_reg(time_registers[time_unit_idx], decimal_time[time_unit_idx].time_value);
		if (status != RTC_OK) {
			return status;
		}
	}
	return status;
}



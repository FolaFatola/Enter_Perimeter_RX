/*
 * rtc.cpp
 *
 *  Created on: Mar 14, 2025
 *      Author: Fola Fatola
 */

#include "rtc.hpp"



Time_RTC::Time_RTC(I2C_HandleTypeDef *i2c_handle, bool use_military_time) :
time_{0}, i2c_handle_{i2c_handle}, military_time_{use_military_time}, time_init_{true}{}


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

RTC_Status_E Time_RTC::rtc_init(Time_TypeDef* curr_time) {
	time_->second_ = curr_time->second_;
	time_->minute_ = curr_time->minute_;
	time_->hour_  = curr_time->hour_;
	time_->week_day_ = curr_time->week_day_;
	time_->date_day_ = curr_time->date_day_;
	time_->month_ = curr_time->month_;
	time_->year_ = curr_time->year_;

	RTC_Status_E status = RTC_OK;

	uint8_t status_msg = 0;

	status_msg |= rtc_set_reg(SECONDS_REG, time_->second_);
	status_msg |= rtc_set_reg(MINUTES_REG, time_->minute_);
	status_msg |= rtc_set_reg(HOURS_REG, time_->hour_);
	status_msg |= rtc_set_reg(WEEK_DAY_REG, time_->week_day_);
	status_msg |= rtc_set_reg(DATE_DAY_REG, time_->date_day_);
	status_msg |= rtc_set_reg(MONTH_REG, time_->month_);
	status_msg |= rtc_set_reg(YEAR_REG, time_->year_);

	if (status_msg == 1) {
		status = RTC_NULL_I2C_HANDLE;
	} else if (status_msg == 2) {
		status = RTC_INIT_FAIL;
	}

	return status;
}


RTC_Status_E Time_RTC::rtc_get_time(Time_TypeDef* curr_time) {

}



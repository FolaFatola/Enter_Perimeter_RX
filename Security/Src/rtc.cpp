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


RTC_Status_E Time_RTC::from_decimal_to_bcd(Time_TypeDef &time) {
    if (decimal_number > 99) {
        return RTC_DATA_TOO_LARGE;
    }

    uint8_t decimal_time[7] = {time.second_, time.minute_, time.hour_,
    		time.week_day_, time.date_day_, time.month_, time.year_};

//    if (time.seconds_ > 60) {
//
//    } else if (time.minute_ > 60) {
//
//    } else if (use_military_time == true && time.hour_ > 24) {
//
//    } else if (use_military_time == false && time.hour_ > 12) {
//
//    } else if (time.week_day_ > 7) {
//
//    } else if (time.date_day_) { //honestly, just be responsible lmao
//
//    } else if (time.month_ > 12) {
//
//    } //no check for year xD





    uint8_t ones = decimal_number % 10;
    decimal_number /= 10;
    uint8_t tens = decimal_number % 10;
    bcd_ret_val = ones + (tens << 4);
    return RTC_OK;
}

RTC_Status_E Time_RTC::from_bcd_to_decimal(Time_TypeDef &time) {
    if (bcd_number > 255) {
        return RTC_DATA_TOO_LARGE;
    }
    uint8_t ones = bcd_number & 0x0F;
    uint8_t tens = (bcd_number & 0xF0) >> 4;
    decimal_ret_val = ones + 10 * tens;
    return RTC_OK;
}

RTC_Status_E Time_RTC::rtc_init(Time_TypeDef* curr_time) {
	uint8_t status_msg = 0;

	//convert each time unit to bcd format for input into RTC registers
	status_msg += from_decimal_to_bcd(curr_time->second_);
	status_msg += from_decimal_to_bcd(curr_time->minute_);
	status_msg += from_decimal_to_bcd(curr_time->hour_);
	status_msg += from_decimal_to_bcd(curr_time->week_day_);
	status_msg += from_decimal_to_bcd(curr_time->date_day_);
	status_msg += from_decimal_to_bcd(curr_time->month_);
	status_msg += from_decimal_to_bcd(curr_time->year_);

	uint8_t current_time[7] = {curr_time->second_, curr_time->minute_, curr_time->hour_,
			curr_time->week_day_, curr_time->date_day_, curr_time->month_, curr_time->year_};

	for (int time_idx = 0; time_idx < sizeof(Time_TypeDef); ++time_idx) {
//		if ()
	}

	if (status_msg < 0) {
		return RTC_DATA_TOO_LARGE;
	}

	//set the time bcd conversions were successful.
	time_->second_ = curr_time->second_;
	time_->minute_ = curr_time->minute_;
	time_->hour_  = curr_time->hour_;
	time_->week_day_ = curr_time->week_day_;
	time_->date_day_ = curr_time->date_day_;
	time_->month_ = curr_time->month_;
	time_->year_ = curr_time->year_;

	status_msg |= rtc_set_reg(SECONDS_REG, time_->second_);
	status_msg |= rtc_set_reg(MINUTES_REG, time_->minute_);
	status_msg |= rtc_set_reg(HOURS_REG, time_->hour_);
	status_msg |= rtc_set_reg(WEEK_DAY_REG, time_->week_day_);
	status_msg |= rtc_set_reg(DATE_DAY_REG, time_->date_day_);
	status_msg |= rtc_set_reg(MONTH_REG, time_->month_);
	status_msg |= rtc_set_reg(YEAR_REG, time_->year_);

	if (status_msg == 1) {
		return RTC_NULL_I2C_HANDLE;
	} else if (status_msg == 2) {
		return RTC_INIT_FAIL;
	}

	return RTC_OK;
}


RTC_Status_E Time_RTC::rtc_get_time(Time_TypeDef* curr_time) {

}



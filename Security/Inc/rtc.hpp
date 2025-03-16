/*
 * rtc.hpp
 *
 *  Created on: Mar 14, 2025
 *      Author: Fola Fatola
 */

#ifndef SECURITY_INC_RTC_HPP_
#define SECURITY_INC_RTC_HPP_

#include "rtc_error_codes.hpp"
#include "stdint.h"

//Register addresses within the Ds1307 RTC
#define SECONDS_REG 	0x00
#define MINUTES_REG 	0x01
#define HOURS_REG 		0x02
#define WEEK_DAY_REG 	0x03
#define DATE_DAY_REG	0x04
#define MONTH_REG		0x05
#define	YEAR_REG		0x06
#define	CONTROL_REG		0x07


typedef struct Time_TypeDef {
	uint8_t second_;
	uint8_t minute_;
	uint8_t hour_;
	uint8_t day_;
	uint8_t month_;
	uint8_t year_;
}Time_TypeDef;


class Time_RTC {
public:
	Time_RTC(bool military_time);
	RTC_Status_E rtc_set_reg(uint8_t *register_address);
	RTC_Status_E rtc_read_reg(uint8_t *register_address);
	RTC_Status_E rtc_init(Time_TypeDef* curr_time);
	RTC_Status_E rtc_get_time(Time_TypeDef* curr_time);

private:
	Time_TypeDef time_;
	bool military_time_;
	bool time_init_;
};








#endif /* SECURITY_INC_RTC_HPP_ */

/*
 * helper.hpp
 *
 *  Created on: Mar 18, 2025
 *      Author: folafatola
 */

#ifndef INC_TIME_UNIT_HPP_
#define INC_TIME_UNIT_HPP_

#include <stdint.h>
#include "rtc_error_codes.hpp"

#define JANUARY 1
#define FEBRUARY 2
#define MARCH 3
#define APRIL 4
#define MAY 5
#define JUNE 6
#define JULY 7
#define AUGUST 8
#define SEPTEMBER 9
#define OCTOBER 10
#define NOVEMBER 11
#define DECEMBER 12


/*
 * time_type details!!!
 * bit 0: seconds
 * bit 1: minutes
 * bit 2: hours
 * bit 3: week day
 * bit 4: date day
 * bit 5: month
 * bit 6: year
 * bit 7:
 */

/*
 * These are the bits representing the different kinds of time
 */
constexpr uint8_t seconds_type = 1 << 0;
constexpr uint8_t minutes_type = 1 << 1;
constexpr uint8_t hours_type = 1 << 2;
constexpr uint8_t week_day_type = 1 << 3;
constexpr uint8_t date_day_type = 1 << 4;
constexpr uint8_t month_type = 1 << 5;
constexpr uint8_t year_type = 1 << 6;

typedef struct Time_TypeDef {
	uint8_t time_type;				//a bit-field representing the current time type
	int time_value;					//The actual time unit measurement.
	int max_time;				//the max time that can be represented by the time type.
}Time_TypeDef;

typedef struct TimeUnit {
	Time_TypeDef seconds_;
	Time_TypeDef minutes_;
	Time_TypeDef hours_;
	Time_TypeDef week_day_;
	Time_TypeDef date_day_;
	Time_TypeDef month_;
	Time_TypeDef year_;
}TimeUnit;

class TimeManager {
public:
	TimeManager(uint8_t seconds, uint8_t minutes, uint8_t hours, uint8_t week_day, uint8_t date_day, uint8_t month, int year, bool use_military_time);
	TimeUnit getTimeUnit() { return passage_of_time_;}
	RTC_Status_E convert_decimal_time_to_bcd();
	RTC_Status_E convert_bcd_time_to_decimal();

private:
	TimeUnit passage_of_time_;
};


#endif /* INC_TIME_UNIT_HPP_ */

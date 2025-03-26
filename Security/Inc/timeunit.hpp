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

#define SUNDAY 1
#define MONDAY 2
#define TUESDAY 3
#define WEDNESDAY 4
#define THUSDAY 5
#define FRIDAY 6
#define SATURDAY 7

constexpr int millennium = 2000;

typedef enum Time_Type_E {
	SECONDS,
	MINUTES,
	HOURS,
	WEEK_DAY,
	DATE_DAY,
	MONTH,
	YEAR
} Time_Type_E;


typedef struct TimeUnit {
	Time_Type_E time_type;				//a bit-field representing the current time type
	uint8_t time_value;					//The actual time unit measurement.
	uint8_t max_time;				//the max time that can be represented by the time type.
}TimeUnit;

typedef struct TimeSpan {
	TimeUnit seconds_;
	TimeUnit minutes_;
	TimeUnit hours_;
	TimeUnit week_day_;
	TimeUnit date_day_;
	TimeUnit month_;
	TimeUnit year_;
} TimeSpan;

class TimeManager {
public:
	TimeManager(uint8_t seconds, uint8_t minutes, uint8_t hours, uint8_t week_day, uint8_t date_day, uint8_t month, int year, bool use_military_time);
	TimeSpan getTimeSpan() { return passage_of_time_;}
	void setTime(uint8_t seconds, uint8_t minutes, uint8_t hours, uint8_t week_day, uint8_t date_day, uint8_t month, int year);
	RTC_Status_E convert_decimal_time_to_bcd();
	RTC_Status_E convert_bcd_time_to_decimal();

private:
	TimeSpan passage_of_time_;
};


#endif /* INC_TIME_UNIT_HPP_ */

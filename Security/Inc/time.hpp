/*
 * helper.hpp
 *
 *  Created on: Mar 18, 2025
 *      Author: folafatola
 */

#ifndef INC_TIME_HPP_
#define INC_TIME_HPP_

#include <stdint.h>
#include <tuple>
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

typedef struct Time_TypeDef {
	uint8_t time_type;				//a bit-field representing the current time type
	int time_data;					//The actual time unit measurement.
	uint8_t max_time;				//the max time that can be represented by the time type.
}Time_TypeDef;

typedef struct TimeUnit {
	Time_TypeDef seconds;
	Time_TypeDef minutes;
	Time_TypeDef hours;
	Time_TypeDef week_day;
	Time_TypeDef day_day;
	Time_TypeDef month;
	Time_TypeDef year;
}TimeUnit;

class TimeManager {
public:


private:
	TimeData passage_of_time;

};


#endif /* INC_TIME_HPP_ */

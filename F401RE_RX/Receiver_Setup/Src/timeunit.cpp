/*
 * helper.cpp
 *
 *  Created on: Mar 18, 2025
 *      Author: folafatola
 */


#include "timeunit.hpp"
#include <climits>


TimeManager::TimeManager(uint8_t seconds, uint8_t minutes, uint8_t hours, uint8_t week_day, uint8_t date_day, uint8_t month, uint8_t year, bool use_twenty_four_hour_time) {
	passage_of_time_.seconds_.time_type = SECONDS;
	passage_of_time_.seconds_.time_value = seconds;
	passage_of_time_.seconds_.max_time = 60;

	passage_of_time_.minutes_.time_type = MINUTES;
	passage_of_time_.minutes_.time_value = minutes;
	passage_of_time_.minutes_.max_time = 60;

	passage_of_time_.hours_.time_type = HOURS;
	passage_of_time_.hours_.time_value = hours;

	if (use_twenty_four_hour_time) {
		passage_of_time_.hours_.max_time = 24;
	} else {
		passage_of_time_.hours_.max_time = 12;
	}

	passage_of_time_.week_day_.time_type = WEEK_DAY;
	passage_of_time_.week_day_.time_value = week_day;
	passage_of_time_.week_day_.max_time = 7;

	passage_of_time_.month_.time_type = MONTH;
	passage_of_time_.month_.time_value = month;
	passage_of_time_.month_.max_time = 12;


	passage_of_time_.date_day_.time_type = DATE_DAY;
	passage_of_time_.date_day_.time_value = date_day;

	if (month == JANUARY || month == MARCH
	 || month == MAY || month == JULY || month == AUGUST
	 || month == OCTOBER || month == DECEMBER){
		passage_of_time_.date_day_.max_time = 31;
	} else if (month == FEBRUARY) {							//We are NOT, doing leap year
		passage_of_time_.date_day_.max_time = 28;
	} else {
		passage_of_time_.date_day_.max_time = 30;
	}

	passage_of_time_.year_.time_type = YEAR;
	passage_of_time_.year_.time_value = year;
	passage_of_time_.year_.max_time = 99;
}


void TimeManager::setTime(uint8_t seconds, uint8_t minutes, uint8_t hours, uint8_t week_day, uint8_t date_day, uint8_t month, uint8_t year) {
	passage_of_time_.seconds_.time_value = seconds;
	passage_of_time_.minutes_.time_value = minutes;
	passage_of_time_.hours_.time_value = hours;
	passage_of_time_.week_day_.time_value = week_day;
	passage_of_time_.date_day_.time_value = date_day;
	passage_of_time_.month_.time_value = month;
	passage_of_time_.year_.time_value = year;
}

RTC_Status_E TimeManager::convert_decimal_time_to_bcd() {

	TimeUnit* decimal_time[7] = {&passage_of_time_.seconds_, &(passage_of_time_.minutes_), &passage_of_time_.hours_,
    		&passage_of_time_.week_day_, &passage_of_time_.date_day_, &passage_of_time_.month_, &passage_of_time_.year_};

    for (int tim_unit_idx = 0; tim_unit_idx < 7; ++tim_unit_idx) {
    	if (decimal_time[tim_unit_idx]->time_value > decimal_time[tim_unit_idx]->max_time) {
    		return RTC_DATA_TOO_LARGE;
    	}

    	//TODO: add century and millenia calculations.
    	uint8_t ones = decimal_time[tim_unit_idx]->time_value % 10;
		decimal_time[tim_unit_idx]->time_value /= 10;
		uint8_t tens = decimal_time[tim_unit_idx]->time_value % 10;
		decimal_time[tim_unit_idx]->time_value = ones + (tens << 4);
    }

    return RTC_OK;
}

RTC_Status_E TimeManager::convert_bcd_time_to_decimal() {

    TimeUnit* bcd_time[7] = {&passage_of_time_.seconds_, &(passage_of_time_.minutes_), &passage_of_time_.hours_,
    		&passage_of_time_.week_day_, &passage_of_time_.date_day_, &passage_of_time_.month_, &passage_of_time_.year_};

    for (int tim_unit_idx = 0; tim_unit_idx < 7; ++tim_unit_idx) {
    	uint8_t ones = bcd_time[tim_unit_idx]->time_value & 0x0F;
    	uint8_t tens = (bcd_time[tim_unit_idx]->time_value & 0xF0) >> 4;
    	bcd_time[tim_unit_idx]->time_value = tens * 10 + ones;
    }

    return RTC_OK;
}

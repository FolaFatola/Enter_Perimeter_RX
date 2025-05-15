/*
 * rtc_error_codes.hpp
 *
 *  Created on: Mar 15, 2025
 *      Author: folafatola
 */

#ifndef INC_RTC_ERROR_CODES_HPP_
#define INC_RTC_ERROR_CODES_HPP_

typedef enum RTC_Status_E {
	RTC_OK,
	RTC_NULL_I2C_HANDLE,
	RTC_INIT_FAIL,
	RTC_RECV_FAIL,
	RTC_DATA_TOO_LARGE
} RTC_Status_E;



#endif /* INC_RTC_ERROR_CODES_HPP_ */

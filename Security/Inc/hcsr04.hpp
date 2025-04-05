/*
 * hcsr04.hpp
 *
 *  Created on: Mar 27, 2025
 *      Author: folafatola
 */

#ifndef INC_HCSR04_HPP_
#define INC_HCSR04_HPP_

#include "stm32f4xx_hal.h"

constexpr double speed_of_sound_cm_ms = 34.300;

//Parameters needed to capture the distance of an object
typedef struct DistCaptParams {
	bool recv_first_edge_;			//Has the rising edge on the echo pin been captured/received?
	uint16_t ic_val1_;				//timer value at which the rising edge of the echo pin is received.
	uint16_t ic_val2_;				//timer value at which the falling edge of the echo pin is received.
} DistCaptParams;


class HC_SR04 {
public:

	/*
	 * Sets up peripherals and data needed for sensor driver.
	 *
	 * Param(s):
	 * Trig_Pin_Port -> Port selected for the trig pin on the HCSR04 sensor.
	 * Trig Pin -> Pin selected for the trig pin on the HCSR04 sensor.
	 * input_capture_timer -> timer used for capturing the toggle of the echo pin on the sensor.
	 * delay timer -> timer used to create a >= 10 microseconds delay.
	 *
	 * return value: None
	 */
	HC_SR04(GPIO_TypeDef* Trig_Pin_Port, uint16_t Trig_Pin, TIM_HandleTypeDef *input_capture_timer, TIM_HandleTypeDef *delay_timer);


	/*
	 * Function to create a delay in microseconds
	 *
	 * Param(s):
	 * microseconds -> microseconds of delay that you want.
	 *
	 * return value: None
	 */
	void delay_microseconds(uint8_t microseconds);


	/*
	 * Sends a TTL trig pin pulse to the HCSR04 sensor in order to start receiving distance data.
	 *
	 * return value: None
	 */
	void send_trig_pulse();

	/*
	 * Updates the parameters needed to calculate the distance of an object.
	 *
	 * Param(s):
	 * input_params -> reference to an instance of the struct DistCaptParams.
	 *
	 * return value: None
	 */
	void update_dist_capt_params(volatile DistCaptParams &input_params);


	/*
	 * Gets the distance of an object from the sensor in cm.
	 *
	 * Param(s):
	 * input_params -> reference to an instance of the struct DistCaptParams.
	 *
	 * return value: Double -> the distance of an object from the sensor in cm.
	 */
	double getDistance(volatile DistCaptParams &input_params);

private:
	GPIO_TypeDef* Trig_Pin_Port_;		//HCSR04 trig pin port.
	uint16_t Trig_Pin_;					//HCSR04 trig pin.

	DistCaptParams distance_capt_params_;	//Parameters used to calculate the distance.

	TIM_HandleTypeDef *ic_timer_;			//timer used for capturing the echo pin toggle inputs
	TIM_HandleTypeDef *delay_timer_;		//timer used to calculate delay in microseconds

	double distance_;						//distance of an object in cm.
};




#endif /* INC_HCSR04_HPP_ */

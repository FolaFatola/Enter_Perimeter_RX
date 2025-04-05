/*
 * hcsr04.cpp
 *
 *  Created on: Mar 27, 2025
 *      Author: folafatola
 */


#include "hcsr04.hpp"

/*
 *
 * Timer 1, we have a 10 KhZ timer frequency with a 100 us period. The counter period is 6400. We can use this to get delay in microseconds up to 100.
 *
 * Timer 2, we have a 100 Hz signal timer freq. We want a period of 10 milliseconds because the speed of sound is 34.3 cm / ms. To get a range of 343 m tracking,
 * we need 10 milliseconds. 64 ticks per 100 milliseconds.
 *
 *
 */


HC_SR04::HC_SR04(GPIO_TypeDef* Trig_Pin_Port, uint16_t Trig_Pin,
		TIM_HandleTypeDef *input_capture_timer, TIM_HandleTypeDef *delay_timer) :
		Trig_Pin_Port_{Trig_Pin_Port}, Trig_Pin_{Trig_Pin}, distance_capt_params_{0}, ic_timer_{input_capture_timer},
		delay_timer_{delay_timer}, distance_{0}
{}


void HC_SR04::delay_microseconds(uint8_t microseconds) {
	uint8_t microsecond_cnt_period = 64;
	uint16_t target_cnt_value = microseconds * microsecond_cnt_period;
	if (target_cnt_value > delay_timer_->Instance->ARR) {
		return;
	}

	delay_timer_->Instance->CNT = 0; //reset the count register.
	while(delay_timer_->Instance->CNT < target_cnt_value){}
}

void HC_SR04::send_trig_pulse() {
	  HAL_GPIO_WritePin(Trig_Pin_Port_, Trig_Pin_, GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(Trig_Pin_Port_, Trig_Pin_, GPIO_PIN_SET);
	  delay_microseconds(10);
	  HAL_GPIO_WritePin(Trig_Pin_Port_, Trig_Pin_, GPIO_PIN_RESET);
}


void HC_SR04::update_dist_capt_params(volatile DistCaptParams &input_params) {
	distance_capt_params_.recv_first_edge_ = input_params.recv_first_edge_;
	distance_capt_params_.ic_val1_ = input_params.ic_val1_;
	distance_capt_params_.ic_val2_ = input_params.ic_val2_;
}

double HC_SR04::getDistance(volatile DistCaptParams &input_params) {
	send_trig_pulse();
	HAL_Delay(100);
	update_dist_capt_params(input_params);

	uint16_t difference = 0;
	double cnt_period_milliseconds = 10.0;
	if (distance_capt_params_.ic_val2_ > distance_capt_params_.ic_val1_) {
		difference = distance_capt_params_.ic_val2_ - distance_capt_params_.ic_val1_;
	} else if (distance_capt_params_.ic_val1_ > distance_capt_params_.ic_val2_) {
		difference = (distance_capt_params_.ic_val2_ - 0) + (ic_timer_->Instance->ARR  - distance_capt_params_.ic_val1_);
	}

	double granularity_ms = cnt_period_milliseconds / ic_timer_->Instance->ARR; //granularity of 0.15625 milliseconds per tick.
	double milliseconds = (double)difference * granularity_ms;
	distance_ = milliseconds * speed_of_sound_cm_ms;

	return distance_;
}



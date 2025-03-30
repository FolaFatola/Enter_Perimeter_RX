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




//
//
//void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
//
//}

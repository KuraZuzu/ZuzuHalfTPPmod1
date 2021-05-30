/*!
* @file my_timer.cpp v1.0
* @Copyright © 2021 Kazushi Kurasawa
* @date 2021.03.31
*
* Released under the MIT license.
* see https://opensource.org/licenses/MIT
*/

#include "my_timer.h"

/**
 * @fn timer_counter
 */
volatile uint32_t my_timer::time_us_count = 0;


/**
 * @fn timer wait function.
 */
void my_timer::wait_us(uint32_t time_us) {
    time_us_count = 0;
    HAL_TIM_Base_Start_IT(&htim6);
    while (time_us_count < time_us) {
    }
    HAL_TIM_Base_Stop_IT(&htim6);
}
/*!
* @file my_timer.h v1.0
* @Copyright © 2021 Kazushi Kurasawa
* @date 2021.03.31
*
* Released under the MIT license.
* see https://opensource.org/licenses/MIT
*/

#ifndef ZUZUHALFTPPMOD1_MY_TIMER_H
#define ZUZUHALFTPPMOD1_MY_TIMER_H

#include "arm_math.h"
#include "stm32f4xx_hal.h"
#include "tim.h"


namespace my_timer {

    extern volatile uint32_t time_us_count;

    void wait_us(uint32_t time_us);
}

#endif //ZUZUHALFTPPMOD1_MY_TIMER_H

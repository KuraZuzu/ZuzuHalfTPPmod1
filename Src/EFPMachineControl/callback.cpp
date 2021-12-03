/*!
* @file callback.cpp v1.0
* @Copyright © 2021 Kazushi Kurasawa
* @date 2021.03.14
*
* Released under the MIT license.
* see https://opensource.org/licenses/MIT
*/

#include "../MSLH/defines.h"
#include "../MSLH/timer.h"
#include "stm32f4xx.h"
#include "test.h"

// タイマーコールバック
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {

    /**
     * @note
     *     90 M[Hz] (APB1) / 1 (Prescaler) = 90,000,000 [Hz] (TIM6)
     *     90,000,000 [Hz] / 90 = 1,000,000 [Hz]
     *     1,000,000 [Hz] = 1 u[sec]
     */
    if(htim == &htim6) {
        timer::count_us();
    }


    /**
     * @note
     *     90 M[Hz]（APB1）/ 9000 (Prescaler) = 10000 [Hz]（TIM7）
     *     10000 [Hz] / 10（Period）= 1000 [Hz]
     *     1000 [Hz] = 1 m[sec]
     */
    if(htim == &htim7) {
//        速度計測関数(); //<  WheelControlの両側タイヤ measureSpeed() を実行．
    }
}
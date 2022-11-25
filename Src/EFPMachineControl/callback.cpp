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
#include "../MSLH/interrupter.h"
#include "machine.h"

Interrupter<Machine> machine_interrupt;


// タイマーコールバック
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {

    /**
     * @note
     *     速度計測のタイマ割り込み
     *     90 M[Hz]（APB1）/ 90 (Prescaler) = 1,000,000 [Hz]（TIM7）
     *     1,000,000 [Hz] / 10000（Period）= 100 [Hz]
     *     100 [Hz] = 10 m[sec]
     */
    if (htim == &htim7) {
        machine_interrupt.run();
    }
}
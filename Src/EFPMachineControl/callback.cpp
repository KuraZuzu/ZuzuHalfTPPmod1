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

Interrupter<Wheel> l_wheel_interrupt;
Interrupter<Wheel> r_wheel_interrupt;

//Wheel l_wheel(Motor(htim1, TIM_CHANNEL_1, GPIOA, GPIO_PIN_6, false), Encoder(htim4 , 500*4 , false), 13.5f, 1);
//Wheel r_wheel(Motor(htim1, TIM_CHANNEL_2, GPIOA,  GPIO_PIN_7, true), Encoder(htim3, 500*4, true), 13.5f, 1);

// タイマーコールバック
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {

    /**c
     * @note
     *     壁センサ発光LEDの割り込み (値をリセットするために、PWMの周期毎に呼ばれる)
     *     90 M[Hz] (APB1) / 900000 (Prescaler) = 100 [Hz] (TIM2)
     *     100 [Hz] / 1 = 100 [Hz]
     *     100 [Hz] = 10 m[sec]
     */
//    if (htim == &htim2) {
//    }


    /**
     * @note
     *     壁センサ受光フォトトランジスタADCのタイマ割り込み (要：受光してからの立ち上がり時間の調査。今は仮の時間)
     *     90 M[Hz] (APB1) / 90 (Prescaler) = 1,000,000 [Hz] (TIM6)
     *     1,000,000 [Hz] / 2000 (Period) = 500 [Hz]
     *     500 [Hz] = 2 m[sec]  //< 最小値を測るだけのものなので、さらに変更できるかも。
     */
//    if (htim == &htim6) {
//    }


    /**
     * @note
     *     速度計測のタイマ割り込み
     *     90 M[Hz]（APB1）/ 90 (Prescaler) = 1,000,000 [Hz]（TIM7）
     *     1,000,000 [Hz] / 10000（Period）= 100 [Hz]
     *     100 [Hz] = 10 m[sec]
     */
    if (htim == &htim7) {
        l_wheel_interrupt.run();
        r_wheel_interrupt.run();
    }
}
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

extern DistanceSensor lf_sensor;
extern DistanceSensor ls_sensor;
extern DistanceSensor rs_sensor;
extern DistanceSensor rf_sensor;

// タイマーコールバック
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {

    /**
     * @note
     *     壁センサ発光LEDの割り込み (値をリセットするために、PWMの周期毎に呼ばれる)
     *     90 M[Hz] (APB1) / 900000 (Prescaler) = 100 [Hz] (TIM6)
     *     100 [Hz] / 1 = 100 [Hz]
     *     100 [Hz] = 1 m[sec]
     */
    if(htim == &htim2) {
        lf_sensor.interruptResetValue();
        ls_sensor.interruptResetValue();
        rs_sensor.interruptResetValue();
        rf_sensor.interruptResetValue();
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_3);;
    }

    /**
     * @note
     *     壁センサ受光フォトトランジスタADCのタイマ割り込み (要：受光してからの立ち上がり時間の調査。今は仮の時間)
     *     90 M[Hz] (APB1) / 90 (Prescaler) = 1,000,000 [Hz] (TIM6)
     *     1,000,000 [Hz] / 100 (Period) = 10,000 [Hz]
     *     10,000 [Hz] = 100 u[sec]  //<
     */
    if(htim == &htim6) {
        lf_sensor.interruptSamplingValue();
        ls_sensor.interruptSamplingValue();
        rs_sensor.interruptSamplingValue();
        rf_sensor.interruptSamplingValue();
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_3);;
    }


    /**
     * @note
     *     速度計測のタイマ割り込み
     *     90 M[Hz]（APB1）/ 90 (Prescaler) = 1,000,000 [Hz]（TIM7）
     *     1,000,000 [Hz] / 1000（Period）= 1,000 [Hz]
     *     1,000 [Hz] = 1 m[sec]
     */
    if(htim == &htim7) {
//        速度計測関数(); //<  WheelControlの両側タイヤ interruptMeasureSpeed() を実行．
    }
}
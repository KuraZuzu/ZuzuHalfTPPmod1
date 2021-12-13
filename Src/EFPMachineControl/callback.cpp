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

//extern DistanceSensor lf_sensor;
//extern DistanceSensor ls_sensor;
//extern DistanceSensor rs_sensor;
//extern DistanceSensor rf_sensor;
extern WheelControl l_wheel;
extern WheelControl r_wheel;

// タイマーコールバック
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {

    /**
     * @note
     *     壁センサ発光LEDの割り込み (値をリセットするために、PWMの周期毎に呼ばれる)
     *     90 M[Hz] (APB1) / 900000 (Prescaler) = 100 [Hz] (TIM2)
     *     100 [Hz] / 1 = 100 [Hz]
     *     100 [Hz] = 10 m[sec]
     */
    if(htim == &htim2) {
//        lf_sensor.interruptSamplingMaxValue();
//        ls_sensor.interruptSamplingMaxValue();
//        rs_sensor.interruptSamplingMaxValue();
//        rf_sensor.interruptSamplingMaxValue();
    }


    /**
     * @note
     *     壁センサ受光フォトトランジスタADCのタイマ割り込み (要：受光してからの立ち上がり時間の調査。今は仮の時間)
     *     90 M[Hz] (APB1) / 90 (Prescaler) = 1,000,000 [Hz] (TIM6)
     *     1,000,000 [Hz] / 2000 (Period) = 500 [Hz]
     *     500 [Hz] = 2 m[sec]  //< 最小値を測るだけのものなので、さらに変更できるかも。
     */
    if(htim == &htim6) {
//        lf_sensor.interruptSamplingMinValue();
//        ls_sensor.interruptSamplingMinValue();
//        rs_sensor.interruptSamplingMinValue();
//        rf_sensor.interruptSamplingMinValue();
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
        l_wheel.interruptMeasureSpeed();
        r_wheel.interruptMeasureSpeed();
    }
}
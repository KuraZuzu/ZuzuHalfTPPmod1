/*!
* @file motor_control.h v1.0
* @Copyright © 2021 Kazushi Kurasawa
* @date 2021.12.18
*
* Released under the MIT license.
* see https://opensource.org/licenses/MIT
*/

#ifndef ZUZUHALFTPPMOD1_MACHINE_H
#define ZUZUHALFTPPMOD1_MACHINE_H

#include "../MSLH/mslh.h"

extern Interrupter<WheelControl> l_wheel_interrupt;
extern Interrupter<WheelControl> r_wheel_interrupt;

class Machine {

public:
    Machine()
    : _l_wheel(Motor(htim1, TIM_CHANNEL_1, GPIOA, GPIO_PIN_6, false), Encoder(htim4 , 500*4 , false), 13.5f, 1)
    , _r_wheel(Motor(htim1, TIM_CHANNEL_2, GPIOA,  GPIO_PIN_7, false), Encoder(htim3, 500*4, true), 13.5f, 1)
    , _lf_sensor(DigitalOut(GPIOA,GPIO_PIN_15), AnalogInDMAStream(hadc1, 1))
    , _ls_sensor(DigitalOut(GPIOB, GPIO_PIN_3), AnalogInDMAStream(hadc1, 2))
    , _rs_sensor(DigitalOut(GPIOB, GPIO_PIN_2), AnalogInDMAStream(hadc1, 3))
    , _rf_sensor(DigitalOut(GPIOB, GPIO_PIN_10), AnalogInDMAStream(hadc1, 4))
    , _battery(hadc1, 5)
    , _led_buss(DigitalOut(GPIOC, GPIO_PIN_3), DigitalOut(GPIOC, GPIO_PIN_4), DigitalOut(GPIOC, GPIO_PIN_5))
    , _buzzer(PWMOut(htim8, TIM_CHANNEL_1)) {

        // Init functions.
        MX_GPIO_Init();
        MX_DMA_Init();
        MX_ADC1_Init();
        MX_TIM1_Init();
        MX_TIM3_Init();
        MX_TIM4_Init();
        MX_TIM6_Init();
        MX_TIM7_Init();
        MX_TIM8_Init();
        MX_USART2_UART_Init();
        MX_SPI3_Init();

        l_wheel_interrupt.attach(&_l_wheel, &WheelControl::interruptMeasureSpeed);
        r_wheel_interrupt.attach(&_r_wheel, &WheelControl::interruptMeasureSpeed);
        HAL_TIM_Base_Start_IT(&htim7);
        _lf_sensor.start();
        _ls_sensor.start();
        _rs_sensor.start();
        _rf_sensor.start();
        _battery.start();
        _l_wheel.start();
        _r_wheel.start();
    }

    void measureSpeed() {
        while (1) {
            printf("L:%6lf   R:%6lf \r\n" , _l_wheel.getSpeed(), _r_wheel.getSpeed());

        }
    }

private:

    WheelControl _l_wheel;
    WheelControl _r_wheel;
    GPIODistanceSensor _lf_sensor;
    GPIODistanceSensor _ls_sensor;
    GPIODistanceSensor _rs_sensor;
    GPIODistanceSensor _rf_sensor;
    Bus3Out _led_buss;
    Buzzer _buzzer;
    AnalogInDMAStream _battery;
};


#endif //ZUZUHALFTPPMOD1_MACHINE_H

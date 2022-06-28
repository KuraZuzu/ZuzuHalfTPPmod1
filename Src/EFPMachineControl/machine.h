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

extern Interrupter<Wheel> l_wheel_interrupt;
extern Interrupter<Wheel> r_wheel_interrupt;


class Machine {

public:
    Machine()
//    : _l_motor(htim1, TIM_CHANNEL_1, GPIOA, GPIO_PIN_6, false)
//    , _r_motor(htim1, TIM_CHANNEL_2, GPIOA,  GPIO_PIN_7, true)
    : _l_motor(htim1, TIM_CHANNEL_1, GPIOA, GPIO_PIN_7, false)
    , _r_motor(htim1, TIM_CHANNEL_2, GPIOA,  GPIO_PIN_6, true)
    , _l_encoder(htim4, machine_parameter::ENCODER_ONE_ROTATION_PULSE, false)
    , _r_encoder(htim3, machine_parameter::ENCODER_ONE_ROTATION_PULSE, true)
//    , _l_wheel(_l_motor, _l_encoder, 13.5f, 0.01f)
//    , _r_wheel(_r_motor, _r_encoder, 13.5f, 0.01f)
    , _battery(hadc1, 5)
    , _l_wheel(_l_motor, _l_encoder, _battery, 13.5f, 0.01f)
    , _r_wheel(_r_motor,_r_encoder, _battery, 13.5f, 0.01f)
    , _lf_sensor(DigitalOut(GPIOA,GPIO_PIN_15), AnalogInDMAStream(hadc1, 1))
    , _ls_sensor(DigitalOut(GPIOB, GPIO_PIN_3), AnalogInDMAStream(hadc1, 2))
    , _rs_sensor(DigitalOut(GPIOB, GPIO_PIN_2), AnalogInDMAStream(hadc1, 3))
    , _rf_sensor(DigitalOut(GPIOB, GPIO_PIN_10), AnalogInDMAStream(hadc1, 4))
    , _led_buss(DigitalOut(GPIOC, GPIO_PIN_3), DigitalOut(GPIOC, GPIO_PIN_4), DigitalOut(GPIOC, GPIO_PIN_5))
    , _buzzer(PWMOut(htim8, TIM_CHANNEL_1)) {


        l_wheel_interrupt.attach(&_l_wheel, &Wheel::interruptControlWheel);
        r_wheel_interrupt.attach(&_r_wheel, &Wheel::interruptControlWheel);
        HAL_TIM_Base_Start_IT(&htim7);
//        _lf_sensor.start();
//        _ls_sensor.start();
//        _rs_sensor.start();
//        _rf_sensor.start();
//        _battery.start();
        _l_wheel.start();
        _r_wheel.start();
    }

    void measureSpeed() {
        while (1) {
            printf("L:%6lf   R:%6lf \r\n" , _l_wheel.getSpeed(), _r_wheel.getSpeed());
        }
    }

    void run(float32_t speed) {
        _l_wheel.setSpeed(speed);
        _r_wheel.setSpeed(speed);
        while (1) {
//            _l_wheel.interruptControlSpeed(speed);
//            _r_wheel.interruptControlSpeed(speed);
//            HAL_Delay(50);
            printf("L:%6lf   R:%6lf \r\n" , _l_wheel.getSpeed(), _r_wheel.getSpeed());
        }
    }

    void ledTurnOn(uint8_t led_buss) {
        _led_buss = led_buss;
    }

    void buzzer() {
        while (1) _buzzer.error_v1();
    }
private:
    Motor _l_motor;
    Motor _r_motor;
    Encoder _l_encoder;
    Encoder _r_encoder;
    Wheel _l_wheel;
    Wheel _r_wheel;
    GPIODistanceSensor _lf_sensor;
    GPIODistanceSensor _ls_sensor;
    GPIODistanceSensor _rs_sensor;
    GPIODistanceSensor _rf_sensor;
    Bus3Out _led_buss;
    Buzzer _buzzer;
    AnalogInDMAStream _battery;
};


#endif //ZUZUHALFTPPMOD1_MACHINE_H

/*!
* @file machine_test.h v1.0
* @Copyright © 2021 Kazushi Kurasawa
* @date 2021.02.13
*
* Released under the MIT license.
* see https://opensource.org/licenses/MIT
*/

#ifndef ZUZUHALFTPPMOD1_TEST_H
#define ZUZUHALFTPPMOD1_TEST_H

#include "usart.h"
#include "adc.h"
#include "spi.h"
#include "dma.h"
#include "../MSLH/mslh.h"
#include "arm_math.h"

class Test {

public:

    Test();

    void buzzer_debug();

    void error_v1_buzzer_debug();

    void error_v2_buzzer_debug();

    void led_debug();

    void battery_console_debug();

    void all_sensor_console_debug();

    void battery_warning_debug();

    float battery_voltage();

    void busout_debug();

    void measure_speed_debug();

    void wheel_move_debug();

    static void measureSpeedSetCallback();

    void gyro_read();

private:
    DigitalOut _led1;
    DigitalOut _led2;
    DigitalOut _led3;
    Bus3Out _led_buss;
    Buzzer _buzzer;
    DistanceSensor _lf_sensor;
    DistanceSensor _ls_sensor;
    DistanceSensor _rs_sensor;
    DistanceSensor _rf_sensor;
    AnalogInDMAStream _battery;
    static WheelControl _l_wheel;
    static WheelControl _r_wheel;
    GyroSensor _gyro_sensor;
};

#endif //ZUZUHALFTPPMOD1_TEST_H

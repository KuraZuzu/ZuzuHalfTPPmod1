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
#include <vector>

class Test {

public:

    Test();

    void buzzerDebug();

    void errorBuzzerDebug1();

    void errorBuzzerDebug2();

    void ledDebug();

    void batteryConsoleDebug();

    void allSensorConsoleDebug();

    void batteryWarningDebug();

    float batteryVoltage();

    void bussOutDebug();

    void gyroRead();


private:

    Motor _l_motor;
    Motor _r_motor;
    Encoder _l_encoder;
    Encoder _r_encoder;
    AnalogInDMAStream _battery;
    Wheel _l_wheel;
    Wheel _r_wheel;
    GyroSensor _gyro_sensor;
    DistanceSensor _lf_sensor;
    DistanceSensor _ls_sensor;
    DistanceSensor _rs_sensor;
    DistanceSensor _rf_sensor;
    Bus3Out _led_buss;
    Buzzer _buzzer;
    Position _position;
    Position _start_position;
    float32_t _odometry_sampling_time;
};

#endif //ZUZUHALFTPPMOD1_TEST_H

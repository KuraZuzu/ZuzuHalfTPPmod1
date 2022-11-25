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


class Machine {

public:
    Machine();

    void start();

    void reset();

    void stop();

    void stopForce();

    inline void interruptMachine() {
        _l_wheel.interruptControlWheel(); //指定の速度に加減速制御
        _r_wheel.interruptControlWheel(); //指定の速度に加減速制御
        interruptCalcOdometry(); //オドメトリによって自己位置を計算する
    }

    inline void interruptCalcOdometry() {
        const float32_t l_delta_distance = _l_wheel.getSpeed() * _odometry_sampling_time;
        const float32_t r_delta_distance = _r_wheel.getSpeed() * _odometry_sampling_time;
        const float32_t delta_distance = (l_delta_distance + r_delta_distance) * 0.5f;  //< *0.5 は 平均(/2)を示す
        _position._rad += (r_delta_distance - l_delta_distance) / machine_parameter::MACHINE_TREAD;
        _position._x += delta_distance * cos(_position._rad);
        _position._y += delta_distance * sin(_position._rad);

        // オドメトリは関係なく左右のWheelで独立した移動距離を積算する
        _l_wheel_distance += l_delta_distance;
        _r_wheel_distance += r_delta_distance;
    }

    [[gnu::warn_unused_result]] inline float32_t getPositionX() const { return _position._x; }
    [[gnu::warn_unused_result]] inline float32_t getPositionY() const { return _position._y; }
    [[gnu::warn_unused_result]] inline float32_t getPositionRad() const { return _position._rad; }

    void measureSpeed();

    void ledTurnOn(uint8_t led_buss);

    void buzzer();

    //壁制御なしの距離指定走行
    void run(float32_t accel, float32_t speed, float32_t distance_mm);

    void moveRunAndStop(float32_t accel, float32_t speed, float32_t distance);

    void turnLeft(float32_t accel, float32_t speed, float32_t distance);

    void turnRight(float32_t accel, float32_t speed, float32_t distance);

    void runLeftMethod(float32_t accel, float32_t speed);


private:

    Motor _l_motor;
    Motor _r_motor;
    Encoder _l_encoder;
    Encoder _r_encoder;
    AnalogInDMAStream _battery;
    Wheel _l_wheel;
    Wheel _r_wheel;
    GyroSensor _gyro_sensor;
    GPIODistanceSensor _lf_sensor;
    GPIODistanceSensor _ls_sensor;
    GPIODistanceSensor _rs_sensor;
    GPIODistanceSensor _rf_sensor;
    Bus3Out _led_buss;
    Buzzer _buzzer;
    Position _position;
    Position _start_position;
    float32_t _odometry_sampling_time;
    float32_t _l_wheel_distance;
    float32_t _r_wheel_distance;
    Map _map;
};


#endif //ZUZUHALFTPPMOD1_MACHINE_H

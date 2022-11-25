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
    Machine()
            : _l_motor(htim1, TIM_CHANNEL_2, GPIOA, GPIO_PIN_7, true)
            , _r_motor(htim1, TIM_CHANNEL_1, GPIOA,  GPIO_PIN_6, false)
            , _l_encoder(htim4, machine_parameter::ENCODER_ONE_ROTATION_PULSE, false)
            , _r_encoder(htim3, machine_parameter::ENCODER_ONE_ROTATION_PULSE, true)
            , _battery(hadc1, 5)
            , _l_wheel(_l_motor, _l_encoder, _battery, 13.5f, 0.01f)
            , _r_wheel(_r_motor, _r_encoder, _battery, 13.5f, 0.01f)
            , _gyro_sensor(hspi3, GPIOA, GPIO_PIN_4)
            , _lf_sensor(DigitalOut(DIST_LED1_GPIO_Port, DIST_LED1_Pin), AnalogInDMAStream(hadc1, 1), htim2, machine_parameter::convert_lf_func)
            , _ls_sensor(DigitalOut(DIST_LED2_GPIO_Port, DIST_LED2_Pin), AnalogInDMAStream(hadc1, 2), htim2, machine_parameter::convert_ls_func)
            , _rs_sensor(DigitalOut(DIST_LED3_GPIO_Port, DIST_LED3_Pin), AnalogInDMAStream(hadc1, 3), htim2, machine_parameter::convert_rs_func)
            , _rf_sensor(DigitalOut(DIST_LED4_GPIO_Port, DIST_LED4_Pin), AnalogInDMAStream(hadc1, 4), htim2, machine_parameter::convert_rf_func)
            , _led_buss(DigitalOut(GPIOC, GPIO_PIN_5), DigitalOut(GPIOC, GPIO_PIN_4), DigitalOut(GPIOC, GPIO_PIN_3))
            , _buzzer(PWMOut(htim8, TIM_CHANNEL_1))
            , _odometry_sampling_time(0.01f)
            , _l_wheel_distance(0.0f)
            , _r_wheel_distance(0.0f)
            , _map(4, 4) {
    }

    void start() {
        HAL_TIM_Base_Start_IT(&htim7);
        _lf_sensor.start();
        _ls_sensor.start();
        _rs_sensor.start();
        _rf_sensor.start();
        _battery.start();
        _l_wheel.start();
        _r_wheel.start();
    }

    void reset() {
        _l_wheel.reset();
        _r_wheel.reset();
        _position.reset();
    }

    void stop() {
        HAL_TIM_Base_Stop_IT(&htim7);
        _l_wheel.stop();
        _r_wheel.stop();
    }

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

    void measureSpeed() {
            printf("L:%6lf   R:%6lf \r\n" , _l_wheel.getSpeed(), _r_wheel.getSpeed());
    }

    void ledTurnOn(uint8_t led_buss) {
        _led_buss = led_buss;
    }

    void buzzer() {
        while (1) _buzzer.error_v1();
    }


    //壁制御なしの距離指定走行
    void run(float32_t accel, float32_t speed, float32_t distance_mm) {
        int32_t offset_distance = _l_wheel_distance;
        if((_l_wheel_distance - offset_distance) < distance_mm) {
            _l_wheel.setSpeed(accel, speed);
            _r_wheel.setSpeed(accel, speed);
        }
    }

    void turnLeft(float32_t accel, float32_t speed, float32_t distance) {
        float32_t offset_distance = _l_wheel_distance;
        if((_l_wheel_distance - offset_distance) > distance/2.0f) {
            _l_wheel.setSpeed(-accel, -speed);
            _r_wheel.setSpeed(+accel, +speed);
        }
        offset_distance = _l_wheel_distance;
        if((_l_wheel_distance - offset_distance) < distance/2.0f) {
            _l_wheel.setSpeed(+accel, 0.0f);
            _r_wheel.setSpeed(-accel, 0.0f);
        }
    }

    void turnRight(float32_t accel, float32_t speed, float32_t distance) {
        float32_t offset_distance = _l_wheel_distance;
        if((_l_wheel_distance - offset_distance) < distance/2.0f) {
            _l_wheel.setSpeed(+accel, +speed);
            _r_wheel.setSpeed(-accel, -speed);
        }
        offset_distance = _l_wheel_distance ;
        if((_l_wheel_distance - offset_distance) > distance/2.0f) {
            _l_wheel.setSpeed(-accel, 0.0f);
            _r_wheel.setSpeed(+accel, 0.0f);
        }
    }

    void runLeftMethod(float32_t accel, float32_t speed) {
        //< ここに最初の距離だけ入れる(後入れ)

        while (1) {
            HAL_Delay(100);
            run(+accel, +speed, machine_parameter::START_BLOCK_DISTANCE + machine_parameter::HALF_BLOCK_DISTANCE);

            if (_ls_sensor.getDistance(1000) > machine_parameter::OPEN_SIDE_WALL_THRESHOLD) {
                run(-accel, 0.0f, machine_parameter::HALF_BLOCK_DISTANCE);
                HAL_Delay(100);
                turnLeft(+accel, +speed, machine_parameter::TURN_90_DEG_DISTANCE);

            } else if (_lf_sensor.getDistance(1000) < machine_parameter::OPEN_FRONT_WALL_THRESHOLD) {
                run(+accel, +speed, machine_parameter::ONE_BLOCK_DISTANCE);

            } else if (_rs_sensor.getDistance(1000) > machine_parameter::OPEN_SIDE_WALL_THRESHOLD) {
                run(-accel, 0.0f, machine_parameter::HALF_BLOCK_DISTANCE);
                HAL_Delay(100);
                turnRight(+accel, +speed, machine_parameter::TURN_90_DEG_DISTANCE);

            } else {
                run(-accel, 0.0f, machine_parameter::HALF_BLOCK_DISTANCE);
                HAL_Delay(100);
                turnLeft(+accel, +speed, machine_parameter::TURN_180_DEG_DISTANCE);
            }

        }
    }


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

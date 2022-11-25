//
// Created by 倉澤　一詩 on 2021/12/18.
//
#include "machine.h"

Machine::Machine()
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

void Machine::start() {
    HAL_TIM_Base_Start_IT(&htim7);
    _lf_sensor.start();
    _ls_sensor.start();
    _rs_sensor.start();
    _rf_sensor.start();
    _battery.start();
    _l_wheel.start();
    _r_wheel.start();
    reset();
}

void Machine::reset() {
    _l_wheel.reset();
    _r_wheel.reset();
    _position.reset();
}

void Machine::stop() {
    HAL_TIM_Base_Stop_IT(&htim7);
    _l_wheel.stop();
    _r_wheel.stop();
}

void Machine::stopForce() {
    _l_wheel.stopForce();
    _r_wheel.stopForce();
}

void Machine::measureSpeed() {
    printf("L:%6lf   R:%6lf \r\n" , _l_wheel.getSpeed(), _r_wheel.getSpeed());
}

void Machine::ledTurnOn(uint8_t led_buss) {
    _led_buss = led_buss;
}

void Machine::buzzer() {
    while (1) _buzzer.error_v1();
}

void Machine::run(float32_t accel, float32_t speed, float32_t distance_mm) {
    const float32_t offset_distance = _l_wheel_distance;
    if((_l_wheel_distance - offset_distance) < distance_mm) {
        _l_wheel.setSpeed(+accel, +speed);
        _r_wheel.setSpeed(+accel, +speed);
    }
}

void Machine::moveRunAndStop(float32_t accel, float32_t speed, float32_t distance) {
    const float32_t init_l_distance = _l_wheel_distance;
    const float32_t half_distance = distance / 2.0f;

    _l_wheel.setSpeed(accel, speed);
    _r_wheel.setSpeed(accel, speed);
    if(distance > 0) while ( ((_l_wheel_distance - init_l_distance) < half_distance)) {}
    else while ( ((_l_wheel_distance - init_l_distance) > half_distance)) {}

    _l_wheel.setSpeed(-accel, 0);
    _r_wheel.setSpeed(-accel, 0);
    if(distance > 0) while ( ((_l_wheel_distance - init_l_distance) < half_distance)) {}
    else while ( ((_l_wheel_distance - init_l_distance) > half_distance)) {}
}

void Machine::turnLeft(float32_t accel, float32_t speed, float32_t distance) {
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
    stopForce();
}

void Machine::turnRight(float32_t accel, float32_t speed, float32_t distance) {
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
    stopForce();
}

void Machine::runLeftMethod(float32_t accel, float32_t speed) {

    while (1) {
        HAL_Delay(100);
        run(+accel, +speed, machine_parameter::START_BLOCK_DISTANCE + machine_parameter::HALF_BLOCK_DISTANCE);

        if (_ls_sensor.getDistance(1000) > machine_parameter::OPEN_SIDE_WALL_THRESHOLD) {
            run(-accel, 0.0f, machine_parameter::HALF_BLOCK_DISTANCE);
            stopForce();
            HAL_Delay(100);
            turnLeft(+accel, +speed, machine_parameter::TURN_90_DEG_DISTANCE);

        } else if (_lf_sensor.getDistance(1000) < machine_parameter::OPEN_FRONT_WALL_THRESHOLD) {
            run(+accel, +speed, machine_parameter::ONE_BLOCK_DISTANCE);

        } else if (_rs_sensor.getDistance(1000) > machine_parameter::OPEN_SIDE_WALL_THRESHOLD) {
            run(-accel, 0.0f, machine_parameter::HALF_BLOCK_DISTANCE);
            stopForce();
            HAL_Delay(100);
            turnRight(+accel, +speed, machine_parameter::TURN_90_DEG_DISTANCE);

        } else {
            run(-accel, 0.0f, machine_parameter::HALF_BLOCK_DISTANCE);
            stopForce();
            HAL_Delay(100);
            turnLeft(+accel, +speed, machine_parameter::TURN_180_DEG_DISTANCE);
        }

    }
}

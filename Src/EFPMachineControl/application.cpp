/*!
 * @file application.h v1.0
 * @Copyright © 2021 Kazushi Kurasawa
 * @date 2021.02.13
 *
 * Released under the MIT license.
 * see https://opensource.org/licenses/MIT
 */

#ifndef ZUZUHALFTPPMOD1_APPLICATION_H
#define ZUZUHALFTPPMOD1_APPLICATION_H

#include "test.h"
#include "../MSLH/timer.h"
//#include <iostream>
#include <cinttypes>
#include "stm32f4xx_it.h"

//extern volatile uint32_t timer::counter_us; //debug
DistanceSensor lf_sensor(PWMOut(htim2, TIM_CHANNEL_1), AnalogInDMAStream(hadc1, 1), htim6);
DistanceSensor ls_sensor(PWMOut(htim2, TIM_CHANNEL_2), AnalogInDMAStream(hadc1, 2), htim6);
DistanceSensor rs_sensor(PWMOut(htim2, TIM_CHANNEL_3), AnalogInDMAStream(hadc1, 3), htim6);
DistanceSensor rf_sensor(PWMOut(htim2, TIM_CHANNEL_4), AnalogInDMAStream(hadc1, 4), htim6);

#ifdef __cplusplus
extern "C" {
#endif

// ここ(グローバルスコープ内)でインスタンスを生成するとコンストラクタが呼ばれないので注意。
void test_myself_wait_led() {
    MX_GPIO_Init();
    MX_TIM6_Init();
    while (1) {
//        printf("%d\r\n", my_timer::time_us_count);
        timer::waitMicroSeconds(1000000);
//        HAL_Delay(1000);
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_3);
    }
}


void test_myself_move() {
    MX_GPIO_Init();
    MX_TIM1_Init();
    Motor _l_motor(htim1, TIM_CHANNEL_1, GPIOA, GPIO_PIN_6, true);
    Motor _r_motor(htim1, TIM_CHANNEL_2, GPIOA,  GPIO_PIN_7, false);

    _l_motor.start();
    _r_motor.start();

    _l_motor.update(0.2);
    _r_motor.update(0.2);
    HAL_Delay(1000);

    _l_motor.update(-0.2);
    _r_motor.update(-0.2);
    HAL_Delay(1000);

    _l_motor.stop();
    _r_motor.stop();
    HAL_Delay(1000);

    _l_motor.start();
    _r_motor.start();

    _l_motor.update(0.8);
    _r_motor.update(0.8);
    HAL_Delay(1000);

    _l_motor.stop();
    _r_motor.stop();
}

void test_myself_encoder() {

    MX_TIM3_Init();
    MX_TIM4_Init();
    Encoder _l_encoder(htim4 , 500*4 , false);
    Encoder _r_encoder(htim3, 500*4, true);
    _l_encoder.reset();
    _r_encoder.reset();
    _l_encoder.start();
    _r_encoder.start();
    while (1) {
        _l_encoder.update();
        _r_encoder.update();
        printf("LT:%d  LC:%d  LD:%d    RT:%d  RC:%d  RD:%d\r\n"
               , static_cast<int>(_l_encoder.getTotalPulse())
               , static_cast<int>(_l_encoder.getRotationCount())
               , static_cast<int>(_l_encoder.getDeltaPulse())
               , static_cast<int>(_r_encoder.getTotalPulse())
               , static_cast<int>(_r_encoder.getRotationCount())
               , static_cast<int>(_r_encoder.getDeltaPulse()) );

        HAL_Delay(50);
    }
}

void test_myself_move_wheel() {
    MX_GPIO_Init();
    MX_TIM1_Init();
    MX_TIM3_Init();
    MX_TIM4_Init();
    WheelControl _l_wheel(Motor(htim1, TIM_CHANNEL_1, GPIOA, GPIO_PIN_6, true),Encoder(htim4,500*4,false), 13.5f, 1000);
    WheelControl _r_wheel(Motor(htim1, TIM_CHANNEL_2, GPIOA, GPIO_PIN_7, false),Encoder(htim3,500*4,true), 13.5f, 1000);
    _l_wheel.start();
    _r_wheel.start();
    _l_wheel.run(500, 10000);
    _r_wheel.run(500, 10000);
}

void test_buzzer() {
    Test test;
    test.buzzer_debug();
}

void test_error_v1_buzzer() {
    Test test;
    test.error_v1_buzzer_debug();
}

void test_error_v2_buzzer() {
    Test test;
    test.error_v2_buzzer_debug();
}

void test_led() {
    Test test;
    test.led_debug();
}

void test_battery_console(){
    // 他のラッパ関数内でMachineのインスタンスを生成するとAnalogInが読めない。
    Test test;  //< ラッパ関数内でインスタンス生成しないとコンストラクタが呼ばれない。
    while (1) {
        test.battery_console_debug();
        HAL_Delay(500);
    }
}

void test_all_console() {
    Test test;
    test.buzzer_debug();
    test.led_debug();
    while(1) {
        test.all_sensor_console_debug();
        printf("\r\n");
        HAL_Delay(100);
        test.battery_warning_debug();
    }
}

void test_battery_warning() {
    Test test;
    test.battery_warning_debug();
}

void test_buss3out() {
    Test test;
    test.busout_debug();
}

void test_measure_speed() {
    Test test;
    while(1) {
        test.measure_speed_debug();
    }
}

void test_wheel_move() {
    Test test;
    test.buzzer_debug();
    test.wheel_move_debug();
}

void test_gyro() {
    Test test;
    test.gyro_read();
}


void test_motor_output() {
    Motor left_motor(htim1, TIM_CHANNEL_1, GPIOA, GPIO_PIN_6, true);
    Motor right_motor(htim1, TIM_CHANNEL_2, GPIOA, GPIO_PIN_7, false);

    left_motor.start();
    right_motor.start();
    left_motor.update(1.0f);
    right_motor.update(1.0f);
}


void test_wait() {
    DigitalOut led1(GPIOC, GPIO_PIN_3);
    MX_GPIO_Init();
    MX_TIM2_Init();
    timer::initTimer(&htim2);
    while (1) {
        printf("%d\r\n", timer::counter_us);
    }
//    while (1) {
//        led1.write(1);
//        timer::waitMicroSeconds(2000000);
//        led1.write(0);
//        timer::waitMicroSeconds(2000000);
//    }
}

void test_global_sensor() {
    MX_DMA_Init();
    MX_ADC1_Init();
    MX_TIM2_Init();
    MX_TIM6_Init();
    MX_USART2_UART_Init();
    lf_sensor.start();
    ls_sensor.start();
    rs_sensor.start();
    rf_sensor.start();
    while (1) {
        printf("LF:%6" PRIu16 "   LS:%6" PRIu16 "   RS:%6" PRIu16 "   RF:%6" PRIu16 "\r\n"
                , lf_sensor.read()
                , ls_sensor.read()
                , rs_sensor.read()
                , rf_sensor.read() );
    }
}

#ifdef __cplusplus
}
#endif


#endif // ZUZUHALFTPPMOD1_APPLICATION_H
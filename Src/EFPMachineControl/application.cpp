/*!
 * @file application.h v1.0
 * @Copyright © 2021 Kazushi Kurasawa
 * @date 2021.02.13
 *
 * Released under the MIT license.
 * see https://opensource.org/licenses/MIT
 */

//#ifndef ZUZUHALFTPPMOD1_APPLICATION_H
//#define ZUZUHALFTPPMOD1_APPLICATION_H

#include "test.h"
#include "../MSLH/timer.h"
//#include <iostream>
#include <cinttypes>
#include "application.h"
#include "stm32f4xx_it.h"
#include "../MSLH/gpio_distance_sensor.h"
#include "../MSLH/interrupter.h"

//extern Interrupter<WheelControl> l_wheel_interrupt;
//extern Interrupter<WheelControl> r_wheel_interrupt;

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
    Motor _l_motor(htim1, TIM_CHANNEL_1, GPIOA, GPIO_PIN_6, false);
    Motor _r_motor(htim1, TIM_CHANNEL_2, GPIOA,  GPIO_PIN_7, false);

    _l_motor.start();
    _r_motor.start();

    _l_motor.update(0.05f);
    _r_motor.update(0.05f);
    HAL_Delay(1000);
    _l_motor.update(0.0f);
    _r_motor.update(0.0f);
    HAL_Delay(500);
//
    _l_motor.update(-0.05f);
    _r_motor.update(-0.05f);
    HAL_Delay(1000);

    _l_motor.update(0.05f);
    _r_motor.update(0.05f);
//
//    _r_motor.update(0.2f);
//    HAL_Delay(500);
//    _r_motor.update(-1.0);
//    HAL_Delay(1000);
//    _r_motor.stop();
//    HAL_Delay(1000);
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
//    WheelControl _l_wheel(Motor(htim1, TIM_CHANNEL_1, GPIOA, GPIO_PIN_6, true),Encoder(htim4,500*4,false), 13.5f, 1000);
//    WheelControl _r_wheel(Motor(htim1, TIM_CHANNEL_2, GPIOA, GPIO_PIN_7, false),Encoder(htim3,500*4,true), 13.5f, 1000);
//    _l_wheel.start();
//    _r_wheel.start();
//    _l_wheel.run(500, 10000);
//    _r_wheel.run(500, 10000);
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
        test.battery_warning_debug();
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
//    MX_TIM7_Init();
//    timer::initTimer(&htim7);
//    while (1) {
//        printf("%d\r\n", timer::counter_us);
//    }
    while (1) {
//        led1.write(1);
        led1 = !led1;
        timer::waitMicroSeconds(2);
    }
}

void test_global_sensor() {
    MX_DMA_Init();
    MX_ADC1_Init();
    MX_TIM2_Init();
    MX_TIM6_Init();
    MX_USART2_UART_Init();
//    lf_sensor.start();
//    ls_sensor.start();
//    rs_sensor.start();
//    rf_sensor.start();
//    while (1) {
//        printf("LF:%6" PRIu16 "   LS:%6" PRIu16 "   RS:%6" PRIu16 "   RF:%6" PRIu16 "\r\n"
//                , lf_sensor.read()
//                , ls_sensor.read()
//                , rs_sensor.read()
//                , rf_sensor.read() );
//    }
}

void test_gpio_distance_sensor() {
    MX_DMA_Init();
    MX_ADC1_Init();
    MX_GPIO_Init();
    GPIODistanceSensor lf_sensor(DigitalOut(GPIOA,GPIO_PIN_15), AnalogInDMAStream(hadc1, 1));
    GPIODistanceSensor ls_sensor(DigitalOut(GPIOB, GPIO_PIN_3), AnalogInDMAStream(hadc1, 2));
    GPIODistanceSensor rs_sensor(DigitalOut(GPIOB, GPIO_PIN_2), AnalogInDMAStream(hadc1, 3));
    GPIODistanceSensor rf_sensor(DigitalOut(GPIOB, GPIO_PIN_10), AnalogInDMAStream(hadc1, 4));
    lf_sensor.start();
    ls_sensor.start();
    rs_sensor.start();
    rf_sensor.start();

    uint32_t lf=0, ls=0, rs=0, rf=0;
    while (1) {
        for (int i = 0; i < 10; ++i) {
            lf += lf_sensor.read(5);
            ls += ls_sensor.read(5);
            rs += rs_sensor.read(5);
            rf += rf_sensor.read(5);
        }
        lf = lf / 10;
        ls = ls / 10;
        rs = rs / 10;
        rf = rf / 10;
        printf("LF:%6" PRIu16 "   LS:%6" PRIu16 "   RS:%6" PRIu16 "   RF:%6" PRIu16 "\r\n", lf, ls, rs, rf);
        lf = 0;
        ls = 0;
        rs = 0;
        rf = 0;
    }
}


void test_measure_speed() {
    MX_GPIO_Init();
    MX_TIM1_Init();
    MX_TIM3_Init();
    MX_TIM4_Init();
    MX_TIM7_Init();
    MX_USART2_UART_Init();
    Motor l_motor(htim1, TIM_CHANNEL_1, GPIOA, GPIO_PIN_6, false);
    Motor r_motor(htim1, TIM_CHANNEL_2, GPIOA,  GPIO_PIN_7, false);
    Encoder l_encoder(htim4 , 500*4 , false);
    Encoder r_encoder(htim3, 500*4, true);
    WheelControl l_wheel(l_motor, l_encoder, 13.5f, 1);
    WheelControl r_wheel(r_motor, r_encoder, 13.5f, 1);
//    l_wheel_interrupt.attach(&l_wheel, &WheelControl::interruptMeasureSpeed);
//    r_wheel_interrupt.attach(&r_wheel, &WheelControl::interruptMeasureSpeed);
    l_wheel.start();
    r_wheel.start();
    while(1) {
        printf("L:%6lf   R:%6lf \r\n" , l_wheel.getSpeed(), r_wheel.getSpeed());
    }
}


#ifdef __cplusplus
}
#endif


//#endif // ZUZUHALFTPPMOD1_APPLICATION_H
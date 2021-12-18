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
#include "machine.h"

//extern Interrupter<WheelControl> l_wheel_interrupt;
//extern Interrupter<WheelControl> r_wheel_interrupt;

#ifdef __cplusplus
extern "C" {
#endif

// ここ(グローバルスコープ内)でインスタンスを生成するとコンストラクタが呼ばれないので注意。


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
    l_wheel_interrupt.attach(&l_wheel, &WheelControl::interruptMeasureSpeed);
    r_wheel_interrupt.attach(&r_wheel, &WheelControl::interruptMeasureSpeed);
    HAL_TIM_Base_Start_IT(&htim7);
    l_wheel.start();
    r_wheel.start();
    while(1) {
        printf("L:%6lf   R:%6lf \r\n" , l_wheel.getSpeed(), r_wheel.getSpeed());
    }
}

void machine_measure_speed() {
    Machine machine;
    machine.measureSpeed();
}

#ifdef __cplusplus
}
#endif


//#endif // ZUZUHALFTPPMOD1_APPLICATION_H
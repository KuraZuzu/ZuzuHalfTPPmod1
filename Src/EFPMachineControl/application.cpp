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
#include "../MSLH/defines.h"
#include <cinttypes>
#include "stm32f4xx_it.h"
#include "../MSLH/gpio_distance_sensor.h"
#include "../MSLH/interrupter.h"
#include "machine.h"
#include <string>

//extern Interrupter<WheelControl> l_wheel_interrupt;
//extern Interrupter<WheelControl> r_wheel_interrupt;

#ifdef __cplusplus
extern "C" {
#endif

// ここ(グローバルスコープ内)でインスタンスを生成するとコンストラクタが呼ばれないので注意。


void testBuzzer() {
    Test test;
    test.buzzerDebug();
}

void testErrorBuzzer1() {
    Test test;
    test.errorBuzzerDebug1();
}

void testErrorBuzzer2() {
    Test test;
    test.errorBuzzerDebug2();
}


void testBatteryConsole(){
    // 他のラッパ関数内でMachineのインスタンスを生成するとAnalogInが読めない。
    Test test;  //< ラッパ関数内でインスタンス生成しないとコンストラクタが呼ばれない。
    while (1) {
        test.batteryConsoleDebug();
        test.batteryWarningDebug();
        HAL_Delay(500);
    }
}

void testAllConsole() {
    Test test;
    test.buzzerDebug();
    test.ledDebug();
    while(1) {
        test.allSensorConsoleDebug();
        printf("\r\n");
        HAL_Delay(100);
        test.batteryWarningDebug();
    }
}

void testBatteryWarning() {
    Test test;
    test.batteryWarningDebug();
}

void testBuss3out() {
    Test test;
    test.bussOutDebug();
}

void testGyro() {
    Test test;
    test.gyroRead();
}


void testMotorOutput() {
    Motor left_motor(htim1, TIM_CHANNEL_1, GPIOA, GPIO_PIN_6, false);
    Motor right_motor(htim1, TIM_CHANNEL_2, GPIOA, GPIO_PIN_7, true);

    left_motor.start();
    right_motor.start();
    left_motor.update(1.0f);
    right_motor.update(1.0f);
}


void testGpioDistanceSensor() {
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



void testMeasureSpeed() {
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
    l_wheel_interrupt.attach(&l_wheel, &WheelControl::interruptControlWheel);
    r_wheel_interrupt.attach(&r_wheel, &WheelControl::interruptControlWheel);
    HAL_TIM_Base_Start_IT(&htim7);
    l_wheel.start();
    r_wheel.start();
    while(1) {
        printf("L:%6lf   R:%6lf \r\n" , l_wheel.getSpeed(), r_wheel.getSpeed());
    }
}

void machineMeasureSpeed() {
    Machine machine;
    machine.measureSpeed();
}

void machineRun(float32_t speed) {
    Machine machine;
    machine.run(speed);
}

void testEncoder() {
    MX_GPIO_Init();
    MX_TIM3_Init();
    MX_TIM4_Init();
    MX_USART2_UART_Init();

    mslh::Encoder _encoder_1(htim4, machine_parameter::MES6_x4_PULSE, false);
    mslh::Encoder _encoder_2(htim3, machine_parameter::MES6_x4_PULSE, true);
    _encoder_1.start();
    _encoder_2.start();
    _encoder_1.reset();
    _encoder_2.reset();
    while (1) {
        _encoder_1.update();
        _encoder_2.update();
        printf("LT:%d  LC:%d  LD:%d    RT:%d  RC:%d  RD:%d\r\n"
                , static_cast<int>(_encoder_1.getTotalPulse())
                , static_cast<int>(_encoder_1.getRotationCount())
                , static_cast<int>(_encoder_1.getDeltaPulse())
                , static_cast<int>(_encoder_2.getTotalPulse())
                , static_cast<int>(_encoder_2.getRotationCount())
                , static_cast<int>(_encoder_2.getDeltaPulse()) );
        HAL_Delay(10);
    }

}

#ifdef __cplusplus
}
#endif


//#endif // ZUZUHALFTPPMOD1_APPLICATION_H
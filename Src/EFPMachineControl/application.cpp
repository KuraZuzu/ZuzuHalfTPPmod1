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
#include "../MSLH/timer.h"

//extern Interrupter<Wheel> l_wheel_interrupt;
//extern Interrupter<Wheel> r_wheel_interrupt;

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
        printf("\r\n");
        test.batteryWarningDebug();
        HAL_Delay(500);
    }
}

void testConsoleDistSensor() {
    Test test;
    while(1) {
        test.consoleDistSensor();
        printf("\r\n");
    }
}

void testConsoleDistSensorVoltage() {
    Test test;
    while(1) {
        test.consoleDistSensorVoltage();
        printf("\r\n");
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
    AnalogInDMAStream _battery(hadc1, 5);
    Wheel l_wheel(l_motor, l_encoder, _battery, 13.5f, 0.01f);
    Wheel r_wheel(r_motor, r_encoder, _battery, 13.5f, 0.01f);
//    l_wheel_interrupt.attach(&l_wheel, &Wheel::interruptControlWheel);
//    r_wheel_interrupt.attach(&r_wheel, &Wheel::interruptControlWheel);
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
    HAL_Delay(3000);
    machine.ledTurnOn(0b00000111);
    machine.run(speed);
}

void testEncoder() {
    MX_GPIO_Init();
    MX_TIM3_Init();
    MX_TIM4_Init();
    MX_USART2_UART_Init();

    mslh::Encoder _encoder_1(htim4, machine_parameter::ENCODER_ONE_ROTATION_PULSE, false);
    mslh::Encoder _encoder_2(htim3, machine_parameter::ENCODER_ONE_ROTATION_PULSE, true);
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

void machineBuzzer() {
    Machine machine;
    machine.buzzer();
}

void testTimer() {
    Machine machine;
    Timer timer(htim2);
    timer.start();
    int i = 0;
    while (1) {
        timer.waitMicroSeconds(1000000);
        machine.ledTurnOn(i);
        i++;
    }
}

void onceExamDistSensor() {
///*lf*/GPIODistanceSensor sensor(DigitalOut(DIST_LED1_GPIO_Port, DIST_LED1_Pin), AnalogInDMAStream(hadc1, 1), htim2, machine_parameter::convert_lf_func);
///*ls*/GPIODistanceSensor sensor(DigitalOut(DIST_LED2_GPIO_Port, DIST_LED2_Pin), AnalogInDMAStream(hadc1, 2), htim2, machine_parameter::convert_ls_func);
/*rs*/GPIODistanceSensor sensor(DigitalOut(DIST_LED3_GPIO_Port, DIST_LED3_Pin), AnalogInDMAStream(hadc1, 3), htim2, machine_parameter::convert_rs_func);
///*rf*/GPIODistanceSensor sensor(DigitalOut(DIST_LED4_GPIO_Port, DIST_LED4_Pin), AnalogInDMAStream(hadc1, 4), htim2, machine_parameter::convert_rf_func);

    sensor.start();

    std::vector<uint16_t> sensor_values(100, 0);

    while (1) {
        for (int i = 0; i < 100; ++i) {
            sensor_values[i] = sensor.getTestRawValue(4000);
//            HAL_Delay(10);
        }
        std::sort(sensor_values.begin(), sensor_values.end());

        uint16_t sensor_value = sensor_values[49];

        printf("Sensor: %d\r\n", sensor_value);
    }
}


#ifdef __cplusplus
}
#endif


//#endif // ZUZUHALFTPPMOD1_APPLICATION_H
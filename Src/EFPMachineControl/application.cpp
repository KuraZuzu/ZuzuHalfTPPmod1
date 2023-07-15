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


extern Interrupter<Machine> machine_interrupt;

#ifdef __cplusplus
extern "C" {
#endif

// ここ(グローバルスコープ内)でインスタンスを生成するとコンストラクタが呼ばれないので注意。


void testBuzzer() {
    Test test;
    test.buzzerDebug();
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

void testConsoleRawDistance() {
    Test test;
    while(1) {
        test.consoleRawDistance();
        printf("\r\n");
    }
}

void testBuss3out() {
    Test test;
    test.bussOutDebug();
}

void testGyro() {
    Test test;
    test.gyroRead();
}


void selfMotorOutput() {
    Motor left_motor(htim1, TIM_CHANNEL_1, GPIOA, GPIO_PIN_6, false);
    Motor right_motor(htim1, TIM_CHANNEL_2, GPIOA, GPIO_PIN_7, true);
    left_motor.start();
    right_motor.start();
    left_motor.update(1.0f);
    right_motor.update(1.0f);
}

void machineMeasureSpeed() {
    Machine machine;
    machine_interrupt.attach(&machine, &Machine::interruptMachine);
    machine.start();
    while (1) {
        machine.measureSpeed();
    }
}

void machineMeasurePosition() {
    Machine machine;
    machine_interrupt.attach(&machine, &Machine::interruptMachine);
    machine.start();
    machine.reset();
    while (1) {
        printf("X:%6lf   Y:%6lf   RAD:%6lf   \r\n", machine.getPositionX(), machine.getPositionY(), machine.getPositionRad());
    }
}

void machineRun(float32_t accel, float32_t speed) {
    Machine machine;
    machine_interrupt.attach(&machine, &Machine::interruptMachine);
    machine.start();
    machine.ledTurnOn(0b00000111);
    machine.run(accel, speed);
    while (1) {}
}

void machineRunSpecifiedDistance(float32_t accel, float32_t speed, float32_t distance) {
    Machine machine;
    machine_interrupt.attach(&machine, &Machine::interruptMachine);
    machine.start();
    machine.ledTurnOn(0b00000111);
    machine.runSpecifiedDistance(+accel, +speed, +distance);
    while (1) {}
}

void machineRunAndStop(float32_t accel, float32_t speed, float32_t distance) {
    Machine machine;
    machine_interrupt.attach(&machine, &Machine::interruptMachine);
    machine.start();
    machine.run(0.0f, 0.0f);
//    machine.ledTurnOn(0b00000100);
    machine.moveRunAndStop(+accel, +speed, +distance);
    while (1) {}
}

void selfEncoder() {
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

void machineTimer() {
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

void selfDistSensor() {
///*lf*/GPIODistanceSensor sensor(DigitalOut(DIST_LED1_GPIO_Port, DIST_LED1_Pin), AnalogInDMAStream(hadc1, 1), htim2, machine_parameter::convert_lf_func);
///*ls*/GPIODistanceSensor sensor(DigitalOut(DIST_LED2_GPIO_Port, DIST_LED2_Pin), AnalogInDMAStream(hadc1, 2), htim2, machine_parameter::convert_ls_func);
///*rs*/GPIODistanceSensor sensor(DigitalOut(DIST_LED3_GPIO_Port, DIST_LED3_Pin), AnalogInDMAStream(hadc1, 3), htim2, machine_parameter::convert_rs_func);
/*rf*/GPIODistanceSensor sensor(DigitalOut(DIST_LED4_GPIO_Port, DIST_LED4_Pin), AnalogInDMAStream(hadc1, 4), htim2, machine_parameter::convert_rf_func);

    sensor.start();
    std::vector<uint16_t> sensor_values(100, 0);

    while (1) {
        for (int i = 0; i < 100; ++i) {
            sensor_values[i] = sensor.getTestRawValue(4000);
        }
        std::sort(sensor_values.begin(), sensor_values.end());
        uint16_t sensor_value = sensor_values[49];
        printf("Sen: %d\r\n", sensor_value);
    }
}

void testConsoleDistance() {
    Test test;
    while (1) {
        test.consoleDistance();
    }
}

void machineTurn(float32_t accel, float32_t speed) {
    Machine machine;
    machine_interrupt.attach(&machine, &Machine::interruptMachine);
    machine.start();

    machine.ledTurnOn(1);
    machine.turnLeft(+accel, +speed, machine_parameter::TURN_90_DEG_DISTANCE);
    machine.ledTurnOn(2);
    HAL_Delay(100);
    machine.turnRight(+accel, +speed, machine_parameter::TURN_90_DEG_DISTANCE);
    machine.ledTurnOn(3);
    HAL_Delay(100);
    machine.turnLeft(+accel, +speed, machine_parameter::TURN_180_DEG_DISTANCE);
    machine.ledTurnOn(4);

    while(1) {}
}

void machineSelectMode() {
    Machine machine;
    machine_interrupt.attach(&machine, &Machine::interruptMachine);
    machine.start();
    machine.reset();

    while(1) {
        const uint8_t mode = machine.getLeftEncoderRotationCount();
        machine.ledTurnOn(mode);
        if(machine.selectedMode()) {

           if(mode == 0) {
               machine.ledTurnOn(7);
               machine.beepBuzzer(mode);
               HAL_Delay(1000);
//               machine.run(500.0f, 1000.0f);
//               while (1) {}
               machine.moveRunAndStop(1000.0f, 1000.0f, 400.0f);
               machine.reset();

            } else if(mode == 1) {
                machine.ledTurnOn(7);
                machine.beepBuzzer(mode);
                HAL_Delay(1000);
                machine.runLeftMethod(2000.0f, 1000.0f);
                machine.reset();

            } else if(mode == 2) {
                machine.ledTurnOn(7);
                machine.beepBuzzer(mode);
                HAL_Delay(1000);
                machine.runSpecifiedDistance(2000.0f, 1000.0f, 150.0f);
                machine.reset();

            } else if(mode == 3) {
                machine.ledTurnOn(7);
                machine.beepBuzzer(mode);
                HAL_Delay(1000);
                machine.moveRunAndStop(100.0f, 1000.0f, 500.0f);
//                machine.reset();

            } else if(mode == 4) {
                machine.ledTurnOn(7);
                machine.beepBuzzer(mode);
                HAL_Delay(1000);
                machine.turnLeft(200.0f, 500.0f,  machine_parameter::TURN_90_DEG_DISTANCE);
                machine.reset();

            } else if(mode == 5) {
                machine.ledTurnOn(7);
                machine.beepBuzzer(mode);
                HAL_Delay(1000);
                machine.consolePosition();
                machine.reset();

            } else if(mode == 6) {

            }

        }
    }
}

#ifdef __cplusplus
}
#endif


//#endif // ZUZUHALFTPPMOD1_APPLICATION_H
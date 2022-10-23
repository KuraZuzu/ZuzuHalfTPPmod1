/*!
* @file machine_test.cpp v1.0
* @Copyright © 2021 Kazushi Kurasawa
* @date 2021.02.13
*
* Released under the MIT license.
* see https://opensource.org/licenses/MIT
*/

#include "test.h"

//std::vector<callbackMemberFunctions> a;
//std::vector<std::function> &a;
//typedef void(Test::*Callback)();
//Callback a;

Test::Test()
: _led1(GPIOC, GPIO_PIN_3)
, _led2(GPIOC, GPIO_PIN_4)
, _led3(GPIOC, GPIO_PIN_5)
, _led_buss(_led3, _led2, _led1)
, _buzzer(PWMOut(htim8, TIM_CHANNEL_1))
, _lf_sensor(PWMOut(htim2, TIM_CHANNEL_1), AnalogInDMAStream(hadc1, 1), htim6)
, _ls_sensor(PWMOut(htim2, TIM_CHANNEL_2), AnalogInDMAStream(hadc1, 2), htim6)
, _rs_sensor(PWMOut(htim2, TIM_CHANNEL_3), AnalogInDMAStream(hadc1, 3), htim6)
, _rf_sensor(PWMOut(htim2, TIM_CHANNEL_4), AnalogInDMAStream(hadc1, 4), htim6)
, _battery(hadc1, 5)
, _gyro_sensor(hspi3, GPIOA, GPIO_PIN_4)
{
//    a = measureSpeedSetCallback;
    // Init functions.
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_ADC1_Init();
    MX_TIM1_Init();
//    MX_TIM2_Init();
    MX_TIM3_Init();
    MX_TIM4_Init();
    MX_TIM6_Init();
    MX_TIM7_Init();
    MX_TIM8_Init();
    MX_USART2_UART_Init();
    MX_SPI3_Init();

    _lf_sensor.start();
    _ls_sensor.start();
    _rs_sensor.start();
    _rf_sensor.start();
//    _battery.start();
    //    HAL_TIM_Base_Start_IT(&htim6);
    HAL_TIM_Base_Start_IT(&htim7);
}

void Test::buzzerDebug() {
    _buzzer.beep_x(3);
}

void Test::errorBuzzerDebug1() {
    _buzzer.error_v1();
}

void Test::errorBuzzerDebug2() {
    _buzzer.error_v2();
}

void Test::ledDebug() {
    _led1 = 1;
    _led2 = 0;
    _led3 = 0;
    HAL_Delay(50);
    _led1 = 0;
    _led2 = 1;
    _led3 = 0;
    HAL_Delay(50);
    _led1 = 0;
    _led2 = 0;
    _led3 = 1;
    HAL_Delay(50);
    _led1 = 0;
    _led2 = 0;
    _led3 = 0;
}

void Test::batteryConsoleDebug() {
    printf("bat: %f", batteryVoltage());
    printf("\r\n");
}

void Test::allSensorConsoleDebug() {
//        uint16_t sensor1 = adc1_dma_stream::adc_read(ADC1_Rank::LEFT_FRONT);
    uint16_t lf = _lf_sensor.read();
    uint16_t ls = _ls_sensor.read();
    uint16_t rs = _rs_sensor.read();
    uint16_t rf = _rf_sensor.read();
    printf("lf: %d   ls: %d  rs: %d  rf: %d  bat: %f"
           , static_cast<int>(lf)
           , static_cast<int>(ls)
           , static_cast<int>(rs)
           , static_cast<int>(rf)
           , batteryVoltage());

    batteryWarningDebug();
}

void Test::batteryWarningDebug() {
//    if(batteryVoltage() < 3.75) _buzzer.beep_x(4);
}

float Test::batteryVoltage() {
//    uint16_t bat = adc1_dma_stream::adc_read(ADC1_Rank::BATTERY);
//    uint16_t bat = _battery.read();
//    return 3.3f * static_cast<float32_t>(bat) / 0x0FFF * (1000 + 1000) / 1000;
}

void Test::bussOutDebug() {
    for (int32_t i = 0; i < 30; ++i) {
        _led_buss = i;
        HAL_Delay(300);
    }
}

void Test::gyroRead() {
    _gyro_sensor.temp();
}

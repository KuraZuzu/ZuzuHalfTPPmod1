/*!
* @file machine_test.cpp v1.0
* @Copyright © 2021 Kazushi Kurasawa
* @date 2021.02.13
*
* Released under the MIT license.
* see https://opensource.org/licenses/MIT
*/

#include "test.h"

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
, _l_wheel(Motor(htim1, TIM_CHANNEL_1, GPIOA, GPIO_PIN_6, true),Encoder(htim4,500*4,false), 13.5f, 10)
, _r_wheel(Motor(htim1, TIM_CHANNEL_2, GPIOA, GPIO_PIN_7, false),Encoder(htim3,500*4,true), 13.5f, 10)
{
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
    _battery.start();

    _l_wheel.start();
    _r_wheel.start();
//    HAL_TIM_Base_Start_IT(&htim6);
    HAL_TIM_Base_Start_IT(&htim7);
}

void Test::buzzer_debug() {
    _buzzer.beep_x(3);
}

void Test::error_v1_buzzer_debug() {
    _buzzer.error_v1();
}

void Test::error_v2_buzzer_debug() {
    _buzzer.error_v2();
}

void Test::led_debug() {
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

void Test::battery_console_debug() {
    printf("bat: %f", battery_voltage());
    printf("\r\n");
}

void Test::all_sensor_console_debug() {
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
           , battery_voltage());

//    battery_warning_debug();
}

void Test::battery_warning_debug() {
    if(battery_voltage() < 3.75) _buzzer.beep_x(4);
}

float Test::battery_voltage() {
//    uint16_t bat = adc1_dma_stream::adc_read(ADC1_Rank::BATTERY);
    uint16_t bat = _battery.read();
    return 3.3f * static_cast<float32_t>(bat) / 0x0FFF * (1000 + 1000) / 1000;
}

void Test::busout_debug() {
    for (int32_t i = 0; i < 30; ++i) {
        _led_buss = i;
        HAL_Delay(300);
    }
}

void Test::measure_speed_debug() {
    printf("L: %d  ", static_cast<int>(_l_wheel.getSpeed()));
    printf("R: %d\r\n", static_cast<int>(_r_wheel.getSpeed()));
}

void Test::wheel_move_debug() {
    while (1) {
        _l_wheel.run(100, 1000);
        _r_wheel.run(100, 1000);
    }
}


void Test::measureSpeedSetCallback() {
    _l_wheel.interruptMeasureSpeed();
    _r_wheel.interruptMeasureSpeed();
}

void Test::gyro_read() {
    _gyro_sensor.temp();
}

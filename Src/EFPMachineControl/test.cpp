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
        : _l_motor(htim1, TIM_CHANNEL_2, GPIOA, GPIO_PIN_7, true)
        , _r_motor(htim1, TIM_CHANNEL_1, GPIOA,  GPIO_PIN_6, false)
        , _l_encoder(htim4, machine_parameter::ENCODER_ONE_ROTATION_PULSE, false)
        , _r_encoder(htim3, machine_parameter::ENCODER_ONE_ROTATION_PULSE, true)
        , _battery(hadc1, 5)
        , _l_wheel(_l_motor, _l_encoder, _battery, 13.5f, 0.01f)
        , _r_wheel(_r_motor, _r_encoder, _battery, 13.5f, 0.01f)
        , _gyro_sensor(hspi3, GPIOA, GPIO_PIN_4)
        , _lf_sensor(DigitalOut(DIST_LED1_GPIO_Port, DIST_LED1_Pin), AnalogInDMAStream(hadc1, 1))
        , _ls_sensor(DigitalOut(DIST_LED2_GPIO_Port, DIST_LED2_Pin), AnalogInDMAStream(hadc1, 2))
        , _rs_sensor(DigitalOut(DIST_LED3_GPIO_Port, DIST_LED3_Pin), AnalogInDMAStream(hadc1, 3))
        , _rf_sensor(DigitalOut(DIST_LED4_GPIO_Port, DIST_LED4_Pin), AnalogInDMAStream(hadc1, 4))
        , _led_buss(DigitalOut(GPIOC, GPIO_PIN_3), DigitalOut(GPIOC, GPIO_PIN_4), DigitalOut(GPIOC, GPIO_PIN_5))
        , _buzzer(PWMOut(htim8, TIM_CHANNEL_1))
        , _odometry_sampling_time(0.01f) {

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
    _led_buss = 0b0111;
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

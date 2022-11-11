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
        , _lf_sensor(DigitalOut(DIST_LED1_GPIO_Port, DIST_LED1_Pin), AnalogInDMAStream(hadc1, 1), machine_parameter::convert_lf_func)
        , _ls_sensor(DigitalOut(DIST_LED2_GPIO_Port, DIST_LED2_Pin), AnalogInDMAStream(hadc1, 2), machine_parameter::convert_ls_func)
        , _rs_sensor(DigitalOut(DIST_LED3_GPIO_Port, DIST_LED3_Pin), AnalogInDMAStream(hadc1, 3), machine_parameter::convert_rs_func)
        , _rf_sensor(DigitalOut(DIST_LED4_GPIO_Port, DIST_LED4_Pin), AnalogInDMAStream(hadc1, 4), machine_parameter::convert_rf_func)
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
//    HAL_TIM_Base_Start_IT(&htim7);
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
    //距離センサとかを実行していると電圧の測定値が下がる。
    printf("bat: %f", batteryVoltage());
}

void Test::DistanceSensorConsoleDebug() {
    uint32_t lf=0, ls=0, rs=0, rf=0;
//    std::vector<uint16_t> lf_values(100, 0);
//    std::vector<uint16_t> ls_values(100, 0);
//    std::vector<uint16_t> rs_values(100, 0);
//    std::vector<uint16_t> rf_values(100, 0);
//    for (int i = 0; i < 100; ++i) {
//        lf_values[i] = _lf_sensor.getTestRawValue();
//        ls_values[i] = _ls_sensor.getTestRawValue();
//        rs_values[i] = _rs_sensor.getTestRawValue();
//        rf_values[i] = _rf_sensor.getTestRawValue();
//        HAL_Delay(10);
//    }
//    std::sort(lf_values.begin(), lf_values.end());
//    std::sort(ls_values.begin(), ls_values.end());
//    std::sort(rs_values.begin(), rs_values.end());
//    std::sort(rf_values.begin(), rf_values.end());
//    lf = lf_values[50];
//    ls = ls_values[50];
//    rs = rs_values[50];
//    rf = rf_values[50];
    lf = _lf_sensor.getTestRawValue();
    ls = _ls_sensor.getTestRawValue();
    rs = _rs_sensor.getTestRawValue();
    rf = _rf_sensor.getTestRawValue();

    printf("LF: %d   LS: %d  RS: %d  RF: %d\r\n"
            , static_cast<int>(lf)
            , static_cast<int>(ls)
            , static_cast<int>(rs)
            , static_cast<int>(rf)
    );
    HAL_Delay(10);
}

void Test::TestSensorLogDebug() {
    uint32_t lf=0, ls=0, rs=0, rf=0;

    std::vector<uint16_t> lf_log(1000, 0);
    std::vector<uint16_t> ls_log(1000, 0);
    std::vector<uint16_t> rs_log(1000, 0);
    std::vector<uint16_t> rf_log(1000, 0);
    for (int i = 0; i < 1000; ++i) {
        lf_log[i] = _lf_sensor.getTestRawValue();
        ls_log[i] = _ls_sensor.getTestRawValue();
        rs_log[i] = _rs_sensor.getTestRawValue();
        rf_log[i] = _rf_sensor.getTestRawValue();
        HAL_Delay(10);
    }
    for (int i = 0; i < 1000; ++i) {
        printf("LF: %d   LS: %d  RS: %d  RF: %d\r\n"
                , static_cast<int>(lf_log[i])
                , static_cast<int>(ls_log[i])
                , static_cast<int>(rs_log[i])
                , static_cast<int>(rf_log[i])
        );
    }
    printf("_____END_______\r\n");
    while (1) {}
}

void Test::batteryWarningDebug() {
    if(batteryVoltage() < 3.75) _buzzer.beep_x(4);
}

float32_t Test::batteryVoltage() {
    return 3.3f * static_cast<float32_t>(_battery.read()) / 0x0FFF * machine_parameter::BATTERY_VOLTAGE_RATIO;
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
//
// Created by 倉澤　一詩 on 2021/12/18.
//

#ifndef ZUZUHALFTPPMOD1_SENSOR_PARAMETER_H
#define ZUZUHALFTPPMOD1_SENSOR_PARAMETER_H

#include "arm_math.h"

uint16_t lfConvert(uint16_t x){
    return a + b*log(x);
}

uint16_t lsConvert(uint16_t x){
    constexpr float32_t a = 1998.7836;
    constexpr float32_t b = -0.0496777255
    return a + b*log(x);
}

uint16_t rsConvert(uint16_t x){
    constexpr float32_t a = -146.603653;
    constexpr float32_t b = 8679.00371;
    return a + b/static_cast<float32_t>(x);
}

uint16_t rfConvert(uint16_t x){
    constexpr float32_t a = -443.029703;
    constexpr float32_t b = 25776.23762;
    return a + b/static_cast<float32_t>(x);
}

#endif //ZUZUHALFTPPMOD1_SENSOR_PARAMETER_H

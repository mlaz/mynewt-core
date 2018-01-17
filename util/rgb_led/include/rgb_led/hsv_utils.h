#ifndef HSV_UTILS_H
#define PWM_RGB_LED_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void hsv2rgb(uint8_t h, uint8_t s, uint8_t v, uint8_t* rgb);

#ifdef __cplusplus
}
#endif

#endif /* HSV_UTILS_H */

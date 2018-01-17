#ifndef PWM_RGB_LED_H
#define PWM_RGB_LED_H

#include "rgb_led/rgb_led.h"
#include <pwm/pwm.h>

#ifdef __cplusplus
extern "C" {
#endif

struct pwm_rgb_led {
    struct rgb_led led_rgb;
    struct pwm_dev* dev;
    uint16_t top_val;
    uint8_t r_chan;
    uint8_t g_chan;
    uint8_t b_chan;
};

struct pwm_led_cfg {
    struct os_eventq* evq;
    struct pwm_dev* dev;
    bool anode;
    uint32_t freq;
    uint8_t r_pin;
    uint8_t g_pin;
    uint8_t b_pin;
    uint8_t r_chan;
    uint8_t g_chan;
    uint8_t b_chan;
};

struct rgb_led* pwm_rgb_led_init(struct pwm_rgb_led *led, struct pwm_led_cfg *cfg);

#ifdef __cplusplus
}
#endif

#endif /* PWM_RGB_LED_H */

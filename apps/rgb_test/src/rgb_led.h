/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#ifndef RGB_LED_H
#define RGB_LED_H

#include <pwm/pwm.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct rgb_led {
    struct pwm_dev *dev;
    struct os_callout_func *g_rgbled_breathe;
    struct os_callout_func *g_rgbled_fade;
    uint16_t top_val; /* PWM maximum value */
    uint8_t r_chan; /* Red channel number. */
    uint8_t g_chan; /* Green channel number. */
    uint8_t b_chan; /* Blue channel number. */
    uint16_t r_val; /* Red component value. */
    uint16_t g_val; /* Green component value. */
    uint16_t b_val; /* Blue component value. */
    uint8_t mode; /* may be either FIXED, FADE or BREATHE*/
    uint16_t r_fade; /* Red component value for fading. */
    uint16_t g_fade; /* Green component value for fading. */
    uint16_t b_fade; /* Blue component value for fading. */
} rgb_led_t;

/**
 * Initialize an RGB LED in a single device setup.
 * (multiple channels per device)
 *
 * @param dev The pwm device. (Previously configured)
 * @param r_chan The channel number for the red component.
 * @param r_chan The channel number for the red component.
 * @param r_chan The channel number for the red component.
 *
 * @return the address of the rgb_led structure on success, NULL on failure.
 */
rgb_led_t* init_rgb_led(struct pwm_dev *dev,
                        uint16_t top_val,
                        uint8_t r_chan,
                        uint8_t g_chan,
                        uint8_t b_chan);

/**
 * Set the RGB LED mode to Constant.
 * (one channel per device)
 *
 * @param les The RGB LED to configure.
 * @param r_val The color value for the red component.
 * @param g_val The color value for the green component.
 * @param g_val The color value for the blue component.
 *
 * @return 0 on success, negative on error.
 */
int rgb_led_set_color(rgb_led_t* led,
                      uint8_t r_val,
                      uint8_t g_val,
                      uint8_t b_val);

/**
 * Set the RGB LED mode to Constant.
 * (one channel per device)
 *
 * @param les The RGB LED to configure.
 * @param r_val The color value for the red component.
 * @param g_val The color value for the green component.
 * @param g_val The color value for the blue component.
 *
 * @return 0 on success, negative on error.
 */
int rgb_fade_to_color(rgb_led_t* led,
                      uint8_t r_val,
                      uint8_t g_val,
                      uint8_t b_val);
/**
 * Set the RGB LED mode to Breathe.
 *
 * @param dev The RGB LED device to configure.
 * @param interval The period of the sequence.
 * @param r_dev The color value for the red component.
 * @param g_dev The color value for the green component.
 * @param g_dev The color value for the blue component.
 *
 * @return 0 on success, negative on error.
 */
int rgb_led_breathe(rgb_led_t* led,
                    uint32_t interval);

/**
 * Set the RGB LED mode to Off.
 *
 * @param dev The pwm device to configure.
 *
 * @return 0 on success, negative on error.
 */
int rgb_led_set_off(rgb_led_t* led);

#ifdef __cplusplus
}
#endif

#endif

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
#include <os/os.h>
#include "os/os_callout.h"

#ifdef __cplusplus
extern "C" {
#endif

struct rgb_led {
    struct pwm_dev *dev;
    struct os_callout c_rgbled_breathe;
    struct os_callout c_rgbled_fade;
    uint16_t max_val; /* Color component maximum value. */
    uint8_t mode; /* may be either FIXED, FADE or BREATHE. */
    uint8_t brightness; /* Brightness level, from 0 to 100. */
    uint8_t save_bness; /* Slot for keeping brightness level. */
    bool breathe_up; /* Breathing up/down flag. */
    uint32_t interval_ticks; /* Breathing/Fading interval. */
    uint8_t r_chan; /* Red channel number. */
    uint8_t g_chan; /* Green channel number. */
    uint8_t b_chan; /* Blue channel number. */
    uint16_t r_val; /* Red component value. */
    uint16_t g_val; /* Green component value. */
    uint16_t b_val; /* Blue component value. */
    uint16_t r_duty; /* Red component duty cycle. */
    uint16_t g_duty; /* Green component duty cycle. */
    uint16_t b_duty; /* Blue component duty cycle. */
};

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
struct rgb_led* init_rgb_led(struct pwm_dev *dev,
                             uint16_t top_val,
                             uint8_t r_chan,
                             uint8_t g_chan,
                             uint8_t b_chan,
                             struct os_eventq *c_rgbled_evq);

/**
 * Set brightness level from 0 to 255.
 *
 * @param led The struct rgb_led structure.
 * @param bness The brightness level.
 */
void rgb_led_set_bness(struct rgb_led *led, uint8_t bness);

/**
 * Set brightness level from 0 to 255.
 *
 * @param led The struct rgb_led structure.
 *
 * @return The brightness level.
 */
uint8_t rgb_led_get_bness(struct rgb_led *led);

/**
 * Set the RGB LED mode to Constant using a given color.
 *
 * @param led The struct rgb_led structure.
 * @param r_val The color value for the red component.
 * @param g_val The color value for the green component.
 * @param b_val The color value for the blue component.
 */
void rgb_led_set_color(struct rgb_led* led,
                       uint8_t r_val,
                       uint8_t g_val,
                       uint8_t b_val);

/**
 * Fade the RGB LED to a given color.
 *
 * @param led The struct rgb_led structure.
 * @param r_val The color value for the red component.
 * @param g_val The color value for the green component.
 * @param g_val The color value for the blue component.
 */
void rgb_fade_to_color(struct rgb_led* led,
                       uint32_t interval,
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
 */
void rgb_led_breathe(struct rgb_led* led, uint32_t interval);

/**
 * Set the RGB LED mode to Fixed on the previously configured color.
 *
 * @param dev The RGB LED device to configure.
 */
void rgb_led_breathe_stop(struct rgb_led *led);

/**
 * Set the RGB LED mode to Off.
 *
 * @param dev The pwm device to configure.
 */
void rgb_led_set_off(struct rgb_led* led);

#ifdef __cplusplus
}
#endif

#endif

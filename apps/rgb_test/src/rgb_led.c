#include "rgb_led.h"
#include <pwm/pwm.h>

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
                        uint8_t b_chan)
{
    if (dev == NULL) {
        return NULL;
    }

    rgb_led_t* led = (rgb_led_t*) calloc(1, sizeof(rgb_led_t));
    led->dev = dev;
    led->top_val = top_val;
    led->r_chan = r_chan;
    led->g_chan = g_chan;
    led->b_chan = b_chan;
    return led;
}

static uint16_t cvt_color_duty(uint8_t color, uint16_t top_val)
{
    return (((uint16_t) color) * top_val) / 255;
}

/**
 * Set the RGB LED mode to Constant.
 * (one channel per device)
 *
 * @param rgb_led The RGB LED to configure.
 * @param r_val The color value for the red component.
 * @param g_val The color value for the green component.
 * @param g_val The color value for the blue component.
 *
 * @return 0 on success, negative on error.
 */
int rgb_led_set_color(rgb_led_t *led,
                      uint8_t r_val,
                      uint8_t g_val,
                      uint8_t b_val)
{
    led->r_val = cvt_color_duty(r_val, led->top_val);
    led->g_val = cvt_color_duty(g_val, led->top_val);
    led->b_val = cvt_color_duty(b_val, led->top_val);

    pwm_enable_duty_cycle(led->dev, led->r_chan, led->r_val);
    pwm_enable_duty_cycle(led->dev, led->g_chan, led->g_val);
    pwm_enable_duty_cycle(led->dev, led->b_chan, led->b_val);
    return 0;
}

/**
 * Set the RGB LED mode to Constant.
 * (one channel per device)
 *
 * @param rgb_led The RGB LED to configure.
 * @param color The color value in the 0x00RRGGBB format.
 *
 * @return 0 on success, negative on error.
 */
int rgb_led_set_webcolor(rgb_led_t *led,
                          uint32_t color)
{
    rgb_led_set_color(led,
                      (uint8_t) ((color & 0xFF0000) >> 16),
                      (uint8_t) (color & 0x00FF00) >> 8,
                      (uint8_t) (color & 0x0000FF));
    return 0;
}

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
int rgb_fade_to_color(rgb_led_t *led,
                      uint8_t r_val,
                      uint8_t g_val,
                      uint8_t b_val)
{
    led->r_val = cvt_color_duty(r_val, led->top_val);
    led->g_val = cvt_color_duty(g_val, led->top_val);
    led->b_val = cvt_color_duty(b_val, led->top_val);

    return 0;
}

/**
 * Set the RGB LED mode to Breathe.
 *
 * @param dev The RGB LED device to configure.
 * @param interval The period of the sequence.
 *
 * @return 0 on success, negative on error.
 */
int rgb_led_breathe(rgb_led_t *led,
                    uint32_t interval)
{
    return (0);
}

/**
 * Set the RGB LED mode to Off.
 *
 * @param dev The pwm device to configure.
 *
 * @return 0 on success, negative on error.
 */
int rgb_led_set_off(rgb_led_t *led)
{
    led->r_val = 0;
    led->g_val = 0;
    led->b_val = 0;
    pwm_enable_duty_cycle(led->dev, led->r_chan, 0);
    pwm_enable_duty_cycle(led->dev, led->g_chan, 0);
    pwm_enable_duty_cycle(led->dev, led->b_chan, 0);
    return 0;
}

#include "rgb_led.h"

#include <sysinit/sysinit.h>
#include <pwm/pwm.h>
#include <os/os.h>

/**
 * Setting a configured color.
 */
static void
set_color_with_brightness(struct rgb_led *led)
{
    uint16_t inv_bness = MAX_BNESS - led->brightness;
    if (inv_bness == 0) {
        led->r_duty = led->r_val;
        led->g_duty = led->g_val;
        led->b_duty = led->b_val;
    } else if (inv_bness ==  MAX_BNESS) {
        led->r_duty = 0;
        led->g_duty = 0;
        led->b_duty = 0;
    } else {
        led->r_duty = led->r_val / inv_bness;
        led->g_duty = led->g_val / inv_bness;
        led->b_duty = led->b_val / inv_bness;
    }

    pwm_enable_duty_cycle(led->dev, led->r_chan, led->r_duty);
    pwm_enable_duty_cycle(led->dev, led->g_chan, led->g_duty);
    pwm_enable_duty_cycle(led->dev, led->b_chan, led->b_duty);
}

/**
 * RGB LED breathe callback.
 */
static void
breathe_cb(struct os_event *ev)
{
    assert(ev != NULL);

    struct rgb_led* led = ev->ev_arg;

    if (led->brightness >= MAX_BNESS || led->brightness == 0) {
        led->breathe_up = ! led->breathe_up;
    }
    led->brightness += (led->breathe_up) ? 1 : -1;
    set_color_with_brightness(led);
    os_callout_reset(&led->c_rgbled_breathe, led->interval_ticks);
}

/**
 * RGB LED fade callback.
 */
static void
fade_cb(struct os_event *ev)
{
    bool done = true;
    struct rgb_led* led;
    int16_t diff;

    assert(ev != NULL);
    led = ev->ev_arg;

    diff = led->r_duty - led->r_target;
    if (diff) {
        done = false;
        led->r_duty += (diff < 0) ? 1 : -1;
    }

    diff = led->g_duty - led->g_target;
    if (diff) {
        done = false;
        led->g_duty += (diff < 0) ? 1 : -1;
    }

    diff = led->b_duty - led->b_target;
    if (diff) {
        done = false;
        led->b_duty += (diff < 0) ? 1 : -1;
    }

    if (!done) {
        set_color_with_brightness(led);
        os_callout_reset(&led->c_rgbled_fade, led->interval_ticks);
    }
}

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
struct rgb_led*
init_rgb_led(struct pwm_dev *dev,
             struct os_eventq *c_rgbled_evq,
             uint16_t top_val,
             uint8_t r_chan,
             uint8_t g_chan,
             uint8_t b_chan)
{
    if (dev == NULL) {
        return NULL;
    }

    struct rgb_led* led = (struct rgb_led*) calloc(1, sizeof(struct rgb_led));
    led->dev = dev;
    led->max_val = top_val;
    led->r_chan = r_chan;
    led->g_chan = g_chan;
    led->b_chan = b_chan;
    led->brightness = MAX_BNESS / 2; /* brightness defaults to 50% */

    /* TODO: add some checking for a null evq pointer */
    os_callout_init(&(led->c_rgbled_breathe),
                    c_rgbled_evq,
                    breathe_cb,
                    led);
    os_callout_init(&(led->c_rgbled_fade),
                    c_rgbled_evq,
                    fade_cb,
                    led);
    return led;
}

void
rgb_led_set_bness(struct rgb_led *led, uint8_t bness)
{
    led->brightness = (bness < MAX_BNESS) ? bness : MAX_BNESS;
    set_color_with_brightness(led);
}

uint8_t
rgb_led_get_bness(struct rgb_led *led)
{
    return led->brightness;
}

/**
 * Set the RGB LED mode to Constant.
 *
 * @param rgb_led The RGB LED to configure.
 * @param r_val The color value for the red component.
 * @param g_val The color value for the green component.
 * @param g_val The color value for the blue component.
 *
 * @return 0 on success, negative on error.
 */
void
rgb_led_set_color(struct rgb_led *led,
                  uint8_t r_val,
                  uint8_t g_val,
                  uint8_t b_val)
{
    led->r_val = (r_val * led->max_val) / 255;
    led->g_val = (g_val * led->max_val) / 255;
    led->b_val = (b_val * led->max_val) / 255;

    set_color_with_brightness(led);
}

/**
 * Set the RGB LED mode to Constant.
 * (one channel per device)
 *
 * @param rgb_led The RGB LED to configure.
 * @param color The color value in the 0x00RRGGBB format.
 */
void
rgb_led_set_webcolor(struct rgb_led *led, uint32_t color)
{
    rgb_led_set_color(led,
                      (uint8_t) ((color & 0xFF0000) >> 16),
                      (uint8_t) (color & 0x00FF00) >> 8,
                      (uint8_t) (color & 0x0000FF));
}

/**
 * Set the RGB LED mode to Constant.
 * (one channel per device)
 *
 * @param les The RGB LED to configure.
 * @param interval The interval between steps.
 * @param r_val The color value for the red component.
 * @param g_val The color value for the green component.
 * @param g_val The color value for the blue component.
 */
void
rgb_fade_to_color(struct rgb_led *led,
                  uint32_t interval,
                  uint8_t r_val,
                  uint8_t g_val,
                  uint8_t b_val)
{
    led->interval_ticks = (interval * (OS_TICKS_PER_SEC / 1000));
    led->r_target = (r_val * led->max_val) / 255;
    led->g_target = (g_val * led->max_val) / 255;
    led->b_target = (b_val * led->max_val) / 255;

    os_callout_reset(&(led->c_rgbled_fade), led->interval_ticks);
}

/**
 * Set the RGB LED mode to Breathe.
 *
 * @param dev The RGB LED device to configure.
 * @param interval The period of the sequence.
 *
 * @return 0 on success, negative on error.
 */
void
rgb_led_breathe(struct rgb_led *led, uint32_t period)
{
    led->save_bness = led->brightness;
    led->breathe_up = true;
    led->interval_ticks = (period * OS_TICKS_PER_SEC / 1000) / MAX_BNESS;
    os_callout_reset(&(led->c_rgbled_breathe), led->interval_ticks);
}

/**
 * Set the RGB LED mode to Breathe.
 *
 * @param dev The RGB LED device to configure.
 * @param interval The period of the sequence.
 *
 * @return 0 on success, negative on error.
 */
void
rgb_led_breathe_stop(struct rgb_led *led)
{
    led->brightness = led->save_bness;
    os_callout_stop(&(led->c_rgbled_breathe));
}

/**
 * Set the RGB LED mode to Off.
 *
 * @param dev The pwm device to configure.
 *
 * @return 0 on success, negative on error.
 */
void
rgb_led_set_off(struct rgb_led *led)
{
    led->r_val = 0;
    led->g_val = 0;
    led->b_val = 0;
    set_color_with_brightness(led);
}

#include "rgb_led/rgb_led.h"
#include "easing/easing.h"

#include <console/console.h>
#include <sysinit/sysinit.h>
#include <pwm/pwm.h>
#include <os/os.h>

struct array_mdata {
    struct rgb_led** array;
    uint8_t len;
};

static struct array_mdata led_array;

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

    if (led->steps >= BREATHE_STEPS || led->steps == 0) {
        led->breathe_up = ! led->breathe_up;
    }
    led->steps += (led->breathe_up) ? 1 : -1;

    led->brightness = exp_sin_custom_io(led->steps);
    /* console_printf("\nbness = %d, steps = %u\n", led->brightness, led->steps); */
    set_color_with_brightness(led);
    os_callout_reset(&led->c_rgbled_breathe, led->interval_ticks);
}

/**
 * Multiple synchronized RGB LED breathing callback.
 */
static void
breathe_array_cb(struct os_event *ev)
{
    uint32_t idx;
    uint16_t bness;
    struct rgb_led** leds = led_array.array; //FIXME!

    if (leds[0]->steps >= BREATHE_STEPS || leds[0]->steps == 0) {
        leds[0]->breathe_up = ! leds[0]->breathe_up;
    }

    leds[0]->steps += (leds[0]->breathe_up) ? 1 : -1;

    bness =  exp_sin_custom_io(leds[0]->steps);
    for (idx = 0; idx < led_array.len; idx++)
    {
        leds[idx]->brightness = bness;
        set_color_with_brightness(leds[idx]);
    }
    os_callout_reset(&leds[0]->c_array_breathe, leds[0]->interval_ticks);
}

/**
 * Multiple synchronized RGB LED breathing callback.
 * Probably less optimized than breathe_array_cb(), computes the brightness
 * value for every LED sparately.
 */
static void
breathe_array_unsync_cb(struct os_event *ev)
{
    uint32_t idx;
    struct rgb_led** leds = led_array.array; //FIXME!

    for (idx = 0; idx < led_array.len; idx++)
    {
        if (leds[idx]->steps >= BREATHE_STEPS || leds[idx]->steps == 0) {
            leds[idx]->breathe_up = ! leds[idx]->breathe_up;
        }

        leds[idx]->steps += (leds[0]->breathe_up) ? 1 : -1;
        leds[idx]->brightness = exp_sin_custom_io(leds[idx]->steps);;
        set_color_with_brightness(leds[idx]);
    }
    os_callout_reset(&leds[0]->c_array_breathe_unsync,
                     leds[0]->interval_ticks);
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
    os_callout_init(&(led->c_array_breathe),
                    c_rgbled_evq,
                    breathe_array_cb,
                    led);
    os_callout_init(&(led->c_array_breathe_unsync),
                    c_rgbled_evq,
                    breathe_array_unsync_cb,
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

/* /\** */
/*  * Fade to brightness level from 0 to 255. */
/*  * */
/*  * @param led The struct rgb_led structure. */
/*  * @param bness The brightness level. */
/*  *\/ */
/* void rgb_fade_to_bness(struct rgb_led *led, uint8_t bness) */
/* { */
/*     led->interval_ticks = (interval * (OS_TICKS_PER_SEC / 1000)); */
/*     led->r_target = (r_val * led->max_val) / 255; */
/*     led->g_target = (g_val * led->max_val) / 255; */
/*     led->b_target = (b_val * led->max_val) / 255; */

/*     os_callout_reset(&(led->c_rgbled_fade), led->interval_ticks); */
/* } */

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
    rgb_led_breathe_delayed(led, period, 0);
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
rgb_led_breathe_delayed(struct rgb_led *led, uint32_t period, uint32_t delay)
{
    led->save_bness = led->brightness;
    led->breathe_up = true;
    led->steps = 1;
    led->interval_ticks = (period * OS_TICKS_PER_SEC / 1000) / BREATHE_STEPS;
    os_callout_reset(&(led->c_rgbled_breathe), delay * OS_TICKS_PER_SEC / 1000);
}

/**
 * Set the RGB LED mode to Fixed.
 *
 * @param dev The RGB LED device to configure.
 */
void
rgb_led_breathe_stop(struct rgb_led *led)
{
    led->brightness = led->save_bness;
    os_callout_stop(&(led->c_rgbled_breathe));
}

/**
 * Set the multiple RGB LED mode to Breathe. Breathing is simultaneous.
 *
 * @param dev The RGB LED device array.
 * @param len The RGB LED device array length.
 * @param period The period of the sequence.
 */
void
rgb_led_breathe_array(struct rgb_led** leds, uint8_t len ,uint32_t period)
{
    int idx;
    leds[0]->breathe_up = true;
    leds[0]->interval_ticks = (period * OS_TICKS_PER_SEC / 1000) / MAX_BNESS;
    leds[0]->steps = 0;
    for (idx = 0; idx < len; idx++)
    {
         leds[idx]->save_bness = leds[idx]->brightness;
    }
    led_array.array = leds;
    led_array.len = len;
    os_callout_reset(&(leds[0]->c_array_breathe), leds[0]->interval_ticks);
}

/**
 * Set the multiple RGB LED mode to Breathe unsynchronously.
 * Brightness values are updated simultaniously, although step values may
 * be different allowing unsynchronized breathing.
 *
 * @param leds The RGB LED array.
 * @param start The array containing the steps value for each RGB LED.
 * @param len The RGB LED device array length.
 * @param period The period of the sequence.
 */
void
rgb_led_breathe_unsync_array(struct rgb_led** leds,
                             uint32_t* start,
                             uint8_t len ,
                             uint32_t period)
{
    int idx;
    leds[0]->interval_ticks = (period * OS_TICKS_PER_SEC / 1000) / MAX_BNESS;
    for (idx = 0; idx < len; idx++)
    {
        leds[idx]->breathe_up = (start[idx] <= BREATHE_STEPS);
        leds[idx]->steps = (start[idx] <= BREATHE_STEPS) ?
            start[idx] :
            start[idx] - BREATHE_STEPS;
        leds[idx]->save_bness = leds[idx]->brightness;
    }
    led_array.array = leds;
    led_array.len = len;
    os_callout_reset(&(leds[0]->c_array_breathe_unsync),
                     0);
}


/**
 * Set the RGB LED mode to Off.
 *
 * @param dev The pwm device to configure.
 */
void
rgb_led_set_off(struct rgb_led *led)
{
    led->r_val = 0;
    led->g_val = 0;
    led->b_val = 0;
    set_color_with_brightness(led);
}

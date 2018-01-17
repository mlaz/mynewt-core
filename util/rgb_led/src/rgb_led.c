#include "rgb_led/rgb_led.h"
#include "rgb_led/hsv_utils.h"
#include "easing/easing.h"

#include <console/console.h>
#include <sysinit/sysinit.h>
#include <os/os.h>

/* #include <math.h> */

/**
 * Update color on a given RGB LED.
 * Preforms the operation on the hardware.
 */
static inline void
update_color(struct rgb_led *led)
{
    uint8_t rgb[3];
    hsv2rgb(led->hue,
            led->saturation,
            led->brightness,
            rgb);
    led->set_color_rgb(led, rgb);
}

/**
 * RGB LED breathe callback.
 */
static void
breathe_cb(struct os_event *ev)
{
    assert(ev != NULL);

    struct rgb_led* led = ev->ev_arg;
    struct led_motion* motion = (struct led_motion*) led;

    if (motion->steps >= motion->breathe_max || motion->steps == motion->breathe_min) {
        motion->breathe_up = ! motion->breathe_up;
    }
    motion->steps += (motion->breathe_up) ? 1 : -1;

    led->brightness = (uint8_t) motion->easing(motion->steps,
                                               BREATHE_STEPS,
                                               MAX_BNESS);
    update_color(led);
    /* console_printf("\nbness = %u, steps = %lu\n", led->brightness, led->steps); */

    os_callout_reset(&motion->c_rgbled_breathe, motion->interval_ticks);
}

/**
 * RGB LED fade callback.
 */
static void
fade_cb(struct os_event *ev)
{
    assert(ev != NULL);

    struct rgb_led* led = ev->ev_arg;
    struct led_motion* motion = (struct led_motion*) led;

    motion->steps += (motion->breathe_up) ? 1 : -1;

    /* if (motion->steps <= motion->max_steps) { */
    /*     led->brightness += (motion->breathe_up) ? */
    /*         motion->fade_ratio : */
    /*         -motion->fade_ratio; */
    /* } else { */
    /*     led->brightness = motion->target_bness; */
    /* } */
    led->brightness = (uint8_t) motion->easing(motion->steps,
                                               motion->max_steps,
                                               MAX_BNESS);
    update_color(led);
    /* console_printf("\nbness = %u, steps = %lu\n", led->brightness, led->steps); */

    if (led->brightness < motion->target_bness &&
        motion->steps < (motion->max_steps)) {
        os_callout_reset(&motion->c_rgbled_fade, motion->interval_ticks);
    }
}

/**
 * Set the easing function to be used on a given RGB LED.
 *
 * @param led The struct rgb_led structure.
 * @param func The easing_funt_t, easing function.
 */
void
rgb_led_set_easing(struct rgb_led *led, easing_func_t func)
{
    struct led_motion* motion = (struct led_motion*) led;
    motion->easing = func;
}

/**
 * Set brightness level from 0 to 255.
 *
 * @param led The struct rgb_led structure.
 * @param bness The brightness level.
 */
void
rgb_led_set_bness(struct rgb_led *led, uint8_t bness)
{
    led->brightness = (bness < MAX_BNESS) ? bness : MAX_BNESS;
    update_color(led);
}

/**
 * Get brightness level.
 *
 * @param led The struct rgb_led structure.
 *
 * @return The brightness level.
 */
uint8_t
rgb_led_get_bness(struct rgb_led *led)
{
    return led->brightness;
}

/**
 * Fade to brightness level from 0 to 255.
 *
 * @param led The struct rgb_led structure.
 * @param tgt_bness The target brightness level.
 * @param duration The duration of the motion, in milliseconds.
 * @param delay The delay after which the motion should start, in milliseconds.
 */
void rgb_led_fade_to_bness(struct rgb_led *led,
                       uint8_t tgt_bness,
                       uint32_t duration,
                       uint32_t delay)
{
    /* uint8_t steps; */
    /* uint8_t result; */
    struct led_motion* motion = (struct led_motion*) led;

    if (duration > 1000) {
        motion->interval_ticks =
            ((float)duration / 1000) / (float)OS_TICKS_PER_SEC;
        motion->max_steps = OS_TICKS_PER_SEC;
    } else {
        motion->interval_ticks = 1;
        motion->max_steps = ((float)duration / 1000) * OS_TICKS_PER_SEC;
    }

    motion->fade_ratio = tgt_bness / motion->max_steps;
    motion->steps = 0;
    console_printf("\nticks = %lu, max_steps = %lu", motion->interval_ticks, motion->max_steps);
    /* if (tgt_bness >= led->brightness) { */
        motion->breathe_up = true;
        /* steps = 0; */
        /* result = 0; */
        /* while ((steps < BREATHE_STEPS) && (result < led->brightness)) */
        /* { */
        /*     result = (uint8_t) led->easing(steps); */
        /*     steps++; */
        /* } */
    /* }  else { */
    /*     led->breathe_up = false; */
    /*     steps = BREATHE_STEPS; */
    /*     result = MAX_BNESS; */
    /*     while ((steps > 0) && (result > led->brightness) */
    /*     { */
    /*         result = (uint8_t) led->easing(steps); */
    /*         steps--; */
    /*     } */
    /* } */

    motion->target_bness = tgt_bness;
    os_callout_reset(&motion->c_rgbled_fade, ((float)delay / 1000) * OS_TICKS_PER_SEC);
}

/**
 * Set an HSV color to the LED.
 *
 * @param rgb_led The RGB LED to configure.
 * @param r_val The color value for the red component.
 * @param g_val The color value for the green component.
 * @param g_val The color value for the blue component.
 *
 * @return 0 on success, negative on error.
 */
void
rgb_led_set_color_hsv(struct rgb_led *led,
                      uint8_t hue,
                      uint8_t saturation,
                      uint8_t value)
{
    led->hue = hue;
    led->saturation = saturation;
    led->brightness = value;
    update_color(led);
}


/**
 * Set an RGB color to the LED.
 *
 * @param rgb_led The RGB LED to configure.
 * @param r_val The color value for the red component.
 * @param g_val The color value for the green component.
 * @param g_val The color value for the blue component.
 *
 * @return 0 on success, negative on error.
 */
void
rgb_led_set_color_rgb(struct rgb_led *led,
                      uint8_t r_val,
                      uint8_t g_val,
                      uint8_t b_val)
{
    uint8_t rgb[3] = {r_val, g_val, b_val};
    led->set_color_rgb(led, rgb);
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
    led->set_color_rgb(led, (uint8_t *) &color);
}

/* /\** */
/*  * Set the RGB LED mode to Constant. */
/*  * (one channel per device) */
/*  * */
/*  * @param les The RGB LED to configure. */
/*  * @param interval The interval between steps. */
/*  * @param r_val The color value for the red component. */
/*  * @param g_val The color value for the green component. */
/*  * @param g_val The color value for the blue component. */
/*  *\/ */
/* void */
/* rgb_led_fade_to_color(struct rgb_led *led, */
/*                   uint32_t interval, */
/*                   uint8_t r_val, */
/*                   uint8_t g_val, */
/*                   uint8_t b_val) */
/* { */
/*     motion = (struct led_motion) led; */

/*     led->interval_ticks = (interval * (OS_TICKS_PER_SEC / 1000)); */
/*     led->r_target = (r_val * led->max_val) / 255; */
/*     led->g_target = (g_val * led->max_val) / 255; */
/*     led->b_target = (b_val * led->max_val) / 255; */

/*     os_callout_reset(&(led->c_rgbled_fade), led->interval_ticks); */
/* } */

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
    rgb_led_breathe_delayed(led, period, 0, BREATHE_STEPS, 0);
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
rgb_led_breathe_delayed(struct rgb_led *led,
                        uint32_t period,
                        uint32_t delay,
                        uint32_t max_bness,
                        uint32_t min_bness)
{
    struct led_motion* motion = (struct led_motion*) led;

    motion->breathe_up = true;
    motion->steps = 1;
    motion->breathe_max = max_bness;
    motion->breathe_min = min_bness;
    motion->interval_ticks = (period * OS_TICKS_PER_SEC / 1000) / BREATHE_STEPS;
    os_callout_reset(&motion->c_rgbled_breathe, delay * OS_TICKS_PER_SEC / 1000);
}

/**
 * Set the RGB LED mode to Fixed.
 *
 * @param dev The RGB LED device to configure.
 */
void
rgb_led_breathe_stop(struct rgb_led *led)
{
    struct led_motion* motion = (struct led_motion*) led;
    os_callout_stop(&motion->c_rgbled_breathe);
}

/**
 * Set the RGB LED mode to off.
 * Preserves configured HSV configured values.
 * @param led
 */
void
rgb_led_set_off(struct rgb_led *led)
{
    uint8_t rgb[3] = {0, 0, 0};
    led->set_color_rgb(led, rgb);
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
rgb_led_init(struct rgb_led *led,
             struct os_eventq *evq)
{
    struct led_motion* motion;

    assert (evq != NULL);

    led->hue = 0;
    led->saturation = 0;
    led->brightness = 0;

    motion = (struct led_motion*) led;
    motion->easing = exp_sin_custom_io;
    motion->breathe_max = BREATHE_STEPS;
    motion->breathe_min = 0;

    os_callout_init(&motion->c_rgbled_breathe,
                    evq,
                    breathe_cb,
                    led);
    os_callout_init(&motion->c_rgbled_fade,
                    evq,
                    fade_cb,
                    led);
    return led;
}

#include "pwm_rgb_led/pwm_rgb_led.h"

#define R 0
#define G 1
#define B 2

/**
 * Setting a color.
 */
static void
pwm_rgb_led_set_color(struct rgb_led *led, uint8_t *rgb)
{
    uint16_t duty[3];
    struct pwm_rgb_led *pwm_led = (struct pwm_rgb_led *) led;

    /* Converting from 8bit values to 16bit dutycycle fraction values. */
    duty[R] = (uint16_t) rgb[R] * 65535 / 255;
    duty[G] = (uint16_t) rgb[G] * 65535 / 255;
    duty[B] = (uint16_t) rgb[B] * 65535 / 255;

    pwm_enable_duty_cycle(pwm_led->dev, pwm_led->r_chan, duty[R]);
    pwm_enable_duty_cycle(pwm_led->dev, pwm_led->g_chan, duty[G]);
    pwm_enable_duty_cycle(pwm_led->dev, pwm_led->b_chan, duty[B]);
}

/**
 * Initialize a PWM RGB LED. Returns a pointer an rgb_led instance, ready to use.
 */
struct rgb_led*
pwm_rgb_led_init(struct pwm_rgb_led *led, struct pwm_led_cfg *cfg)
{
    uint32_t base_freq;
    struct rgb_led *led_rgb;
    struct pwm_chan_cfg chan_conf = {
        .pin = cfg->r_pin,
        .inverted = cfg->anode,
        .data = NULL
    };

    led->dev = cfg->dev;
    base_freq = pwm_get_clock_freq(led->dev);
    led->top_val = (uint16_t) (base_freq / cfg->freq);

    pwm_chan_config(led->dev, cfg->r_chan, &chan_conf);
    chan_conf.pin = cfg->g_pin;
    pwm_chan_config(led->dev, cfg->g_chan, &chan_conf);
    chan_conf.pin = cfg->b_pin;
    pwm_chan_config(led->dev, cfg->b_chan, &chan_conf);

    led->r_chan = cfg->r_chan;
    led->g_chan = cfg->g_chan;
    led->b_chan = cfg->b_chan;

    led_rgb = (struct rgb_led*) led;
    led_rgb->set_color_rgb = pwm_rgb_led_set_color;

    return rgb_led_init(led_rgb, cfg->evq);
}

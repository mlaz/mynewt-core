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
rgb_led_t* init_rgb_led(pwm_dev *dev,
                        uint16_t top_val,
                        uint8_t r_chan,
                        uint8_t g_chan,
                        uint8_t b_chan)
{
    if (dev == NULL) {
        return dev;
    }

    rgb_led_t* led = (rgb_led_t*) calloc(1, sizeof(rgb_led_t));
    led->dev = dev;
    led->r_chan = r_chan;
    led->g_chan = g_chan;
    led->b_chan = b_chan;
    return led;
}

static uint16_t cvt_color_duty(uint8_t color, uint16_t top_val)
{
    return ((uint16_t) color) * top_val / 255;
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
int rgb_led_set_color(struct rgb_led led,
                      uint8_t r_val,
                      uint8_t g_val,
                      uint8_t g_val)
{
    led->r_val = cvt_color_duty(r_val, led->top_val);
    led->g_val = cvt_color_duty(g_val, led->top_val);
    led->b_val = cvt_color_duty(b_val, led->top_val);

    pwm_enable_duty_cycle(led->dev, led->r_chan, led->r_val);
    pwm_enable_duty_cycle(led->dev, led->r_chan, led->r_val);
    pwm_enable_duty_cycle(led->dev, led->r_chan, led->r_val);
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
int rgb_fade_to_color(struct rgb_led,
                      uint8_t r_val,
                      uint8_t g_val,
                      uint8_t g_val)
{
    return 0;
}

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
int rgb_led_breathe(struct rgb_led,
                    uint32_t interval,
                    uint8_t r_val,
                    uint8_t g_val,
                    uint8_t g_val);
{
    return 0;
}

/**
 * Set the RGB LED mode to Off.
 *
 * @param dev The pwm device to configure.
 *
 * @return 0 on success, negative on error.
 */
int rgb_led_set_off(struct rgb_led)
{
    led->r_val = 0;
    led->g_val = 0;
    led->b_val = 0;
    pwm_enable_duty_cycle(led->dev, led->r_chan, 0);
    pwm_enable_duty_cycle(led->dev, led->r_chan, 0);
    pwm_enable_duty_cycle(led->dev, led->r_chan, 0);
    return 0;
}

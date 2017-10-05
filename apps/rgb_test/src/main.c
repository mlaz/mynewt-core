/**
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
#include <sysinit/sysinit.h>
#include <os/os.h>
#include <pwm/pwm.h>
#include <bsp/bsp.h>
#include <console/console.h>

#include "rgb_led.h"

/* Define task stack and task object */
#define RGBLED_PRI (5)
#define RGBLED_SSIZE (512)
static struct os_task rgbled_task;
os_stack_t rgbled_stack[RGBLED_SSIZE];

/* Define task stack and task object for fade to color */
/* #define RGBFADE_PRI (4) */
/* #define RGBFADE_SSIZE (512) */
/* static struct os_task rgbfade_task; */
/* os_stack_t rgbled_stack[RGBLED_SSIZE]; */

static struct os_eventq c_rgbled_evq;
static struct pwm_dev *pwm;
/* static struct pwm_dev *pwm2; */
static uint16_t top_val;

/* void */
/* rgbled_task_handler(void *arg) */
/* { */
/*     struct rgb_led* led1; */
/*     led1 = init_rgb_led(pwm, &c_rgbled_evq, top_val, 0, 1, 2); */
/*     rgb_led_set_color(led1, 127, 30, 127); */
/*     rgb_led_set_bness(led1, 5); */

/*     rgb_led_breathe(led1, 3000); */

/*     while(1) { */
/*         os_eventq_run(&c_rgbled_evq); */
/*     } */
/* } */


void
rgbled_task_handler(void *arg)
{
    struct rgb_led* leds[6];
    leds[0] = init_rgb_led(pwm, &c_rgbled_evq, top_val, 0, 1, 2);
    leds[1] = init_rgb_led(pwm, &c_rgbled_evq, top_val, 3, 4, 5);
    leds[2] = init_rgb_led(pwm, &c_rgbled_evq, top_val, 6, 7, 8);
    leds[3] = init_rgb_led(pwm, &c_rgbled_evq, top_val, 9, 10, 11);
    leds[4] = init_rgb_led(pwm, &c_rgbled_evq, top_val, 12, 13, 14);
    leds[5] = init_rgb_led(pwm, &c_rgbled_evq, top_val, 15, 16, 17);

    rgb_led_set_color(leds[0], 127, 30, 127);
    rgb_led_set_color(leds[1], 127, 30, 127);
    rgb_led_set_color(leds[2], 127, 30, 127);
    rgb_led_set_color(leds[3], 127, 30, 127);
    rgb_led_set_color(leds[4], 127, 30, 127);
    rgb_led_set_color(leds[5], 127, 30, 127);

    rgb_led_breathe(leds[0], 5000);
    rgb_led_breathe_delayed(leds[1], 5000, 2500);
    rgb_led_breathe_delayed(leds[2], 5000, 3800);
    rgb_led_breathe_delayed(leds[3], 5000, 1000);
    rgb_led_breathe_delayed(leds[4], 5000, 3000);
    rgb_led_breathe_delayed(leds[5], 5000, 2300);

    /* rgb_led_breathe_array(leds, 6, 6500); */

    while(1) {
        os_eventq_run(&c_rgbled_evq);
    }
}

void
init_tasks()
{
    struct pwm_chan_cfg chan_conf = {
        .pin = 11,
        .inverted = false,
        .data = NULL
    };
    uint32_t base_freq;
    uint32_t pwm_freq = 200;
    int rc;

    os_eventq_init(&c_rgbled_evq);

    pwm = (struct pwm_dev *) os_dev_open("spwm", 0, NULL);
    pwm_set_frequency(pwm, pwm_freq);
    base_freq = pwm_get_clock_freq(pwm);
    top_val = (uint16_t) (base_freq / pwm_freq);

    /* led 1 */
    /* setup red channel */
    pwm_chan_config(pwm, 0, &chan_conf);

    /* setup green channel */
    chan_conf.pin = 12;
    pwm_chan_config(pwm, 1, &chan_conf);

    /* setup blue channel */
    chan_conf.pin = 13;
    pwm_chan_config(pwm, 2, &chan_conf);

    /* led 2 */
    /* setup red channel */
    chan_conf.pin = 14;
    pwm_chan_config(pwm, 3, &chan_conf);

    /* setup green channel */
    chan_conf.pin = 15;
    pwm_chan_config(pwm, 4, &chan_conf);

    /* setup blue channel */
    chan_conf.pin = 16;
    pwm_chan_config(pwm, 5, &chan_conf);

    /* led 3 */
    /* setup red channel */
    chan_conf.pin = 17;
    pwm_chan_config(pwm, 6, &chan_conf);

    /* setup green channel */
    chan_conf.pin = 18;
    pwm_chan_config(pwm, 7, &chan_conf);

    /* setup blue channel */
    chan_conf.pin = 19;
    pwm_chan_config(pwm, 8, &chan_conf);

    /* led 4 */
    /* setup red channel */
    chan_conf.pin = 20;
    pwm_chan_config(pwm, 9, &chan_conf);

    /* setup green channel */
    chan_conf.pin = 22;
    pwm_chan_config(pwm, 10, &chan_conf);

    /* setup blue channel */
    chan_conf.pin = 23;
    pwm_chan_config(pwm, 11, &chan_conf);

    /* led 5 */
    /* setup red channel */
    chan_conf.pin = 24;
    pwm_chan_config(pwm, 12, &chan_conf);

    /* setup green channel */
    chan_conf.pin = 25;
    pwm_chan_config(pwm, 13, &chan_conf);

    /* setup blue channel */
    chan_conf.pin = 26;
    pwm_chan_config(pwm, 14, &chan_conf);

    /* led 6 */
    /* setup red channel */
    chan_conf.pin = 2;
    pwm_chan_config(pwm, 15, &chan_conf);

    /* setup green channel */
    chan_conf.pin = 3;
    pwm_chan_config(pwm, 16, &chan_conf);

    /* setup blue channel */
    chan_conf.pin = 4;
    pwm_chan_config(pwm, 17, &chan_conf);

    rc = os_task_init(&rgbled_task,
                      "rgbled",
                      rgbled_task_handler,
                      NULL,
                      RGBLED_PRI,
                      OS_WAIT_FOREVER,
                      rgbled_stack,
                      RGBLED_SSIZE);
    assert(rc == 0);

}

int
main(int argc, char **argv)
{
    sysinit();
    init_tasks();

    while (1) {
       os_eventq_run(os_eventq_dflt_get());
    }
    assert(0);
    return(0);
}

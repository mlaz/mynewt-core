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
#include "syscfg/syscfg.h"
#include "sysinit/sysinit.h"
#include "os/os.h"
#include <pwm/pwm.h>
#include <bsp/bsp.h>

#include "rgb_led.h"

/* Define task stack and task object */
#define RGBLED_PRI (OS_TASK_PRI_HIGHEST)
#define RGBLED_SIZE (512)
struct os_task rgbled_task;
os_stack_t rgbled_stack[RGBLED_SIZE];

struct os_eventq c_rgbled_evq;
struct pwm_dev *pwm;
uint16_t top_val;

void
rgbled_task_handler(void *arg)
{
    struct rgb_led* led1;
    led1 = init_rgb_led(pwm, &c_rgbled_evq, top_val, 0, 1, 2);
    rgb_led_set_color(led1, 127, 30, 127);

    rgb_led_set_bness(led1, 5);

    rgb_led_breathe(led1, 2000);

    while(1) {
        os_eventq_run(&c_rgbled_evq);
    }
}

int
main(int argc, char **argv)
{
    struct pwm_chan_cfg chan_conf = {
        .pin = 11,
        .inverted = false,
        .data = NULL
    };
    uint32_t base_freq;
    uint32_t pwm_freq = 2000;
    int rc;

    sysinit();

    pwm = (struct pwm_dev *) os_dev_open("pwm0", 0, NULL);
    /* set the PWM frequency */
    pwm_set_frequency(pwm, pwm_freq);
    base_freq = pwm_get_clock_freq(pwm);
    top_val = (uint16_t) (base_freq / pwm_freq);

    /* setup red channel */
    pwm_chan_config(pwm, 0, &chan_conf);

    /* setup green channel */
    chan_conf.pin = 12;
    pwm_chan_config(pwm, 1, &chan_conf);

    /* setup blue channel */
    chan_conf.pin = 13;
    pwm_chan_config(pwm, 2, &chan_conf);

    os_eventq_init(&c_rgbled_evq);
    rc = os_task_init(&rgbled_task,
                      "rgbled",
                      rgbled_task_handler,
                      NULL,
                      RGBLED_PRI,
                      OS_WAIT_FOREVER,
                      rgbled_stack,
                      RGBLED_SIZE);
    assert(rc == 0);

    while (1) {
       os_eventq_run(os_eventq_dflt_get());
    }
    assert(0);
    return(0);
}

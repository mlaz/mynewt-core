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

#include <pwm_rgb_led/pwm_rgb_led.h>

/* Define task stack and task object */
#define RGBLED_PRI (0)
#define RGBLED_SSIZE (512)

static struct os_task rgbled_task;
os_stack_t rgbled_stack[RGBLED_SSIZE];

/* Define RGB event queue and RGB LED pointer */
struct os_eventq c_rgbled_evq;
struct rgb_led *led;
struct pwm_rgb_led pwm_led;

void
rgbled_task_handler(void *arg)
{
    rgb_led_set_color_hsv(led, 127, 127, 127);

    /* Making the fade pass along the LEDs */
    rgb_led_breathe_delayed(led, 6000, 100, 255, 0);

    while(1) {
        os_eventq_run(&c_rgbled_evq);
    }
}

/* Configure RGB LEDs and tasks */
void
init_tasks()
{
    int rc;
    /* struct rgb_led *leds[6]; */
    struct pwm_led_cfg cfg = {
        .evq = &c_rgbled_evq,
        .dev = (struct pwm_dev *) os_dev_open("spwm", 0, NULL),
        .anode = false,
        .freq = 200,
        .r_pin = 11,
        .g_pin = 12,
        .b_pin = 13,
        .r_chan = 0,
        .g_chan = 1,
        .b_chan = 2
    };

    led = pwm_rgb_led_init(&pwm_led, &cfg);

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

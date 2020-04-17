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
#include <os/os.h>
#include <bsp/bsp.h>
#include <adc/adc.h>
#include <console/console.h>

#include <string.h>

#define ADC_NUMBER_SAMPLES (2)
#define ADC_NUMBER_CHANNELS (1)

/* /\* ADC Task settings *\/ */
#define ADC_TASK_PRIO           (0)
#define ADC_STACK_SIZE          (OS_STACK_ALIGN(128))
struct os_task adc_task;
os_stack_t adc_stack[ADC_STACK_SIZE];

struct adc_dev *adc;

#define BLOCKING 0

#if !BLOCKING
static int value;
uint8_t *sample_buffer1;
uint8_t *sample_buffer2;

int
adc_read(void *buffer, int buffer_len)
{
    int i;
    int adc_result;
    int result_mv = 0;
    int rc;

    for (i = 0; i < ADC_NUMBER_SAMPLES - 1; i++) {
        rc = adc_buf_read(adc, buffer, buffer_len, i, &adc_result);
        if (rc != 0) {
            goto err;
        }
        result_mv = adc_result_mv(adc, 0, adc_result);
    }
    adc_buf_release(adc, buffer, buffer_len);
    return result_mv;
err:
    return (-rc);
}

int
adc_read_event(struct adc_dev *dev,
               void *arg,
               uint8_t etype,
               void *buffer,
               int buffer_len)
{
    int rc = 0xFFFFFFFF;

    switch (etype) {
    case ADC_EVENT_CALIBRATED:
        //console_printf("CALIBRATED\n");
        break;
    case ADC_EVENT_RESULT:
        value = adc_read(buffer, buffer_len);
        (void) value;
        break;
    default:
        goto err;
    }

    return (0);
err:
    return (rc);
}
#endif

static void
adc_init()
{
    struct adc_dev_cfg cfg = {
        .resolution = ADC_RESOLUTION_12BIT,
        .oversample = ADC_OVERSAMPLING_DISABLED,
        .calibrate = true
    };

    struct adc_chan_cfg cc = {
        .pin = MYNEWT_VAL(ADC_TEST_PIN),
        .differential = false,
        .pin_n = 0,
        .gain = ADC_GAIN1_6,
        .reference = ADC_REFERENCE_INTERNAL
    };
    adc = (struct adc_dev *) os_dev_open("adc0",
                                         OS_TIMEOUT_NEVER,
                                         &cfg);
    assert(adc != NULL);
    adc_chan_config(adc, MYNEWT_VAL(ADC_TEST_CHANNEL), &cc);

#if !BLOCKING
    int rc;
    sample_buffer1 = malloc(adc_buf_size(adc,
                                         ADC_NUMBER_CHANNELS,
                                         ADC_NUMBER_SAMPLES));
    sample_buffer2 = malloc(adc_buf_size(adc,
                                         ADC_NUMBER_CHANNELS,
                                         ADC_NUMBER_SAMPLES));

    memset(sample_buffer1, 0, adc_buf_size(adc,
                                           ADC_NUMBER_CHANNELS,
                                           ADC_NUMBER_SAMPLES));
    memset(sample_buffer2, 0, adc_buf_size(adc,
                                           ADC_NUMBER_CHANNELS,
                                           ADC_NUMBER_SAMPLES));
    rc = adc_buf_set(adc,
                sample_buffer1,
                sample_buffer2,
                adc_buf_size(adc, ADC_NUMBER_CHANNELS, ADC_NUMBER_SAMPLES));
    assert(rc == 0);
#endif
}

#if BLOCKING
/**
 * Event loop for the sensor task, blocking version.
 */
static void
adc_task_handler(void *unused)
{
    int rc;
    int result;

    adc_init();

    while (1) {
        rc = adc_read_channel(adc, 0, &result);
        assert(rc == 0);
        if (result >= 0) {
            result = adc_result_mv(adc, MYNEWT_VAL(ADC_TEST_CHANNEL), result);
            console_printf("Got %dmv\n", result);
        } else {
            console_printf("Error while reading: %d\n", result);
        }

        /* Wait before next sample */
        os_time_delay(OS_TICKS_PER_SEC * MYNEWT_VAL(ADC_TEST_SAMPLE_INTERVAL));
    }
}
#else

/**
 * Event loop for the sensor task.
 */
static void
adc_task_handler(void *unused)
{
    int rc;

    adc_init();

    rc = adc_event_handler_set(adc, adc_read_event, (void *) NULL);
    assert(rc == 0);

    while (1) {
        rc = adc_sample(adc);
        assert(rc == 0);
        if (value >= 0) {
            console_printf("Got %dmv\n", value);
        } else {
            console_printf("Error while reading: %d\n", value);
        }

        /* Wait before next sample */
        os_time_delay(OS_TICKS_PER_SEC * MYNEWT_VAL(ADC_TEST_SAMPLE_INTERVAL));
    }
}
#endif

int
main(int argc, char **argv)
{
    sysinit();
    console_printf("Started \n");
    os_task_init(&adc_task,
                 "sensor",
                 adc_task_handler,
                 NULL,
                 ADC_TASK_PRIO,
                 OS_WAIT_FOREVER,
                 adc_stack,
                 ADC_STACK_SIZE);

    while (1) {
        os_eventq_run(os_eventq_dflt_get());
    }
    assert(0);
    return(0);
}

/*
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

#include <assert.h>
#include "os/mynewt.h"
#include <hal/hal_bsp.h>
#include <adc/adc.h>
#include <mcu/cmsis_nvic.h>

/* Nordic headers */
#include <nrfx.h>
#include <nrf_saadc.h>
#include <nrfx_saadc.h>

#include "adc_nrf52/adc_nrf52.h"

struct nrf52_saadc_stats {
    uint16_t saadc_events;
    uint16_t saadc_events_failed;
};
static struct nrf52_saadc_stats nrf52_saadc_stats;

static struct adc_dev *global_adc_dev;
static nrfx_saadc_config_t global_adc_config;
static nrf_saadc_channel_config_t nrf52_adc_chans[NRF_SAADC_CHANNEL_COUNT];
static void *g_buf1;
static void *g_buf2;
static int g_buf_len;

/* static int calibrated = 0; */
void
nrf52_saadc_event_handler(const nrfx_saadc_evt_t *event)
{
    nrfx_saadc_done_evt_t *done_ev;
    int rc;

    if (global_adc_dev == NULL || !global_adc_dev->ad_event_handler_func) {
        ++nrf52_saadc_stats.saadc_events_failed;
        return;
    }

    ++nrf52_saadc_stats.saadc_events;

    switch (event->type) {
        case NRFX_SAADC_EVT_DONE:
            done_ev = (nrfx_saadc_done_evt_t * const) &event->data.done;

            rc = global_adc_dev->ad_event_handler_func(global_adc_dev,
                                       global_adc_dev->ad_event_handler_arg,
                                       ADC_EVENT_RESULT, done_ev->p_buffer,
                                       done_ev->size * sizeof(nrf_saadc_value_t));
            if (global_adc_config.low_power_mode /* && (calibrated >= 2) */) {
                nrf_saadc_disable();
                NVIC_ClearPendingIRQ(SAADC_IRQn);
                /* calibrated = 0; */
            }

            break;
        case NRFX_SAADC_EVT_CALIBRATEDONE:
            rc = global_adc_dev->ad_event_handler_func(global_adc_dev,
                                       global_adc_dev->ad_event_handler_arg,
                                       ADC_EVENT_CALIBRATED, NULL, 0);
            /* if (g_buf2) { */
            /*     calibrated++; */
            /* } */
            break;
        default:
            assert(0);
            break;
    }

    if (rc != 0) {
        ++nrf52_saadc_stats.saadc_events_failed;
    }
}

/**
 * Initialize the driver instance.
 */
static void
init_instance(nrfx_saadc_config_t* od_conf)
{
    nrfx_saadc_config_t dflt_dev_cfg = NRFX_SAADC_DEFAULT_CONFIG;
    nrf_saadc_channel_config_t dflt_chan_cfg =
        NRFX_SAADC_DEFAULT_CHANNEL_CONFIG_SE (NRF_SAADC_INPUT_DISABLED);
    int idx;

    if (od_conf) {
        memcpy(&global_adc_config, &od_conf, sizeof(nrfx_saadc_config_t));
    } else {
        memcpy(&global_adc_config, &dflt_dev_cfg, sizeof(nrfx_saadc_config_t));
    }

    for (idx = 0; idx < NRF_SAADC_CHANNEL_COUNT; idx++) {
        memcpy(&nrf52_adc_chans[idx],
               &dflt_chan_cfg,
               sizeof(nrf_saadc_channel_config_t));
    }

    g_buf1 = NULL;
    g_buf2 = NULL;
    g_buf_len = 0;
}

/**
 * Open the NRF52 ADC device
 *
 * This function locks the device for access from other tasks.
 *
 * @param odev The OS device to open
 * @param wait The time in MS to wait.  If 0 specified, returns immediately
 *             if resource unavailable.  If OS_WAIT_FOREVER specified, blocks
 *             until resource is available.
 * @param arg  Argument provided by higher layer to open, in this case
 *             it can be a nrfx_saadc_config_t, to override the default
 *             configuration.
 *
 * @return 0 on success, non-zero on failure.
 */
static int
nrf52_adc_open(struct os_dev *odev, uint32_t wait, void *arg)
{
    struct adc_dev *dev;
    int rc = 0;
    int unlock = 0;

    dev = (struct adc_dev *) odev;

    if (os_started()) {
        rc = os_mutex_pend(&dev->ad_lock, wait);
        if (rc != OS_OK) {
            goto err;
        }
        unlock = 1;
    }

    /* if (++(dev->ad_ref_cnt) == 1) { */
        /* Initialize the driver */
        init_instance((nrfx_saadc_config_t *) arg);

        if (!global_adc_config.low_power_mode) {
            rc = nrfx_saadc_init(&global_adc_config,
                                 nrf52_saadc_event_handler);
            if (rc != NRFX_SUCCESS) {
                goto err;
            }
        }

        rc = 0;
        global_adc_dev = dev;
    /* } */

err:
    if (unlock) {
        os_mutex_release(&dev->ad_lock);
    }
    return (rc);
}


/**
 * Close the NRF52 ADC device.
 *
 * This function unlocks the device.
 *
 * @param odev The device to close.
 */
static int
nrf52_adc_close(struct os_dev *odev)
{
    struct adc_dev *dev;

    int rc = 0;
    int unlock = 0;

    dev = (struct adc_dev *) odev;

    if (os_started()) {
        rc = os_mutex_pend(&dev->ad_lock, OS_TIMEOUT_NEVER);
        if (rc != OS_OK) {
            goto err;
        }
        unlock = 1;
    }
    /* if (--(dev->ad_ref_cnt) == 0) { */
        if (!global_adc_config.low_power_mode) {
            nrfx_saadc_uninit();
        }

        global_adc_dev = NULL;
    /* } */

err:
    if (unlock) {
        os_mutex_release(&dev->ad_lock);
    }

    return rc;
}

/**
 * Configure an ADC channel on the Nordic ADC.
 *
 * @param dev The ADC device to configure
 * @param cnum The channel on the ADC device to configure
 * @param cfgdata An opaque pointer to channel config, expected to be
 *                a nrf_saadc_channel_config_t
 *
 * @return 0 on success, non-zero on failure.
 */
static int
nrf52_adc_configure_channel(struct adc_dev *dev,
                            uint8_t cnum,
                            void *cfgdata)
{
    nrf_saadc_channel_config_t *cc;
    uint16_t refmv;
    uint8_t res;
    int rc;

    memcpy (&nrf52_adc_chans[cnum], cfgdata, sizeof(nrf_saadc_channel_config_t));
    if (!global_adc_config.low_power_mode) {
        rc = nrfx_saadc_channel_init(cnum, &nrf52_adc_chans[cnum]);
        if (rc != NRFX_SUCCESS) {
            goto err;
        }
    }

    cc = &nrf52_adc_chans[cnum];

    /* if (global_adc_config) { */
    /*     /\* Set the resolution and reference voltage for this channel to */
    /*     * enable conversion functions. */
    /*     *\/ */
    switch (global_adc_config.resolution) {
    case NRF_SAADC_RESOLUTION_8BIT:
        res = 8;
        break;
    case NRF_SAADC_RESOLUTION_10BIT:
        res = 10;
        break;
    case NRF_SAADC_RESOLUTION_12BIT:
        res = 12;
        break;
    case NRF_SAADC_RESOLUTION_14BIT:
        res = 14;
        break;
    default:
        assert(0);
    }
    /* } else { */
    /*     /\* Default to 10-bit resolution. *\/ */
    /*     res = 10; */
    /* } */

    switch (cc->reference) {
        case NRF_SAADC_REFERENCE_INTERNAL:
            refmv = 600; /* 0.6V for NRF52 */
            break;
        case NRF_SAADC_REFERENCE_VDD4:
            refmv = MYNEWT_VAL(ADC_0_REFMV_0 / 4);
            break;
        default:
            assert(0);
    }

    /* Adjust reference voltage for gain. */
    switch (cc->gain) {
        case NRF_SAADC_GAIN1_6:
            refmv *= 6;
            break;
        case NRF_SAADC_GAIN1_5:
            refmv *= 5;
            break;
        case NRF_SAADC_GAIN1_4:
            refmv *= 4;
            break;
        case NRF_SAADC_GAIN1_3:
            refmv *= 3;
            break;
        case NRF_SAADC_GAIN1_2:
            refmv *= 2;
            break;
        case NRF_SAADC_GAIN2:
            refmv /= 2;
            break;
        case NRF_SAADC_GAIN4:
            refmv /= 4;
            break;
        default:
            break;
    }

    /* Store these values in channel definitions, for conversions to
     * milivolts.
     */
    dev->ad_chans[cnum].c_res = res;
    dev->ad_chans[cnum].c_refmv = refmv;
    dev->ad_chans[cnum].c_configured = 1;

    return (0);
err:
    return (rc);
}

/**
 * Set buffer to read data into.  Implementation of setbuffer handler.
 * Sets both the primary and secondary buffers for DMA.
 */
static int
nrf52_adc_set_buffer(struct adc_dev *dev,
                     void *buf1,
                     void *buf2,
                     int buf_len)
{
    int rc;

    /* Convert overall buffer length, into a total number of samples which
     * Nordic APIs expect.
     */
    buf_len /= sizeof(nrf_saadc_value_t);

    g_buf1 = buf1;
    g_buf2 = buf2;
    g_buf_len = buf_len;

    if (!global_adc_config.low_power_mode) {

        rc = nrfx_saadc_buffer_convert((nrf_saadc_value_t *) buf1, buf_len);
        if (rc != NRFX_SUCCESS) {
            goto err;
        }

        if (buf2) {
            rc = nrfx_saadc_buffer_convert((nrf_saadc_value_t *) buf2,
                                           buf_len);
            if (rc != NRFX_SUCCESS) {
                goto err;
            }
        }
    }

    return (0);
err:
    return (rc);
}

static int
nrf52_adc_release_buffer(struct adc_dev *dev, void *buf, int buf_len)
{
    int rc;

    buf_len /= sizeof(nrf_saadc_value_t);

    rc = nrfx_saadc_buffer_convert((nrf_saadc_value_t *) buf, buf_len);
    if (rc != NRFX_SUCCESS) {
        goto err;
    }

    return (0);
err:
    return (rc);
}

/**
 * Trigger an ADC sample.
 */
static int
nrf52_adc_sample(struct adc_dev *dev)
{
    int idx;
    int rc = NRFX_SUCCESS;

    if (global_adc_config.low_power_mode) {
        /* Init Device */
        rc = nrfx_saadc_init(&global_adc_config, nrf52_saadc_event_handler);
        if (rc != NRFX_SUCCESS) {
          goto err;
        }

        /* Configure Channels */
        for (idx = 0; idx < NRF_SAADC_CHANNEL_COUNT; idx++) {
            if (nrf52_adc_chans[idx].pin_p == NRF_SAADC_INPUT_DISABLED) {
                continue;
            }
            rc = nrfx_saadc_channel_init(idx, &nrf52_adc_chans[idx]);
            if (rc != NRFX_SUCCESS) {
                goto err;
            }
        }

        /* Set Buffers */
        rc = nrfx_saadc_buffer_convert((nrf_saadc_value_t *) g_buf1, g_buf_len);
        if (rc != NRFX_SUCCESS) {
            goto err;
        }

        if (g_buf2) {
            rc = nrfx_saadc_buffer_convert((nrf_saadc_value_t *) g_buf2,
                                           g_buf_len);
            if (rc != NRFX_SUCCESS) {
                goto err;
            }
        }
    }

    nrfx_saadc_sample();

    return (0);
 err:
    return (rc);
}

/**
 * Blocking read of an ADC channel, returns result as an integer.
 */
static int
nrf52_adc_read_channel(struct adc_dev *dev, uint8_t cnum, int *result)
{
    nrf_saadc_value_t adc_value;
    int rc;
    int unlock = 0;

    if (os_started()) {
        rc = os_mutex_pend(&dev->ad_lock, OS_TIMEOUT_NEVER);
        if (rc != OS_OK) {
            goto err;
        }
        unlock = 1;
    }
    rc = nrfx_saadc_sample_convert(cnum, &adc_value);
    if (rc != NRFX_SUCCESS) {
        goto err;
    }

    *result = (int) adc_value;
    rc = 0;

err:
    if (unlock) {
        os_mutex_release(&dev->ad_lock);
    }
    return (rc);
}

static int
nrf52_adc_read_buffer(struct adc_dev *dev, void *buf, int buf_len, int off,
        int *result)
{
    nrf_saadc_value_t val;
    int data_off;

    data_off = off * sizeof(nrf_saadc_value_t);
    assert(data_off < buf_len);

    val = *(nrf_saadc_value_t *) ((uint8_t *) buf + data_off);
    *result = val;

    return (0);
}

static int
nrf52_adc_size_buffer(struct adc_dev *dev, int chans, int samples)
{
    return (sizeof(nrf_saadc_value_t) * chans * samples);
}

#if MYNEWT_VAL(OS_SYSVIEW)
static void
saadc_irq_handler(void)
{
    os_trace_isr_enter();
    nrfx_saadc_irq_handler();
    os_trace_isr_exit();
}
#endif

/**
 * ADC device driver functions
 */
static const struct adc_driver_funcs nrf52_adc_funcs = {
        .af_configure_channel = nrf52_adc_configure_channel,
        .af_sample = nrf52_adc_sample,
        .af_read_channel = nrf52_adc_read_channel,
        .af_set_buffer = nrf52_adc_set_buffer,
        .af_release_buffer = nrf52_adc_release_buffer,
        .af_read_buffer = nrf52_adc_read_buffer,
        .af_size_buffer = nrf52_adc_size_buffer,
};

/**
 * Callback to initialize an adc_dev structure from the os device
 * initialization callback.  This sets up a nrf52_adc_device(), so
 * that subsequent lookups to this device allow us to manipulate it.
 */
int
nrf52_adc_dev_init(struct os_dev *odev, void *unused)
{
    struct adc_dev *dev;

    dev = (struct adc_dev *) odev;

    os_mutex_init(&dev->ad_lock);

    dev->ad_chans = (void *) nrf52_adc_chans;
    dev->ad_chan_count = NRF_SAADC_CHANNEL_COUNT;

    OS_DEV_SETHANDLERS(odev, nrf52_adc_open, nrf52_adc_close);

    dev->ad_funcs = &nrf52_adc_funcs;

#if MYNEWT_VAL(OS_SYSVIEW)
    NVIC_SetVector(SAADC_IRQn, (uint32_t) saadc_irq_handler);
#else
    NVIC_SetVector(SAADC_IRQn, (uint32_t) nrfx_saadc_irq_handler);
#endif

    return (0);
}

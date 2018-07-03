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
#include "os/mynewt.h"

struct 1_wire_bus_master {

};

struct 1_wire_bus_master instances[DEV_COUNT];

/**
 * Open the bitbang 1-Wire master device
 *
 * This function locks the device for access from other tasks.
 *
 * @param odev The OS device to open
 * @param wait The time in MS to wait.  If 0 specified, returns immediately
 *             if resource unavailable.  If OS_WAIT_FOREVER specified, blocks
 *             until resource is available.
 * @param arg
 * @return 0 on success, non-zero on failure.
 */
static int
1_wire_bb_open(struct os_dev *odev, uint32_t wait, void *arg)
{
    uint32_t stat = 0;
    struct 1_wire_dev *dev = (struct 1_wire_dev *) odev;
    struct soft_1_wire_dev *instance = &instances[dev->1_wire_instance_id];

    if (os_started()) {
        stat = os_mutex_pend(&dev->1_wire_lock, wait);
        if (stat != OS_OK) {
            return (stat);
        }
    }

    if (odev->od_flags & OS_DEV_F_STATUS_OPEN) {
        os_mutex_release(&dev->1_wire_lock);
        stat = OS_EBUSY;
        return (stat);
    }

    /* Initialize whatever is needed */

    return (0);
}

/**
 * Close the bitbang 1-Wire master device.
 *
 * This function unlocks the device.
 *
 * @param odev The device to close.
 */
static int
1_wire_bb_close(struct os_dev *odev)
{
    uint8_t cnum;
    struct 1_wire_dev *dev = (struct 1_wire_dev *) odev;
    struct soft_1_wire_dev *instance = &instances[dev->1_wire_instance_id];

    /* Stop whatever is going on and exit gracefully */
    if (os_started()) {
        os_mutex_release(&dev->1_wire_lock);
    }

    return (0);
}

/**
 * Configure master device.
 *
 */
int
1_wire_bb_configure(struct 1_wire_dev *dev, uint8_t pin, bool pullup)
{

}

/**
 * Bus reset.
 *
 */
int
1_wire_bb_reset(struct 1_wire_dev *dev)
{

}

/**
 * Read a bit from selected slave.
 *
 */
uint8_t
1_wire_bb_read_bit(struct 1_wire_dev *dev)
{

}

/**
 * Write a bit to selected slave.
 *
 */
void
1_wire_bb_write_bit(struct 1_wire_dev *dev, uint8_t)
{

}

/**
 * Read a byte from selected slave.
 *
 */
uint8_t
1_wire_bb_read_byte(struct 1_wire_dev *dev)
{

}

/**
 * Write a byte to selected slave.
 *
 */
void
1_wire_bb_write_byte(struct 1_wire_dev *dev, uint8_t)
{

}

/**
 * Read a block of len bytes from selected slave.
 *
 */
uint8_t
1_wire_bb_read_block(struct 1_wire_dev *dev, uint8_t *block, int len)
{

}

/**
 * Write a block of len bytes to selected slave.
 *
 */
void
1_wire_bb_write_block(struct 1_wire_dev *dev, const uint8_t *block, int len)
{

}

/**
 *
 *
 */
int
1_wire_bb_reset_select_slave(struct 1_wire_slave *slave)
{

}

/**
 *
 *
 */
int
1_wire_bb_reset_resume_command(struct 1_wire_dev *dev)
{

}

/**
 *
 *
 */
void
1_wire_bb_next_pullup(struct 1_wire_dev *dev, int)
{

}

/**
 * Callback to initialize a 1_wire_dev structure from the os device
 * initialization callback.  This sets up a 1_wire_device(), so
 * that subsequent lookups to this device allow us to manipulate it.
 */
int
1_wire_bitbang_dev_init(struct os_dev *odev, void *arg)
{
    struct 1_wire_dev *dev;
    struct 1_wire_driver_funcs *1_wire_funcs;

    assert(odev);
    dev = (struct 1_wire_dev *) odev;

    assert(arg);
    dev->1_wire_instance_id = *((uint8_t *) arg);
    assert(dev->1_wire_instance_id < DEV_COUNT);

    os_mutex_init(&dev->1_wire_lock);

    OS_DEV_SETHANDLERS(odev, 1_wire_bb_open, 1_wire_bb_close);

    1_wire_funcs = &dev->1_wire_funcs;
    1_wire_funcs->1_wire_configure = 1_wire_bb_configure;
    1_wire_funcs->1_wire_reset = 1_wire_bb_reset;
    1_wire_funcs->1_wire_read_bit = 1_wire_bb_read_bit;
    1_wire_funcs->1_wire_write_bit = 1_wire_bb_write_bit;
    1_wire_funcs->1_wire_read_byte = 1_wire_bb_read_byte;
    1_wire_funcs->1_wire_write_byte = 1_wire_bb_write_byte;
    1_wire_funcs->1_wire_read_block = 1_wire_bb_read_block;
    1_wire_funcs->1_wire_write_block = 1_wire_bb_write_block;
    1_wire_funcs->1_wire_select_slave = 1_wire_bb_select_slave;
    1_wire_funcs->1_wire_next_pullup = 1_wire_bb_next_pullup;

    return (0);
}

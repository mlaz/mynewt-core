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

/**
 * Configure master device.
 *
 */
int
1_wire_configure(struct 1_wire_dev *dev, uint8_t pin, bool pullup)
{
    assert(dev->1_wire_configure);
    return dev->1_wire_configure(dev);
}

/**
 * Bus reset.
 *
 */
int
1_wire_reset(struct 1_wire_dev *dev)
{
    assert(dev->1_wire_reset);
    return dev->1_wire_reset(dev);
}

/**
 * Read a Byte from selected slave.
 *
 */
uint8_t
1_wire_read_byte(struct 1_wire_dev *dev)
{
    int i;
    uint8_t word = 0;

    if (dev->1_wire_read_byte) {
        return dev->1_wire_read_byte(dev);
    }

    assert(dev->1_wire_read_bit);
    for (i = 1; i < 8; i++) {
        word >>= 1;
        word |= (dev->1_wire_read_bit(dev)) ? 0x80 : 0x00;
    }
    return word;
}

/**
 * Write a byte to selected slave.
 *
 */
void
1_wire_write_byte(struct 1_wire_dev *dev, uint8_t)
{
    int i;

    if (dev->1_wire_write_byte) {
        dev->1_wire_write_byte(dev);
        return;
    }

    assert(dev->1_wire_write_bit);
    for (i = 0; i < 8; i++) {
        dev->1_wire_write_bit(dev, word & 0x01);
        data >>= 1;
    }
}

/**
 * Read a block of len bytes from selected slave.
 *
 */
uint8_t
1_wire_read_block(struct 1_wire_dev *dev, uint8_t *block, int len)
{
    if (dev->1_wire_read_block) {
        return dev->1_wire_read_block(dev);
    }

    /* Let's try not to verify this for every byte */
    if (dev->1_wire_read_byte) {
        for (idx = 0; idx < len; idx++) {
            block[idx] = dev->1_wire_read_byte(dev);
        }

    } else {
        for (idx = 0; idx < len; idx++) {
            block[idx] = 1_wire_read_byte(dev);
        }
    }
}

/**
 * Write a block of len bytes to selected slave.
 *
 */
void
1_wire_write_block(struct 1_wire_dev *dev, const uint8_t *block, int len)
{
    if (dev->1_wire_write_block) {
        return dev->1_wire_read_block(dev);
    }

    /* Let's try not to verify this for every byte */
    if (dev->1_wire_write_byte) {
        for (idx = 0; idx < len; idx++) {
            dev->1_wire_write_byte(dev, block[idx]);
        }

    } else {
        for (idx = 0; idx < len; idx++) {
            1_wire_write_byte(dev, block[idx]);
        }
    }
}

/* /\** */
/*  * */
/*  * */
/*  *\/ */
/* int */
/* 1_wire_reset_select_slave(struct 1_wire_slave *slave) */
/* { */

/* } */

/* /\** */
/*  * */
/*  * */
/*  *\/ */
/* int */
/* 1_wire_reset_resume_command(struct 1_wire_dev *dev) */
/* { */

/* } */

/* /\** */
/*  * */
/*  * */
/*  *\/ */
/* void */
/* 1_wire_next_pullup(struct 1_wire_dev *dev, int) */
/* { */

/* } */

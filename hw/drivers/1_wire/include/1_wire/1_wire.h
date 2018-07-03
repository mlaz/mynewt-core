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

#ifndef __1_WIRE_H__
#define __1_WIRE_H__

#include "os/mynewt.h"

#ifdef __cplusplus
extern "C" {
#endif

struct 1_wire_dev;
struct 1_wire_slave;

/**
 * Configure master device.
 *
 */
typedef int (*1_wire_configure_t) (struct 1_wire_dev *, uint8_t,bool);

/**
 * Bus reset.
 *
 */
typedef int (*1_wire_reset_t) (struct 1_wire_dev *);

/**
 * Read a Byte from selected slave.
 *
 */
typedef uint8_t (*1_wire_read_byte_t) (struct 1_wire_dev *);

/**
 * Write a byte to selected slave.
 *
 */
typedef void (*1_wire_write_byte_t) (struct 1_wire_dev *, uint8_t);

/**
 * Read a block of len bytes from selected slave.
 *
 */
typedef uint8_t (*1_wire_read_block_t) (struct 1_wire_dev *, uint8_t *, int);

/**
 * Write a block of len bytes to selected slave.
 *
 */
typedef void (*1_wire_write_block_t) (struct 1_wire_dev *, const uint8_t, int);

/**
 *
 *
 */
typedef int (*1_wire_reset_select_slave_t) (struct 1_wire_slave *);

/**
 *
 *
 */
typedef void (*1_wire_next_pullup_t) (struct 1_wire_dev *, int);


struct 1_wire_driver_funcs {
    1_wire_configure_t 1_wire_configure;
    1_wire_reset_t 1_wire_reset;
    1_wire_read_bit_t 1_wire_read_bit;
    1_wire_write_bit_t 1_wire_write_bit;
    1_wire_read_byte_t 1_wire_read_byte;
    1_wire_write_byte_t 1_wire_write_byte;
    1_wire_read_block_t 1_wire_read_block;
    1_wire_write_block_t 1_wire_write_block;
    1_wire_reset_select_slave_t 1_wire_reset_select_slave;
    1_wire_reset_resume_command_t 1_wire_reset_resume_command;
    1_wire_next_pullup_t 1_wire_next_pullup;
};

struct 1_wire_dev {
    struct os_dev 1_wire_os_dev;
    struct os_mutex 1_wire_lock;
    struct 1_wire_driver_funcs 1_wire_funcs;
    uint8_t 1_wire_instance_id;
};

struct 1_wire_reg_num {
    uint64_t family:8;
    uint64_t id:48;
    uint64_t crc:8;
};

struct 1_wire_slave {
    struct 1_wire_reg_num rrn;
};

int 1_wire_configure(struct 1_wire_dev *dev, uint8_t pin, bool pullup);
int 1_wire_reset(struct 1_wire_dev *dev);
uint8_t 1_wire_read_bit(struct 1_wire_dev *dev);
void 1_wire_write_bit(struct 1_wire_dev *dev, uint8_t);
uint8_t 1_wire_read_byte(struct 1_wire_dev *dev);
void 1_wire_write_byte(struct 1_wire_dev *dev, uint8_t);
uint8_t 1_wire_read_block(struct 1_wire_dev *dev, uint8_t *block, int len);
void 1_wire_write_block(struct 1_wire_dev *dev, const uint8_t *block,int len);
int 1_wire_reset_select_slave(struct 1_wire_slave *slave);
int 1_wire_reset_resume_command(struct 1_wire_dev *dev);
void 1_wire_next_pullup(struct 1_wire_dev *dev, int v);

#ifdef __cplusplus
}
#endif

#endif /* __1_WIRE_H__ */

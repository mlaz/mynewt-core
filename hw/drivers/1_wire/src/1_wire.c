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

}

/**
 * Bus reset.
 *
 */
int
1_wire_reset(struct 1_wire_dev *dev)
{

}

/**
 * Read a Byte from selected slave.
 *
 */
uint8_t
1_wire_read_byte(struct 1_wire_dev *dev)
{

}

/**
 * Write a byte to selected slave.
 *
 */
void
1_wire_write_byte(struct 1_wire_dev *dev, uint8_t)
{

}

/**
 * Read a block of len bytes from selected slave.
 *
 */
uint8_t
1_wire_read_block(struct 1_wire_dev *dev, uint8_t *block, int len)
{

}

/**
 * Write a block of len bytes to selected slave.
 *
 */
void
1_wire_write_block(struct 1_wire_dev *dev, const uint8_t *block, int len)
{

}

/**
 *
 *
 */
int
1_wire_reset_select_slave(struct 1_wire_slave *slave)
{

}

/**
 *
 *
 */
int
1_wire_reset_resume_command(struct 1_wire_dev *dev)
{

}

/**
 *
 *
 */
void
1_wire_next_pullup(struct 1_wire_dev *dev, int)
{

}

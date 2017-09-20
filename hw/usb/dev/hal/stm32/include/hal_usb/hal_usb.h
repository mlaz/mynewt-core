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

#ifndef __HAL_USB_H__
#define __HAL_USB_H__

#include <usb/usb.h>

void usb_hal_init_clocks(void);
const usb_device_controller_interface_struct_t * usb_hal_controller_interface(void);
void usb_hal_set_dev_handle(void *);
void usb_hal_enable_irq(void);
void usb_hal_clear_memory(void);

#endif /* __HAL_USB_H__ */

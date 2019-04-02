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
#include "hal/hal_os_tick.h"

#include "soc/rtc.h"
#include "soc/soc.h"
#include "soc/rtc.h"
#include "soc/dport_reg.h"
#include "soc/efuse_reg.h"
#include "soc/rtc_cntl_reg.h"

#define RTC_FREQ            32768       /* in Hz */

struct hal_os_tick
{
    int ticks_per_ostick;
    os_time_t max_idle_ticks;
    uint32_t lastocmp;
};

struct hal_os_tick g_hal_os_tick;

void
os_tick_idle(os_time_t ticks)
{
    asm("isync");
    asm("memw");
    asm("waiti 0");
}

void
os_tick_init(uint32_t os_ticks_per_sec, int prio)
{
    uint32_t rc;

    /* assert(RTC_FREQ % os_ticks_per_sec == 0); */

    /* g_hal_os_tick.lastocmp = 0; */
    /* g_hal_os_tick.ticks_per_ostick = RTC_FREQ / os_ticks_per_sec; */

    /* /\* */
    /*  * The maximum number of OS ticks allowed to elapse during idle is */
    /*  * limited to 1/4th the number of timer ticks before the 24-bit counter */
    /*  * rolls over. */
    /*  *\/ */
    /* g_hal_os_tick.max_idle_ticks = (1UL << 22) / g_hal_os_tick.ticks_per_ostick; */

    /* Set CPU to 80MHz. Keep other clocks unmodified. */
    int cpu_freq_mhz = 80;

    /* On ESP32 rev 0, switching to 80/160 MHz if clock was previously set to
     * 240 MHz may cause the chip to lock up (see section 3.5 of the errata
     * document). For rev. 0, switch to 240 instead if it has been enabled
     * previously.
     */
    uint32_t chip_ver_reg = REG_READ(EFUSE_BLK0_RDATA3_REG);
    if ((chip_ver_reg & EFUSE_RD_CHIP_VER_REV1_M) == 0 &&
            DPORT_REG_GET_FIELD(DPORT_CPU_PER_CONF_REG, DPORT_CPUPERIOD_SEL) == DPORT_CPUPERIOD_SEL_240) {
        cpu_freq_mhz = 240;
    }

    rtc_clk_config_t clk_cfg = RTC_CLK_CONFIG_DEFAULT();
    clk_cfg.xtal_freq = CONFIG_ESP32_XTAL_FREQ;
    clk_cfg.cpu_freq_mhz = cpu_freq_mhz;
    clk_cfg.slow_freq = rtc_clk_slow_freq_get();
    clk_cfg.fast_freq = rtc_clk_fast_freq_get();
    rtc_clk_init(clk_cfg);
}

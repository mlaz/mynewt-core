/* Copyright (c) 2012 ARM LIMITED
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice, this
 *     list of conditions and the following disclaimer.
 *
 *   * Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *
 *   * Neither the name of ARM nor the names of its contributors may be used to
 *     endorse or promote products derived from this software without specific
 *     prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <stdint.h>
#include <stdbool.h>
#include "mcu/cmsis_nvic.h"
#include "nrf.h"

#ifdef NRF9160_XXAA
#include "system_nrf9160.h"
#include "nrf9160.h"
#endif

/*lint ++flb "Enter library region" */

#define __SYSTEM_CLOCK      (64000000UL)

#ifdef NRF9160_XXAA
static bool nrf91_errata_1(void) __UNUSED;
static bool nrf91_errata_2(void) __UNUSED;
static bool nrf91_errata_4(void) __UNUSED;
static bool nrf91_errata_6(void) __UNUSED;
static bool nrf91_errata_7(void) __UNUSED;
static bool nrf91_errata_8(void) __UNUSED;
static bool nrf91_errata_9(void) __UNUSED;
static bool nrf91_errata_10(void) __UNUSED;
static bool nrf91_errata_12(void) __UNUSED;
static bool nrf91_errata_14(void) __UNUSED;
static bool nrf91_errata_15(void) __UNUSED;
static bool nrf91_errata_16(void) __UNUSED;
static bool nrf91_errata_17(void) __UNUSED;
static bool nrf91_errata_20(void) __UNUSED;
static bool nrf91_errata_21(void) __UNUSED;
static bool nrf91_errata_23(void) __UNUSED;
static bool nrf91_errata_24(void) __UNUSED;
static bool nrf91_errata_26(void) __UNUSED;
static bool nrf91_errata_27(void) __UNUSED;
static bool nrf91_errata_28(void) __UNUSED;
static bool nrf91_errata_29(void) __UNUSED;
static bool nrf91_errata_30(void) __UNUSED;
static bool nrf91_errata_31(void) __UNUSED;
#endif

#define TRACE_PIN_CNF_VALUE (   (GPIO_PIN_CNF_DIR_Output << GPIO_PIN_CNF_DIR_Pos) | \
                                (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos) | \
                                (GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos) | \
                                (GPIO_PIN_CNF_DRIVE_H0H1 << GPIO_PIN_CNF_DRIVE_Pos) | \
                                (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos) )
#define TRACE_TRACECLK_PIN   (21)
#define TRACE_TRACEDATA0_PIN (22)
#define TRACE_TRACEDATA1_PIN (23)
#define TRACE_TRACEDATA2_PIN (24)
#define TRACE_TRACEDATA3_PIN (25)

#if defined ( __CC_ARM )
    uint32_t SystemCoreClock __attribute__((used)) = __SYSTEM_CLOCK;  
#elif defined ( __ICCARM__ )
    __root uint32_t SystemCoreClock = __SYSTEM_CLOCK;
#elif defined ( __GNUC__ )
    uint32_t SystemCoreClock __attribute__((used)) = __SYSTEM_CLOCK;
#endif

/* Global values used used in Secure mode SystemInit. */
#if !defined(NRF_TRUSTZONE_NONSECURE)
    /* Global values used by UICR erase fix algorithm. */
    static uint32_t uicr_erased_value;
    static uint32_t uicr_new_value;
#endif

/* Errata are only handled in secure mode since they usually need access to FICR. */
#if !defined(NRF_TRUSTZONE_NONSECURE)
    static bool uicr_HFXOSRC_erased(void);
    static bool uicr_HFXOCNT_erased(void);
#endif

void SystemCoreClockUpdate(void)
{
    SystemCoreClock = __SYSTEM_CLOCK;
}

void SystemInit(void)
{
    #if !defined(NRF_TRUSTZONE_NONSECURE)
        /* Perform Secure-mode initialization routines. */

        /* Set all ARM SAU regions to NonSecure if TrustZone extensions are enabled.
        * Nordic SPU should handle Secure Attribution tasks */
        #if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
          SAU->CTRL |= (1 << SAU_CTRL_ALLNS_Pos);
        #endif
        
        /* Workaround for Errata 6 "POWER: SLEEPENTER and SLEEPEXIT events asserted after pin reset" found at the Errata document
            for your device located at https://infocenter.nordicsemi.com/index.jsp  */
        if (nrf91_errata_6()){
            NRF_POWER_S->EVENTS_SLEEPENTER = (POWER_EVENTS_SLEEPENTER_EVENTS_SLEEPENTER_NotGenerated << POWER_EVENTS_SLEEPENTER_EVENTS_SLEEPENTER_Pos);
            NRF_POWER_S->EVENTS_SLEEPEXIT = (POWER_EVENTS_SLEEPEXIT_EVENTS_SLEEPEXIT_NotGenerated << POWER_EVENTS_SLEEPEXIT_EVENTS_SLEEPEXIT_Pos);
        }

        /* Workaround for Errata 14 "REGULATORS: LDO mode at startup" found at the Errata document
            for your device located at https://infocenter.nordicsemi.com/index.jsp  */
        if (nrf91_errata_14()){
            *((volatile uint32_t *)0x50004A38) = 0x01ul;
            NRF_REGULATORS_S->DCDCEN = REGULATORS_DCDCEN_DCDCEN_Enabled << REGULATORS_DCDCEN_DCDCEN_Pos;
        }

        /* Workaround for Errata 15 "REGULATORS: LDO mode at startup" found at the Errata document
            for your device located at https://infocenter.nordicsemi.com/index.jsp  */
        if (nrf91_errata_15()){
            NRF_REGULATORS_S->DCDCEN = REGULATORS_DCDCEN_DCDCEN_Enabled << REGULATORS_DCDCEN_DCDCEN_Pos;
        }

        /* Workaround for Errata 20 "RAM content cannot be trusted upon waking up from System ON Idle or System OFF mode" found at the Errata document
            for your device located at https://infocenter.nordicsemi.com/index.jsp  */
        if (nrf91_errata_20()){
            *((volatile uint32_t *)0x5003AEE4) = 0xE;
        }

        /* Workaround for Errata 31 "XOSC32k Startup Failure" found at the Errata document
            for your device located at https://infocenter.nordicsemi.com/index.jsp  */
        if (nrf91_errata_31()){
            *((volatile uint32_t *)0x5000470Cul) = 0x0;
            *((volatile uint32_t *)0x50004710ul) = 0x1;
        }

        /* Trimming of the device. Copy all the trimming values from FICR into the target addresses. Trim
         until one ADDR is not initialized. */
        uint32_t index = 0;
        for (index = 0; index < 256ul && NRF_FICR_S->TRIMCNF[index].ADDR != 0xFFFFFFFFul; index++){
          #if defined ( __ICCARM__ )
              #pragma diag_suppress=Pa082
          #endif
          *(volatile uint32_t *)NRF_FICR_S->TRIMCNF[index].ADDR = NRF_FICR_S->TRIMCNF[index].DATA;
          #if defined ( __ICCARM__ )
              #pragma diag_default=Pa082
          #endif
        }

        /* Set UICR->HFXOSRC and UICR->HFXOCNT to working defaults if UICR was erased */
        if (uicr_HFXOSRC_erased() || uicr_HFXOCNT_erased()) {
          /* Wait for pending NVMC operations to finish */
          while (NRF_NVMC_S->READY != NVMC_READY_READY_Ready);

          /* Enable write mode in NVMC */
          NRF_NVMC_S->CONFIG = NVMC_CONFIG_WEN_Wen;
          while (NRF_NVMC_S->READY != NVMC_READY_READY_Ready);

          if (uicr_HFXOSRC_erased()){
            /* Write default value to UICR->HFXOSRC */
            uicr_erased_value = NRF_UICR_S->HFXOSRC;
            uicr_new_value = (uicr_erased_value & ~UICR_HFXOSRC_HFXOSRC_Msk) | UICR_HFXOSRC_HFXOSRC_TCXO;
            NRF_UICR_S->HFXOSRC = uicr_new_value;
            while (NRF_NVMC_S->READY != NVMC_READY_READY_Ready);
          }

          if (uicr_HFXOCNT_erased()){
            /* Write default value to UICR->HFXOCNT */
            uicr_erased_value = NRF_UICR_S->HFXOCNT;
            uicr_new_value = (uicr_erased_value & ~UICR_HFXOCNT_HFXOCNT_Msk) | 0x20;
            NRF_UICR_S->HFXOCNT = uicr_new_value;
            while (NRF_NVMC_S->READY != NVMC_READY_READY_Ready);
          }

          /* Enable read mode in NVMC */
          NRF_NVMC_S->CONFIG = NVMC_CONFIG_WEN_Ren;
          while (NRF_NVMC_S->READY != NVMC_READY_READY_Ready);

          /* Reset to apply clock select update */
          NVIC_SystemReset();
        }

        /* Enable Trace functionality. If ENABLE_TRACE is not defined, TRACE pins will be used as GPIOs (see Product
           Specification to see which ones). */
        #if defined (ENABLE_TRACE)
            // Enable Trace And Debug peripheral
            NRF_TAD_S->ENABLE = TAD_ENABLE_ENABLE_Msk;
            NRF_TAD_S->CLOCKSTART = TAD_CLOCKSTART_START_Msk;

            // Set up Trace pads SPU firewall
            NRF_SPU_S->GPIOPORT[0].PERM &= ~(1 << TRACE_TRACECLK_PIN);
            NRF_SPU_S->GPIOPORT[0].PERM &= ~(1 << TRACE_TRACEDATA0_PIN);
            NRF_SPU_S->GPIOPORT[0].PERM &= ~(1 << TRACE_TRACEDATA1_PIN);
            NRF_SPU_S->GPIOPORT[0].PERM &= ~(1 << TRACE_TRACEDATA2_PIN);
            NRF_SPU_S->GPIOPORT[0].PERM &= ~(1 << TRACE_TRACEDATA3_PIN);

            // Configure trace port pads
            NRF_P0_S->PIN_CNF[TRACE_TRACECLK_PIN] =   TRACE_PIN_CNF_VALUE;
            NRF_P0_S->PIN_CNF[TRACE_TRACEDATA0_PIN] = TRACE_PIN_CNF_VALUE;
            NRF_P0_S->PIN_CNF[TRACE_TRACEDATA1_PIN] = TRACE_PIN_CNF_VALUE;
            NRF_P0_S->PIN_CNF[TRACE_TRACEDATA2_PIN] = TRACE_PIN_CNF_VALUE;
            NRF_P0_S->PIN_CNF[TRACE_TRACEDATA3_PIN] = TRACE_PIN_CNF_VALUE;

            // Select trace pins
            NRF_TAD_S->PSEL.TRACECLK   = TRACE_TRACECLK_PIN;
            NRF_TAD_S->PSEL.TRACEDATA0 = TRACE_TRACEDATA0_PIN;
            NRF_TAD_S->PSEL.TRACEDATA1 = TRACE_TRACEDATA1_PIN;
            NRF_TAD_S->PSEL.TRACEDATA2 = TRACE_TRACEDATA2_PIN;
            NRF_TAD_S->PSEL.TRACEDATA3 = TRACE_TRACEDATA3_PIN;

            // Set trace port speed to 32 MHz
            NRF_TAD_S->TRACEPORTSPEED = TAD_TRACEPORTSPEED_TRACEPORTSPEED_32MHz;

            *((uint32_t *)(0xE0053000ul)) = 0x00000001ul;
            
            *((uint32_t *)(0xE005AFB0ul))  = 0xC5ACCE55ul;
            *((uint32_t *)(0xE005A000ul)) &= 0xFFFFFF00ul;
            *((uint32_t *)(0xE005A004ul))  = 0x00000009ul;
            *((uint32_t *)(0xE005A000ul))  = 0x00000303ul;
            *((uint32_t *)(0xE005AFB0ul))  = 0x00000000ul;

            *((uint32_t *)(0xE005BFB0ul))  = 0xC5ACCE55ul;
            *((uint32_t *)(0xE005B000ul)) &= 0xFFFFFF00ul;
            *((uint32_t *)(0xE005B004ul))  = 0x00003000ul;
            *((uint32_t *)(0xE005B000ul))  = 0x00000308ul;
            *((uint32_t *)(0xE005BFB0ul))  = 0x00000000ul;

            *((uint32_t *)(0xE0058FB0ul)) = 0xC5ACCE55ul;
            *((uint32_t *)(0xE0058000ul)) = 0x00000000ul;
            *((uint32_t *)(0xE0058004ul)) = 0x00000000ul;
            *((uint32_t *)(0xE0058FB0ul)) = 0x00000000ul;

            /* Rom table does not list ETB, or TPIU base addresses.
             * Some debug probes may require manual configuration of these peripherals to enable tracing.
             * ETB_BASE = 0xE0051000
             * TPIU_BASE = 0xE0054000
             */
        #endif

        /* Allow Non-Secure code to run FPU instructions. 
         * If only the secure code should control FPU power state these registers should be configured accordingly in the secure application code. */
        SCB->NSACR |= (3UL << 10);
    #endif
    
    /* Enable the FPU if the compiler used floating point unit instructions. __FPU_USED is a MACRO defined by the
    * compiler. Since the FPU consumes energy, remember to disable FPU use in the compiler if floating point unit
    * operations are not used in your code. */
    #if (__FPU_USED == 1)
      SCB->CPACR |= (3UL << 20) | (3UL << 22);
      __DSB();
      __ISB();
    #endif
    
/*     SystemCoreClockUpdate(); */
/* } */


#if !defined(NRF_TRUSTZONE_NONSECURE)

    bool uicr_HFXOCNT_erased()
    {
        if (NRF_UICR_S->HFXOCNT == 0xFFFFFFFFul) {
            return true;
        }
        return false;
    }
    
    
    bool uicr_HFXOSRC_erased()
    {
        if ((NRF_UICR_S->HFXOSRC & UICR_HFXOSRC_HFXOSRC_Msk) != UICR_HFXOSRC_HFXOSRC_TCXO) {
            return true;
        }
        return false;
    }
#endif


#ifdef NRF9160_XXAA
        /* Workaround for Errata 36 "CLOCK: Some registers are not reset when expected" found at the Errata document
           for your device located at https://infocenter.nordicsemi.com/  */
        if (errata_36()){
            NRF_CLOCK->EVENTS_DONE = 0;
            NRF_CLOCK->EVENTS_CTTO = 0;
            NRF_CLOCK->CTIV = 0;
        }

        /* Workaround for Errata 98 "NFCT: Not able to communicate with the peer" found at the Errata document
           for your device located at https://infocenter.nordicsemi.com/  */
        if (errata_98()){
            *(volatile uint32_t *)0x4000568Cul = 0x00038148ul;
        }

        /* Workaround for Errata 103 "CCM: Wrong reset value of CCM MAXPACKETSIZE" found at the Errata document
           for your device located at https://infocenter.nordicsemi.com/  */
        if (errata_103()){
            NRF_CCM->MAXPACKETSIZE = 0xFBul;
        }

        /* Workaround for Errata 115 "RAM: RAM content cannot be trusted upon waking up from System ON Idle or System OFF mode" found at the Errata document
           for your device located at https://infocenter.nordicsemi.com/  */
        if (errata_115()){
            *(volatile uint32_t *)0x40000EE4 = (*(volatile uint32_t *)0x40000EE4 & 0xFFFFFFF0) | (*(uint32_t *)0x10000258 & 0x0000000F);
        }

        /* Workaround for Errata 120 "QSPI: Data read or written is corrupted" found at the Errata document
           for your device located at https://infocenter.nordicsemi.com/  */
        if (errata_120()){
            *(volatile uint32_t *)0x40029640ul = 0x200ul;
        }

        /* Workaround for Errata 120 "QSPI: Second read and long read commands fail" found at the Errata document
           for your device located at https://infocenter.nordicsemi.com/  */
        if (errata_121()){
            *(volatile uint32_t *)0x40029600ul = 0x00040400ul;
        }

        /* Enable the FPU if the compiler used floating point unit instructions. __FPU_USED is a MACRO defined by the
         * compiler. Since the FPU consumes energy, remember to disable FPU use in the compiler if floating point unit
         * operations are not used in your code. */
        #if (__FPU_USED == 1)
            SCB->CPACR |= (3UL << 20) | (3UL << 22);
            __DSB();
            __ISB();
        #endif

        /* Configure NFCT pins as GPIOs if NFCT is not to be used in your code. If CONFIG_NFCT_PINS_AS_GPIOS is not defined,
           two GPIOs (see Product Specification to see which ones) will be reserved for NFC and will not be available as
           normal GPIOs. */
        #if defined (CONFIG_NFCT_PINS_AS_GPIOS)
            if ((NRF_UICR->NFCPINS & UICR_NFCPINS_PROTECT_Msk) == (UICR_NFCPINS_PROTECT_NFC << UICR_NFCPINS_PROTECT_Pos)){
                NRF_NVMC->CONFIG = NVMC_CONFIG_WEN_Wen << NVMC_CONFIG_WEN_Pos;
                while (NRF_NVMC->READY == NVMC_READY_READY_Busy){}
                NRF_UICR->NFCPINS &= ~UICR_NFCPINS_PROTECT_Msk;
                while (NRF_NVMC->READY == NVMC_READY_READY_Busy){}
                NRF_NVMC->CONFIG = NVMC_CONFIG_WEN_Ren << NVMC_CONFIG_WEN_Pos;
                while (NRF_NVMC->READY == NVMC_READY_READY_Busy){}
                NVIC_SystemReset();
            }
        #endif

        /* Configure GPIO pads as pPin Reset pin if Pin Reset capabilities desired. If CONFIG_GPIO_AS_PINRESET is not
          defined, pin reset will not be available. One GPIO (see Product Specification to see which one) will then be
          reserved for PinReset and not available as normal GPIO. */
        #if defined (CONFIG_GPIO_AS_PINRESET)
            if (((NRF_UICR->PSELRESET[0] & UICR_PSELRESET_CONNECT_Msk) != (UICR_PSELRESET_CONNECT_Connected << UICR_PSELRESET_CONNECT_Pos)) ||
                ((NRF_UICR->PSELRESET[1] & UICR_PSELRESET_CONNECT_Msk) != (UICR_PSELRESET_CONNECT_Connected << UICR_PSELRESET_CONNECT_Pos))){
                NRF_NVMC->CONFIG = NVMC_CONFIG_WEN_Wen << NVMC_CONFIG_WEN_Pos;
                while (NRF_NVMC->READY == NVMC_READY_READY_Busy){}
                NRF_UICR->PSELRESET[0] = 18;
                while (NRF_NVMC->READY == NVMC_READY_READY_Busy){}
                NRF_UICR->PSELRESET[1] = 18;
                while (NRF_NVMC->READY == NVMC_READY_READY_Busy){}
                NRF_NVMC->CONFIG = NVMC_CONFIG_WEN_Ren << NVMC_CONFIG_WEN_Pos;
                while (NRF_NVMC->READY == NVMC_READY_READY_Busy){}
                NVIC_SystemReset();
            }
        #endif

        /* Enable SWO trace functionality. If ENABLE_SWO is not defined, SWO pin will be used as GPIO (see Product
           Specification to see which one). */
        #if defined (ENABLE_SWO)
            CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
            NRF_CLOCK->TRACECONFIG |= CLOCK_TRACECONFIG_TRACEMUX_Serial << CLOCK_TRACECONFIG_TRACEMUX_Pos;
            NRF_P1->PIN_CNF[0] = (GPIO_PIN_CNF_DRIVE_H0H1 << GPIO_PIN_CNF_DRIVE_Pos) | (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos) | (GPIO_PIN_CNF_DIR_Output << GPIO_PIN_CNF_DIR_Pos);
        #endif

        /* Enable Trace functionality. If ENABLE_TRACE is not defined, TRACE pins will be used as GPIOs (see Product
           Specification to see which ones). */
        #if defined (ENABLE_TRACE)
            CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
            NRF_CLOCK->TRACECONFIG |= CLOCK_TRACECONFIG_TRACEMUX_Parallel << CLOCK_TRACECONFIG_TRACEMUX_Pos;
            NRF_P0->PIN_CNF[7]  = (GPIO_PIN_CNF_DRIVE_H0H1 << GPIO_PIN_CNF_DRIVE_Pos) | (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos) | (GPIO_PIN_CNF_DIR_Output << GPIO_PIN_CNF_DIR_Pos);
            NRF_P1->PIN_CNF[0]  = (GPIO_PIN_CNF_DRIVE_H0H1 << GPIO_PIN_CNF_DRIVE_Pos) | (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos) | (GPIO_PIN_CNF_DIR_Output << GPIO_PIN_CNF_DIR_Pos);
            NRF_P0->PIN_CNF[12] = (GPIO_PIN_CNF_DRIVE_H0H1 << GPIO_PIN_CNF_DRIVE_Pos) | (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos) | (GPIO_PIN_CNF_DIR_Output << GPIO_PIN_CNF_DIR_Pos);
            NRF_P0->PIN_CNF[11] = (GPIO_PIN_CNF_DRIVE_H0H1 << GPIO_PIN_CNF_DRIVE_Pos) | (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos) | (GPIO_PIN_CNF_DIR_Output << GPIO_PIN_CNF_DIR_Pos);
            NRF_P1->PIN_CNF[9]  = (GPIO_PIN_CNF_DRIVE_H0H1 << GPIO_PIN_CNF_DRIVE_Pos) | (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos) | (GPIO_PIN_CNF_DIR_Output << GPIO_PIN_CNF_DIR_Pos);
        #endif
#endif
    SystemCoreClockUpdate();

    NVIC_Relocate();
}

#ifdef NRF52
static bool nrf91_errata_1(void)
{
    #ifndef NRF91_SERIES
        return false;
    #else
        #if defined (NRF9160_XXAA) || defined (DEVELOP_IN_NRF9160)
            uint32_t var1 = *(uint32_t *)0x00FF0130ul;
            uint32_t var2 = *(uint32_t *)0x00FF0134ul;
        #endif
        #if defined (NRF9160_XXAA) || defined (DEVELOP_IN_NRF9160)
            if (var1 == 0x09)
            {
                switch(var2)
                {
                    case 0x01ul:
                        return true;
                    case 0x02ul:
                        return true;
                }
            }
        #endif
        return false;
    #endif
}

static bool nrf91_errata_2(void)
{
    #ifndef NRF91_SERIES
        return false;
    #else
        #if defined (NRF9160_XXAA) || defined (DEVELOP_IN_NRF9160)
            uint32_t var1 = *(uint32_t *)0x00FF0130ul;
            uint32_t var2 = *(uint32_t *)0x00FF0134ul;
        #endif
        #if defined (NRF9160_XXAA) || defined (DEVELOP_IN_NRF9160)
            if (var1 == 0x09)
            {
                switch(var2)
                {
                    case 0x01ul:
                        return true;
                    case 0x02ul:
                        return true;
                }
            }
        #endif
        return false;
    #endif
}

static bool nrf91_errata_4(void)
{
    #ifndef NRF91_SERIES
        return false;
    #else
        #if defined (NRF9160_XXAA) || defined (DEVELOP_IN_NRF9160)
            uint32_t var1 = *(uint32_t *)0x00FF0130ul;
            uint32_t var2 = *(uint32_t *)0x00FF0134ul;
        #endif
        #if defined (NRF9160_XXAA) || defined (DEVELOP_IN_NRF9160)
            if (var1 == 0x09)
            {
                switch(var2)
                {
                    case 0x01ul:
                        return true;
                    case 0x02ul:
                        return true;
                }
            }
        #endif
        return false;
    #endif
}

static bool nrf91_errata_6(void)
{
    #ifndef NRF91_SERIES
        return false;
    #else
        #if defined (NRF9160_XXAA) || defined (DEVELOP_IN_NRF9160)
            uint32_t var1 = *(uint32_t *)0x00FF0130ul;
            uint32_t var2 = *(uint32_t *)0x00FF0134ul;
        #endif
        #if defined (NRF9160_XXAA) || defined (DEVELOP_IN_NRF9160)
            if (var1 == 0x09)
            {
                switch(var2)
                {
                    case 0x01ul:
                        return true;
                    case 0x02ul:
                        return true;
                }
            }
        #endif
        return false;
    #endif
}

static bool nrf91_errata_7(void)
{
    #ifndef NRF91_SERIES
        return false;
    #else
        #if defined (NRF9160_XXAA) || defined (DEVELOP_IN_NRF9160)
            uint32_t var1 = *(uint32_t *)0x00FF0130ul;
            uint32_t var2 = *(uint32_t *)0x00FF0134ul;
        #endif
        #if defined (NRF9160_XXAA) || defined (DEVELOP_IN_NRF9160)
            if (var1 == 0x09)
            {
                switch(var2)
                {
                    case 0x01ul:
                        return true;
                    case 0x02ul:
                        return true;
                }
            }
        #endif
        return false;
    #endif
}

static bool nrf91_errata_8(void)
{
    #ifndef NRF91_SERIES
        return false;
    #else
        #if defined (NRF9160_XXAA) || defined (DEVELOP_IN_NRF9160)
            uint32_t var1 = *(uint32_t *)0x00FF0130ul;
            uint32_t var2 = *(uint32_t *)0x00FF0134ul;
        #endif
        #if defined (NRF9160_XXAA) || defined (DEVELOP_IN_NRF9160)
            if (var1 == 0x09)
            {
                switch(var2)
                {
                    case 0x01ul:
                        return true;
                    case 0x02ul:
                        return false;
                }
            }
        #endif
        return false;
    #endif
}

static bool nrf91_errata_9(void)
{
    #ifndef NRF91_SERIES
        return false;
    #else
        #if defined (NRF9160_XXAA) || defined (DEVELOP_IN_NRF9160)
            uint32_t var1 = *(uint32_t *)0x00FF0130ul;
            uint32_t var2 = *(uint32_t *)0x00FF0134ul;
        #endif
        #if defined (NRF9160_XXAA) || defined (DEVELOP_IN_NRF9160)
            if (var1 == 0x09)
            {
                switch(var2)
                {
                    case 0x01ul:
                        return false;
                    case 0x02ul:
                        return true;
                }
            }
        #endif
        return false;
    #endif
}

static bool nrf91_errata_10(void)
{
    #ifndef NRF91_SERIES
        return false;
    #else
        #if defined (NRF9160_XXAA) || defined (DEVELOP_IN_NRF9160)
            uint32_t var1 = *(uint32_t *)0x00FF0130ul;
            uint32_t var2 = *(uint32_t *)0x00FF0134ul;
        #endif
        #if defined (NRF9160_XXAA) || defined (DEVELOP_IN_NRF9160)
            if (var1 == 0x09)
            {
                switch(var2)
                {
                    case 0x01ul:
                        return true;
                    case 0x02ul:
                        return false;
                }
            }
        #endif
        return false;
    #endif
}

static bool nrf91_errata_12(void)
{
    #ifndef NRF91_SERIES
        return false;
    #else
        #if defined (NRF9160_XXAA) || defined (DEVELOP_IN_NRF9160)
            uint32_t var1 = *(uint32_t *)0x00FF0130ul;
            uint32_t var2 = *(uint32_t *)0x00FF0134ul;
        #endif
        #if defined (NRF9160_XXAA) || defined (DEVELOP_IN_NRF9160)
            if (var1 == 0x09)
            {
                switch(var2)
                {
                    case 0x01ul:
                        return true;
                    case 0x02ul:
                        return false;
                }
            }
        #endif
        return false;
    #endif
}

static bool nrf91_errata_14(void)
{
    #ifndef NRF91_SERIES
        return false;
    #else
        #if defined (NRF9160_XXAA) || defined (DEVELOP_IN_NRF9160)
            uint32_t var1 = *(uint32_t *)0x00FF0130ul;
            uint32_t var2 = *(uint32_t *)0x00FF0134ul;
        #endif
        #if defined (NRF9160_XXAA) || defined (DEVELOP_IN_NRF9160)
            if (var1 == 0x09)
            {
                switch(var2)
                {
                    case 0x01ul:
                        return true;
                    case 0x02ul:
                        return false;
                }
            }
        #endif
        return false;
    #endif
}

static bool nrf91_errata_15(void)
{
    #ifndef NRF91_SERIES
        return false;
    #else
        #if defined (NRF9160_XXAA) || defined (DEVELOP_IN_NRF9160)
            uint32_t var1 = *(uint32_t *)0x00FF0130ul;
            uint32_t var2 = *(uint32_t *)0x00FF0134ul;
        #endif
        #if defined (NRF9160_XXAA) || defined (DEVELOP_IN_NRF9160)
            if (var1 == 0x09)
            {
                switch(var2)
                {
                    case 0x01ul:
                        return false;
                    case 0x02ul:
                        return true;
                }
            }
        #endif
        return false;
    #endif
}

static bool nrf91_errata_16(void)
{
    #ifndef NRF91_SERIES
        return false;
    #else
        #if defined (NRF9160_XXAA) || defined (DEVELOP_IN_NRF9160)
            uint32_t var1 = *(uint32_t *)0x00FF0130ul;
            uint32_t var2 = *(uint32_t *)0x00FF0134ul;
        #endif
        #if defined (NRF9160_XXAA) || defined (DEVELOP_IN_NRF9160)
            if (var1 == 0x09)
            {
                switch(var2)
                {
                    case 0x01ul:
                        return true;
                    case 0x02ul:
                        return false;
                }
            }
        #endif
        return false;
    #endif
}

static bool nrf91_errata_17(void)
{
    #ifndef NRF91_SERIES
        return false;
    #else
        #if defined (NRF9160_XXAA) || defined (DEVELOP_IN_NRF9160)
            uint32_t var1 = *(uint32_t *)0x00FF0130ul;
            uint32_t var2 = *(uint32_t *)0x00FF0134ul;
        #endif
        #if defined (NRF9160_XXAA) || defined (DEVELOP_IN_NRF9160)
            if (var1 == 0x09)
            {
                switch(var2)
                {
                    case 0x01ul:
                        return true;
                    case 0x02ul:
                        return false;
                }
            }
        #endif
        return false;
    #endif
}

static bool nrf91_errata_20(void)
{
    #ifndef NRF91_SERIES
        return false;
    #else
        #if defined (NRF9160_XXAA) || defined (DEVELOP_IN_NRF9160)
            uint32_t var1 = *(uint32_t *)0x00FF0130ul;
            uint32_t var2 = *(uint32_t *)0x00FF0134ul;
        #endif
        #if defined (NRF9160_XXAA) || defined (DEVELOP_IN_NRF9160)
            if (var1 == 0x09)
            {
                switch(var2)
                {
                    case 0x01ul:
                        return true;
                    case 0x02ul:
                        return false;
                }
            }
        #endif
        return false;
    #endif
}

static bool nrf91_errata_21(void)
{
    #ifndef NRF91_SERIES
        return false;
    #else
        #if defined (NRF9160_XXAA) || defined (DEVELOP_IN_NRF9160)
            uint32_t var1 = *(uint32_t *)0x00FF0130ul;
            uint32_t var2 = *(uint32_t *)0x00FF0134ul;
        #endif
        #if defined (NRF9160_XXAA) || defined (DEVELOP_IN_NRF9160)
            if (var1 == 0x09)
            {
                switch(var2)
                {
                    case 0x01ul:
                        return true;
                    case 0x02ul:
                        return true;
                }
            }
        #endif
        return false;
    #endif
}

static bool nrf91_errata_23(void)
{
    #ifndef NRF91_SERIES
        return false;
    #else
        #if defined (NRF9160_XXAA) || defined (DEVELOP_IN_NRF9160)
            uint32_t var1 = *(uint32_t *)0x00FF0130ul;
            uint32_t var2 = *(uint32_t *)0x00FF0134ul;
        #endif
        #if defined (NRF9160_XXAA) || defined (DEVELOP_IN_NRF9160)
            if (var1 == 0x09)
            {
                switch(var2)
                {
                    case 0x01ul:
                        return true;
                    case 0x02ul:
                        return true;
                }
            }
        #endif
        return false;
    #endif
}

static bool nrf91_errata_24(void)
{
    #ifndef NRF91_SERIES
        return false;
    #else
        #if defined (NRF9160_XXAA) || defined (DEVELOP_IN_NRF9160)
            uint32_t var1 = *(uint32_t *)0x00FF0130ul;
            uint32_t var2 = *(uint32_t *)0x00FF0134ul;
        #endif
        #if defined (NRF9160_XXAA) || defined (DEVELOP_IN_NRF9160)
            if (var1 == 0x09)
            {
                switch(var2)
                {
                    case 0x01ul:
                        return true;
                    case 0x02ul:
                        return true;
                }
            }
        #endif
        return false;
    #endif
}

static bool nrf91_errata_26(void)
{
    #ifndef NRF91_SERIES
        return false;
    #else
        #if defined (NRF9160_XXAA) || defined (DEVELOP_IN_NRF9160)
            uint32_t var1 = *(uint32_t *)0x00FF0130ul;
            uint32_t var2 = *(uint32_t *)0x00FF0134ul;
        #endif
        #if defined (NRF9160_XXAA) || defined (DEVELOP_IN_NRF9160)
            if (var1 == 0x09)
            {
                switch(var2)
                {
                    case 0x01ul:
                        return true;
                    case 0x02ul:
                        return true;
                }
            }
        #endif
        return false;
    #endif
}

static bool nrf91_errata_27(void)
{
    #ifndef NRF91_SERIES
        return false;
    #else
        #if defined (NRF9160_XXAA) || defined (DEVELOP_IN_NRF9160)
            uint32_t var1 = *(uint32_t *)0x00FF0130ul;
            uint32_t var2 = *(uint32_t *)0x00FF0134ul;
        #endif
        #if defined (NRF9160_XXAA) || defined (DEVELOP_IN_NRF9160)
            if (var1 == 0x09)
            {
                switch(var2)
                {
                    case 0x01ul:
                        return false;
                    case 0x02ul:
                        return true;
                }
            }
        #endif
        return false;
    #endif
}

static bool nrf91_errata_28(void)
{
    #ifndef NRF91_SERIES
        return false;
    #else
        #if defined (NRF9160_XXAA) || defined (DEVELOP_IN_NRF9160)
            uint32_t var1 = *(uint32_t *)0x00FF0130ul;
            uint32_t var2 = *(uint32_t *)0x00FF0134ul;
        #endif
        #if defined (NRF9160_XXAA) || defined (DEVELOP_IN_NRF9160)
            if (var1 == 0x09)
            {
                switch(var2)
                {
                    case 0x01ul:
                        return true;
                    case 0x02ul:
                        return true;
                }
            }
        #endif
        return false;
    #endif
}

static bool nrf91_errata_29(void)
{
    #ifndef NRF91_SERIES
        return false;
    #else
        #if defined (NRF9160_XXAA) || defined (DEVELOP_IN_NRF9160)
            uint32_t var1 = *(uint32_t *)0x00FF0130ul;
            uint32_t var2 = *(uint32_t *)0x00FF0134ul;
        #endif
        #if defined (NRF9160_XXAA) || defined (DEVELOP_IN_NRF9160)
            if (var1 == 0x09)
            {
                switch(var2)
                {
                    case 0x01ul:
                        return true;
                    case 0x02ul:
                        return true;
                }
            }
        #endif
        return false;
    #endif
}

static bool nrf91_errata_30(void)
{
    #ifndef NRF91_SERIES
        return false;
    #else
        #if defined (NRF9160_XXAA) || defined (DEVELOP_IN_NRF9160)
            uint32_t var1 = *(uint32_t *)0x00FF0130ul;
            uint32_t var2 = *(uint32_t *)0x00FF0134ul;
        #endif
        #if defined (NRF9160_XXAA) || defined (DEVELOP_IN_NRF9160)
            if (var1 == 0x09)
            {
                switch(var2)
                {
                    case 0x01ul:
                        return true;
                    case 0x02ul:
                        return true;
                }
            }
        #endif
        return false;
    #endif
}

static bool nrf91_errata_31(void)
{
    #ifndef NRF91_SERIES
        return false;
    #else
        #if defined (NRF9160_XXAA) || defined (DEVELOP_IN_NRF9160)
            uint32_t var1 = *(uint32_t *)0x00FF0130ul;
            uint32_t var2 = *(uint32_t *)0x00FF0134ul;
        #endif
        #if defined (NRF9160_XXAA) || defined (DEVELOP_IN_NRF9160)
            if (var1 == 0x09)
            {
                switch(var2)
                {
                    case 0x01ul:
                        return true;
                    case 0x02ul:
                        return true;
                }
            }
        #endif
        return false;
    #endif
}

#endif /* NRF91_ERRATAS_H */
/*lint --flb "Leave library region" */

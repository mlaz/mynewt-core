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

#ifndef NRFX9160_CONFIG_H__
#define NRFX9160_CONFIG_H__

#include "syscfg/syscfg.h"

/* XXX: review this file, there are probably macros that are not configured. */
#define NRF_CLOCK      NRF_CLOCK_S
#define NRF_DPPIC      NRF_DPPIC_S
#define NRF_EGU0       NRF_EGU0_S
#define NRF_EGU1       NRF_EGU1_S
#define NRF_EGU2       NRF_EGU2_S
#define NRF_EGU3       NRF_EGU3_S
#define NRF_EGU4       NRF_EGU4_S
#define NRF_EGU5       NRF_EGU5_S
#define NRF_FPU        NRF_FPU_S
#define NRF_I2S        NRF_I2S_S
#define NRF_IPC        NRF_IPC_S
#define NRF_KMU        NRF_KMU_S
#define NRF_NVMC       NRF_NVMC_S
#define NRF_P0         NRF_P0_S
#define NRF_PDM        NRF_PDM_S
#define NRF_POWER      NRF_POWER_S
#define NRF_PWM0       NRF_PWM0_S
#define NRF_PWM1       NRF_PWM1_S
#define NRF_PWM2       NRF_PWM2_S
#define NRF_PWM3       NRF_PWM3_S
#define NRF_REGULATORS NRF_REGULATORS_S
#define NRF_RTC0       NRF_RTC0_S
#define NRF_RTC1       NRF_RTC1_S
#define NRF_SAADC      NRF_SAADC_S
#define NRF_SPIM0      NRF_SPIM0_S
#define NRF_SPIM1      NRF_SPIM1_S
#define NRF_SPIM2      NRF_SPIM2_S
#define NRF_SPIM3      NRF_SPIM3_S
#define NRF_SPIS0      NRF_SPIS0_S
#define NRF_SPIS1      NRF_SPIS1_S
#define NRF_SPIS2      NRF_SPIS2_S
#define NRF_SPIS3      NRF_SPIS3_S
#define NRF_TIMER0     NRF_TIMER0_S
#define NRF_TIMER1     NRF_TIMER1_S
#define NRF_TIMER2     NRF_TIMER2_S
#define NRF_TWIM0      NRF_TWIM0_S
#define NRF_TWIM1      NRF_TWIM1_S
#define NRF_TWIM2      NRF_TWIM2_S
#define NRF_TWIM3      NRF_TWIM3_S
#define NRF_TWIS0      NRF_TWIS0_S
#define NRF_TWIS1      NRF_TWIS1_S
#define NRF_TWIS2      NRF_TWIS2_S
#define NRF_TWIS3      NRF_TWIS3_S
#define NRF_UARTE0     NRF_UARTE0_S
#define NRF_UARTE1     NRF_UARTE1_S
#define NRF_UARTE2     NRF_UARTE2_S
#define NRF_UARTE3     NRF_UARTE3_S
#define NRF_VMC        NRF_VMC_S
#define NRF_WDT        NRF_WDT_S

/*
 * The following section provides the name translation for peripherals with
 * only one type of access available. For these peripherals, you cannot choose
 * between secure and non-secure mapping.
 */
#define NRF_CRYPTOCELL NRF_CRYPTOCELL_S
#define NRF_FICR       NRF_FICR_S
#define NRF_GPIOTE0    NRF_GPIOTE0_S
#define NRF_GPIOTE1    NRF_GPIOTE1_NS
#define NRF_SPU        NRF_SPU_S
#define NRF_UICR       NRF_UICR_S

/* Fixups for GPIOTE driver. */
#define NRF_GPIOTE        NRF_GPIOTE0_S
#define GPIOTE_IRQHandler GPIOTE0_IRQHandler

#ifndef NRFX_CLOCK_ENABLED
#define NRFX_CLOCK_ENABLED 0
#endif

#ifndef NRFX_COMP_ENABLED
#define NRFX_COMP_ENABLED 0
#endif

#ifndef NRFX_GPIOTE_ENABLED
#define NRFX_GPIOTE_ENABLED 0
#endif

#ifndef NRFX_I2S_ENABLED
#define NRFX_I2S_ENABLED 0
#endif

#ifndef NRFX_LPCOMP_ENABLED
#define NRFX_LPCOMP_ENABLED 0
#endif

#ifndef NRFX_PDM_ENABLED
#define NRFX_PDM_ENABLED 0
#endif

#ifndef NRFX_POWER_ENABLED
#define NRFX_POWER_ENABLED 0
#endif

#ifndef NRFX_PPI_ENABLED
#define NRFX_PPI_ENABLED 0
#endif

#ifndef NRFX_PRS_ENABLDE
#define NRFX_PRS_ENABLDE 0
#endif

#ifndef NRFX_QDEC_ENABLED
#define NRFX_QDEC_ENABLED 0
#endif

#ifndef NRFX_RNG_ENABLED
#define NRFX_RNG_ENABLED 0
#endif

#ifndef NRFX_RTC_ENABLED
#define NRFX_RTC_ENABLED 0
#endif

#ifndef NRFX_SPIM_ENABLED
#define NRFX_SPIM_ENABLED 0
#endif

#ifndef NRFX_SPIS_ENABLED
#define NRFX_SPIS_ENABLED 0
#endif

#ifndef NRFX_SPI_ENABLED
#define NRFX_SPI_ENABLED 0
#endif

#ifndef NRFX_SWI_ENABLED
#define NRFX_SWI_ENABLED 0
#endif

#ifndef NRFX_SYSTICK_ENABLED
#define NRFX_SYSTICK_ENABLED 0
#endif

#ifndef NRFX_TIMER_ENABLED
#define NRFX_TIMER_ENABLED 0
#endif

#ifndef NRFX_TWIM_ENABLED
#define NRFX_TWIM_ENABLED 0
#endif

#ifndef NRFX_TWIS_ENABLED
#define NRFX_TWIS_ENABLED 0
#endif

#ifndef NRFX_TWI_ENABLED
#define NRFX_TWI_ENABLED 0
#endif

#ifndef NRFX_UARTE_ENABLED
#define NRFX_UARTE_ENABLED 0
#endif

#ifndef NRFX_UART_ENABLED
#define NRFX_UART_ENABLED 0
#endif

#ifndef NRFX_WDT_ENABLED
#define NRFX_WDT_ENABLED 0
#endif

#if MYNEWT_VAL(ADC_0)
#define NRFX_SAADC_ENABLED 1
#define NRFX_SAADC_CONFIG_RESOLUTION 1
#define NRFX_SAADC_CONFIG_OVERSAMPLE 0
#define NRFX_SAADC_CONFIG_LP_MODE 0
#define NRFX_SAADC_CONFIG_IRQ_PRIORITY 7
#endif

#if (MYNEWT_VAL(PWM_0) || MYNEWT_VAL(PWM_1) || MYNEWT_VAL(PWM_2) || MYNEWT_VAL(PWM_3))
#define NRFX_PWM_ENABLED 1
#define NRFX_PWM_DEFAULT_CONFIG_OUT0_PIN 31
#define NRFX_PWM_DEFAULT_CONFIG_OUT1_PIN 31
#define NRFX_PWM_DEFAULT_CONFIG_OUT2_PIN 31
#define NRFX_PWM_DEFAULT_CONFIG_OUT3_PIN 31
#define NRFX_PWM_DEFAULT_CONFIG_BASE_CLOCK 4
#define NRFX_PWM_DEFAULT_CONFIG_COUNT_MODE 0
#define NRFX_PWM_DEFAULT_CONFIG_TOP_VALUE 1000
#define NRFX_PWM_DEFAULT_CONFIG_LOAD_MODE 0
#define NRFX_PWM_DEFAULT_CONFIG_STEP_MODE 0
#define NRFX_PWM_DEFAULT_CONFIG_IRQ_PRIORITY 7
#endif

#if MYNEWT_VAL(PWM_0)
#define NRFX_PWM0_ENABLED 1
#endif

#if MYNEWT_VAL(PWM_1)
#define NRFX_PWM1_ENABLED 1
#endif

#if MYNEWT_VAL(PWM_2)
#define NRFX_PWM2_ENABLED 1
#endif

#if MYNEWT_VAL(PWM_3)
#define NRFX_PWM3_ENABLED 1
#endif

#endif // NRFX9160_CONFIG_H__

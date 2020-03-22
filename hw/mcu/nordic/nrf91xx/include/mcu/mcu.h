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

#ifndef __MCU_MCU_H_
#define __MCU_MCU_H_

#ifdef __cplusplus
extern "C" {
#endif

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
 * Defines for naming GPIOs. NOTE: the nordic chip docs use numeric labels for
 * ports. Port A corresponds to Port 0, B to 1, etc. The nrf52832 has only one
 * port and thus uses pins 0 - 31. The nrf52840 has two ports but Port 1 only
 * has 16 pins.
 */
#define MCU_GPIO_PORTA(pin)	((0 * 16) + (pin))
#define MCU_GPIO_PORTB(pin)	((1 * 16) + (pin))

#if NRF52

#define MCU_SYSVIEW_INTERRUPTS \
    "I#1=Reset,I#2=MNI,I#3=HardFault,I#4=MemoryMgmt,I#5=BusFault,I#6=UsageFault," \
    "I#11=SVCall,I#12=DebugMonitor,I#14=PendSV,I#15=SysTick," \
    "I#16=POWER_CLOCK,I#17=RADIO,I#18=UARTE0_UART0,I#19=SPIx0_TWIx0," \
    "I#20=SPIx1_TWIx1,I#21=NFCT,I#22=GPIOTE,I#23=SAADC," \
    "I#24=TIMER0,I#25=TIMER1,I#26=TIMER2,I#27=RTC0,I#28=TEMP,I#29=RNG,I#30=ECB," \
    "I#31=CCM_AAR,I#32=WDT,I#33=RTC1,I#34=QDEC,I#35=COMP_LPCOMP,I#36=SWI0_EGU0," \
    "I#37=SWI1_EGU1,I#38=SWI2_EGU2,I#39=SWI3_EGU3,I#40=SWI4_EGU4,I#41=SWI5_EGU5," \
    "I#42=TIMER3,I#43=TIMER4,I#44=PWM0,I#45=PDM,I#48=MWU,I#49=PWM1,I#50=PWM2," \
    "I#51=SPIx2,I#52=RTC2,I#53=I2S,I#54=FPU"

#elif NRF52840_XXAA

#define MCU_SYSVIEW_INTERRUPTS \
    "I#1=Reset,I#2=MNI,I#3=HardFault,I#4=MemoryMgmt,I#5=BusFault,I#6=UsageFault," \
    "I#11=SVCall,I#12=DebugMonitor,I#14=PendSV,I#15=SysTick," \
    "I#16=POWER_CLOCK,I#17=RADIO,I#18=UARTE0_UART0,I#19=SPIx0_TWIx0," \
    "I#20=SPIx1_TWIx1,I#21=NFCT,I#22=GPIOTE,I#23=SAADC," \
    "I#24=TIMER0,I#25=TIMER1,I#26=TIMER2,I#27=RTC0,I#28=TEMP,I#29=RNG,I#30=ECB," \
    "I#31=CCM_AAR,I#32=WDT,I#33=RTC1,I#34=QDEC,I#35=COMP_LPCOMP,I#36=SWI0_EGU0," \
    "I#37=SWI1_EGU1,I#38=SWI2_EGU2,I#39=SWI3_EGU3,I#40=SWI4_EGU4,I#41=SWI5_EGU5," \
    "I#42=TIMER3,I#43=TIMER4,I#44=PWM0,I#45=PDM,I#48=MWU,I#49=PWM1,I#50=PWM2," \
    "I#51=SPIx2,I#52=RTC2,I#53=I2S,I#54=FPU,I#55=USBD," \
    "I#56=UARTE1,I#57=QSPI,I#58=CRYPTOCELL,I#61=PWM3,I#63=SPIM3"

#endif

#ifdef __cplusplus
}
#endif

#endif /* __MCU_MCU_H_ */

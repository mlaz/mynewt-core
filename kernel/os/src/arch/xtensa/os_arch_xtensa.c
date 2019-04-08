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
#include "os_priv.h"

#include <hal/hal_bsp.h>
#include <hal/hal_os_tick.h>

#include "xtensa/xtensa_api"


/* extern void trap_entry(); */

struct stack_frame {
    uint32_t pc;
    uint32_t ps;
    uint32_t a0;
    uint32_t a1;
    uint32_t a2;
    uint32_t a3;
    uint32_t a4;
    uint32_t a5;
    uint32_t a6;
    uint32_t a7;
    uint32_t a8;
    uint32_t a9;
    uint32_t a10;
    uint32_t a11;
    uint32_t a12;
    uint32_t a13;
    uint32_t a14;
    uint32_t a15;
    uint32_t sar;
    uint32_t exccause;
    uint32_t excvaddr;
    uint32_t loopsst_wintmps;
};

/* uint32_t */
/* mtime_lo(void) */
/* { */
/*     return CLINT_REG(CLINT_MTIME); */
/* } */

/* uint32_t */
/* mtime_hi(void) */
/* { */
/*     return CLINT_REG(CLINT_MTIME + 4); */
/* } */

/* uint64_t */
/* get_timer_value(void) */
/* { */
/*     while (1) { */
/*         uint32_t hi = mtime_hi(); */
/*         uint32_t lo = mtime_lo(); */
/*         if (hi == mtime_hi()) */
/*             return ((uint64_t)hi << 32) | lo; */
/*     } */
/* } */

/* void */
/* set_mtimecmp(uint64_t time) */
/* { */
/*     CLINT_REG(CLINT_MTIMECMP + 4) = -1; */
/*     CLINT_REG(CLINT_MTIMECMP) = (uint32_t) time; */
/*     CLINT_REG(CLINT_MTIMECMP + 4) = (uint32_t) (time >> 32); */
/* } */

/* unsigned long */
/* get_timer_freq() */
/* { */
/*   return 32768; */
/* } */

/* XXX: determine how to deal with running un-privileged */
/* only priv currently supported */
uint32_t os_flags = OS_RUN_PRIV;

extern struct os_task g_idle_task;

#define OS_TICK_PRIO 0

static int
os_in_isr(void)
{
    // TODO:
    return 0;
}

void
timer_handler(void)
{
    os_time_advance(1);
}

void
os_arch_ctx_sw(struct os_task *t)
{
    os_sched_ctx_sw_hook(t);

    /*
     * This request software interrupt that is used for contect switching
     */
    CLINT_REG(CLINT_MSIP) = 1;
}

os_sr_t
os_arch_save_sr(void)
{
    uint32_t isr_ctx;

    isr_ctx = clear_csr(mstatus, MSTATUS_MIE) & MSTATUS_MIE;

    return isr_ctx;
}

void
os_arch_restore_sr(os_sr_t isr_ctx)
{
    if (isr_ctx) {
        set_csr(mstatus, MSTATUS_MIE);
    }
}

int
os_arch_in_critical(void)
{
    return !(read_csr(mstatus) & MSTATUS_MIE);
}

/* assumes stack_top will be 8 aligned */
os_stack_t *
os_arch_task_stack_init(struct os_task *t, os_stack_t *stack_top, int size)
{
    int i;
    os_stack_t *s;
    struct stack_frame *sf;

    /* Get stack frame pointer */
    s = (struct stack_frame *) ((uint8_t *) stack_top - sizeof(*sf));

    /* Zero out general use registers. */
    for (i = 4; i < 22; ++i) {
       s[i] = 0;
    }

    /* Set the ps register. */
#ifdef __XTENSA_CALL0_ABI__
    sf->ps = PS_UM | PS_EXCM;
#else
    /* For windowed ABI set WOE and CALLINC (pretend task was 'call4'd). */
    sf->ps = PS_UM | PS_EXCM | PS_WOE | PS_CALLINC(1);
#endif

    /* Set remaining portions of stack frame. */
    sf->pc = (uint32_t) t->t_func;
    sf->a0 = (uint32_t) t->t_arg;

    /* Set the initial stack pointer. */
    sf->a1 = (uint32_t) sf;

    return (os_stack_t *) sf;
}

void
os_arch_init(void)
{
    os_init_idle_task();
    /* Should we fire an interrupt to force context switch ? */
}

os_error_t
os_arch_os_init(void)
{
    os_error_t err = OS_OK;
/* #ifdef CONFIG_SMP */
/*     int i; */

/*     /\* Initialize global variables *\/ */

/*     for (i = 0; i < CONFIG_SMP_NCPUS; i++) { */
/*        g_current_regs[i] = NULL; */
/*     } */
/* #else */
/*     CURRENT_REGS = NULL; */
/* #endif */

    /* /\* Calibrate the timing loop *\/ */
    /* up_calibratedelay(); */

    /* /\* Add any extra memory fragments to the memory manager *\/ */
    /* xtensa_add_region(); */

    /* Initialize the interrupt subsystem */
    xtensa_irq_initialize();


    os_arch_init();

    return err;
}

uint32_t
os_arch_start(void)
{
    struct os_task *t;
    struct os_task fake_task;

    /* Get the highest priority ready to run to set the current task */
    t = os_sched_next_task();
    os_sched_set_current_task(t);

    /* Clean software interrupt, and enable it */

    /* Enable external interrupts */

    /* Intitialize and start system clock timer, this enables timer interrupts */
    os_tick_init(OS_TICKS_PER_SEC, OS_TICK_PRIO);

    /* Mark the OS as started, right before we run our first task */
    g_os_started = 1;

    /* Perform context switch */
    os_arch_ctx_sw(t);

    /* This should not be reached */
    return (uint32_t) (t->t_arg);
}

os_error_t
os_arch_os_start(void)
{
    os_error_t err;

    err = OS_ERR_IN_ISR;
    if (os_in_isr() == 0) {
        err = OS_OK;
        /* should be in kernel mode here */
        os_arch_start();
    }

    return err;
}

void
software_interrupt_handler(uintptr_t mcause)
{
}

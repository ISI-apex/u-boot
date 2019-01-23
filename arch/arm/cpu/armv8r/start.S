/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * armboot - Startup Code for ARMv8-R CPUs
 *
 * Based on startup code for ARMv7 CPUs, copyright below:
 * Copyright (c) 2004	Texas Instruments <r-woodruff2@ti.com>
 * Copyright (c) 2001	Marius Gröger <mag@sysgo.de>
 * Copyright (c) 2002	Alex Züpke <azu@sysgo.de>
 * Copyright (c) 2002	Gary Jennejohn <garyj@denx.de>
 * Copyright (c) 2003	Richard Woodruff <r-woodruff2@ti.com>
 * Copyright (c) 2003	Kshitij <kshitij@ti.com>
 * Copyright (c) 2006-2008 Syed Mohammed Khasim <x0khasim@ti.com>
 */

#include <asm-offsets.h>
#include <config.h>
#include <asm/system.h>
#include <linux/linkage.h>
#include <asm/armv7.h>

/*************************************************************************
 *
 * Startup Code (reset vector)
 *
 * Setup interrupt vectors and system registers, then switch to EL1.
 *
 *************************************************************************/

	.globl	reset
	.globl	save_boot_params_ret
	.type   save_boot_params_ret,%function
	.global	switch_to_hypervisor_ret

reset:
	/* Allow the board to save important registers */
	b	save_boot_params
save_boot_params_ret:
/*
 * check for Hypervisor support
 */
	mrc	p15, 0, r0, c0, c1, 1		@ read ID_PFR1
	and	r0, r0, #CPUID_ARM_VIRT_MASK	@ mask virtualization bits
	cmp	r0, #(1 << CPUID_ARM_VIRT_SHIFT)
	beq	switch_to_hypervisor
switch_to_hypervisor_ret:
	/*
	 * disable interrupts (FIQ and IRQ), also set the cpu to SVC32 mode,
	 * except if in HYP mode already
	 */
	mrs	r0, cpsr
	and	r1, r0, #0x1f		@ mask mode bits
	teq	r1, #0x1a		@ test for HYP mode
	bicne	r0, r0, #0x1f		@ clear all mode bits
	orrne	r0, r0, #0x13		@ set SVC mode
	orr	r0, r0, #0xc0		@ disable FIQ and IRQ
	msr	cpsr,r0
/*
 * Setup vector:
 */
	/* Set V=0 in CP15 SCTLR register - for VBAR to point to vector */
	mrc	p15, 0, r0, c1, c0, 0	@ Read CP15 SCTLR Register
	bic	r0, #CR_V		@ V = 0
	mcr	p15, 0, r0, c1, c0, 0	@ Write CP15 SCTLR Register

#ifdef CONFIG_HAS_VBAR
	/* Set vector address in CP15 VBAR register */
	ldr	r0, =_start
	mcr	p15, 0, r0, c12, c0, 0	@Set VBAR
#endif

/* Check if ARM Generic Timer is present, and if so, set CNTFRQ */
#ifdef COUNTER_FREQUENCY
	mrc	p15, 0, r0, c0, c1, 1		@ read ID_PFR1
	and	r0, r0, #CPUID_ARM_GENTIMER_MASK	@ mask arch timer bits
	cmp	r0, #(1 << CPUID_ARM_GENTIMER_SHIFT)
	ldreq	r1, =COUNTER_FREQUENCY
	mcreq	p15, 0, r1, c14, c0, 0		@ write CNTFRQ
#endif /* COUNTER_FREQUENCY */

	/* Enable EL1 access to all IMP DEF registers */
	LDR r0, =0x7F81
	DSB
	MCR p15, 4, r0, c1, c0, 1       /* Write to HACTLR */
	ISB

	bl	armv8r_switch_to_el1
NEXT:
	bl	_main

/*------------------------------------------------------------------------------*/

ENTRY(c_runtime_cpu_setup)
/*
 * If I-cache is enabled invalidate it
 */
#ifndef CONFIG_SYS_ICACHE_OFF
	mcr	p15, 0, r0, c7, c5, 0	@ invalidate icache
#	mcr     p15, 0, r0, c7, c10, 4	@ DSB
	DSB
#	mcr     p15, 0, r0, c7, c5, 4	@ ISB
	ISB
#endif

	bx	lr

ENDPROC(c_runtime_cpu_setup)

/*************************************************************************
 *
 * void save_boot_params(u32 r0, u32 r1, u32 r2, u32 r3)
 *	__attribute__((weak));
 *
 * Stack pointer is not yet initialized at this moment
 * Don't save anything to stack even if compiled with -O0
 *
 *************************************************************************/
ENTRY(save_boot_params)
	b	save_boot_params_ret		@ back to my caller
ENDPROC(save_boot_params)
	.weak	save_boot_params

ENTRY(switch_to_hypervisor)
	b	switch_to_hypervisor_ret
ENDPROC(switch_to_hypervisor)
	.weak	switch_to_hypervisor

ENTRY(armv8r_switch_to_el1)
	MRS	r0, cpsr
	MOV	r1, #0x13
	BFI	r0, r1, #0, #5
	MSR	spsr_hyp, r0
	LDR	r0, =NEXT
	MSR 	elr_hyp, r0
	DSB
	ISB
	ERET
ENDPROC(armv8r_switch_to_el1)

/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Configuration for HPSC HPPS.
 */

#ifndef __HPSC_RTPS_A53_H
#define __HPSC_RTPS_A53_H

/* #define CONFIG_ARMV8_SWITCH_TO_EL1 */

/* Generic Interrupt Controller Definitions */
#define CONFIG_GICV3
#define GICD_BASE	0x30c00000
#define GICR_BASE	0x30c40000

#define CONFIG_SYS_INIT_SP_ADDR		CONFIG_SYS_TEXT_BASE

/* Generic Timer Definitions - setup in EL3. Setup by ATF for other cases */
#if !defined(COUNTER_FREQUENCY)
# define COUNTER_FREQUENCY		100000000
#endif

/* Size of malloc() pool */
#define CONFIG_SYS_MALLOC_LEN		(CONFIG_ENV_SIZE + 0x2000000)

/* Serial setup */
#define CONFIG_ARM_DCC
#define CONFIG_CPU_ARMV8

/* Only exact multiples of CONFIG_DEBUG_UART_CLOCK -- 16 MHz in Zebu emulator */
#define CONFIG_SYS_BAUDRATE_TABLE { 125000, 500000 }

/* BOOTP options */
#define CONFIG_BOOTP_BOOTFILESIZE
#define CONFIG_BOOTP_MAY_FAIL

/* Miscellaneous configurable options */
#define CONFIG_SYS_LOAD_ADDR		0x0 /* should not be used at runtime */

/* Monitor Command Prompt */
/* Console I/O Buffer Size */
#define CONFIG_SYS_CBSIZE		2048
#define CONFIG_SYS_BARGSIZE		CONFIG_SYS_CBSIZE
#define CONFIG_PANIC_HANG
#define CONFIG_SYS_MAXARGS		64

#define CONFIG_SYS_BOOTM_LEN	(60 * 1024 * 1024)

#define CONFIG_CLOCKS

#define CONFIG_BOARD_EARLY_INIT_F

#endif /* __HPSC_RTPS_A53_H */

/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Configuration for HPSC HPPS.
 */

#ifndef __HPSC_HPPS_H
#define __HPSC_HPPS_H

#include <configs/hpsc.h>

/* #define CONFIG_ARMV8_SWITCH_TO_EL1 */

/* Generic Interrupt Controller Definitions */
#define CONFIG_GICV3
#define GICD_BASE	0xF9000000
#define GICR_BASE	0xF9100000

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
#define CONFIG_SYS_NS16550_REG_SIZE (-4) /* Synopsis DWC UART has 32-bit regs */

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

/* initrd_high=~0 means do not relocate ramdisk */
#ifndef CONFIG_EXTRA_ENV_SETTINGS
#define CONFIG_EXTRA_ENV_SETTINGS \
	"fdtcontroladdr=0x8005d000\0" \
	"kernel_addr=0x80064000\0" \
	"initrd_addr=0x84000000\0" \
        "initrd_high=0xffffffffffffffff\0" \
	"fdt_addr=0x80060000\0" \
        "bootargs=earlycon=uart8250,mmio32,0xF92C0000,500000n8 console=ttyS0,500000n8 loglevel=8\0" \
        "memboot=bootm $kernel_addr $initrd_addr $fdt_addr\0"
#endif

#define CONFIG_BOARD_EARLY_INIT_F

#endif /* __HPSC_HPPS_H */

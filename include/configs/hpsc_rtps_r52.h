/* SPDX-License-Identifier: GPL-2.0 */
/*
 * (C) Copyright 2018 Xilinx, Inc. (Michal Simek)
 */

#ifndef __CONFIG_RTPS_R52_H
#define __CONFIG_RTPS_R52_H

#define CONFIG_EXTRA_ENV_SETTINGS

#define CONFIG_REMAKE_ELF

/* CPU clock */
#define CONFIG_CPU_FREQ_HZ	500000000

/* System Counter (aka. Elapsed Timer): backend to ARM Generic Timers */
#define COUNTER_FREQUENCY       125000000

/* Serial drivers */
/* The following table includes the supported baudrates */
#define CONFIG_SYS_BAUDRATE_TABLE  { 125000 }

# define CONFIG_ENV_SIZE	(128 << 10)

/* Allow to overwrite serial and ethaddr */
#define CONFIG_ENV_OVERWRITE

/* Boot configuration */
#define CONFIG_SYS_LOAD_ADDR		0 /* default? */

#define CONFIG_SYS_MAXARGS		32 /* max number of command args */

#define CONFIG_SYS_MALLOC_LEN		0x1400000

/* #define CONFIG_SYS_INIT_RAM_ADDR	0xFFFF0000 */
#define CONFIG_SYS_INIT_RAM_ADDR	0x7FFF0000
#define CONFIG_SYS_INIT_RAM_SIZE	0x1000
#define CONFIG_SYS_INIT_SP_ADDR		(CONFIG_SYS_INIT_RAM_ADDR + \
					CONFIG_SYS_INIT_RAM_SIZE - \
					GENERATED_GBL_DATA_SIZE)
/*#define	CONFIG_STACKBASE		0x80000000 */

/* Extend size of kernel image for uncompression */
#define CONFIG_SYS_BOOTM_LEN	(60 * 1024 * 1024)

#define CONFIG_SYS_UBOOT_START	CONFIG_SYS_TEXT_BASE

#define CONFIG_SKIP_LOWLEVEL_INIT

/* 0x0 - 0x40 is used for placing exception vectors */
#define CONFIG_SYS_MEMTEST_START	0x40
#define CONFIG_SYS_MEMTEST_END		0x100
#define CONFIG_SYS_MEMTEST_SCRATCH	0

#endif /* __CONFIG_ZYNQ_RTPS_R52_H */
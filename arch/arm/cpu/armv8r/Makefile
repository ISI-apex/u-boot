# SPDX-License-Identifier: GPL-2.0+
#
# (C) Copyright 2000-2003
# Wolfgang Denk, DENX Software Engineering, wd@denx.de.

extra-y	:= start.o

obj-y	+= cache_v7.o cache_v7_asm.o

obj-y	+= cpu.o cp15.o
obj-y	+= syslib.o

obj-$(CONFIG_SYS_ARM_MPU) += mpu_v7r.o

ifneq ($(CONFIG_SKIP_LOWLEVEL_INIT),y)
obj-y	+= lowlevel_init.o
endif

obj-$(CONFIG_SYS_ARCH_TIMER) += arch_timer.o

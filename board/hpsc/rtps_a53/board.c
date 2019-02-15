// SPDX-License-Identifier: GPL-2.0+

#include <common.h>
#include <asm/arch/sys_proto.h>
#include <dm/device.h>
#include <cpu_func.h>

DECLARE_GLOBAL_DATA_PTR;

int checkboard(void)
{
	puts("Board: HPSC RTPS A53\n");
	return 0;
}

int board_early_init_f(void)
{
	int ret = 0;
	return ret;
}

int board_init(void)
{
	printf("EL Level:\tEL%d\n", current_el());
	return 0;
}

int board_early_init_r(void)
{
	return 0;
}

int board_late_init(void)
{
	return 0;
}

unsigned long do_go_exec(ulong (*entry)(int, char * const []), int argc,
			 char * const argv[])
{
	int ret = 0;

	if (current_el() > 1) {
		smp_kick_all_cpus();
		dcache_disable();
		armv8_switch_to_el1(0x0, 0, 0, 0, (unsigned long)entry,
				    ES_TO_AARCH64);
	} else {
		printf("FAIL: current EL is not above EL1\n");
		ret = EINVAL;
	}
	return ret;
}

int dram_init_banksize(void)
{
	int ret;

	ret = fdtdec_setup_memory_banksize();
	if (ret)
		return ret;

	mem_map_fill();

	return 0;
}

int dram_init(void)
{
	if (fdtdec_setup_mem_size_base() != 0)
		return -EINVAL;

	return 0;
}

void reset_cpu(ulong addr)
{
}

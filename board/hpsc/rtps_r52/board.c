// SPDX-License-Identifier: GPL-2.0

#include <common.h>
#include <fdtdec.h>
#include <asm/arch/sys_proto.h>

int board_init(void)
{
	return 0;
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

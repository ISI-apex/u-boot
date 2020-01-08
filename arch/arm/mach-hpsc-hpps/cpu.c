// SPDX-License-Identifier: GPL-2.0+

#include <common.h>
#include <asm/armv8/mmu.h>

DECLARE_GLOBAL_DATA_PTR;

#define HPSC

/*
 * Number of filled static entries and also the first empty
 * slot in hpps_mem_map.
 */
#define HPPS_MEM_MAP_USED	6

/* +1 is end of list which needs to be empty */
#define HPPS_MEM_MAP_MAX (HPPS_MEM_MAP_USED + CONFIG_NR_DRAM_BANKS + 1)

static struct mm_region hpps_mem_map[HPPS_MEM_MAP_MAX] = {
	{
		.virt = 0x80000000UL,
		.phys = 0x80000000UL,
		.size = 0x40000000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_NORMAL) |
			 PTE_BLOCK_INNER_SHARE
	},
	{
		.virt = 0x100000000UL,
		.phys = 0x100000000UL,
		.size = 0x80000000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_NORMAL) |
			 PTE_BLOCK_INNER_SHARE
	},
	{
		.virt = 0x200000000UL,
		.phys = 0x200000000UL,
		.size = 0x200000000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_NORMAL) |
			 PTE_BLOCK_INNER_SHARE
	},
	{
		.virt = 0xC0000000UL,
		.phys = 0xC0000000UL,
		.size = 0x30000000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_DEVICE_NGNRNE) |
			 PTE_BLOCK_NON_SHARE |
			 PTE_BLOCK_PXN | PTE_BLOCK_UXN
	},
	{
		.virt = 0xf8000000UL,
		.phys = 0xf8000000UL,
		.size = 0x07e00000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_DEVICE_NGNRNE) |
			 PTE_BLOCK_NON_SHARE |
			 PTE_BLOCK_PXN | PTE_BLOCK_UXN
	}, {
		.virt = 0x400000000UL,
		.phys = 0x400000000UL,
		.size = 0x400000000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_DEVICE_NGNRNE) |
			 PTE_BLOCK_NON_SHARE |
			 PTE_BLOCK_PXN | PTE_BLOCK_UXN
	},
	{
		.virt = 0x1000000000UL,
		.phys = 0x1000000000UL,
		.size = 0xf000000000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_DEVICE_NGNRNE) |
			 PTE_BLOCK_NON_SHARE |
			 PTE_BLOCK_PXN | PTE_BLOCK_UXN
	},
	{
		.virt = 0,
		.phys = 0,
		.size = 0,
		.attrs = 0
	}
};

/* Called from code in board/ */
void mem_map_fill(void)
{
	int banks = HPPS_MEM_MAP_USED;

#ifdef HPSC
	return ;
#endif

	for (int i = 0; i < CONFIG_NR_DRAM_BANKS; i++) {
		/* Zero size means no more DDR that's this is end */
		if (!gd->bd->bi_dram[i].size)
			break;

		hpps_mem_map[banks].virt = gd->bd->bi_dram[i].start;
		hpps_mem_map[banks].phys = gd->bd->bi_dram[i].start;
		hpps_mem_map[banks].size = gd->bd->bi_dram[i].size;
		hpps_mem_map[banks].attrs = PTE_BLOCK_MEMTYPE(MT_NORMAL) |
			PTE_BLOCK_INNER_SHARE;
		banks = banks + 1;
	}
}

struct mm_region *mem_map = hpps_mem_map;


#ifdef CONFIG_CLOCKS
/**
 * set_cpu_clk_info() - Initialize clock framework
 * Always returns zero.
 *
 * This function is called from common code after relocation and sets up the
 * clock framework. The framework must not be used before this function had been
 * called.
 */
int set_cpu_clk_info(void)
{
	gd->cpu_clk = get_tbclk();
	gd->bd->bi_arm_freq = gd->cpu_clk / 1000000;
	gd->bd->bi_dsp_freq = 0;
	return 0;
}
#endif

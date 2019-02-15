// SPDX-License-Identifier: GPL-2.0+

#include <common.h>
#include <asm/armv8/mmu.h>

DECLARE_GLOBAL_DATA_PTR;

/* Helpers for printf'ing 64-bit values (%016llx breaks) */
#define U64_FMT "0x%08lx%08lx"
#define U64_ARG(x) (unsigned long)((x) >> 32), (unsigned long)(x)

/* Number of filled static entries in rtps_a53_mem_map. */
#define MEM_MAP_STATIC	2

/* +1 is end of list which needs to be empty */
#define MEM_MAP_MAX (MEM_MAP_STATIC + CONFIG_NR_DRAM_BANKS + 1)

/* If you add/remove entries, make sure to update MEM_MAP_STATIC */
static struct mm_region rtps_a53_mem_map[MEM_MAP_MAX] = {
	{ /* LSIO peripherals */
		.virt = 0x30000000UL,
		.phys = 0x30000000UL,
		.size = 0x10000000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_DEVICE_NGNRNE) |
			 PTE_BLOCK_NON_SHARE |
			 PTE_BLOCK_PXN | PTE_BLOCK_UXN
	},
	{ /* HPPS peripherals */
		.virt = 0xf9000000UL,
		.phys = 0xf9000000UL,
		.size = 0x07000000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_DEVICE_NGNRNE) |
			 PTE_BLOCK_NON_SHARE |
			 PTE_BLOCK_PXN | PTE_BLOCK_UXN
	},
	/* Variable number of entries for DRAM j start here, at index
	 * MEM_MAP_STATIC, when DRAM bank entries are populated in
	 * mem_map_fill, the list is terminated by an entry with all zeroes. */
};

struct mm_region *mem_map = rtps_a53_mem_map;

/* Called from code in board/ */
void mem_map_fill(void)
{
	int i, j;
	for (i = 0, j = MEM_MAP_STATIC; i < CONFIG_NR_DRAM_BANKS; i++, j++) {
		/* Zero size means no more DDR that's this is end */
		if (!gd->bd->bi_dram[i].size)
			break;
		rtps_a53_mem_map[j].virt = gd->bd->bi_dram[i].start;
		rtps_a53_mem_map[j].phys = gd->bd->bi_dram[i].start;
		rtps_a53_mem_map[j].size = gd->bd->bi_dram[i].size;
		rtps_a53_mem_map[j].attrs = PTE_BLOCK_MEMTYPE(MT_NORMAL) |
			PTE_BLOCK_INNER_SHARE;
	}
	/* Write terminator entry for the variable-length list */
	assert(j < MEM_MAP_MAX);
	rtps_a53_mem_map[j].virt = 0;
	rtps_a53_mem_map[j].phys = 0;
	rtps_a53_mem_map[j].size = 0;
	rtps_a53_mem_map[j].attrs = 0;

#ifdef DEBUG
	debug("\nMMU map (virt -> phys, size):\n");
	for (j = 0; j < MEM_MAP_MAX && rtps_a53_mem_map[j].size; j++) {
		/* breaks for 64-bit %llx (incl. breaking later execution) */
		debug(U64_FMT " -> " U64_FMT " sz " U64_FMT "\n",
			U64_ARG(rtps_a53_mem_map[j].virt),
			U64_ARG(rtps_a53_mem_map[j].phys),
			U64_ARG(rtps_a53_mem_map[j].size));
	}
#endif /* DEBUG */
}

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

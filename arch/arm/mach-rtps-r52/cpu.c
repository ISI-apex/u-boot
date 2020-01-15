// SPDX-License-Identifier: GPL-2.0+

#include <common.h>
#include <asm/armv8/armv8_mpu.h>

DECLARE_GLOBAL_DATA_PTR;

#define DRAM_BANK_REGION 0 /* index */

struct mpu_region_config el2_region_config[] = {
	/* DRAM filled based on Device Tree */
	[DRAM_BANK_REGION] = { 		0x0,		 0x0, REGION_0,
	  XN_DIS, PRIV_RW_USR_RW, SH_NONE, AttrIndex0, ENABLE },

	{ 0x28000000, 0x307FFFC0, REGION_1, XN_EN, PRIV_RW_USR_RW,	/* LSIO */
	    SH_NONE, AttrIndex1, ENABLE,
 	  },
	{ 0x30800000, 0x3FFFFFC0, REGION_2, XN_EN, PRIV_RW_USR_RW,	/* RTPS devices */
	    SH_NONE, AttrIndex1, ENABLE,
 	  },
	{ 0x80000000, 0xBFFFFFC0, REGION_3, XN_DIS, PRIV_RW_USR_RW,	/* HPPS DDR-Low 1 & 2 */
	    SH_NONE, AttrIndex0, ENABLE,
	  },
	{ 0xC0000000, 0xDFFFFFC0, REGION_4, XN_DIS, PRIV_RW_USR_RW,	/* Window to 40bit */
	    SH_NONE, AttrIndex0, ENABLE,
	  },
	{ 0xE0100000, 0xE2FFFFC0, REGION_5, XN_EN, PRIV_RW_USR_RW,	/* HPPS Device */
	    SH_NONE, AttrIndex1, ENABLE,
	  },
	{ 0xE3000000, 0xF7FFFFC0, REGION_6, XN_EN, PRIV_RW_USR_RW,	/* HSIO */
	    SH_NONE, AttrIndex1, ENABLE,
	  },
	{ 0xF8000000, 0xFFFFFFC0, REGION_7, XN_EN, PRIV_RW_USR_RW,	/* HPSC Device 2*/
	    SH_NONE, AttrIndex1, ENABLE,
 	  },
	{ 0x00000000, 0x600000, REGION_8, XN_DIS, PRIV_RW_USR_RW,	/* TCMS - need to revisit */
	    SH_NONE, AttrIndex1, ENABLE,
 	  },
};
struct mpu_region_config el1_region_config[] = {
	/* DRAM filled based on Device Tree */
	[DRAM_BANK_REGION] = { 0x0, 0x0, REGION_0,
		XN_DIS, PRIV_RW_USR_RW, SH_NONE, AttrIndex0, ENABLE },

	{ 0x00000000, 0x300000, REGION_1, XN_DIS, PRIV_RW_USR_RW,	/* TCMS */
	    SH_NONE, AttrIndex1, ENABLE,
 	  },
	{ 0x28000000, 0x3FFFFFC0, REGION_2, XN_EN, PRIV_RW_USR_RW,	/* LSIO + RTPS devices */
	    SH_NONE, AttrIndex1, ENABLE,
 	  },
	{ 0xF8000000, 0xFFFFFFC0, REGION_7, XN_EN, PRIV_RW_USR_RW,	/* HPSC Device 2*/
	    SH_NONE, AttrIndex1, ENABLE,
 	  },
};

/* Called from code in board/ */
void mem_map_fill(void)
{
#ifdef DEBUG
	int i;
#endif
	bool hyp = is_hyp();
	struct mpu_region_config *regions =
		hyp ? el2_region_config : el1_region_config;
	int num_regions = hyp ?
		sizeof(el2_region_config) / sizeof(struct mpu_region_config) :
		sizeof(el1_region_config) / sizeof(struct mpu_region_config);

#if CONFIG_NR_DRAM_BANKS != 1
#error This code supports only one DRAM bank.
#endif
	regions[DRAM_BANK_REGION].start_addr = gd->bd->bi_dram[0].start;
	regions[DRAM_BANK_REGION].end_addr = /* last 6 bits zero */
		(gd->bd->bi_dram[0].start + gd->bd->bi_dram[0].size) - 64;

#ifdef DEBUG
	debug("\nMPU regions (start:end):\n");
	for (i = 0; i < num_regions; i++) {
		debug("0x%08x:0x%08x\n", regions[i].start_addr, regions[i].end_addr);
	}
#endif /* DEBUG */

	setup_mpu_regions(regions, num_regions);
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
#endif /* CONFIG_CLOCKS */

/*
 * Perform the low-level reset.
 */
void reset_cpu(ulong addr)
{
	while (1)
		;
}

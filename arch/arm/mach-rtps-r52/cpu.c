// 
/*
 */

#include <common.h>
#include <asm/armv8/armv8_mpu.h>

DECLARE_GLOBAL_DATA_PTR;

struct mpu_region_config el2_region_config[] = {
	{ 0x28000000, 0x30800000, REGION_0, XN_EN, PRIV_RW_USR_RW,	/* LSIO */
	    SH_NONE, AttrIndex0, ENABLE,
 	  },
	{ 0x30800000, 0x38000000, REGION_1, XN_DIS, PRIV_RW_USR_RW,	/* Unused: it can be used to map TCMs */
	    SH_NONE, AttrIndex0, ENABLE,
 	  },
	{ 0x38000000, 0x40000000, REGION_2, XN_EN, PRIV_RW_USR_RW,	/* RTPS devices */
	    SH_NONE, AttrIndex0, ENABLE,
 	  },
	{ 0x40000000, 0x60000000, REGION_3, XN_DIS, PRIV_RW_USR_RW,	/* RTPS DDR-Low 1 */
	    SH_NONE, AttrIndex0, ENABLE,
	  },
	{ 0x60000000, 0x80000000, REGION_4, XN_DIS, PRIV_RW_USR_RW,	/* RTPS DDR-Low 2 */
	    SH_NONE, AttrIndex0, ENABLE,
	  },
	{ 0x80000000, 0xA0000000, REGION_5, XN_EN, PRIV_RW_USR_RW,	/* HPPS DDR-Low 1 */
	    SH_NONE, AttrIndex0, ENABLE,
	  },
	{ 0xA0000000, 0xC0000000, REGION_6, XN_EN, PRIV_RW_USR_RW,	/* HPPS DDR-Low 2 */
	    SH_NONE, AttrIndex0, ENABLE,
	  },
	{ 0xC0000000, 0xE0000000, REGION_7, XN_EN, PRIV_RW_USR_RW,	/* TRCH/RTPS to 40 bit */
	    SH_NONE, AttrIndex0, ENABLE,
	  },
};
struct mpu_region_config el1_region_config[] = {
	{ 0x28000000, 0x30800000, REGION_0, XN_EN, PRIV_RW_USR_RW,	/* LSIO */
	    SH_NONE, AttrIndex0, ENABLE,
 	  },
	{ 0x30800000, 0x38000000, REGION_1, XN_DIS, PRIV_RW_USR_RW,	/* Unused: it can be used to map TCMs */
	    SH_NONE, AttrIndex0, ENABLE,
 	  },
	{ 0x38000000, 0x40000000, REGION_2, XN_EN, PRIV_RW_USR_RW,	/* RTPS devices */
	    SH_NONE, AttrIndex0, ENABLE,
 	  },
	{ 0x40000000, 0x60000000, REGION_3, XN_DIS, PRIV_RW_USR_RW,	/* RTPS DDR-Low 1 */
	    SH_NONE, AttrIndex0, ENABLE,
	  },
	{ 0x60000000, 0x80000000, REGION_4, XN_DIS, PRIV_RW_USR_RW,	/* RTPS DDR-Low 2 */
	    SH_NONE, AttrIndex0, ENABLE,
	  },
	{ 0x80000000, 0xA0000000, REGION_5, XN_EN, PRIV_RW_USR_RW,	/* HPPS DDR-Low 1 */
	    SH_NONE, AttrIndex0, ENABLE,
	  },
	{ 0xA0000000, 0xC0000000, REGION_6, XN_EN, PRIV_RW_USR_RW,	/* HPPS DDR-Low 2 */
	    SH_NONE, AttrIndex0, ENABLE,
	  },
	{ 0xC0000000, 0xE0000000, REGION_7, XN_EN, PRIV_RW_USR_RW,	/* TRCH/RTPS to 40 bit */
	    SH_NONE, AttrIndex0, ENABLE,
	  },
};

int arch_cpu_init(void)
{
	gd->cpu_clk = CONFIG_CPU_FREQ_HZ;

	setup_mpu_regions((is_hyp() ? el2_region_config : el1_region_config), sizeof(is_hyp()? el2_region_config : el1_region_config) /
			  sizeof(struct mpu_region_config));

	return 0;
}

/*
 * Perform the low-level reset.
 */
void reset_cpu(ulong addr)
{
	while (1)
		;
}

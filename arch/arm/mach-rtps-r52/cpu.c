// 
/*
 */

#include <common.h>
#include <asm/armv8/armv8_mpu.h>

DECLARE_GLOBAL_DATA_PTR;

struct mpu_region_config el2_region_config[] = {
	{ 0x00000000, 0x40000000, REGION_0, XN_DIS, PRIV_RW_USR_RW,
	    SH_NONE, AttrIndex0, ENABLE,
	  },
	{ 0x20000000, 0x40000000, REGION_1, XN_EN, PRIV_RO_USR_RO,
	    SH_NONE, AttrIndex0, ENABLE,
 	  },
	{ 0x40000000, 0x80000000, REGION_2, XN_EN, PRIV_RO_USR_RO,
	    SH_NONE, AttrIndex0, ENABLE,
	  },
	{ 0xF0000000, 0xFFFFFF00, REGION_3, XN_EN, PRIV_RW_USR_NO,
	    SH_NONE, AttrIndex0, ENABLE,
	  },
};
struct mpu_region_config el1_region_config[] = {
	{ 0x00000000, 0x40000000, REGION_0, XN_DIS, PRIV_RW_USR_RW,
	    SH_NONE, AttrIndex0, ENABLE,
	  },
	{ 0x20000000, 0x40000000, REGION_1, XN_EN, PRIV_RO_USR_RO,
	    SH_NONE, AttrIndex0, ENABLE,
 	  },
	{ 0x40000000, 0x80000000, REGION_2, XN_EN, PRIV_RO_USR_RO,
	    SH_NONE, AttrIndex0, ENABLE,
	  },
	{ 0xF0000000, 0xFFFFFF00, REGION_3, XN_EN, PRIV_RW_USR_NO,
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

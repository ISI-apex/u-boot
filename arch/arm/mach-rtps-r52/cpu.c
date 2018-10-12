// 
/*
 */

#include <common.h>
#include <asm/armv8/armv8_mpu.h>

DECLARE_GLOBAL_DATA_PTR;

struct mpu_region_config el2_region_config[] = {
	{ 0x28000000, 0x307FFFC0, REGION_0, XN_EN, PRIV_RW_USR_RW,	/* LSIO */
	    SH_NONE, AttrIndex1, ENABLE,
 	  },
	{ 0x30800000, 0x3FFFFFC0, REGION_1, XN_EN, PRIV_RW_USR_RW,	/* RTPS devices */
	    SH_NONE, AttrIndex1, ENABLE,
 	  },
	{ 0x40000000, 0x7FFFFFC0, REGION_2, XN_DIS, PRIV_RW_USR_RW,	/* RTPS DDR-Low 1 & 2*/
	    SH_NONE, AttrIndex0, ENABLE,
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
};
struct mpu_region_config el1_region_config[] = {
	{ 0x28000000, 0x307FFFC0, REGION_0, XN_EN, PRIV_RW_USR_RW,	/* LSIO */
	    SH_NONE, AttrIndex1, ENABLE,
 	  },
	{ 0x30800000, 0x3FFFFFC0, REGION_1, XN_EN, PRIV_RW_USR_RW,	/* RTPS devices */
	    SH_NONE, AttrIndex1, ENABLE,
 	  },
	{ 0x40000000, 0x7FFFFFC0, REGION_2, XN_DIS, PRIV_RW_USR_RW,	/* RTPS DDR-Low 1 & 2*/
	    SH_NONE, AttrIndex0, ENABLE,
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
};

int arch_cpu_init(void)
{
	gd->cpu_clk = CONFIG_CPU_FREQ_HZ;
#ifdef ORG
	setup_mpu_regions((is_hyp() ? el2_region_config : el1_region_config), sizeof(is_hyp()? el2_region_config : el1_region_config) /
			  sizeof(struct mpu_region_config));
#else
	if (is_hyp()) {
		setup_mpu_regions(el2_region_config, sizeof(el2_region_config) / sizeof(struct mpu_region_config));
	} else {
		setup_mpu_regions(el1_region_config, sizeof(el1_region_config) / sizeof(struct mpu_region_config));
	}
#endif

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

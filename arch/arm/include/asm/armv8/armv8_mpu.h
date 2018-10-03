/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (C) 2017, STMicroelectronics - All Rights Reserved
 * Author(s): Vikas Manocha, <vikas.manocha@st.com> for STMicroelectronics.
 */

#ifndef _ASM_ARMV8_MPU_H
#define _ASM_ARMV8_MPU_H

/* [H]PRBAR */
#define XN_SHIFT			0
#define AP_SHIFT			1
#define S_SHIFT				3
#define C_SHIFT				1

/* [H]PRLAR */
#define REGION_ADDR_SHIFT		6
#define ATTR_SHIFT			1
#define EN_SHIFT			0
#define Non_Shareable			0b00	// SH[1:0]
#define	Outer_Shareable			0b10
#define Inner_Shareable			0b11

enum region_number {
	REGION_0 = 0,
	REGION_1,
	REGION_2,
	REGION_3,
	REGION_4,
	REGION_5,
	REGION_6,
	REGION_7,
	REGION_8,
	REGION_9,
	REGION_10,
	REGION_11,
	REGION_12,
	REGION_13,
	REGION_14,
	REGION_15,
};

// Protection BASE Address Register
enum ap {	// AP[2:1]
	PRIV_RW_USR_NO = 0,
	PRIV_RW_USR_RW,
	PRIV_RO_USR_NO,
	PRIV_RO_USR_RO,
};

enum sh {	// SH[1:0]
	SH_NONE = 0,
	SH_UNPRED,
	SH_OUTER,
	SH_INNTER,
};

enum xn {	// Bit 0
	XN_DIS = 0,
	XN_EN,
};

// Protection Limite Address Register

enum mr_attr {		// AttrIndx[2:0]
	AttrIndex0 = 0,
	AttrIndex1,
	AttrIndex2,
	AttrIndex3,
	AttrIndex4,
	AttrIndex5,
	AttrIndex6,
	AttrIndex7,
};

enum mpu_enable {
	DISABLE = 0,
	ENABLE, 
};

struct mpu_region_config {
	uint32_t start_addr;
	uint32_t end_addr;
	enum region_number region_no;
	enum xn xn;
	enum ap ap;
	enum sh sh;
	enum mr_attr attr;
	enum mpu_enable enable;
};

void disable_mpu(void);
void enable_mpu(void);
int mpu_enabled(void);
void mpu_config(struct mpu_region_config *reg_config);
void setup_mpu_regions(struct mpu_region_config *rgns, u32 num_rgns);

#endif /* _ASM_ARMV8_MPU_H */

// SPDX-License-Identifier: GPL-2.0+
/*
 * Cortex-R Memory Protection Unit specific code
 *
 * Copyright (C) 2018 Texas Instruments Incorporated - http://www.ti.com/
 *	Lokesh Vutla <lokeshvutla@ti.com>
 */

#include <common.h>
#include <command.h>
#include <asm/armv7.h>
#include <asm/system.h>
#include <asm/barriers.h>
#include <linux/compiler.h>

#include <asm/armv8/armv8_mpu.h>

/* MPU Type register definitions */
#define MPUIR_S_SHIFT		0
#define MPUIR_S_MASK		BIT(MPUIR_S_SHIFT)
#define MPUIR_DREGION_SHIFT	8
#define MPUIR_DREGION_MASK	(0xff << 8)

/**
 * Note:
 * The Memory Protection Unit(MPU) allows to partition memory into regions
 * and set individual protection attributes for each region. In absence
 * of MPU a default map[1] will take effect. make sure to run this code
 * from a region which has execution permissions by default.
 * [1] http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0460d/I1002400.html
 */

void disable_mpu(void)
{
	u32 reg;

	reg = get_cr();
	reg &= ~CR_M;
	dsb();
	set_cr(reg);
	isb();
}

void enable_mpu(void)
{
	u32 reg;

	reg = get_cr();
	reg |= CR_M;
	dsb();
	set_cr(reg);
	isb();
}

int mpu_enabled(void)
{
	return get_cr() & CR_M;
}

void mpu_config(struct mpu_region_config *rgn)
{
	uint32_t bar, lar;

	/* MPU Region Size and Enable Register */
	if (rgn->start_addr < rgn->end_addr) {
		bar = (rgn->start_addr & ~(REGION_ADDR_SHIFT - 1)) | (rgn->sh << S_SHIFT) | (rgn->ap << AP_SHIFT) | (rgn->xn << XN_SHIFT);
		lar = (rgn->end_addr & ~(REGION_ADDR_SHIFT - 1)) | (rgn->attr << ATTR_SHIFT) | (rgn->enable << EN_SHIFT);
	} else {
		bar = 0;
		lar = 0 << EN_SHIFT;
	}

	switch (rgn->region_no) {
	/* [H]PRBAR */
	/* [H]PRLAR */
	  case 0:
	    if (!is_hyp()) {
		asm volatile ("mcr p15, 0, %0, c6, c8, 0" : : "r" (bar));
		asm volatile ("mcr p15, 0, %0, c6, c8, 1" : : "r" (lar));
	    }
	    else {
		asm volatile ("mcr p15, 4, %0, c6, c8, 0" : : "r" (bar));
		asm volatile ("mcr p15, 4, %0, c6, c8, 1" : : "r" (lar));
	    }
	    break;
	  case 1:
	    if (!is_hyp()) {
		asm volatile ("mcr p15, 0, %0, c6, c8, 4" : : "r" (bar));
		asm volatile ("mcr p15, 0, %0, c6, c8, 5" : : "r" (lar));
	    }
	    else {
		asm volatile ("mcr p15, 4, %0, c6, c8, 4" : : "r" (bar));
		asm volatile ("mcr p15, 4, %0, c6, c8, 5" : : "r" (lar));
	    }
	    break;
	  case 2:
	    if (!is_hyp()) {
		asm volatile ("mcr p15, 0, %0, c6, c9, 0" : : "r" (bar));
		asm volatile ("mcr p15, 0, %0, c6, c9, 1" : : "r" (lar));
	    }
	    else {
		asm volatile ("mcr p15, 4, %0, c6, c9, 0" : : "r" (bar));
		asm volatile ("mcr p15, 4, %0, c6, c9, 1" : : "r" (lar));
	    }
	    break;
	  case 3:
	    if (!is_hyp()) {
		asm volatile ("mcr p15, 0, %0, c6, c9, 4" : : "r" (bar));
		asm volatile ("mcr p15, 0, %0, c6, c9, 5" : : "r" (lar));
	    }
	    else {
		asm volatile ("mcr p15, 4, %0, c6, c9, 4" : : "r" (bar));
		asm volatile ("mcr p15, 4, %0, c6, c9, 5" : : "r" (lar));
	    }
	    break;
	  case 4:
	    if (!is_hyp()) {
		asm volatile ("mcr p15, 0, %0, c6, c10, 0" : : "r" (bar));
		asm volatile ("mcr p15, 0, %0, c6, c10, 1" : : "r" (lar));
	    }
	    else {
		asm volatile ("mcr p15, 4, %0, c6, c10, 0" : : "r" (bar));
		asm volatile ("mcr p15, 4, %0, c6, c10, 1" : : "r" (lar));
	    }
	    break;
	  case 5:
	    if (!is_hyp()) {
		asm volatile ("mcr p15, 0, %0, c6, c10, 4" : : "r" (bar));
		asm volatile ("mcr p15, 0, %0, c6, c10, 5" : : "r" (lar));
	    }
	    else {
		asm volatile ("mcr p15, 4, %0, c6, c10, 4" : : "r" (bar));
		asm volatile ("mcr p15, 4, %0, c6, c10, 5" : : "r" (lar));
	    }
	    break;
	  case 6:
	    if (!is_hyp()) {
		asm volatile ("mcr p15, 0, %0, c6, c11, 0" : : "r" (bar));
		asm volatile ("mcr p15, 0, %0, c6, c11, 1" : : "r" (lar));
	    }
	    else {
		asm volatile ("mcr p15, 4, %0, c6, c11, 0" : : "r" (bar));
		asm volatile ("mcr p15, 4, %0, c6, c11, 1" : : "r" (lar));
	    }
	    break;
	  case 7:
	    if (!is_hyp()) {
		asm volatile ("mcr p15, 0, %0, c6, c11, 4" : : "r" (bar));
		asm volatile ("mcr p15, 0, %0, c6, c11, 5" : : "r" (lar));
	    }
	    else {
		asm volatile ("mcr p15, 4, %0, c6, c11, 4" : : "r" (bar));
		asm volatile ("mcr p15, 4, %0, c6, c11, 5" : : "r" (lar));
	    }
	    break;
	  case 8:
	    if (!is_hyp()) {
		asm volatile ("mcr p15, 0, %0, c6, c12, 4" : : "r" (bar));
		asm volatile ("mcr p15, 0, %0, c6, c12, 5" : : "r" (lar));
	    }
	    else {
		asm volatile ("mcr p15, 4, %0, c6, c12, 4" : : "r" (bar));
		asm volatile ("mcr p15, 4, %0, c6, c12, 5" : : "r" (lar));
	    }
	    break;
	  case 9:
	    if (!is_hyp()) {
		asm volatile ("mcr p15, 0, %0, c6, c12, 0" : : "r" (bar));
		asm volatile ("mcr p15, 0, %0, c6, c12, 1" : : "r" (lar));
	    }
	    else {
		asm volatile ("mcr p15, 4, %0, c6, c12, 0" : : "r" (bar));
		asm volatile ("mcr p15, 4, %0, c6, c12, 1" : : "r" (lar));
	    }
	    break;
	  case 10:
	    if (!is_hyp()) {
		asm volatile ("mcr p15, 0, %0, c6, c13, 4" : : "r" (bar));
		asm volatile ("mcr p15, 0, %0, c6, c13, 5" : : "r" (lar));
	    }
	    else {
		asm volatile ("mcr p15, 4, %0, c6, c13, 4" : : "r" (bar));
		asm volatile ("mcr p15, 4, %0, c6, c13, 5" : : "r" (lar));
	    }
	    break;
	  case 11:
	    if (!is_hyp()) {
		asm volatile ("mcr p15, 0, %0, c6, c13, 0" : : "r" (bar));
		asm volatile ("mcr p15, 0, %0, c6, c13, 1" : : "r" (lar));
	    }
	    else {
		asm volatile ("mcr p15, 4, %0, c6, c13, 0" : : "r" (bar));
		asm volatile ("mcr p15, 4, %0, c6, c13, 1" : : "r" (lar));
	    }
	    break;
	  case 12:
	    if (!is_hyp()) {
		asm volatile ("mcr p15, 0, %0, c6, c14, 0" : : "r" (bar));
		asm volatile ("mcr p15, 0, %0, c6, c14, 1" : : "r" (lar));
	    }
	    else {
		asm volatile ("mcr p15, 4, %0, c6, c14, 0" : : "r" (bar));
		asm volatile ("mcr p15, 4, %0, c6, c14, 1" : : "r" (lar));
	    }
	    break;
	  case 13:
	    if (!is_hyp()) {
		asm volatile ("mcr p15, 0, %0, c6, c14, 4" : : "r" (bar));
		asm volatile ("mcr p15, 0, %0, c6, c14, 5" : : "r" (lar));
	    }
	    else {
		asm volatile ("mcr p15, 4, %0, c6, c14, 4" : : "r" (bar));
		asm volatile ("mcr p15, 4, %0, c6, c14, 5" : : "r" (lar));
	    }
	    break;
	  case 14:
	    if (!is_hyp()) {
		asm volatile ("mcr p15, 0, %0, c6, c15, 0" : : "r" (bar));
		asm volatile ("mcr p15, 0, %0, c6, c15, 1" : : "r" (lar));
	    }
	    else {
		asm volatile ("mcr p15, 4, %0, c6, c15, 0" : : "r" (bar));
		asm volatile ("mcr p15, 4, %0, c6, c15, 1" : : "r" (lar));
	    }
	    break;
	  case 15:
	    if (!is_hyp()) {
		asm volatile ("mcr p15, 0, %0, c6, c15, 4" : : "r" (bar));
		asm volatile ("mcr p15, 0, %0, c6, c15, 5" : : "r" (lar));
	    }
	    else {
		asm volatile ("mcr p15, 4, %0, c6, c15, 4" : : "r" (bar));
		asm volatile ("mcr p15, 4, %0, c6, c15, 5" : : "r" (lar));
	    }
	    break;
	  default:
		return;
	} 
}

void setup_mpu_regions(struct mpu_region_config *rgns, u32 num_rgns)
{
	u32 num, i;

	asm volatile ("mrc p15, 0, %0, c0, c0, 4" : "=r" (num));
	num = (num & MPUIR_DREGION_MASK) >> MPUIR_DREGION_SHIFT;
	/* Regions to be configured cannot be greater than available regions */
	if (num < num_rgns)
		num_rgns = num;
	/**
	 * Assuming dcache might not be enabled at this point, disabling
	 * and invalidating only icache.
	 */
	icache_disable();
	invalidate_icache_all();

	disable_mpu();

	for (i = 0; i < num_rgns; i++)
		mpu_config(&rgns[i]);

	enable_mpu();

	icache_enable();
}

void enable_caches(void)
{
	/*
	 * setup_mpu_regions() might have enabled Icache. So add a check
	 * before enabling Icache
	 */
	if (!icache_status())
		icache_enable();
	dcache_enable();
}

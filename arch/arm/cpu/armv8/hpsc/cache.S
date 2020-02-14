/* SPDX-License-Identifier: GPL-2.0+ */
/* HPSC Chiplet uses CCN-502 interconnect. */

#include <config.h>
#include <linux/linkage.h>
#include <asm/macro.h>

#ifndef CCN_HNF_BASE
#error The platform does not define CCN_HNF_BASE in include/configs/
#endif
#ifndef CCN_HNF_NODES
#error The platform does not define CCN_HNF_NODES in include/configs/
#endif

ENTRY(__asm_flush_l3_dcache)
	mov	x0, #0
	movk	x0, #(CCN_HNF_BASE >> 16), lsl #16
	mov	x1, #CCN_HNF_NODES
	b ccn50x_flush_l3_cache
ENDPROC(__asm_flush_l3_dcache)

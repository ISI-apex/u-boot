/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * (C) Copyright 2015 Freescale Semiconductor
 *
 * Extracted from gic_64.S
 */

#include <config.h>
#include <linux/linkage.h>
#include <asm/macro.h>

#define REGION_SIZE			0x10000 /* 64 KB regions */

#define	HNF_PSTATE_REQ			0x10
#define	HNF_PSTATE_STATUS		0x18

#define HNF_PM_REQ_NOL3			0x0
#define HNF_PM_REQ_SFONLY		0x1
#define HNF_PM_REQ_HALF			0x2
#define HNF_PM_REQ_FULL			0x3
#define HNF_PM_REQ_MASK			0xfffffffffffffffc

#define HNF_PM_STATUS_NOL3		0x0
#define HNF_PM_STATUS_NOL3_2_SFONLY	0x1
#define HNF_PM_STATUS_NOL3_2_HALF  	0x2
#define HNF_PM_STATUS_NOL3_2_FULL  	0x3
#define HNF_PM_STATUS_SFONLY		0x4
#define HNF_PM_STATUS_SFONLY_2_NOL3	0x5
#define HNF_PM_STATUS_SFONLY_2_HALF	0x6
#define HNF_PM_STATUS_SFONLY_2_FULL	0x7
#define HNF_PM_STATUS_HALF		0x8
#define HNF_PM_STATUS_HALF_2_NOL3	0x9
#define HNF_PM_STATUS_HALF_2_SFONLY	0xa
#define HNF_PM_STATUS_HALF_2_FULL	0xb
#define HNF_PM_STATUS_FULL		0xc
#define HNF_PM_STATUS_FULL_2_NOL3	0xd
#define HNF_PM_STATUS_FULL_2_SFONLY	0xe
#define HNF_PM_STATUS_FULL_2_HALF	0xf

/*************************************************************************
 *
 * void ccn504_add_masters_to_dvm(CCI_MN_BASE, CCI_MN_RNF_NODEID_LIST,
 * 				  CCI_MN_DVM_DOMAIN_CTL_SET);
 *
 * Add fully-coherent masters to DVM domain
 *
 *************************************************************************/
ENTRY(ccn504_add_masters_to_dvm)
	/*
	 * x0: CCI_MN_BASE
	 * x1: CCI_MN_RNF_NODEID_LIST
	 * x2: CCI_MN_DVM_DOMAIN_CTL_SET
	 */

	/* Add fully-coherent masters to DVM domain */
	ldr	x9, [x0, x1]
	str	x9, [x0, x2]
1:	ldr	x10, [x0, x2]
	mvn	x11, x10
	tst	x11, x10 /* Wait for domain addition to complete */
	b.ne	1b

	ret
ENDPROC(ccn504_add_masters_to_dvm)

/*************************************************************************
 *
 * void ccn504_set_qos(CCI_Sx_QOS_CONTROL_BASE, QoS Value);
 *
 * Initialize QoS settings for AR/AW override.
 * Right now, this function sets the same QoS value for all RN-I ports
 *
 *************************************************************************/
ENTRY(ccn504_set_qos)
	/*
	 * x0: CCI_Sx_QOS_CONTROL_BASE
	 * x1: QoS Value
	 */

	/* Set all RN-I ports to QoS value denoted by x1 */
	ldr	x9, [x0]
	mov	x10, x1
	orr	x9, x9, x10
	str	x9, [x0]

	ret
ENDPROC(ccn504_set_qos)

/*************************************************************************
 *
 * void ccn504_set_aux(CCI_AUX_CONTROL_BASE, Value);
 *
 * Initialize AUX control settings
 *
 *************************************************************************/
ENTRY(ccn504_set_aux)
	/*
	 * x0: CCI_AUX_CONTROL_BASE
	 * x1: Value
	 */

	ldr	x9, [x0]
	mov	x10, x1
	orr	x9, x9, x10
	str	x9, [x0]

	ret
ENDPROC(ccn504_set_aux)

ENTRY(ccn50x_flush_l3_cache)
	/*
	 * x0: HNF_BASE (overwritten with return status)
	 * x1: HN-F node count (preserved)
	 * Return status in x0 (0: success)
         * clobber: x2
	 */
	mov	x29, lr

	dsb	sy
	mov	x2, #HNF_PM_REQ_SFONLY
	bl	hnf_set_pstate

	mov	x2, #HNF_PM_STATUS_SFONLY
	bl	hnf_pstate_poll

	dsb	sy
	mov	x2, #HNF_PM_REQ_FULL
	bl	hnf_set_pstate

	mov	x2, #HNF_PM_STATUS_FULL
	bl	hnf_pstate_poll

	mov	x0, #0 /* return code */
	mov	lr, x29
	ret
ENDPROC(ccn50x_flush_l3_cache)

hnf_pstate_poll:
	/* x0: HNF_BASE (preserved)
         * x1: HN-F node count (preserved)
	 * x2: the desired power state (preserved)
	 * returns only if operation succeeds
	 * clobber: x9, x10, x11
	 */
	mov	x9, x1
	orr	x10, x0, #HNF_PSTATE_STATUS
1:
	ldr	x11, [x10]
	cmp	x11, x2			/* check status */
	b.eq	2f
	b	1b
2:
	add	x10, x10, #REGION_SIZE	/* move to next HN-F node */
	subs	x9, x9, #1
	cbnz	x9, 1b
	ret

hnf_set_pstate:
	/* x0: HNF_BASE (preserved)
         * x1: HN-F node count (preserved)
	 * x2: the desired power state (preserved)
	 * clobber: x9, x10, x11
	 */
	mov	x9, x1
	orr	x10, x0, #HNF_PSTATE_REQ
1:
	ldr	x11, [x10]
	and	x11, x11, #HNF_PM_REQ_MASK
	orr	x11, x11, x2
	str	x11, [x10]
	add	x10, x10, #REGION_SIZE	/* move to next HN-F node */
	subs	x9, x9, #1
	cbnz	x9, 1b
	ret

/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Configuration for HPSC Chiplet (common across subsystems).
 */

#ifndef __HPSC_H
#define __HPSC_H

#define CCN_BASE	0xE1000000
#define CCN_HNF_REGION  32 /* index of first HN-F region */
#define CCN_REGION_SIZE 0x10000 /* 64 KB regions */
#define CCN_HNF_NODES   2 /* 6XP/2HNF HW configuration */
#define CCN_HNF_BASE	(CCN_BASE + CCN_HNF_REGION * CCN_REGION_SIZE)

#endif /* __HPSC_H */

/*
 * include/configs/h3ulcb.h
 *     This file is VC2 board configuration.
 *     CPU r8a7795.
 *
 * Copyright (C) 2018 Renesas Electronics Corporation
 * Copyright (C) 2018 Cogent Embedded, Inc.
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#ifndef __H3VC2_H
#define __H3VC2_H

#include "rcar-gen3-common.h"

/* Ethernet RAVB */
#define CONFIG_BITBANGMII
#define CONFIG_BITBANGMII_MULTI

/* Board Clock */
/* XTAL_CLK : 33.33MHz */
#define RCAR_XTAL_CLK		33333333u
#define CONFIG_SYS_CLK_FREQ	RCAR_XTAL_CLK

/* Generic Timer Definitions (use in assembler source) */
#define COUNTER_FREQUENCY	0xFE502A /* 16.66MHz from CPclk */
#define CONFIG_SH_SDHI_FREQ	200000000

/* Environment in eMMC, at the end of 2nd "boot sector" */
#define CONFIG_ENV_OFFSET	(-CONFIG_ENV_SIZE)
#define CONFIG_SYS_MMC_ENV_DEV	1
#define CONFIG_SYS_MMC_ENV_PART	1

#define CONFIG_SUPPORT_EMMC_BOOT
#endif /* __H3VC2_H */

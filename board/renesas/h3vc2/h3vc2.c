/*
 * board/renesas/h3vc2/h3vc2.c
 *     This file is H3VC2 board support.
 *
 * Copyright (C) 2018 Renesas Electronics Corporation
 * Copyright (C) 2018 Cogent Embedded, Inc.
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#include <common.h>
#include <malloc.h>
#include <netdev.h>
#include <dm.h>
#include <dm/platform_data/serial_sh.h>
#include <asm/processor.h>
#include <asm/mach-types.h>
#include <asm/io.h>
#include <linux/errno.h>
#include <asm/arch/sys_proto.h>
#include <asm/gpio.h>
#include <asm/arch/gpio.h>
#include <asm/arch/rmobile.h>
#include <asm/arch/rcar-mstp.h>
#include <asm/arch/sh_sdhi.h>
#include <i2c.h>
#include <mmc.h>

DECLARE_GLOBAL_DATA_PTR;

#define CPGWPCR	0xE6150904
#define CPGWPR  0xE615090C

void s_init(void)
{
	struct rcar_rwdt *rwdt = (struct rcar_rwdt *)RWDT_BASE;
	struct rcar_swdt *swdt = (struct rcar_swdt *)SWDT_BASE;

	/* Watchdog init */
	writel(0xA5A5A500, &rwdt->rwtcsra);
	writel(0xA5A5A500, &swdt->swtcsra);

	writel(0xA5A50000, CPGWPCR);
	writel(0xFFFFFFFF, CPGWPR);
}

#define SCIF2_MSTP310	(1 << 10)
#define DVFS_MSTP926	(1 << 26)
#define RPC_MSTP917	(1 << 17)
#define SD0_MSTP314	(1 << 14)
#define SD1_MSTP313	(1 << 13)
#define SD2_MSTP312	(1 << 12)
#define SD3_MSTP311	(1 << 11)

#define SD0CKCR		0xE6150074
#define SD1CKCR		0xE6150078
#define SD2CKCR		0xE6150268
#define SD3CKCR		0xE615026C

int board_early_init_f(void)
{
	int freq;


	/* SCIF2 */
	mstp_clrbits_le32(SMSTPCR3, SMSTPCR3, SCIF2_MSTP310);
	/* RPC */
	mstp_clrbits_le32(SMSTPCR9, SMSTPCR9, RPC_MSTP917);
	/* eMMC */
	mstp_clrbits_le32(SMSTPCR3, SMSTPCR3, SD1_MSTP313 | SD2_MSTP312);
	/* SDHI0 */
	mstp_clrbits_le32(SMSTPCR3, SMSTPCR3, SD0_MSTP314);

#if defined(CONFIG_SYS_I2C) && defined(CONFIG_SYS_I2C_SH)
	/* DVFS for reset */
	mstp_clrbits_le32(SMSTPCR9, SMSTPCR9, DVFS_MSTP926);
#endif
	return 0;
}

DECLARE_GLOBAL_DATA_PTR;

int board_init(void)
{
	/* adress of boot parameters */
	gd->bd->bi_boot_params = CONFIG_SYS_TEXT_BASE + 0x50000;

	/* Init PFC controller */

	return 0;
}


int dram_init(void)
{
	if (fdtdec_setup_memory_size() != 0)
		return -EINVAL;

	return 0;
}

int dram_init_banksize(void)
{
	fdtdec_setup_memory_banksize();

	return 0;
}

